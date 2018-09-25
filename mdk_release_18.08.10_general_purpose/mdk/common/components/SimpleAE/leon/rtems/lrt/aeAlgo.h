/// =====================================================================================
///
///        @file:      aeAlgo.h
///        @brief:     simpleMonoAE Algorithm API
///        @copyright: All code copyright Movidius Ltd 2015, all rights reserved.
///                  For License Warranty see: common/license.txt
/// =====================================================================================
///

#ifndef AE_ALGO_H
#define AE_ALGO_H

/// System Includes
/// -------------------------------------------------------------------------------------
#include <inttypes.h>
#include <DrvI2c.h>
#include <swcFrameTypes.h>
#include <mv_types.h>
/// Application Includes
#include "aeCommStructs.h"
/// -------------------------------------------------------------------------------------

/// Source Specific #defines and types (typedef,enum,struct)
/// -------------------------------------------------------------------------------------

/// Defines for Convert Gain to Register Value routine
/// Status codes:
#define STS_CONVGAIN_OK         (0)     // Success
#define STS_CONVGAIN_CLIP_MIN   (1)     // Gain underflow -> clipped to Min supported gain value
#define STS_CONVGAIN_CLIP_MAX   (2)     // Gain overflow  -> clipped to Max supported gain value
/// Rounding options:
#define CONV_GAIN_ROUND         (0)     // Round gain to the nearest supported value by sensor
#define CONV_GAIN_FLOOR         (1)     // Round gain down to the nearest supported value by sensor
#define CONV_GAIN_CEIL          (2)     // Round gain up to the nearest supported value by sensor

/// Status codes for Multi Camera Exposure Select routine
#define STS_MULTICAM_OK         (0)     // Success
#define STS_MULTICAM_WRONG_EXP  (-1)    // Unexpected error when selecting common exposure time for all cameras
#define STS_MULTICAM_GAIN_OVF   (1)     // Some camera/s can't get to the targeted Total Exp since compensated gain reached MAX

/// Simple 50Hz / 60Hz anti-flicker solution
//#define ANTI_FLICKER_50HZ                       // Flicker reduction for 50Hz mains frequency
#define ANTI_FLICKER_60HZ                       // Flicker reduction for 60Hz mains frequency

#if defined(ANTI_FLICKER_50HZ)
    #define HALF_PERIOD_50HZ        1000000ul / 50 / 2      // 10000 us
    #define FLICK_50HZ_10MS         (1 * HALF_PERIOD_50HZ)
    #define FLICK_50HZ_20MS         (2 * HALF_PERIOD_50HZ)
    #define FLICK_50HZ_30MS         (3 * HALF_PERIOD_50HZ)

    #define FLICK_EXP_THR1          FLICK_50HZ_10MS
    #define FLICK_EXP_THR2          FLICK_50HZ_20MS
    #define FLICK_EXP_THR3          FLICK_50HZ_30MS
#elif defined(ANTI_FLICKER_60HZ)
    #define HALF_PERIOD_60HZ        1000000ul / 60 / 2      // 8333 us
    #define FLICK_60HZ_8MS          (1 * HALF_PERIOD_60HZ)
    #define FLICK_60HZ_17MS         (2 * HALF_PERIOD_60HZ)
    #define FLICK_60HZ_25MS         (3 * HALF_PERIOD_60HZ)

    #define FLICK_EXP_THR1          FLICK_60HZ_8MS
    #define FLICK_EXP_THR2          FLICK_60HZ_17MS
    #define FLICK_EXP_THR3          FLICK_60HZ_25MS
#endif  // #elif defined(ANTI_FLICKER_60HZ)

typedef struct aeExposure {
    u32 integrationTime;
    float gainCode;
} aeExposure_t;

typedef struct aeCamCfg {
    aeExposure_t min;
    aeExposure_t max;
    u32          gainFractBits;
} aeCamCfg_t;

typedef enum aeRegionType
{
    AE_REGION_TYPE_NA = 0,
    AE_REGION_TYPE_CONST_EXP,
    AE_REGION_TYPE_CONST_GAIN
} aeRegionType_t;

typedef struct aeExpGainRegion
{
    aeRegionType_t  type;
    aeExposure_t    range;
    float           totalExp;
} aeExpGainRegion_t;

typedef struct aeExpGainTable {
    uint32_t            regionNmbr;
    aeExpGainRegion_t   region[AE_EXPGAIN_TBL_MAX_SZ];
} aeExpGainTable_t;

typedef struct {
    int             camBitMask;
    uint32_t        frameNo;
    aeExposure_t    exp[MAX_NR_CAMS];
} multiCamExp_t;

/// Function Prototypes
/// -------------------------------------------------------------------------------------

/// ===  FUNCTION  ======================================================================
///  Name:  aeGetNewExposure
///  Description: main function of exposure calculation
/// =====================================================================================
aeExposure_t aeGetNewExposure(ae_patch_stats* statPatchesBuffer,
                              int stats_patches_h, int stats_patches_v,
                              int stats_patch_w, int stats_patch_h, int cam_id,
                              float curTotalExp, float statsTotalExp,
                              aeCamCfg_t *camAECfg, aeExpGainTable_t *tblExpGain);

/// ===  FUNCTION  ======================================================================
///  Name:  aeMultiCamExpSelect
///  Description: select common exposure time for all cameras and compensate the gain
///               respectively (each camera can have different total exposure)
/// =====================================================================================
int aeMultiCamExpSelect(multiCamExp_t *multiCamExp, aeCamCfg_t *camAECfg, int camNmbr);

#endif /// AE_ALGO_H
