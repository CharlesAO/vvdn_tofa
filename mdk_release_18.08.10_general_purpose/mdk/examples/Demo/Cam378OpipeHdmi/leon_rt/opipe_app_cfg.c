#include <stdio.h>
#include <string.h> //memcpy
#include <Fp16Convert.h>
#include "isp_params_wrap.h"
#include "Opipe.h"
#include "OpipeApps.h"

//#################################################################################################
static SigmaDnsCfg   sigCfg;
static BlcCfg        blcCfg;
static LscCfg        lscCfg;
static RawCfg        rawCfg;
static DbyrCfg       dbyrCfg;
static LtmCfg        ltmCfg;
static DogCfg        dogCfg;
static LumaDnsCfg    lumaDnsCfg;
static LumaDnsRefCfg lumaDnsRefCfg;
static SharpenCfg    sharpCfg;
static ChromaGenCfg  cGenCfg;
static MedianCfg     medCfg;
static ChromaDnsCfg  chrDnsCfg;
static ColCombCfg    ccCfg;
static LutCfg        lutCfg;
static ColConvCfg    colConvCfg;
static UpfirdnCfg    updnCfg0;
static UpfirdnCfg    updnCfg12;

void appSetParamsMFPipe(Opipe *p)
{
  //Global stuff
    p->rawBits      = ISPC_BAYER_BITS;
    p->bayerPattern = BAYER_PATTERN;
  
  //Filter specific
   
   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   //BlackLevelCorrection
    p->pBlcCfg   = &blcCfg;
    blcCfg.r    = ISPC_D65_A_TL84_BLACK_LEVEL_P0;
    blcCfg.b    = ISPC_D65_A_TL84_BLACK_LEVEL_P1;
    blcCfg.gr   = ISPC_D65_A_TL84_BLACK_LEVEL_P2;
    blcCfg.gb   = ISPC_D65_A_TL84_BLACK_LEVEL_P3;

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   //Sigma
    p->pSigmaCfg       = &sigCfg;
    sigCfg.noiseFloor = ISPC_D65_A_TL84_SIGMA_NOISE_FLOOR;
    sigCfg.thresh1P0  = ISPC_D65_A_TL84_SIGMA_THRESH1_P0;
    sigCfg.thresh2P0  = ISPC_D65_A_TL84_SIGMA_THRESH2_P0;
    sigCfg.thresh1P1  = ISPC_D65_A_TL84_SIGMA_THRESH1_P1;
    sigCfg.thresh2P1  = ISPC_D65_A_TL84_SIGMA_THRESH2_P1;
    sigCfg.thresh1P2  = ISPC_D65_A_TL84_SIGMA_THRESH1_P2;
    sigCfg.thresh2P2  = ISPC_D65_A_TL84_SIGMA_THRESH2_P2;
    sigCfg.thresh1P3  = ISPC_D65_A_TL84_SIGMA_THRESH1_P3;
    sigCfg.thresh2P3  = ISPC_D65_A_TL84_SIGMA_THRESH2_P3;

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   //Lsc
    p->pLscCfg       = &lscCfg;
    lscCfg.lscWidth  = ISPC_D65_A_TL84_LSC_GAIN_MAP_W;
    lscCfg.lscStride = ISPC_D65_A_TL84_LSC_GAIN_MAP_STRIDE;
    lscCfg.lscHeight = ISPC_D65_A_TL84_LSC_GAIN_MAP_H;
    lscCfg.pLscTable = (uint16_t*)ispcTL84LscMesh; // ispcD65LscMesh or ispcALscMesh

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   //Raw
    p->pRawCfg      = &rawCfg;
    rawCfg.gainR   = ISPC_TL84_RAW_GAIN_0; // ISPC_D65_RAW_GAIN_0 or ISPC_A_RAW_GAIN_0
    rawCfg.gainGr  = ISPC_TL84_RAW_GAIN_1; // ISPC_D65_RAW_GAIN_1 or ISPC_A_RAW_GAIN_1
    rawCfg.gainGb  = ISPC_TL84_RAW_GAIN_2; // ISPC_D65_RAW_GAIN_2 or ISPC_A_RAW_GAIN_2
    rawCfg.gainB   = ISPC_TL84_RAW_GAIN_3; // ISPC_D65_RAW_GAIN_3 or ISPC_A_RAW_GAIN_3

    rawCfg.clampGr = ISPC_D65_A_TL84_RAW_CLAMP_0;
    rawCfg.clampR  = ISPC_D65_A_TL84_RAW_CLAMP_1;
    rawCfg.clampB  = ISPC_D65_A_TL84_RAW_CLAMP_2;
    rawCfg.clampGb = ISPC_D65_A_TL84_RAW_CLAMP_3;

    rawCfg.grgbImbalPlatDark    = ISPC_D65_A_TL84_GRGB_IMBAL_PLAT_DARK;
    rawCfg.grgbImbalDecayDark   = ISPC_D65_A_TL84_GRGB_IMBAL_DECAY_DARK;
    rawCfg.grgbImbalPlatBright  = ISPC_D65_A_TL84_GRGB_IMBAL_PLAT_BRIGHT;
    rawCfg.grgbImbalDecayBright = ISPC_D65_A_TL84_GRGB_IMBAL_DECAY_BRIGHT;

    rawCfg.grgbImbalThr = ISPC_D65_A_TL84_GRGB_IMBAL_THRESHOLD;

    rawCfg.dpcAlphaHotG   = ISPC_D65_A_TL84_BAD_PIX_ALPHA_G_HOT;
    rawCfg.dpcAlphaHotRb  = ISPC_D65_A_TL84_BAD_PIX_ALPHA_RB_HOT;
    rawCfg.dpcAlphaColdG  = ISPC_D65_A_TL84_BAD_PIX_ALPHA_G_COLD;
    rawCfg.dpcAlphaColdRb = ISPC_D65_A_TL84_BAD_PIX_ALPHA_RB_COLD;
    rawCfg.dpcNoiseLevel  = ISPC_D65_A_TL84_BAD_PIX_NOISE_LEVEL;

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   //Debayer
    p->pDbyrCfg      = &dbyrCfg;
    dbyrCfg.dewormGradientMul = ISPC_D65_A_TL84_DEMOSAIC_MIX_GRADIENT_MUL;
    dbyrCfg.dewormSlope       = ISPC_D65_A_TL84_DEMOSAIC_MIX_SLOPE;
    dbyrCfg.dewormOffset      = ISPC_D65_A_TL84_DEMOSAIC_MIX_OFFSET;
    dbyrCfg.lumaWeightR       = ISPC_D65_A_TL84_DEMOSAIC_LUMA_WEIGHT_RED;
    dbyrCfg.lumaWeightG       = ISPC_D65_A_TL84_DEMOSAIC_LUMA_WEIGHT_GREEN;
    dbyrCfg.lumaWeightB       = ISPC_D65_A_TL84_DEMOSAIC_LUMA_WEIGHT_BLUE;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Dog-[Ltm]
    p->pLtmCfg      = &ltmCfg;
    ltmCfg.thr     = ISPC_D65_A_TL84_LTM_FILTER_TH1;

    {//Convert curve params from float32 to U12F
       int curve, point, regoff;
       regoff = 0;
       for (curve = 0; curve < 8; curve++) {
           for (point = 0; point < 16; point += 2) {
               int off = curve + point * 8;
               ltmCfg.curves[off  ] = ispcLtmCurves[off]   * 0xfff; // Convert to U12F
               ltmCfg.curves[off+8] = ispcLtmCurves[off+8] * 0xfff; // Convert to U12F
               regoff += 4;
           }
        }
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //[Dog]-Ltm
    p->pDogCfg       = &dogCfg;
    dogCfg.thr      = ISPC_D65_A_TL84_DOG_THR;
    dogCfg.strength = ISPC_D65_A_TL84_DOG_STRENGTH;
    dogCfg.sigma11  = ISPC_D65_A_TL84_DOG_SIGMA11;
    dogCfg.sigma15  = ISPC_D65_A_TL84_DOG_SIGMA15;
    memcpy(dogCfg.coeffs11, dogCoeffs11, sizeof(dogCoeffs11));
    memcpy(dogCfg.coeffs15, dogCoeffs15, sizeof(dogCoeffs15));

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Luma denoise
     p->pLumaDnsCfg     = &lumaDnsCfg;
     lumaDnsCfg.bitpos = ISPC_D65_A_TL84_LUMA_DNS_BITPOS;
     lumaDnsCfg.alpha  = ISPC_D65_A_TL84_LUMA_DNS_ALPHA;
     lumaDnsCfg.f2     = ISPC_D65_A_TL84_LUMA_DNS_F2;
     lumaDnsCfg.strength = ISPC_D65_A_TL84_LUMA_DNS_STRENGTH;
     memcpy(lumaDnsCfg.lut, ispcLumaDnsLut, sizeof(ispcLumaDnsLut));
     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     p->pLumaDnsRefCfg      = &lumaDnsRefCfg;
     lumaDnsRefCfg.shift   = ISPC_D65_A_TL84_LUMA_DNS_REF_SHIFT;
     lumaDnsRefCfg.gamma   = ISPC_D65_A_TL84_LUMA_DNS_GAMMA;
	 lumaDnsRefCfg.angle_of_view = ISPC_D65_A_TL84_LUMA_DNS_ANGLE_OF_VIEW;
     memcpy(lumaDnsRefCfg.lutGamma0_32  , ispcGammaLut0_32  , sizeof(ispcGammaLut0_32  ));
     memcpy(lumaDnsRefCfg.lutGamma32_255, ispcGammaLut32_255, sizeof(ispcGammaLut32_255));
     memcpy(lumaDnsRefCfg.lutDist       , ispcYDnsDistLut   , sizeof(ispcYDnsDistLut   ));

     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     //Luma Sharpen
     p->pSharpCfg = &sharpCfg;
     sharpCfg.sigma = ISPC_D65_A_TL84_SHARP_SIGMA;
     sharpCfg.strengthLighten = ISPC_D65_A_TL84_SHARP_STRENGTH_LIGHTEN;//fp16
     sharpCfg.strengthDarken  = ISPC_D65_A_TL84_SHARP_STRENGTH_DARKEN;//fp16
     sharpCfg.minThr          = f32Tof16(ISPC_D65_A_TL84_SHARP_MIN_THR);
     sharpCfg.alpha           = ISPC_D65_A_TL84_SHARP_ALPHA;//fp16
     sharpCfg.undershoot      = ISPC_D65_A_TL84_SHARP_UNDERSHOOT;//fp16
     sharpCfg.overshoot       = ISPC_D65_A_TL84_SHARP_OVERSHOOT;//fp16

     sharpCfg.rangeStop0 =  ISPC_D65_A_TL84_SHARP_RANGE_STOP_0;
     sharpCfg.rangeStop1 =  ISPC_D65_A_TL84_SHARP_RANGE_STOP_1;
     sharpCfg.rangeStop2 =  ISPC_D65_A_TL84_SHARP_RANGE_STOP_2;
     sharpCfg.rangeStop3 =  ISPC_D65_A_TL84_SHARP_RANGE_STOP_3;//fp16

     sharpCfg.sharpenCoeffs[0] = ISPC_D65_A_TL84_SHARP_COEF0;
     sharpCfg.sharpenCoeffs[1] = ISPC_D65_A_TL84_SHARP_COEF1;
     sharpCfg.sharpenCoeffs[2] = ISPC_D65_A_TL84_SHARP_COEF2;
     sharpCfg.sharpenCoeffs[3] = ISPC_D65_A_TL84_SHARP_COEF3; //fp16

     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     //GenChroma
     p->pChrGenCfg = &cGenCfg;
     cGenCfg.desatOffset = ISPC_D65_A_TL84_CGEN_DESAT_OFFSET;
     cGenCfg.desatSlope  = ISPC_D65_A_TL84_CGEN_DESAT_SLOPE;
     cGenCfg.pfrStrength = ISPC_D65_A_TL84_CGEN_PFR_STRENGTH;

     cGenCfg.lumaCoeffR  = ISPC_D65_A_TL84_CGEN_LUMA_COEFF_R;
     cGenCfg.lumaCoeffG  = ISPC_D65_A_TL84_CGEN_LUMA_COEFF_G;
     cGenCfg.lumaCoeffB  = ISPC_D65_A_TL84_CGEN_LUMA_COEFF_B;

     cGenCfg.epsilon     = ISPC_D65_A_TL84_CGEN_EPSILON;
     cGenCfg.kr          = ISPC_D65_A_TL84_CGEN_KR;
     cGenCfg.kg          = ISPC_D65_A_TL84_CGEN_KG;
     cGenCfg.kb          = ISPC_D65_A_TL84_CGEN_KB;

     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     //Median
     p->pMedCfg = &medCfg;
     medCfg.kernelSize = ISPC_D65_A_TL84_CHROMA_MEDIAN_SIZE;
     medCfg.slope      = ISPC_D65_A_TL84_CHROMA_MEDIAN_MIX_SLOPE;
     medCfg.offset     = ISPC_D65_A_TL84_CHROMA_MEDIAN_MIX_OFFSET;

     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     //ChromaDns
     p->pChromaDnsCfg = &chrDnsCfg;

     chrDnsCfg.hEnab           = ISPC_D65_A_TL84_CHROMA_DNS_H_ENAB;
     chrDnsCfg.limit           = ISPC_D65_A_TL84_CHROMA_DNS_LIMIT;
     chrDnsCfg.greyDesatOffset = ISPC_D65_A_TL84_GREY_DESAT_OFFSET;
     chrDnsCfg.greyDesatSlope  = ISPC_D65_A_TL84_GREY_DESAT_SLOPE;

     chrDnsCfg.th_r = ISPC_D65_A_TL84_CHROMA_DNS_TH_R;
     chrDnsCfg.th_g = ISPC_D65_A_TL84_CHROMA_DNS_TH_G;
     chrDnsCfg.th_b = ISPC_D65_A_TL84_CHROMA_DNS_TH_B;

     chrDnsCfg.greyCb = ISPC_D65_A_TL84_GREY_POINT_B;
     chrDnsCfg.greyCg = ISPC_D65_A_TL84_GREY_POINT_G;
     chrDnsCfg.greyCr = ISPC_D65_A_TL84_GREY_POINT_R;

     chrDnsCfg.convCoeffCorner = ispcLowpassKernel[0]; // [0] [1] [2]
     chrDnsCfg.convCoeffEdge   = ispcLowpassKernel[1]; // [3] [4] [5]
     chrDnsCfg.convCoeffCenter = ispcLowpassKernel[4]; // [6] [7] [8]

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Col Combine
     p->pColCombCfg = &ccCfg;
     ccCfg.kr = ISPC_D65_A_TL84_CC_KR;
     ccCfg.kg = ISPC_D65_A_TL84_CC_KG;
     ccCfg.kb = ISPC_D65_A_TL84_CC_KB;
     memcpy(ccCfg.ccm   , ispc_TL84_CCM   , sizeof(ispc_TL84_CCM  )); //ispc_D65_CCM or ipsc_A_CCM
     memcpy(ccCfg.ccmOff, ispcCCMOff, sizeof(ispcCCMOff));

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Lut
     p->pLutCfg        = &lutCfg;
     lutCfg.table      = gammaLutFp16;
     lutCfg.size       = 512;        //debug only really...
     lutCfg.rgnSize[0] = 0x55555555; //2^5 = 32, 32x8x2 = 512
     lutCfg.rgnSize[1] = 0x55555555;

     p->pColConvCfg   = &colConvCfg;
     memcpy(colConvCfg.mat   , ispcCSC   , sizeof(ispcCSC   ));
     memcpy(colConvCfg.offset, ispcCSCOff, sizeof(ispcCSCOff));

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //Luma-plane resize
     p->pUpfirdn0Cfg = &updnCfg0;
     updnCfg0.vN        = N;
     updnCfg0.vD        = D;
     updnCfg0.hN        = N;
     updnCfg0.hD        = D;
     updnCfg0.kerSz     = 7;
     updnCfg0.vCoefs    = vCoefs;
     updnCfg0.hCoefs    = hCoefs;

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //U/V resize as for 420 (same factors as Y, but double Denominator)
     p->pUpfirdn12Cfg   = &updnCfg12;
     updnCfg12.vN       = N;
     updnCfg12.vD       = (D << 1);
     updnCfg12.hN       = N;
     updnCfg12.hD       = (D << 1);
     updnCfg12.kerSz    = 7;
     updnCfg12.vCoefs   = vCoefs;
     updnCfg12.hCoefs   = hCoefs;

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

