/// =====================================================================================
///
///        @file:      aeAlgo.c
///        @brief:     simpleMonoAE Algorithm
///        @copyright: All code copyright Movidius Ltd 2015, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///

/// System Includes
/// -------------------------------------------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <DrvI2cMaster.h>
#include <sipp.h>

/// Application Includes
/// -------------------------------------------------------------------------------------
#include "aeAlgo.h"

#if 0
#define CMX_NOCACHE __attribute__((section(".cmx_direct.data")))
#else
#define CMX_NOCACHE static const
#endif

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------
/// Global control affecting image brightness after AE - tune to taste
CMX_NOCACHE u32 AE_BRIGHTNESS_CONTROL_HC = 85; // (255/3)
CMX_NOCACHE u32 AE_BRIGHTNESS_CONTROL_LC = 85; // (255/2)

/// Control affecting AE convergence speed.  Range is [0, 1.0]
CMX_NOCACHE float AE_CONVERGENCE_SPEED = 0.2f;

/// AE controls controlling fast convergence feature.  Fast convergence
/// increases the convergence speed when the image is greatly overexposed
/// (high number of saturated patches detected) in proportion to the number
/// of saturated patches.
/// If the number of saturated patches is below AE_SAT_FAST_CONVERGE_FLOOR,
/// then fast convergence is disabled.
/// AE_SAT_FAST_CONVERGE_SCALE controls the convergence acceleration.  A value
/// of zero effectively disables fast convergence.
#define AE_SAT_FAST_CONVERGE_FLOOR  0
#define AE_SAT_FAST_CONVERGE_SCALE  0.04f
#define AE_SAT_FAST_CONVERGE_LIMIT  0.8f

CMX_NOCACHE u32 STDDEV_HIGH_CONTRAST_THRESH   = 30;
CMX_NOCACHE u32 STDDEV_CONTRAST_SWITCH_THRESH = 15;

#define AE_STATS_SAT_THRESHOLD(NPIXELS)  (u32)((NPIXELS)*255)
#define AE_STATS_DARK_THRESHOLD(NPIXELS) (u32)((NPIXELS)*18)

/// Global Data (Only if absolutely necessary)
/// -------------------------------------------------------------------------------------

/// Static Local Data
/// -------------------------------------------------------------------------------------

/// Static Function Prototypes
/// -------------------------------------------------------------------------------------
static float aeCalcExpGain(float totalExposure, aeCamCfg_t *camAECfg, aeExpGainTable_t *tblExpGain, u32 *intTime, float *gainCode);
static unsigned aeGetTargetBrightness(unsigned stddev, int cam_id);
static float aeTemporalFilter(float currentExp, float targetExp, int saturatedPatches, int darkPatches);

/// Functions Implementation
/// -------------------------------------------------------------------------------------

/// Converts fp32 gain to linear gain value (in Q format) and fills back the actual fp32 gain
static s32 calcSensorGain(fp32 *gain, int round, aeCamCfg_t *camAECfg)
{
    /// This routine is valid only for Cameras with linear gain !!!

    float minGain = camAECfg->min.gainCode;
    float maxGain = camAECfg->max.gainCode;
    u32   gainConvScaler  = 1 << camAECfg->gainFractBits;
    float gainStep        = 1.0f / gainConvScaler;
    float gainRoundFactor = (float)(gainConvScaler - 1) / gainConvScaler;

    s32 status = STS_CONVGAIN_OK;

    if (*gain > maxGain) {
        *gain = maxGain;
        status = STS_CONVGAIN_CLIP_MAX;
    } else if (*gain < minGain) {
        *gain = minGain;
        status = STS_CONVGAIN_CLIP_MIN;
    }

    if (round == CONV_GAIN_CEIL) {
        *gain += gainStep * gainRoundFactor;
    } else if (round == CONV_GAIN_FLOOR) {
        *gain -= gainStep * gainRoundFactor;
    } else {//(round == CONV_GAIN_ROUND)
        *gain += gainStep / 2;
    }
    *gain = (float)((s32)(*gain * gainConvScaler)) / gainConvScaler;

    return status;
}

/// ===  FUNCTION  ======================================================================
///  Name:  aeCalcExpGain
///  Description:  Find a gain and integration that yield the desired total exposure.
///                Use the smallest gain possible, to minimize noise amplification.
///                'totalExposure' is the integration time, times the analog gain.
/// =====================================================================================
static float aeCalcExpGain(float totalExposure, aeCamCfg_t *camAECfg, aeExpGainTable_t *tblExpGain, u32 *intTime, float *gainCode)
{
    u32 maxIntTime;
    u32 regionNmbr;

    maxIntTime = camAECfg->max.integrationTime;
    regionNmbr = tblExpGain->regionNmbr;

    if (regionNmbr && regionNmbr < AE_EXPGAIN_TBL_MAX_SZ) { /// {Exposure,Gain} regions table is valid
        u32 rgnCntr;

        for (rgnCntr = 0; rgnCntr <= regionNmbr; rgnCntr++)
        {
            if (totalExposure <= tblExpGain->region[rgnCntr].totalExp)
                break;
        }

        if (rgnCntr == 0 || rgnCntr > regionNmbr) {         /// Total Exposure is out of Table's supported range => clip to min/max
            if (rgnCntr > regionNmbr)
                rgnCntr = regionNmbr;
            *intTime  = tblExpGain->region[rgnCntr].range.integrationTime;
            *gainCode = tblExpGain->region[rgnCntr].range.gainCode;
            calcSensorGain(gainCode, CONV_GAIN_ROUND, camAECfg);
        } else {                                            /// Convert Total Exposure according to current region's type and range
            if (tblExpGain->region[rgnCntr].type == AE_REGION_TYPE_CONST_EXP) {
                *intTime  = tblExpGain->region[rgnCntr].range.integrationTime;
                *gainCode = totalExposure / *intTime;
                calcSensorGain(gainCode, CONV_GAIN_ROUND, camAECfg);
            } else {//(tblExpGain->region[rgnCntr].type == AE_REGION_TYPE_CONST_GAIN)
                *gainCode = tblExpGain->region[rgnCntr].range.gainCode;
                calcSensorGain(gainCode, CONV_GAIN_CEIL, camAECfg);
                *intTime  = totalExposure / *gainCode + 0.5f;
            }
        }
    } else {                                                /// {Exposure,Gain} regions table is empty/corrupt
        float gain;
        float gainOverExpRatio = 1.0f;

        gain = totalExposure / maxIntTime;          /// Find the minimum gain that's sufficient (at max exposure time)
        /// Round to the closest upper sensor gain step for better precision:
        ///     At low light can still compensate the gain step delta by slightly lowering the exposure.
        ///     If rounding down - the exposure is clipped to max and can't be used for compensation.
        calcSensorGain(&gain, CONV_GAIN_CEIL, camAECfg);

        *gainCode = gain;
        *intTime = totalExposure / gain / gainOverExpRatio + .5;
    }

    if (*intTime > maxIntTime) {
        *intTime = maxIntTime;
    }

    return *gainCode * *intTime;
}

// Temporary: store contrast selection params separately for each camera; TODO: introduce camera context
static int      stddevWhenChanged[MAX_NR_CAMS] = { 0, 0 };
static int      highContrastLast[MAX_NR_CAMS]  = { 0, 0 };

/// ===  FUNCTION  ======================================================================
///  Name:  aeGetTargetBrightness
///  Description:  Work out the desired average brightness of the scene (target average
///                brightness).  When there's more contrast in the scene, use a lower
///                target average brightness.
/// =====================================================================================
static unsigned aeGetTargetBrightness(unsigned stddev, int cam_id)
{
    int     highContrast;

    highContrast = stddev > STDDEV_HIGH_CONTRAST_THRESH;

    if (highContrast != highContrastLast[cam_id]) {
        if (fabs(stddev-stddevWhenChanged[cam_id]) > STDDEV_CONTRAST_SWITCH_THRESH) {
            highContrastLast[cam_id] = highContrast;
            stddevWhenChanged[cam_id] = stddev;
        }
    }

    return highContrastLast[cam_id] ?
      AE_BRIGHTNESS_CONTROL_HC : AE_BRIGHTNESS_CONTROL_LC;
}

/// ===  FUNCTION  ======================================================================
///  Name:  aeTemporalFilter
///  Description:
/// =====================================================================================
static float aeTemporalFilter(float currentExp, float targetExp, int saturatedPatches, int darkPatches)
{
    float   alpha = AE_CONVERGENCE_SPEED;
    float   exposure;
    int     badPatches = saturatedPatches;

    if (darkPatches > saturatedPatches) {
        badPatches = darkPatches;
    }

    badPatches -= AE_SAT_FAST_CONVERGE_FLOOR;
    if (badPatches < 0) {
        badPatches = 0;
    }

    alpha = alpha * (1.0f + (float)badPatches*AE_SAT_FAST_CONVERGE_SCALE);
    if (alpha > AE_SAT_FAST_CONVERGE_LIMIT) {
        alpha = AE_SAT_FAST_CONVERGE_LIMIT;
    }

    exposure = currentExp * (1.0f-alpha) + targetExp * alpha;

    return exposure;
}

#if defined(ANTI_FLICKER_50HZ) || defined(ANTI_FLICKER_60HZ)
/// ===  FUNCTION  ======================================================================
///  Name:  applyAntiFlicker
///  Description: Simple 50Hz/60Hz anti-flicker solution
/// =====================================================================================
static float applyAntiFlicker(aeCamCfg_t *camAECfg, u32 *intTime, float *gainCode)
{
    u32   maxIntTime = camAECfg->max.integrationTime;
    u32   tmpIntTime = *intTime;
    float minGain    = camAECfg->min.gainCode;
    float tmpGain    = *gainCode;

    if (tmpIntTime >= FLICK_EXP_THR3) {
        tmpIntTime = FLICK_EXP_THR3;
    } else if (tmpIntTime >= FLICK_EXP_THR2) {
        if (tmpIntTime >= (FLICK_EXP_THR2 + FLICK_EXP_THR3) / 2) {
            if (tmpGain * tmpIntTime / FLICK_EXP_THR3 >= minGain)
                tmpIntTime = FLICK_EXP_THR3;
            else
                tmpIntTime = FLICK_EXP_THR2;
        } else
            tmpIntTime = FLICK_EXP_THR2;
    } else if (tmpIntTime >= FLICK_EXP_THR1) {
        if (tmpIntTime >= (FLICK_EXP_THR1 + FLICK_EXP_THR2) / 2) {
            if (tmpGain * tmpIntTime / FLICK_EXP_THR2 >= minGain)
                tmpIntTime = FLICK_EXP_THR2;
            else
                tmpIntTime = FLICK_EXP_THR1;
        } else
            tmpIntTime = FLICK_EXP_THR1;
    }
    else
    {}  /// Do nothing: can't reduce flicker without having overexposed image

    /// Compensate reduced Integration Time by increasing the Analog Gain
    tmpGain = *gainCode * *intTime / tmpIntTime;
    /// Round to the nearest sensor gain step; update TE history respectively to avoid slight oscillation
    calcSensorGain(&tmpGain, CONV_GAIN_ROUND, camAECfg);

    if (tmpIntTime > maxIntTime)
        tmpIntTime = maxIntTime;

    *intTime  = tmpIntTime;
    *gainCode = tmpGain;

    return *gainCode * *intTime;
}
#endif  // #if defined(ANTI_FLICKER_50HZ) || defined(ANTI_FLICKER_60HZ)

/// ===  FUNCTION  ======================================================================
///  Name:  aeGetNewExposure
///  Description: main function of exposure calculation
/// =====================================================================================
aeExposure_t aeGetNewExposure(ae_patch_stats* statPatchesBuffer,
                              int stats_patches_h, int stats_patches_v,
                              int stats_patch_w, int stats_patch_h, int cam_id,
                              float curTotalExp, float statsTotalExp,
                              aeCamCfg_t *camAECfg, aeExpGainTable_t *tblExpGain)
{
    int             y, n;
    int             saturatedPatches = 0, darkPatches = 0, total = 0;
    unsigned        avgBrightness, targetBrightness;
    unsigned        variance, stddev;
    float           ratio;
    float           totalExposure;
    u32             exposure;
    float           gainCode;
    int npixels = (stats_patch_w * stats_patch_h);

    for (y = 0; y < (stats_patches_h * stats_patches_v); y+=1)
    {
        uint32_t patchBrightness =
            statPatchesBuffer[y].accum[0]+
            statPatchesBuffer[y].accum[1]+
            statPatchesBuffer[y].accum[2]+
            statPatchesBuffer[y].accum[3];

        if (patchBrightness >= AE_STATS_SAT_THRESHOLD(npixels)) {
            saturatedPatches++;
        }
        if (patchBrightness <= AE_STATS_DARK_THRESHOLD(npixels)) {
            darkPatches++;
        }
        total += patchBrightness;
    }

    avgBrightness = total /
      (npixels*(stats_patches_h*stats_patches_v));

    /* Work out the variance of the stats (scene contrast) */
    variance = 0;
    for (y = 0; y < (stats_patches_h*stats_patches_v); y+=1)
    {
        uint32_t patchBrightness =
            statPatchesBuffer[y].accum[0]+
            statPatchesBuffer[y].accum[1]+
            statPatchesBuffer[y].accum[2]+
            statPatchesBuffer[y].accum[3];

            n = patchBrightness / npixels - avgBrightness;
            variance += n*n;
    }

    stddev = sqrt(variance / (stats_patches_h*stats_patches_v));

    targetBrightness = aeGetTargetBrightness(stddev, cam_id);

    /*
     * Calculate ratio of current brightness to new target brightness.
     * This is the same as the ratio of current total exposure to target
     * total exposure.
     */
    ratio = (float)targetBrightness / (avgBrightness+1);

    totalExposure = aeTemporalFilter(curTotalExp,
      statsTotalExp * ratio, saturatedPatches, darkPatches);

    /* Calculate exposure to program for next frame */
    aeCalcExpGain(totalExposure, camAECfg, tblExpGain, &exposure, &gainCode);

#if defined(ANTI_FLICKER_50HZ) || defined(ANTI_FLICKER_60HZ)
    applyAntiFlicker(camAECfg, &exposure, &gainCode);
#endif  // #if defined(ANTI_FLICKER_50HZ) || defined(ANTI_FLICKER_60HZ)

    aeExposure_t exposureOut = { exposure, gainCode };

    return exposureOut;
}

/// ===  FUNCTION  ======================================================================
///  Name:  aeMultiCamExpSelect
///  Description: select common exposure time for all cameras and compensate the gain
///               respectively (each camera can have different total exposure)
/// =====================================================================================
int aeMultiCamExpSelect(multiCamExp_t *multiCamExp, aeCamCfg_t *camAECfg, int camNmbr)
{
    int camId;
    u32 minIntegrTime = UINT_MAX;
    int status = STS_MULTICAM_OK;

    for (camId = 0; camId < camNmbr; camId++)               // Find the shortest integration time camera
    {
        if ((multiCamExp->camBitMask & (1 << camId)) &&     // Ensure that current camera has valid stats (exp,gain)
            (multiCamExp->exp[camId].integrationTime < minIntegrTime))
        {
            minIntegrTime = multiCamExp->exp[camId].integrationTime;
        }
    }

    if (minIntegrTime == 0 || minIntegrTime == UINT_MAX)
        return (status = STS_MULTICAM_WRONG_EXP);

    for (camId = 0; camId < camNmbr; camId++)               // Set common exposure and compensate the gain respectively
    {
        if (multiCamExp->camBitMask & (1 << camId))         // Ensure that current camera has valid stats (exp,gain)
        {
            float ratio, gain;

            ratio = (float) multiCamExp->exp[camId].integrationTime / minIntegrTime;
            multiCamExp->exp[camId].integrationTime = minIntegrTime;
            gain = ratio * multiCamExp->exp[camId].gainCode;

            /// Round to the nearest sensor gain step; update TE history respectively to avoid slight oscillation
            if (calcSensorGain(&gain, CONV_GAIN_ROUND, &camAECfg[camId]) == STS_CONVGAIN_CLIP_MAX)
                status = STS_MULTICAM_GAIN_OVF;             // Compensated gain higher than MAX => darker image for this camera
            multiCamExp->exp[camId].gainCode = gain;
        }
    }

    return (status);
}
