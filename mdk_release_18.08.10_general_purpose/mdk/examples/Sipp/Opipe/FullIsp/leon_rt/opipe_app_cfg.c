///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    Opipe application configuration file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <string.h>
#include <Fp16Convert.h>
#include "mv_types.h"
#include "isp_params_wrap.h"
#include "Opipe.h"
#include "OpipeApps.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
SigmaDnsCfg   sigCfg;
BlcCfg        blcCfg;
LscCfg        lscCfg;
RawCfg        rawCfg;
DbyrCfg       dbyrCfg;
LtmCfg        ltmCfg;
DogCfg        dogCfg;
LumaDnsCfg    lumaDnsCfg;
LumaDnsRefCfg lumaDnsRefCfg;
SharpenCfg    sharpCfg;
ChromaGenCfg  cGenCfg;
MedianCfg     medCfg;
ChromaDnsCfg  chrDnsCfg;
ColCombCfg    ccCfg;
LutCfg        lutCfg;
ColConvCfg    colConvCfg;
UpfirdnCfg    updnCfg0;
UpfirdnCfg    updnCfg12;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------
void appSetParams(Opipe *p)
{
    // Global stuff
    p->rawBits      = ISPC_BAYER_BITS;
    p->bayerPattern = BAYER_PATTERN;

    // Filter specific
    // BlackLevelCorrection
    p->pBlcCfg   = &blcCfg;
    blcCfg.r    = 16;
    blcCfg.b    = 16;
    blcCfg.gr   = 16;
    blcCfg.gb   = 16;

    // Sigma
    p->pSigmaCfg       = &sigCfg;
    sigCfg.noiseFloor = ISPC_SIGMA_NOISE_FLOOR;
    sigCfg.thresh1P0  = ISPC_SIGMA_THRESH1_P0;
    sigCfg.thresh2P0  = ISPC_SIGMA_THRESH2_P0;
    sigCfg.thresh1P1  = ISPC_SIGMA_THRESH1_P1;
    sigCfg.thresh2P1  = ISPC_SIGMA_THRESH2_P1;
    sigCfg.thresh1P2  = ISPC_SIGMA_THRESH1_P2;
    sigCfg.thresh2P2  = ISPC_SIGMA_THRESH2_P2;
    sigCfg.thresh1P3  = ISPC_SIGMA_THRESH1_P3;
    sigCfg.thresh2P3  = ISPC_SIGMA_THRESH2_P3;

    // Lsc
    p->pLscCfg        = &lscCfg;
    lscCfg.lscWidth  = ISPC_LSC_GAIN_MAP_W;
    lscCfg.lscHeight = ISPC_LSC_GAIN_MAP_H;
    lscCfg.lscStride = lscCfg.lscWidth;
    lscCfg.pLscTable = (uint16_t*)ispcLscMesh;

    // Raw
    p->pRawCfg      = &rawCfg;
    rawCfg.gainR   = ISPC_RAW_GAIN_0;
    rawCfg.gainGr  = ISPC_RAW_GAIN_1;
    rawCfg.gainGb  = ISPC_RAW_GAIN_2;
    rawCfg.gainB   = ISPC_RAW_GAIN_3;

    rawCfg.clampGr = ISPC_RAW_CLAMP_0;
    rawCfg.clampR  = ISPC_RAW_CLAMP_1;
    rawCfg.clampB  = ISPC_RAW_CLAMP_2;
    rawCfg.clampGb = ISPC_RAW_CLAMP_3;

    rawCfg.grgbImbalPlatDark    = ISPC_GRGB_IMBAL_PLAT_DARK;
    rawCfg.grgbImbalDecayDark   = ISPC_GRGB_IMBAL_DECAY_DARK;
    rawCfg.grgbImbalPlatBright  = ISPC_GRGB_IMBAL_PLAT_BRIGHT;
    rawCfg.grgbImbalDecayBright = ISPC_GRGB_IMBAL_DECAY_BRIGHT;

    rawCfg.grgbImbalThr = ISPC_GRGB_IMBAL_THRESHOLD;

    rawCfg.dpcAlphaHotG   = ISPC_BAD_PIX_ALPHA_G_HOT;
    rawCfg.dpcAlphaHotRb  = ISPC_BAD_PIX_ALPHA_RB_HOT;
    rawCfg.dpcAlphaColdG  = ISPC_BAD_PIX_ALPHA_G_COLD;
    rawCfg.dpcAlphaColdRb = ISPC_BAD_PIX_ALPHA_RB_COLD;
    rawCfg.dpcNoiseLevel  = ISPC_BAD_PIX_NOISE_LEVEL;

    // Debayer
    p->pDbyrCfg      = &dbyrCfg;
    dbyrCfg.dewormGradientMul = 0;
    dbyrCfg.dewormSlope       = ISPC_DEMOSAIC_MIX_SLOPE;
    dbyrCfg.dewormOffset      = ISPC_DEMOSAIC_MIX_OFFSET;
    dbyrCfg.lumaWeightR       = ISPC_DEMOSAIC_LUMA_WEIGHT_RED;
    dbyrCfg.lumaWeightG       = ISPC_DEMOSAIC_LUMA_WEIGHT_GREEN;
    dbyrCfg.lumaWeightB       = ISPC_DEMOSAIC_LUMA_WEIGHT_BLUE;

    // Dog-[Ltm]
    p->pLtmCfg     = &ltmCfg;
    ltmCfg.thr     = ISPC_LTM_FILTER_TH1;

    {
        // Convert curve params from float32 to U12F
        int curve, point, regoff;
        regoff = 0;
        for (curve = 0; curve < 8; curve++)
        {
            for (point = 0; point < 16; point += 2)
            {
                int off = curve + point * 8;
                ltmCfg.curves[off  ] = ispcLtmCurves[off]   * 0xfff; // Convert to U12F
                ltmCfg.curves[off+8] = ispcLtmCurves[off+8] * 0xfff; // Convert to U12F
                regoff += 4;
            }
        }
    }

    // [Dog]-Ltm
    p->pDogCfg       = &dogCfg;
    dogCfg.thr      = ISPC_DOG_THR;
    dogCfg.strength = ISPC_DOG_STRENGTH;
    memcpy(dogCfg.coeffs11, dogCoeffs11, sizeof(dogCoeffs11));
    memcpy(dogCfg.coeffs15, dogCoeffs15, sizeof(dogCoeffs15));

    // Luma denoise
    p->pLumaDnsCfg     = &lumaDnsCfg;
    lumaDnsCfg.bitpos  = ISPC_LUMA_DNS_BITPOS;
    lumaDnsCfg.alpha   = ISPC_LUMA_DNS_ALPHA;
    lumaDnsCfg.f2      = ISPC_LUMA_DNS_F2;
    memcpy(lumaDnsCfg.lut, ispcLumaDnsLut, sizeof(ispcLumaDnsLut));

    p->pLumaDnsRefCfg     = &lumaDnsRefCfg;
    lumaDnsRefCfg.shift   = ISPC_LUMA_DNS_REF_SHIFT;
    memcpy(lumaDnsRefCfg.lutGamma0_32  , ispcGammaLut0_32  , sizeof(ispcGammaLut0_32  ));
    memcpy(lumaDnsRefCfg.lutGamma32_255, ispcGammaLut32_255, sizeof(ispcGammaLut32_255));
    memcpy(lumaDnsRefCfg.lutDist       , ispcYDnsDistLut   , sizeof(ispcYDnsDistLut   ));

    // Luma Sharpen
    p->pSharpCfg = &sharpCfg;
    sharpCfg.strengthLighten = 0x0000;//fp16
    sharpCfg.strengthDarken  = 0x0000;//fp16
    sharpCfg.minThr          = f32Tof16(ISPC_SHARP_MIN_THR);
    sharpCfg.alpha           = 0x0000;//fp16
    sharpCfg.undershoot      = 0x0000;//fp16
    sharpCfg.overshoot       = 0x4000;//fp16

    sharpCfg.rangeStop0 = sharpCfg.rangeStop1 =
    sharpCfg.rangeStop2 = sharpCfg.rangeStop3 = 0x0000;//fp16

    sharpCfg.sharpenCoeffs[0] = sharpCfg.sharpenCoeffs[1] =
    sharpCfg.sharpenCoeffs[2] = sharpCfg.sharpenCoeffs[3] = 0x0000; //fp16

    // GenChroma
    p->pChrGenCfg = &cGenCfg;
    cGenCfg.desatOffset = ISPC_CGEN_DESAT_OFFSET;
    cGenCfg.desatSlope  = ISPC_CGEN_DESAT_SLOPE;
    cGenCfg.pfrStrength = ISPC_CGEN_PFR_STRENGTH;

    cGenCfg.lumaCoeffR  = ISPC_CGEN_LUMA_COEFF_R;
    cGenCfg.lumaCoeffG  = ISPC_CGEN_LUMA_COEFF_G;
    cGenCfg.lumaCoeffB  = ISPC_CGEN_LUMA_COEFF_B;

    cGenCfg.epsilon     = ISPC_CGEN_EPSILON;
    cGenCfg.kr          = ISPC_CGEN_KR;
    cGenCfg.kg          = ISPC_CGEN_KG;
    cGenCfg.kb          = ISPC_CGEN_KB;

    // Median
    p->pMedCfg = &medCfg;
    medCfg.kernelSize = ISPC_CHROMA_MEDIAN_SIZE;
    medCfg.slope      = ISPC_CHROMA_MEDIAN_MIX_SLOPE;
    medCfg.offset     = ISPC_CHROMA_MEDIAN_MIX_OFFSET;

    // ChromaDns
    p->pChromaDnsCfg = &chrDnsCfg;

    chrDnsCfg.hEnab           = ISPC_CHROMA_DNS_H_ENAB;
    chrDnsCfg.limit           = ISPC_CHROMA_DNS_LIMIT;
    chrDnsCfg.greyDesatOffset = ISPC_GREY_DESAT_OFFSET;
    chrDnsCfg.greyDesatSlope  = ISPC_GREY_DESAT_SLOPE;

    chrDnsCfg.th_r = ISPC_CHROMA_DNS_TH_R;
    chrDnsCfg.th_g = ISPC_CHROMA_DNS_TH_G;
    chrDnsCfg.th_b = ISPC_CHROMA_DNS_TH_B;

    chrDnsCfg.greyCb = ISPC_GREY_POINT_B;
    chrDnsCfg.greyCg = ISPC_GREY_POINT_G;
    chrDnsCfg.greyCr = ISPC_GREY_POINT_R;

    chrDnsCfg.convCoeffCorner = ispcLowpassKernel[0]; // [0] [1] [2]
    chrDnsCfg.convCoeffEdge   = ispcLowpassKernel[1]; // [3] [4] [5]
    chrDnsCfg.convCoeffCenter = ispcLowpassKernel[4]; // [6] [7] [8]

    // Col Combine
    p->pColCombCfg = &ccCfg;
    ccCfg.kr = ISPC_CC_KR;
    ccCfg.kg = ISPC_CC_KG;
    ccCfg.kb = ISPC_CC_KB;
    memcpy(ccCfg.ccm   , ispcCCM   , sizeof(ispcCCM   ));
    memcpy(ccCfg.ccmOff, ispcCCMOff, sizeof(ispcCCMOff));

    // Lut
    p->pLutCfg        = &lutCfg;
    lutCfg.table      = gammaLutFp16;
    lutCfg.size       = 512;        //debug only really...
    lutCfg.rgnSize[0] = 0x55555555; //2^5 = 32, 32x8x2 = 512
    lutCfg.rgnSize[1] = 0x55555555;
    p->pColConvCfg    = &colConvCfg;
    memcpy(colConvCfg.mat   , ispcCSC   , sizeof(ispcCSC   ));
    memcpy(colConvCfg.offset, ispcCSCOff, sizeof(ispcCSCOff));

    //Luma-plane resize
    p->pUpfirdn0Cfg  = &updnCfg0;
    updnCfg0.vN      = N; updnCfg0.vD = D;
    updnCfg0.hN      = N; updnCfg0.hD = D;
    updnCfg0.kerSz   = 7;
    updnCfg0.vCoefs  = vCoefs;
    updnCfg0.hCoefs  = hCoefs;

    //U/V resize as for 420 (same factors as Y, but double Denominator)
    p->pUpfirdn12Cfg  = &updnCfg12;
    updnCfg12.vN      = N; updnCfg12.vD = (D<<1);
    updnCfg12.hN      = N; updnCfg12.hD = (D<<1);
    updnCfg12.kerSz   = 7;
    updnCfg12.vCoefs  = vCoefs;
    updnCfg12.hCoefs  = hCoefs;

    //Default CFG words:
    OpipeDefaultCfg(p, SIPP_SIGMA_ID  );
    OpipeDefaultCfg(p, SIPP_RAW_ID    );
    OpipeDefaultCfg(p, SIPP_DBYR_ID   );
    OpipeDefaultCfg(p, SIPP_DOGL_ID   );
    OpipeDefaultCfg(p, SIPP_SHARPEN_ID);
    OpipeDefaultCfg(p, SIPP_CGEN_ID   );
    OpipeDefaultCfg(p, SIPP_MED_ID    );
    OpipeDefaultCfg(p, SIPP_CHROMA_ID );
    OpipeDefaultCfg(p, SIPP_CC_ID     );
    OpipeDefaultCfg(p, SIPP_LUT_ID    );
}
