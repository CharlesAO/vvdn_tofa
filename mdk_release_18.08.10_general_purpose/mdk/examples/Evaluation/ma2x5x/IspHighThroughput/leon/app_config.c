///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon file
///

// 1: Includes
// ----------------------------------------------------------------------------
#include "app_config.h"
#include <stdio.h>
#include <string.h> //memcpy
#include <Fp16Convert.h>
#include <DrvCpr.h>
#include <DrvDdr.h>
#include <DrvTempSensor.h>
#include <DrvLeonL2C.h>
#include "isp_params_wrap.h"
#include "swcLeonUtils.h"
#include "UnitTestApi.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define MSS_CLOCKS_APP   (DEV_MSS_APB_SLV     |     \
                          DEV_MSS_APB2_CTRL   |     \
                          DEV_MSS_RTBRIDGE    |     \
                          DEV_MSS_RTAHB_CTRL  |     \
                          DEV_MSS_LRT         |     \
                          DEV_MSS_LRT_DSU     |     \
                          DEV_MSS_LRT_L2C     |     \
                          DEV_MSS_LRT_ICB     |     \
                          DEV_MSS_AXI_BRIDGE  |     \
                          DEV_MSS_MXI_CTRL    |     \
                          DEV_MSS_MXI_DEFSLV  |     \
                          DEV_MSS_AXI_MON     |     \
                          DEV_MSS_AMC         |     \
                          DEV_MSS_SIPP        |     \
                          DEV_MSS_TIM         )

#define OPIPE_CLOCKS (DEV_SIPP_SIGMA          |   \
                      DEV_SIPP_LSC            |   \
                      DEV_SIPP_RAW            |   \
                      DEV_SIPP_DBYR           |   \
                      DEV_SIPP_DOGL           |   \
                      DEV_SIPP_LUMA           |   \
                      DEV_SIPP_SHARPEN        |   \
                      DEV_SIPP_CGEN           |   \
                      DEV_SIPP_MED            |   \
                      DEV_SIPP_CHROMA         |   \
                      DEV_SIPP_CC             |   \
                      DEV_SIPP_LUT            |   \
                      DEV_SIPP_UPFIRDN0       |   \
                      DEV_SIPP_UPFIRDN1       |   \
                      DEV_SIPP_UPFIRDN2       |   \
                      DEV_SIPP_MIPI_RX0       |   \
                      DEV_MSS_SIPP_CLK_GATES  |   \
                      DEV_SIPP_MIPI           |   \
                      DEV_SIPP_SIPP_ABPSLV    |   \
                      DEV_SIPP_APB_SLV)

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

// 4: Static Local Data
// ----------------------------------------------------------------------------
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

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
void appSetParams(Opipe *p)
{
  //Global stuff
    p->rawBits      = ISPC_BAYER_BITS;
    p->bayerPattern = BAYER_PATTERN;

  //Filter specific

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   //BlackLevelCorrection
    p->pBlcCfg  = &blcCfg;
    blcCfg.r    = ISPC_BLACK_LEVEL_P0;
    blcCfg.b    = ISPC_BLACK_LEVEL_P1;
    blcCfg.gr   = ISPC_BLACK_LEVEL_P2;
    blcCfg.gb   = ISPC_BLACK_LEVEL_P3;

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   //Sigma
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

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   //Lsc
    p->pLscCfg        = &lscCfg;
    lscCfg.lscWidth  = ISPC_LSC_GAIN_MAP_W;
    lscCfg.lscStride = lscCfg.lscWidth;
    lscCfg.lscHeight = ISPC_LSC_GAIN_MAP_H;
    lscCfg.pLscTable = (uint16_t*)ispcLscMesh;

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   //Raw
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

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   //Debayer
    p->pDbyrCfg      = &dbyrCfg;
    dbyrCfg.dewormGradientMul = 0;
    dbyrCfg.dewormSlope       = ISPC_DEMOSAIC_MIX_SLOPE;
    dbyrCfg.dewormOffset      = ISPC_DEMOSAIC_MIX_OFFSET;
    dbyrCfg.lumaWeightR       = ISPC_DEMOSAIC_LUMA_WEIGHT_RED;
    dbyrCfg.lumaWeightG       = ISPC_DEMOSAIC_LUMA_WEIGHT_GREEN;
    dbyrCfg.lumaWeightB       = ISPC_DEMOSAIC_LUMA_WEIGHT_BLUE;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Dog-[Ltm]
    p->pLtmCfg     = &ltmCfg;
    ltmCfg.thr     = ISPC_LTM_FILTER_TH1;

    {//memcopy basically...
       int curve, point, regoff;
       regoff = 0;
       for (curve = 0; curve < 8; curve++) {
           for (point = 0; point < 16; point += 2) {
               int off = curve + point * 8;
               ltmCfg.curves[off  ] = ispcLtmCurves[off]  ;
               ltmCfg.curves[off+8] = ispcLtmCurves[off+8];
               regoff += 4;
           }
        }
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //[Dog]-Ltm
    p->pDogCfg       = &dogCfg;
    dogCfg.thr      = ISPC_DOG_THR;
    dogCfg.strength = ISPC_DOG_STRENGTH;
    memcpy(dogCfg.coeffs11, dogCoeffs11, sizeof(dogCoeffs11));
    memcpy(dogCfg.coeffs15, dogCoeffs15, sizeof(dogCoeffs15));

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Luma denoise
     p->pLumaDnsCfg    = &lumaDnsCfg;
     lumaDnsCfg.bitpos = ISPC_LUMA_DNS_BITPOS;
     lumaDnsCfg.alpha  = ISPC_LUMA_DNS_ALPHA;
     lumaDnsCfg.f2     = ISPC_LUMA_DNS_F2;
     memcpy(lumaDnsCfg.lut, ispcLumaDnsLut, sizeof(ispcLumaDnsLut));
     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     p->pLumaDnsRefCfg     = &lumaDnsRefCfg;
     lumaDnsRefCfg.shift   = ISPC_LUMA_DNS_REF_SHIFT;
     memcpy(lumaDnsRefCfg.lutGamma0_32  , ispcGammaLut0_32  , sizeof(ispcGammaLut0_32  ));
     memcpy(lumaDnsRefCfg.lutGamma32_255, ispcGammaLut32_255, sizeof(ispcGammaLut32_255));
     memcpy(lumaDnsRefCfg.lutDist       , ispcYDnsDistLut   , sizeof(ispcYDnsDistLut   ));

     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     //Luma Sharpen
     p->pSharpCfg = &sharpCfg;
     sharpCfg.strengthLighten = ISPC_SHARP_STRENGTH_LIGHTEN;//fp16
     sharpCfg.strengthDarken  = ISPC_SHARP_STRENGTH_DARKEN; //fp16
     sharpCfg.minThr          = ISPC_SHARP_MIN_THR;
     sharpCfg.alpha           = ISPC_SHARP_ALPHA;     //fp16
     sharpCfg.undershoot      = ISPC_SHARP_UNDERSHOOT;//fp16
     sharpCfg.overshoot       = ISPC_SHARP_OVERSHOOT; //fp16

     sharpCfg.rangeStop0 = ISPC_SHARP_RANGE_STOP_0;
     sharpCfg.rangeStop1 = ISPC_SHARP_RANGE_STOP_1;
     sharpCfg.rangeStop2 = ISPC_SHARP_RANGE_STOP_2;
     sharpCfg.rangeStop3 = ISPC_SHARP_RANGE_STOP_3;//fp16

     sharpCfg.sharpenCoeffs[0] = ISPC_SHARP_COEF0;
     sharpCfg.sharpenCoeffs[1] = ISPC_SHARP_COEF1;
     sharpCfg.sharpenCoeffs[2] = ISPC_SHARP_COEF2;
     sharpCfg.sharpenCoeffs[3] = ISPC_SHARP_COEF3; //fp16

     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     //GenChroma
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

     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     //Median
     p->pMedCfg = &medCfg;
     medCfg.kernelSize = ISPC_CHROMA_MEDIAN_SIZE;
     medCfg.slope      = ISPC_CHROMA_MEDIAN_MIX_SLOPE;
     medCfg.offset     = ISPC_CHROMA_MEDIAN_MIX_OFFSET;

     //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
     //ChromaDns
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

     chrDnsCfg.convCoeffCorner = 28;
     chrDnsCfg.convCoeffEdge   = 28;
     chrDnsCfg.convCoeffCenter = 31;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Col Combine
     p->pColCombCfg = &ccCfg;
     ccCfg.kr = ISPC_CC_KR;
     ccCfg.kg = ISPC_CC_KG;
     ccCfg.kb = ISPC_CC_KB;
     memcpy(ccCfg.ccm   , ispcCCM   , sizeof(ispcCCM   ));
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
     updnCfg0.vN      = N; updnCfg0.vD = D;
     updnCfg0.hN      = N; updnCfg0.hD = D;
     updnCfg0.kerSz   = 5;
     updnCfg0.vCoefs  = vCoefs;
     updnCfg0.hCoefs  = hCoefs;

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //U/V resize as for 420 (same factors as Y, but double Denominator)
     p->pUpfirdn12Cfg = &updnCfg12;
     updnCfg12.vN      = N; updnCfg12.vD = (D<<1);
     updnCfg12.hN      = N; updnCfg12.hD = (D<<1);
     updnCfg12.kerSz   = 5;
     updnCfg12.vCoefs  = vCoefs;
     updnCfg12.hCoefs  = hCoefs;

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

void reduceBaselinePower()
{
    s32 i;

    // Disable all the SHAVE and USB ISLANDS
    for (i=0;i<12;i++)
        DrvCprPowerTurnOffIsland(POWER_ISLAND_SHAVE_0 + i);
    DrvCprPowerTurnOffIsland(POWER_ISLAND_USB);

    u32 upaClocksToTurnOff = ~(0
            | DEV_UPA_CDMA
            | DEV_UPA_CTRL
            | (1 << UPA_MTX)
            );
    u32 auxClocksToTurnOff = ~(0
            | AUX_CLK_MASK_DDR_REF
            | AUX_CLK_MASK_DDR_CORE_CTRL
            | AUX_CLK_MASK_DDR_CORE_PHY
            | AUX_CLK_MASK_UART
            | (1<< CSS_AUX_TSENS)
            );
    u32 mssClocksToTurnOff = 0
            | DEV_MSS_AXI_BRIDGE
            | DEV_MSS_MXI_DEFSLV
            | DEV_MSS_AXI_MON
            | DEV_MSS_NAL
            | DEV_MSS_CIF0
            | DEV_MSS_CIF1
            | DEV_MSS_LCD
            ;
    u64 cssClocksToTurnOff = ~(0
            | DEV_CSS_AON
            | DEV_CSS_DSS_BUS
            | DEV_CSS_DSS_BUS_AAXI
            | DEV_CSS_LAHB2SHB
            | DEV_CSS_SAHB2MAHB
            | DEV_CSS_LAHB_CTRL
            | DEV_CSS_APB4_CTRL
            | DEV_CSS_MAHB_CTRL
            | DEV_CSS_APB1_CTRL
            | DEV_CSS_APB3_CTRL
            | DEV_CSS_SAHB_CTRL
            | DEV_CSS_UART
            | DEV_CSS_JTAG
            | DEV_CSS_LOS_L2C
            | DEV_CSS_MSS_MAS
            | DEV_CSS_UPA_MAS
            | DEV_CSS_I2C2
            | DEV_CSS_GPIO
          );

    // Turn off unneeded clocks
    DrvCprSysDeviceAction(UPA_DOMAIN,     DISABLE_CLKS, upaClocksToTurnOff);
    DrvCprSysDeviceAction(UPA_DOMAIN,     ASSERT_RESET, upaClocksToTurnOff);
    DrvCprSysDeviceAction(MSS_DOMAIN,     DISABLE_CLKS, mssClocksToTurnOff);
    DrvCprSysDeviceAction(MSS_DOMAIN,     ASSERT_RESET, mssClocksToTurnOff);
    DrvCprSysDeviceAction(CSS_AUX_DOMAIN, DISABLE_CLKS, auxClocksToTurnOff);
    DrvCprSysDeviceAction(CSS_AUX_DOMAIN, ASSERT_RESET, auxClocksToTurnOff);
    DrvCprSysDeviceAction(CSS_DOMAIN,     DISABLE_CLKS, cssClocksToTurnOff);
}


s32 initClocksAndMemory(void)
{
    s32 sc;
    DrvTempSensConfig tempParam = {1};

    tyAuxClkDividerCfg auxClkCfg[] =
    {    {
        .auxClockEnableMask = (u32)(1 << CSS_AUX_TSENS),
        .auxClockSource = CLK_SRC_REFCLK0,
        .auxClockDivNumerator = 1,
        .auxClockDivDenominator = 10,
    },
        {AUX_CLK_MASK_UART, CLK_SRC_REFCLK0, 96, 625},   // Give the UART an SCLK that allows it to generate an output baud rate of of 115200 Hz (the uart divides by 16)
        {0,0,0,0}, // Null Terminated List
    };

    tySocClockConfig clockConfig =
    {
        .refClk0InputKhz         = 12000,           // Default 12Mhz input clock
        .refClk1InputKhz         = 0,               // Assume no secondary oscillator for now
        .targetPll0FreqKhz       = 600000,
        .targetPll1FreqKhz       = 0,
        .clkSrcPll1              = CLK_SRC_REFCLK0, // Supply both PLLS from REFCLK0
        .masterClkDivNumerator   = 1,
        .masterClkDivDenominator = 1,
        .cssDssClockEnableMask   = DEFAULT_CORE_CSS_DSS_CLOCKS,
        .mssClockEnableMask      = MSS_CLOCKS_APP,             // Basic clocks to allow LRT and busses to function
        .sippClockEnableMask     = OPIPE_CLOCKS,               // Opipe clocks
        .upaClockEnableMask      = DEFAULT_UPA_CLOCKS,         // Enable all Shave clocks, CMXDMA, Shave L2Cache and UPA Control interfaces by default
        .pAuxClkCfg              = auxClkCfg,
    };

    swcLeonSetPIL(0);
    DrvLL2CInitWriteThrough ();

    sc = DrvCprInit();
    if(sc)
        return sc;
    sc = DrvCprSetupClocks(&clockConfig);
    if(sc)
        return sc;
    DrvDdrInitialise(NULL);
    reduceBaselinePower();   // Disable unneeded clocks and power islands

    DrvTempSensorInitialise(&tempParam);

    return 0;
}
