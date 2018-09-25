///
/// @file      init_ic_isp_config.cpp
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test case for Isp configuration
///            Platform(s) supported : ma2x8x
///

#include <stdio.h>
#include <string.h>
#include <Fp16Convert.h>
#include "isp_params_wrap.h"
#include "Opipe.h"
#include "OpipeApps.h"
#include "IspCommon.h"

//This should be somewhre in isp_params, but anyway...
#ifndef BAYER_PATTERN
#define BAYER_PATTERN  RGGB
#endif

#define AE_HPATCHES_NO      16
#define AE_VPATCHES_NO       5
#define AE_PATCH_WIDTH      16
#define AE_PATCH_HEIGHT     16
#define AE_X_START           6
#define AE_Y_START           2
#define AE_X_OFFSET         10 //gap between patches
#define AE_Y_OFFSET          2 //gap between patches
#define AE_X_SKIP           (AE_PATCH_WIDTH  + AE_X_OFFSET)
#define AE_Y_SKIP           (AE_PATCH_HEIGHT + AE_Y_OFFSET)
#define AE_PATCHES_NO       (AE_VPATCHES_NO * AE_HPATCHES_NO)

#define AF_HPATCHES_NO      20
#define AF_VPATCHES_NO       7
#define AF_PATCH_WIDTH      18
#define AF_PATCH_HEIGHT     14
#define AF_X_START           8
#define AF_Y_START           2
#define AF_PATCHES_NO       (AF_VPATCHES_NO * AF_HPATCHES_NO)

#define NR_OF_CIRCULAR_CONFIGS_BUF  8
#define MAX_NR_OF_PIPES             6

int32_t         af1coefs[] = {44,  288, -200,   16,  28,  16,  368, -212, 536, -1076, 536};
int32_t         af2coefs[] = {44, -288, -200,  104, 204, 104, -368, -212,  76,  -152,  76};

AeAwbPatchStats aeStats[AE_PATCHES_NO] ALIGNED(8);
AfPatchStats    afStats[AF_PATCHES_NO] ALIGNED(8);

//##############################################################################
//Fill in icIspConfig
//##############################################################################
void setIcIspConfig(icIspConfig *ic)
{
    uint32_t i;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Global settings
    ic->bayerOrder   = BAYER_PATTERN;
    ic->pipelineBits = ISPC_BAYER_BITS;
    ic->pipeControl  = IC_PIPECTL_MIPI_ENABLE;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //BlackLevelCorrection
    #ifndef BLACK_LEVEL
    #define BLACK_LEVEL 16
    #endif
    ic->blc.r  = BLACK_LEVEL;
    ic->blc.b  = BLACK_LEVEL;
    ic->blc.gr = BLACK_LEVEL;
    ic->blc.gb = BLACK_LEVEL;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Sigma
    ic->sigma.noiseFloor = ISPC_SIGMA_NOISE_FLOOR;
    ic->sigma.thresh1P0  = ISPC_SIGMA_THRESH1_P0;
    ic->sigma.thresh2P0  = ISPC_SIGMA_THRESH2_P0;
    ic->sigma.thresh1P1  = ISPC_SIGMA_THRESH1_P1;
    ic->sigma.thresh2P1  = ISPC_SIGMA_THRESH2_P1;
    ic->sigma.thresh1P2  = ISPC_SIGMA_THRESH1_P2;
    ic->sigma.thresh2P2  = ISPC_SIGMA_THRESH2_P2;
    ic->sigma.thresh1P3  = ISPC_SIGMA_THRESH1_P3;
    ic->sigma.thresh2P3  = ISPC_SIGMA_THRESH2_P3;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //LSC
    ic->lsc.lscWidth  = ISPC_LSC_GAIN_MAP_W;
    ic->lsc.lscStride = ic->lsc.lscWidth;
    ic->lsc.lscHeight = ISPC_LSC_GAIN_MAP_H;
    ic->lsc.pLscTable = (uint16_t*)ispcLscMesh;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //RAW
    ic->raw.gainR   = ISPC_RAW_GAIN_0;
    ic->raw.gainGr  = ISPC_RAW_GAIN_1;
    ic->raw.gainGb  = ISPC_RAW_GAIN_2;
    ic->raw.gainB   = ISPC_RAW_GAIN_3;

    ic->raw.clampGr = ISPC_RAW_CLAMP_0;
    ic->raw.clampR  = ISPC_RAW_CLAMP_1;
    ic->raw.clampB  = ISPC_RAW_CLAMP_2;
    ic->raw.clampGb = ISPC_RAW_CLAMP_3;

    ic->raw.grgbImbalPlatDark    = ISPC_GRGB_IMBAL_PLAT_DARK;
    ic->raw.grgbImbalDecayDark   = ISPC_GRGB_IMBAL_DECAY_DARK;
    ic->raw.grgbImbalPlatBright  = ISPC_GRGB_IMBAL_PLAT_BRIGHT;
    ic->raw.grgbImbalDecayBright = ISPC_GRGB_IMBAL_DECAY_BRIGHT;

    ic->raw.grgbImbalThr   = ISPC_GRGB_IMBAL_THRESHOLD;
    ic->raw.dpcAlphaHotG   = ISPC_BAD_PIX_ALPHA_G_HOT;
    ic->raw.dpcAlphaHotRb  = ISPC_BAD_PIX_ALPHA_RB_HOT;
    ic->raw.dpcAlphaColdG  = ISPC_BAD_PIX_ALPHA_G_COLD;
    ic->raw.dpcAlphaColdRb = ISPC_BAD_PIX_ALPHA_RB_COLD;
    ic->raw.dpcNoiseLevel  = ISPC_BAD_PIX_NOISE_LEVEL;

    //RAW AE STATS
    ic->aeAwbConfig.firstPatchX = AE_X_START;     ic->aeAwbConfig.firstPatchY  = AE_Y_START;
    ic->aeAwbConfig.patchWidth  = AE_PATCH_WIDTH; ic->aeAwbConfig.patchHeight  = AE_PATCH_HEIGHT;
    ic->aeAwbConfig.nPatchesX   = AE_HPATCHES_NO; ic->aeAwbConfig.nPatchesY    = AE_VPATCHES_NO;
    ic->aeAwbConfig.patchGapX   = AE_X_SKIP;      ic->aeAwbConfig.patchGapY    = AE_Y_SKIP;
    ic->aeAwbConfig.darkThresh  = 0;              ic->aeAwbConfig.brightThresh = 1024;
    ic->aeAwbStats     = aeStats;
    //RAW AF STATS
    ic->afConfig.firstPatchX = AF_X_START;     ic->afConfig.firstPatchY  = AF_Y_START;
    ic->afConfig.patchWidth  = AF_PATCH_WIDTH; ic->afConfig.patchHeight  = AF_PATCH_HEIGHT;
    ic->afConfig.nPatchesX   = AF_HPATCHES_NO; ic->afConfig.nPatchesY    = AF_VPATCHES_NO;
    ic->afConfig.f1Threshold  = 0;             ic->afConfig.f2Threshold  = 0;
    ic->afConfig.initialSubtractionValue = 512>>4;
    for(i=0; i<11; i++){
        ic->afConfig.f1Coeffs[i] = af1coefs[i];
        ic->afConfig.f2Coeffs[i] = af2coefs[i];
    }
    ic->afStats     = afStats;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Debayer
    ic->demosaic.dewormGradientMul = 0;
    ic->demosaic.dewormSlope       = ISPC_DEMOSAIC_MIX_SLOPE;
    ic->demosaic.dewormOffset      = ISPC_DEMOSAIC_MIX_OFFSET;
    ic->demosaic.lumaWeightR       = ISPC_DEMOSAIC_LUMA_WEIGHT_RED;
    ic->demosaic.lumaWeightG       = ISPC_DEMOSAIC_LUMA_WEIGHT_GREEN;
    ic->demosaic.lumaWeightB       = ISPC_DEMOSAIC_LUMA_WEIGHT_BLUE;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Dog-[Ltm]
    ic->ltm.thr     = ISPC_LTM_FILTER_TH1;

    {//Convert curve params from float32 to U12F
       int curve, point, regoff;
       regoff = 0;
       for (curve = 0; curve < 8; curve++) {
           for (point = 0; point < 16; point += 2) {
               int off = curve + point * 8;
               ic->ltm.curves[off  ] = ispcLtmCurves[off]   * 0xfff; // Convert to U12F
               ic->ltm.curves[off+8] = ispcLtmCurves[off+8] * 0xfff; // Convert to U12F
               regoff += 4;
           }
        }
    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //[Dog]-Ltm
    ic->dog.thr      = ISPC_DOG_THR;
    ic->dog.strength = ISPC_DOG_STRENGTH;
    memcpy(ic->dog.coeffs11, dogCoeffs11, sizeof(dogCoeffs11));
    memcpy(ic->dog.coeffs15, dogCoeffs15, sizeof(dogCoeffs15));

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Luma denoise
    ic->lumaDenoise.bitpos = ISPC_LUMA_DNS_BITPOS;
    ic->lumaDenoise.alpha  = ISPC_LUMA_DNS_ALPHA;
    ic->lumaDenoise.f2     = ISPC_LUMA_DNS_F2;
    memcpy(ic->lumaDenoise.lut, ispcLumaDnsLut, sizeof(ispcLumaDnsLut));

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Luma denoise ref
    ic->lumaDenoiseRef.shift = ISPC_LUMA_DNS_REF_SHIFT;
    memcpy(ic->lumaDenoiseRef.lutGamma0_32  , ispcGammaLut0_32  , sizeof(ispcGammaLut0_32  ));
    memcpy(ic->lumaDenoiseRef.lutGamma32_255, ispcGammaLut32_255, sizeof(ispcGammaLut32_255));
    memcpy(ic->lumaDenoiseRef.lutDist       , ispcYDnsDistLut   , sizeof(ispcYDnsDistLut   ));

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Luma Sharpen
    ic->sharpen.strengthLighten = 0x0000;//fp16
    ic->sharpen.strengthDarken  = 0x0000;//fp16
    ic->sharpen.minThr          = f32Tof16(ISPC_SHARP_MIN_THR);
    ic->sharpen.alpha           = 0x0000;//fp16
    ic->sharpen.undershoot      = 0x0000;//fp16
    ic->sharpen.overshoot       = 0x4000;//fp16

    ic->sharpen.rangeStop0 =  ic->sharpen.rangeStop1 =
    ic->sharpen.rangeStop2 =  ic->sharpen.rangeStop3 = 0x0000;//fp16

    ic->sharpen.sharpenCoeffs[0] = ic->sharpen.sharpenCoeffs[1] =
    ic->sharpen.sharpenCoeffs[2] = ic->sharpen.sharpenCoeffs[3] = 0x0000; //fp16

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //GenChroma
    ic->chromaGen.desatOffset = ISPC_CGEN_DESAT_OFFSET;
    ic->chromaGen.desatSlope  = ISPC_CGEN_DESAT_SLOPE;
    ic->chromaGen.pfrStrength = ISPC_CGEN_PFR_STRENGTH;

    ic->chromaGen.lumaCoeffR  = ISPC_CGEN_LUMA_COEFF_R;
    ic->chromaGen.lumaCoeffG  = ISPC_CGEN_LUMA_COEFF_G;
    ic->chromaGen.lumaCoeffB  = ISPC_CGEN_LUMA_COEFF_B;

    ic->chromaGen.epsilon     = ISPC_CGEN_EPSILON;
    ic->chromaGen.kr          = ISPC_CGEN_KR;
    ic->chromaGen.kg          = ISPC_CGEN_KG;
    ic->chromaGen.kb          = ISPC_CGEN_KB;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Chroma median
    ic->median.kernelSize = ISPC_CHROMA_MEDIAN_SIZE;
    ic->median.slope      = ISPC_CHROMA_MEDIAN_MIX_SLOPE;
    ic->median.offset     = ISPC_CHROMA_MEDIAN_MIX_OFFSET;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Chroma denoise
    ic->chromaDenoise.hEnab           = ISPC_CHROMA_DNS_H_ENAB;
    ic->chromaDenoise.limit           = ISPC_CHROMA_DNS_LIMIT;
    ic->chromaDenoise.greyDesatOffset = ISPC_GREY_DESAT_OFFSET;
    ic->chromaDenoise.greyDesatSlope  = ISPC_GREY_DESAT_SLOPE;

    ic->chromaDenoise.th_r = ISPC_CHROMA_DNS_TH_R;
    ic->chromaDenoise.th_g = ISPC_CHROMA_DNS_TH_G;
    ic->chromaDenoise.th_b = ISPC_CHROMA_DNS_TH_B;

    ic->chromaDenoise.greyCb = ISPC_GREY_POINT_B;
    ic->chromaDenoise.greyCg = ISPC_GREY_POINT_G;
    ic->chromaDenoise.greyCr = ISPC_GREY_POINT_R;

    ic->chromaDenoise.convCoeffCorner = ispcLowpassKernel[0]; // [0] [1] [2]
    ic->chromaDenoise.convCoeffEdge   = ispcLowpassKernel[1]; // [3] [4] [5]
    ic->chromaDenoise.convCoeffCenter = ispcLowpassKernel[4]; // [6] [7] [8]

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Chroma denoise
    ic->colorCombine.kr = ISPC_CC_KR;
    ic->colorCombine.kg = ISPC_CC_KG;
    ic->colorCombine.kb = ISPC_CC_KB;
    memcpy(ic->colorCombine.ccm   , ispcCCM   , sizeof(ispcCCM   ));
    memcpy(ic->colorCombine.ccmOff, ispcCCMOff, sizeof(ispcCCMOff));

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //RGB Lut
    ic->gamma.table      = gammaLutFp16;
    ic->gamma.size       = 512;        //debug only really...
    ic->gamma.rgnSize[0] = 0x55555555; //2^5 = 32, 32x8x2 = 512
    ic->gamma.rgnSize[1] = 0x55555555;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //RGB->YUV convert
    memcpy(ic->colorConvert.mat   , ispcCSC   , sizeof(ispcCSC   ));
    memcpy(ic->colorConvert.offset, ispcCSCOff, sizeof(ispcCSCOff));

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //PolyFir(s)
    ic->updnCfg0.vN  = 1; ic->updnCfg0.vD  = 1;
    ic->updnCfg0.hN  = 1; ic->updnCfg0.hD  = 1;
}
