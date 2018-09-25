///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application to test running SIPP pipelines concurrently
///            in direct streaming runtime

#include <sipp.h>
#include <sippTestCommon.h>
#include <include/sippHwDefs.h>
#include <arch/ma2x5x/include/sippHwDefs_ma2x5x.h>
#if defined(SIPP_VCS)
#include <UnitTestApi.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#endif

#include "ISPPipeSettings.h"

/////////////////////////////////////////////////////
// Test specific macros
/////////////////////////////////////////////////////

#define SIPP_NUM_PIPELINES           2
#define SIPP_TEST_NUM_FRAMES         3
#define CC_APP_EVENT_HISTORY_SIZE    8

// Pipeline 1 Setup
#define SIPP_ISP_PIPE_BUFFER_WIDTH   512
#define SIPP_ISP_PIPE_BUFFER_HEIGHT  128
#define ISPC_BAYER_BITS              10
#define BAYER_PATTERN                RGGB
#define FORMAT                       BAYER
#define ISPC_CHROMA_PIPE_WIDTH      (SIPP_ISP_PIPE_BUFFER_WIDTH >> 0x1)
#define ISPC_CHROMA_PIPE_HEIGHT     (SIPP_ISP_PIPE_BUFFER_HEIGHT >> 0x1)

#define POLY_Y_IMAGE_WIDTH          ((((SIPP_ISP_PIPE_BUFFER_WIDTH * POLYFIR_Y_H_NUM) - 1) / POLYFIR_Y_H_DEN) + 1)
#define POLY_Y_IMAGE_HEIGHT         ((((SIPP_ISP_PIPE_BUFFER_HEIGHT * POLYFIR_Y_V_NUM) - 1) / POLYFIR_Y_V_DEN) + 1)
#define POLY_UV_IMAGE_WIDTH         ((((SIPP_ISP_PIPE_BUFFER_WIDTH * POLYFIR_U_H_NUM) - 1) / POLYFIR_U_H_DEN) + 1)
#define POLY_UV_IMAGE_HEIGHT        ((((SIPP_ISP_PIPE_BUFFER_HEIGHT * POLYFIR_U_V_NUM) - 1) / POLYFIR_U_V_DEN) + 1)

// Pipeline 2 Setup
#define SIPP_CONV_PIPE_BUFFER_WIDTH  512
#define SIPP_CONV_PIPE_BUFFER_HEIGHT 512

/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

volatile u32 testComplete = 0;
u32    sippErrorLog[SIPP_ERROR_HISTORY_SIZE];

// Pipeline state enum
typedef enum
{
    CC_APP_PIPE_STATE_UNINIT     = 0x00,
    CC_APP_PIPE_STATE_IDLE       = 0x01,
    CC_APP_PIPE_STATE_RUNNING    = 0x02,
    CC_APP_PIPE_STATE_COMPLETE   = 0x03

} CC_APP_PIPE_STATE;

// Pipeline event enum
typedef enum
{
    CC_APP_PIPE_EVENT_NONE       = 0x00,
    CC_APP_PIPE_EVENT_FRAME_DONE = 0x01

} CC_APP_PIPE_EVENT;

typedef struct
{
    CC_APP_PIPE_STATE eState;
    CC_APP_PIPE_EVENT eventList[CC_APP_EVENT_HISTORY_SIZE];
    u32               framesComplete;
    u32               eventListWrPtr;
    u32               eventListRdPtr;

    SippPipeline    * sippFwHandle;

    SippFilter      * dmaIn;
    SippFilter      * dmaOut;

    SippFilter      * convMa2x5x0;

    SippFilter      * sigmaDnsMa2x5x0;
    SippFilter      * lscMa2x5x0;
    SippFilter      * rawMa2x5x0;
    SippFilter      * debayerMa2x5x0;
    SippFilter      * dogLtmMa2x5x0;
    SippFilter      * ydnsMa2x5x0;
    SippFilter      * sharpenMa2x5x0;
    SippFilter      * chromaGenMa2x5x0;
    SippFilter      * medianMa2x5x0;
    SippFilter      * chromadnsMa2x5x0;
    SippFilter      * colorcombMa2x5x0;
    SippFilter      * lutMa2x5x0;
    SippFilter      * PolyFirY;
    SippFilter      * PolyFirUV;
    SippFilter      * dmaOutY;
    SippFilter      * dmaOutUV;

} ConcurrencyAppPipe, * pConcurrencyAppPipe;

ConcurrencyAppPipe concurrencyPipeList[SIPP_NUM_PIPELINES];

UInt16 DDR_DATA iBufA[SIPP_ISP_PIPE_BUFFER_WIDTH * SIPP_ISP_PIPE_BUFFER_HEIGHT] ALIGNED(8);
UInt8  DDR_DATA oBufA[SIPP_ISP_PIPE_BUFFER_WIDTH * SIPP_ISP_PIPE_BUFFER_HEIGHT + (2 * POLY_UV_IMAGE_WIDTH) * POLY_UV_IMAGE_HEIGHT] ALIGNED(8);
UInt8  DDR_DATA iBufB[SIPP_CONV_PIPE_BUFFER_WIDTH * SIPP_CONV_PIPE_BUFFER_HEIGHT] ALIGNED(8);
UInt8  DDR_DATA oBufB[SIPP_CONV_PIPE_BUFFER_WIDTH * SIPP_CONV_PIPE_BUFFER_HEIGHT] ALIGNED(8);

/////////////////////////////////////////////////////
// Function prototypes
/////////////////////////////////////////////////////
void appSippCallback (SippPipeline *             pPipeline,
                      eSIPP_PIPELINE_EVENT       eEvent,
                      SIPP_PIPELINE_EVENT_DATA * ptEventData);

/////////////////////////////////////////////////////
// Code
/////////////////////////////////////////////////////

//###################################################################################################
// Build the pipeline-A
void buildISPPipe ( )
{
   // create new pipeline
   ConcurrencyAppPipe * plA = &concurrencyPipeList[0];
   plA->sippFwHandle        =  sippCreatePipeline(0, 4, SIPP_MBIN(mbinImgSipp));

   // create filters
   plA->dmaIn            =  sippCreateFilter(plA->sippFwHandle, 0x0,  SIPP_ISP_PIPE_BUFFER_WIDTH, SIPP_ISP_PIPE_BUFFER_HEIGHT, N_PL(1), SZ(UInt16), SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID,    0);
   plA->sigmaDnsMa2x5x0  =  sippCreateFilter(plA->sippFwHandle, 0x0,  SIPP_ISP_PIPE_BUFFER_WIDTH, SIPP_ISP_PIPE_BUFFER_HEIGHT, N_PL(1), SZ(UInt16), SIPP_AUTO, (FnSvuRun)SIPP_SIGMA_ID,  0);
   plA->lscMa2x5x0       =  sippCreateFilter(plA->sippFwHandle, 0x0,  SIPP_ISP_PIPE_BUFFER_WIDTH, SIPP_ISP_PIPE_BUFFER_HEIGHT, N_PL(1), SZ(UInt16), SIPP_AUTO, (FnSvuRun)SIPP_LSC_ID,    0);
   plA->rawMa2x5x0       =  sippCreateFilter(plA->sippFwHandle, 0x0,  SIPP_ISP_PIPE_BUFFER_WIDTH, SIPP_ISP_PIPE_BUFFER_HEIGHT, N_PL(1), SZ(UInt8),  SIPP_AUTO, (FnSvuRun)SIPP_RAW_ID,    0);
   plA->debayerMa2x5x0   =  sippCreateFilter(plA->sippFwHandle, 0x0,  SIPP_ISP_PIPE_BUFFER_WIDTH, SIPP_ISP_PIPE_BUFFER_HEIGHT, N_PL(3), SZ(UInt16), SIPP_AUTO, (FnSvuRun)SIPP_DBYR_ID,   0);
   sippFilterAddOBuf(plA->debayerMa2x5x0, 1, sizeof(fp16));
   plA->dogLtmMa2x5x0    =  sippCreateFilter(plA->sippFwHandle, 0x0,  SIPP_ISP_PIPE_BUFFER_WIDTH, SIPP_ISP_PIPE_BUFFER_HEIGHT, N_PL(1), SZ(fp16),   SIPP_AUTO,  (FnSvuRun)SIPP_DOGL_ID,   0);
   plA->ydnsMa2x5x0      =  sippCreateFilter(plA->sippFwHandle, 0x0,  SIPP_ISP_PIPE_BUFFER_WIDTH, SIPP_ISP_PIPE_BUFFER_HEIGHT, N_PL(1), SZ(fp16),   SIPP_AUTO,  (FnSvuRun)SIPP_LUMA_ID,   0);
   plA->sharpenMa2x5x0   =  sippCreateFilter(plA->sippFwHandle, 0x0,  SIPP_ISP_PIPE_BUFFER_WIDTH, SIPP_ISP_PIPE_BUFFER_HEIGHT, N_PL(1), SZ(fp16),   SIPP_AUTO,  (FnSvuRun)SIPP_SHARPEN_ID,0);
   plA->chromaGenMa2x5x0 =  sippCreateFilter(plA->sippFwHandle, SIPP_RESIZE, ISPC_CHROMA_PIPE_WIDTH, ISPC_CHROMA_PIPE_HEIGHT,  N_PL(3), SZ(UInt8),  SIPP_AUTO,  (FnSvuRun)SIPP_CGEN_ID,   0);
   plA->medianMa2x5x0    =  sippCreateFilter(plA->sippFwHandle, 0x0,  ISPC_CHROMA_PIPE_WIDTH,     ISPC_CHROMA_PIPE_HEIGHT,     N_PL(3), SZ(UInt8),  SIPP_AUTO,  (FnSvuRun)SIPP_MED_ID,    0);
   plA->chromadnsMa2x5x0 =  sippCreateFilter(plA->sippFwHandle, 0x0,  ISPC_CHROMA_PIPE_WIDTH,     ISPC_CHROMA_PIPE_HEIGHT,     N_PL(3), SZ(UInt8),  SIPP_AUTO,  (FnSvuRun)SIPP_CHROMA_ID, 0);
   plA->colorcombMa2x5x0 =  sippCreateFilter(plA->sippFwHandle, 0x0,  SIPP_ISP_PIPE_BUFFER_WIDTH, SIPP_ISP_PIPE_BUFFER_HEIGHT, N_PL(3), SZ(UInt16), SIPP_AUTO,  (FnSvuRun)SIPP_CC_ID,     0);
   plA->lutMa2x5x0       =  sippCreateFilter(plA->sippFwHandle, 0x0,  SIPP_ISP_PIPE_BUFFER_WIDTH, SIPP_ISP_PIPE_BUFFER_HEIGHT, N_PL(3), SZ(UInt8),  SIPP_AUTO,  (FnSvuRun)SIPP_LUT_ID,    0);
   plA->PolyFirY         =  sippCreateFilter(plA->sippFwHandle, SIPP_RESIZE, POLY_Y_IMAGE_WIDTH,  POLY_Y_IMAGE_HEIGHT,         N_PL(1), SZ(UInt8),  SIPP_AUTO,  (FnSvuRun)SIPP_UPFIRDN0_ID, 0);
   plA->PolyFirUV        =  sippCreateFilter(plA->sippFwHandle, SIPP_RESIZE, POLY_UV_IMAGE_WIDTH, POLY_UV_IMAGE_HEIGHT,        N_PL(2), SZ(UInt8),  SIPP_AUTO,  (FnSvuRun)SIPP_UPFIRDN1_ID, 0);
   plA->dmaOutY          =  sippCreateFilter(plA->sippFwHandle, 0x00, POLY_Y_IMAGE_WIDTH,         POLY_Y_IMAGE_HEIGHT,         N_PL(1), SZ(UInt8),  SIPP_AUTO,  (FnSvuRun)SIPP_DMA_ID,    0);
   plA->dmaOutUV         =  sippCreateFilter(plA->sippFwHandle, 0x00, POLY_UV_IMAGE_WIDTH,        POLY_UV_IMAGE_HEIGHT,        N_PL(2), SZ(UInt8),  SIPP_AUTO,  (FnSvuRun)SIPP_DMA_ID,    0);

   // link filters
   sippLinkFilter(plA->sigmaDnsMa2x5x0,      plA->dmaIn ,           5,5);
   sippLinkFilter(plA->lscMa2x5x0,           plA->sigmaDnsMa2x5x0,  1,1);
   sippLinkFilter(plA->rawMa2x5x0,           plA->lscMa2x5x0,       5,5);
   sippLinkFilter(plA->debayerMa2x5x0,       plA->rawMa2x5x0,       11,11);
   sippLinkFilter(plA->dogLtmMa2x5x0,        plA->debayerMa2x5x0,   15,15);
   sippLinkFilterSetOBuf(plA->dogLtmMa2x5x0, plA->debayerMa2x5x0,   1);
   sippLinkFilter(plA->ydnsMa2x5x0,          plA->dogLtmMa2x5x0,    11,11);
   sippLinkFilter(plA->sharpenMa2x5x0,       plA->ydnsMa2x5x0,      7,7);
   sippLinkFilter(plA->chromaGenMa2x5x0,     plA->debayerMa2x5x0,   6,6);
   sippLinkFilter(plA->medianMa2x5x0,        plA->chromaGenMa2x5x0, 7,7);
   sippLinkFilter(plA->medianMa2x5x0,        plA->debayerMa2x5x0,   1,0);
   sippLinkFilterSetOBuf(plA->medianMa2x5x0, plA->debayerMa2x5x0,   1);
   sippLinkFilter(plA->chromadnsMa2x5x0,     plA->medianMa2x5x0,    3,3);
   sippLinkFilter(plA->colorcombMa2x5x0,     plA->sharpenMa2x5x0,   1,1);
   sippLinkFilter(plA->colorcombMa2x5x0,     plA->chromadnsMa2x5x0, 5,5);
   sippLinkFilter(plA->lutMa2x5x0,           plA->colorcombMa2x5x0, 1,1);
   sippLinkFilter(plA->PolyFirY,             plA->lutMa2x5x0,       7,7);
   sippLinkFilter(plA->PolyFirUV,            plA->lutMa2x5x0,       7,7);
   sippLinkFilter(plA->dmaOutY,              plA->PolyFirY,         1,1);
   sippLinkFilter(plA->dmaOutUV,             plA->PolyFirUV,        1,1);

   if (sippPipeGetErrorStatus (plA->sippFwHandle))
   {
        u32 errNum;
        printf ("Pipeline-A creation error\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
    }

   // Register callback
   sippRegisterEventCallback (plA->sippFwHandle, appSippCallback);

   // Set a flag to enable consideration of oPipe runtime (that which we wish to use here)
   plA->sippFwHandle->flags |= PLF_CONSIDER_OPIPE_RT;
}

void ISPPipeCreateParams ();

void configISPPipe ( )
{
   ConcurrencyAppPipe * plA           = &concurrencyPipeList[0];
   DmaParam      *dmaInCfg            = (DmaParam     *)plA->dmaIn->params;
   SigmaParam    *sigmaDnsMa2x5x0Cfg  = (SigmaParam   *)plA->sigmaDnsMa2x5x0->params;
   LscParam      *lscMa2x5x0Cfg       = (LscParam     *)plA->lscMa2x5x0->params;
   RawParam      *rawMa2x5x0Cfg       = (RawParam     *)plA->rawMa2x5x0->params;
   DbyrParam     *debayerMa2x5x0Cfg   = (DbyrParam    *)plA->debayerMa2x5x0->params;
   DogLtmParam   *dogLtmMa2x5x0Cfg    = (DogLtmParam  *)plA->dogLtmMa2x5x0->params;
   YDnsParam     *ydnsMa2x5x0Cfg      = (YDnsParam    *)plA->ydnsMa2x5x0->params;
   UsmParam      *sharpenMa2x5x0Cfg   = (UsmParam     *)plA->sharpenMa2x5x0->params;
   GenChrParam   *chromaGenMa2x5x0Cfg = (GenChrParam  *)plA->chromaGenMa2x5x0->params;
   MedParam      *medianMa2x5x0Cfg    = (MedParam     *)plA->medianMa2x5x0->params;
   ChrDnsParam   *chromadnsMa2x5x0Cfg = (ChrDnsParam  *)plA->chromadnsMa2x5x0->params;
   ColCombParam  *colorcombMa2x5x0Cfg = (ColCombParam *)plA->colorcombMa2x5x0->params;
   LutParam      *lutMa2x5x0Cfg       = (LutParam     *)plA->lutMa2x5x0->params;
   PolyFirParam  *polyFirYCfg         = (PolyFirParam *)plA->PolyFirY->params;
   PolyFirParam  *polyFirUVCfg        = (PolyFirParam *)plA->PolyFirUV->params;
   DmaParam      *dmaOutYCfg          = (DmaParam     *)plA->dmaOutY->params;
   DmaParam      *dmaOutUVCfg         = (DmaParam     *)plA->dmaOutUV->params;

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // here go through all the filter instances and assign the value for all parameter
   // fields as defined in the filter property window
   sigmaDnsMa2x5x0Cfg->thresh[0]=(ISPC_SIGMA_THRESH1_P0 <<  0 | ISPC_SIGMA_THRESH2_P0 <<  8 | ISPC_SIGMA_THRESH1_P1 << 16 | ISPC_SIGMA_THRESH2_P1 << 24);
   sigmaDnsMa2x5x0Cfg->thresh[1]=(ISPC_SIGMA_THRESH1_P2 <<  0 | ISPC_SIGMA_THRESH2_P2 <<  8 | ISPC_SIGMA_THRESH1_P3 << 16 | ISPC_SIGMA_THRESH2_P3 << 24);
   sigmaDnsMa2x5x0Cfg->cfg= SIGMA_DNS_CFG(ISPC_SIGMA_NOISE_FLOOR, (ISPC_BAYER_BITS - 1), SIGMA_DNS_PASSTHRU_BIT,FORMAT);
   sigmaDnsMa2x5x0Cfg->bayerPattern=GRBG;
   sigmaDnsMa2x5x0Cfg->blcGR=16;
   sigmaDnsMa2x5x0Cfg->blcR=16;
   sigmaDnsMa2x5x0Cfg->blcB=16;
   sigmaDnsMa2x5x0Cfg->blcGB=16;
   lscMa2x5x0Cfg->gmBase=   ispcLscMesh;
   lscMa2x5x0Cfg->gmWidth=  ISPC_LSC_GAIN_MAP_W;
   lscMa2x5x0Cfg->gmHeight= ISPC_LSC_GAIN_MAP_H;
   lscMa2x5x0Cfg->dataFormat=SIPP_LSC_CFG_FORMAT(BAYER);
   lscMa2x5x0Cfg->dataWidth=ISPC_BAYER_BITS;
   rawMa2x5x0Cfg->grgbPlat= RAW_GRGB_PLATO(ISPC_GRGB_IMBAL_PLAT_DARK,ISPC_GRGB_IMBAL_PLAT_BRIGHT);
   rawMa2x5x0Cfg->grgbDecay=RAW_GRGB_DECAY(ISPC_GRGB_IMBAL_DECAY_BRIGHT, ISPC_GRGB_IMBAL_DECAY_DARK);
   rawMa2x5x0Cfg->badPixCfg=RAW_BAD_PIX_CFG(ISPC_BAD_PIX_NOISE_LEVEL, ISPC_BAD_PIX_ALPHA_G_HOT, ISPC_BAD_PIX_ALPHA_G_COLD, ISPC_BAD_PIX_ALPHA_RB_HOT, ISPC_BAD_PIX_ALPHA_RB_COLD);
   rawMa2x5x0Cfg->cfg=      RAW_CFG(BAYER,BAYER_PATTERN,GRGB_IMBAL_EN,0,AE_PATCH_STATS_EN,AE_RGB_HIST_STATS_EN,(ISPC_BAYER_BITS - 1),ISPC_GRGB_IMBAL_THRESHOLD,0,AF_PATCH_STATS_EN,BAYER_2x2_MODE,AE_Y_HIST_STATS_EN,HOT_COLD_PIX_SUPPRESS_EN);
   rawMa2x5x0Cfg->gainSat[0]=((ISPC_RAW_CLAMP_0<<16) | ISPC_RAW_GAIN_GR );
   rawMa2x5x0Cfg->gainSat[1]= ((ISPC_RAW_CLAMP_1<<16) | ISPC_RAW_GAIN_R );
   rawMa2x5x0Cfg->gainSat[2]= ((ISPC_RAW_CLAMP_2<<16) | ISPC_RAW_GAIN_B );
   rawMa2x5x0Cfg->gainSat[3]= ((ISPC_RAW_CLAMP_3<<16) | ISPC_RAW_GAIN_GB );
   rawMa2x5x0Cfg->statsBase=0;
   rawMa2x5x0Cfg->statsPlanes=0;
   rawMa2x5x0Cfg->statsPatchCfg=0;
   rawMa2x5x0Cfg->statsPatchStart=0;
   rawMa2x5x0Cfg->statsPatchSkip=0;
   rawMa2x5x0Cfg->statsThresh=0;
   rawMa2x5x0Cfg->afF1coefs[0]=0;
   rawMa2x5x0Cfg->afF1coefs[1]=0;
   rawMa2x5x0Cfg->afF1coefs[2]=0;
   rawMa2x5x0Cfg->afF1coefs[3]=0;
   rawMa2x5x0Cfg->afF1coefs[4]=0;
   rawMa2x5x0Cfg->afF1coefs[5]=0;
   rawMa2x5x0Cfg->afF1coefs[6]=0;
   rawMa2x5x0Cfg->afF1coefs[7]=0;
   rawMa2x5x0Cfg->afF1coefs[8]=0;
   rawMa2x5x0Cfg->afF1coefs[9]=0;
   rawMa2x5x0Cfg->afF1coefs[10]=0;
   rawMa2x5x0Cfg->afF2coefs[0]=0;
   rawMa2x5x0Cfg->afF2coefs[1]=0;
   rawMa2x5x0Cfg->afF2coefs[2]=0;
   rawMa2x5x0Cfg->afF2coefs[3]=0;
   rawMa2x5x0Cfg->afF2coefs[4]=0;
   rawMa2x5x0Cfg->afF2coefs[5]=0;
   rawMa2x5x0Cfg->afF2coefs[6]=0;
   rawMa2x5x0Cfg->afF2coefs[7]=0;
   rawMa2x5x0Cfg->afF2coefs[8]=0;
   rawMa2x5x0Cfg->afF2coefs[9]=0;
   rawMa2x5x0Cfg->afF2coefs[10]=0;
   rawMa2x5x0Cfg->afMinThresh=0;
   rawMa2x5x0Cfg->afSubtract=0;
   rawMa2x5x0Cfg->afPatchCfg=0;
   rawMa2x5x0Cfg->afPatchStart=0;
   rawMa2x5x0Cfg->afStatsBase=0;
   rawMa2x5x0Cfg->histLumaBase=0;
   rawMa2x5x0Cfg->histRgbBase=0;
   debayerMa2x5x0Cfg->cfg=  DEBAYER_CFG(BAYER_PATTERN,0,0,ISPC_BAYER_BITS,ISPC_DEBAYER_BITS,IMAGE_ORDER_OUT, OUTPUT_PLANE_NO,0);
   debayerMa2x5x0Cfg->thresh=DEBAYER_THRESH(0,0,1,1,1,0) ;
   debayerMa2x5x0Cfg->dewormCfg=DEBAYER_DEWORM(ISPC_DEMOSAIC_MIX_OFFSET,ISPC_DEMOSAIC_MIX_SLOPE);
   debayerMa2x5x0Cfg->lumaWeight=DEBAYER_LUMA_COEFS_CFG(ISPC_DEMOSAIC_LUMA_WEIGHT_RED , ISPC_DEMOSAIC_LUMA_WEIGHT_GREEN, ISPC_DEMOSAIC_LUMA_WEIGHT_BLUE);
   dogLtmMa2x5x0Cfg->cfg=   DOGL_CFG_SET(DOG_LTM_MODE,ISPC_DOG_THR,DOG_OUTPUT_CLAMP,DS_MODE,ISPC_LTM_FILTER_TH1,0,DOG_HEIGHT);;
   dogLtmMa2x5x0Cfg->dogCoeffs11=dogCoeffs11;
   dogLtmMa2x5x0Cfg->dogCoeffs15=dogCoeffs15;
   dogLtmMa2x5x0Cfg->dogStrength=ISPC_DOG_STRENGTH;
   dogLtmMa2x5x0Cfg->ltmCurves=(u16 *)ltm_curves;
   ydnsMa2x5x0Cfg->cfg=     LUMA_DNS_CFG(0x1, ISPC_LUMA_DNS_REF_SHIFT, ISPC_LUMA_DNS_ALPHA, ISPC_LUMA_DNS_BITPOS);
   ydnsMa2x5x0Cfg->gaussLut[0]=gaussLut[0];
   ydnsMa2x5x0Cfg->gaussLut[1]=gaussLut[1];
   ydnsMa2x5x0Cfg->gaussLut[2]=gaussLut[2];
   ydnsMa2x5x0Cfg->gaussLut[3]=gaussLut[3];
   ydnsMa2x5x0Cfg->f2=      ISPC_LUMA_DNS_F2;
   ydnsMa2x5x0Cfg->gammaLut[0]=gammaLut[0];
   ydnsMa2x5x0Cfg->gammaLut[1]=gammaLut[1];
   ydnsMa2x5x0Cfg->gammaLut[2]=gammaLut[2];
   ydnsMa2x5x0Cfg->gammaLut[3]=gammaLut[3];
   ydnsMa2x5x0Cfg->gammaLut[4]=gammaLut[4];
   ydnsMa2x5x0Cfg->distCfg= (u32)ispcYDnsDistLut;
   ydnsMa2x5x0Cfg->distOffsets=X_OFFSET | (Y_OFFSET << 16);
   ydnsMa2x5x0Cfg->fullFrmDim=(SIPP_ISP_PIPE_BUFFER_WIDTH << 16 ) | SIPP_ISP_PIPE_BUFFER_HEIGHT;
   sharpenMa2x5x0Cfg->cfg=  SHARPEN_CFG( THR, SHARPEN_KERNEL_SIZE,OUTPUT_CLAMP ,SHARPEN_MODE,OUTPUT_DELTAS);
   sharpenMa2x5x0Cfg->strength=SHARPEN_STRENGTH_CFG(SHARPEN_STRENGTH_LIGHTEN, SHARPEN_STRENGTH_DARKEN );
   sharpenMa2x5x0Cfg->clip= SHARPEN_CLIPPING_ALPHA;
   sharpenMa2x5x0Cfg->limit= SHARPEN_LIMITS_CFG(0x0,0x4000);
   sharpenMa2x5x0Cfg->rgnStop01=SHARPEN_RANGESTOP01_CFG(SHARPEN_RANGE_STOP0, SHARPEN_RANGE_STOP1);
   sharpenMa2x5x0Cfg->rgnStop23= SHARPEN_RANGESTOP01_CFG(SHARPEN_RANGE_STOP2, SHARPEN_RANGE_STOP3);
   sharpenMa2x5x0Cfg->coef01=SHARPEN_COEFS01_CFG(0x0,0x0);
   sharpenMa2x5x0Cfg->coef23=SHARPEN_COEFS23_CFG(0x0,0x0);
   chromaGenMa2x5x0Cfg->cfg=CGEN_CFG_SET(ISPC_CGEN_PFR_STRENGTH,ISPC_CGEN_DESAT_OFFSET,ISPC_CGEN_DESAT_SLOPE,ISPC_DEBAYER_BITS,BYPASS_PF_DAD,DS_MODE);
   chromaGenMa2x5x0Cfg->yCoefs=CGEN_LUMA_COEFF_SET(ISPC_CGEN_LUMA_COEFF_R,ISPC_CGEN_LUMA_COEFF_G,ISPC_CGEN_LUMA_COEFF_B) ;
   chromaGenMa2x5x0Cfg->chrCoefs=CGEN_CHROMA_COEFF_SET(ISPC_CGEN_EPSILON,ISPC_CGEN_KR,ISPC_CGEN_KG,ISPC_CGEN_KB);
   medianMa2x5x0Cfg->cfg=   MED_CFG(MEDIAN_KERNEL_SIZE,MEDIAN_OUT_SEL,MEDIAN_THRESHOLD,MEDIAN_LUMA_ABLEND_EN,MEDIAN_LUMA_SAMPLE_EN,0);
   medianMa2x5x0Cfg->lumaAlpha=MED_LUMA_ALPHA(ISPC_CHROMA_MEDIAN_MIX_SLOPE,ISPC_CHROMA_MEDIAN_MIX_OFFSET);
   chromadnsMa2x5x0Cfg->cfg=CHROMA_DNS_CFG(ISPC_CHROMA_DNS_H_ENAB,ISPC_CHROMA_DNS_LIMIT,FORCE_WT_H,FORCE_WT_V,(PLANES_PER_CYCLE-1),ISPC_GREY_DESAT_OFFSET,ISPC_GREY_DESAT_SLOPE);
   chromadnsMa2x5x0Cfg->thr[0]=CHROMA_DNS_THRESH0;
   chromadnsMa2x5x0Cfg->thr[1]=CHROMA_DNS_THRESH1;
   chromadnsMa2x5x0Cfg->greyPt=CHORMA_GREY_PT_SET(ISPC_GREY_POINT_B,ISPC_GREY_POINT_G,ISPC_GREY_POINT_R,GREY_PT_EN);
   chromadnsMa2x5x0Cfg->chrCoefs=CHROMA_DNS_COEFFS_SET(ispcLowpassKernel[4],ispcLowpassKernel[1],ispcLowpassKernel[0]);
   colorcombMa2x5x0Cfg->cfg=CC_CFG(FORCE_LUMA_ONE,0,LUT_3D_BYPASS,LUT_3D_LOAD,U12F_OUTPUT,CC_OUTPUT_PLANE);
   colorcombMa2x5x0Cfg->krgb[0]=ISPC_CC_KR | (ISPC_CC_KG << 16);
   colorcombMa2x5x0Cfg->krgb[1]=(ISPC_CC_KB | (ISPC_CGEN_EPSILON << 16));
   colorcombMa2x5x0Cfg->ccm[0]=(ccm_lut_coeffs[0] | (ccm_lut_coeffs[3] << 16));
   colorcombMa2x5x0Cfg->ccm[1]=(ccm_lut_coeffs[6] | (ccm_lut_coeffs[1] << 16));
   colorcombMa2x5x0Cfg->ccm[2]=(ccm_lut_coeffs[4] | (ccm_lut_coeffs[7] << 16));
   colorcombMa2x5x0Cfg->ccm[3]=(ccm_lut_coeffs[2] | (ccm_lut_coeffs[5] << 16));
   colorcombMa2x5x0Cfg->ccm[4]=(ccm_lut_coeffs[8] | (ccm_lut_offsets[0] << 16));
   colorcombMa2x5x0Cfg->ccOffs=(ccm_lut_offsets[1] | (ccm_lut_offsets[2] << 16));
   colorcombMa2x5x0Cfg->threeDLut=0x0;
   colorcombMa2x5x0Cfg->lutFormat=0x0;
   lutMa2x5x0Cfg->cfg=      INTERP_MODE | CHANNEL_MODE<< 1 | (CHANNELS_NO - 1) << 12 | LUT_LOAD << 14 | CSC_ENABLE<< 16;
   lutMa2x5x0Cfg->sizeA=    LUT_REGION0_SIZE_I | (LUT_REGION1_SIZE_I <<  4) | (LUT_REGION2_SIZE_I <<  8) | (LUT_REGION3_SIZE_I << 12) | (LUT_REGION4_SIZE_I << 16) | (LUT_REGION5_SIZE_I << 20) | (LUT_REGION6_SIZE_I << 24) | (LUT_REGION7_SIZE_I << 28);
   lutMa2x5x0Cfg->sizeB=    LUT_REGION8_SIZE_I | (LUT_REGION9_SIZE_I  <<  4) | (LUT_REGION10_SIZE_I <<  8) | (LUT_REGION11_SIZE_I << 12) | (LUT_REGION12_SIZE_I << 16) | (LUT_REGION13_SIZE_I << 20) | (LUT_REGION14_SIZE_I << 24) | (LUT_REGION15_SIZE_I << 28);
   lutMa2x5x0Cfg->lut=      (void *)gammaLutFp16;
   lutMa2x5x0Cfg->lutFormat=LUT_LD_FORMAT;
   lutMa2x5x0Cfg->mat[0]=   LUT_MAT_11;
   lutMa2x5x0Cfg->mat[1]=   LUT_MAT_12;
   lutMa2x5x0Cfg->mat[2]=   LUT_MAT_13;
   lutMa2x5x0Cfg->mat[3]=   LUT_MAT_21;
   lutMa2x5x0Cfg->mat[4]=   LUT_MAT_22;
   lutMa2x5x0Cfg->mat[5]=   LUT_MAT_23;
   lutMa2x5x0Cfg->mat[6]=   LUT_MAT_31;
   lutMa2x5x0Cfg->mat[7]=   LUT_MAT_32;
   lutMa2x5x0Cfg->mat[8]=   LUT_MAT_33;
   lutMa2x5x0Cfg->offset[0]=LUT_OFF_1;
   lutMa2x5x0Cfg->offset[1]=LUT_OFF_2;
   lutMa2x5x0Cfg->offset[2]=LUT_OFF_3;
   polyFirYCfg->planeMode=POLY_PLANE_Y;
   polyFirYCfg->clamp    =0;
   polyFirYCfg->horzD    =POLYFIR_Y_H_DEN;
   polyFirYCfg->horzN    =POLYFIR_Y_H_NUM;
   polyFirYCfg->vertD    =POLYFIR_Y_V_DEN;
   polyFirYCfg->vertN    =POLYFIR_Y_V_NUM;
   polyFirYCfg->horzCoefs=(UInt8*)hCoefs;
   polyFirYCfg->vertCoefs=(UInt8*)vCoefs;
   polyFirUVCfg->planeMode=POLY_PLANE_UV;
   polyFirUVCfg->clamp     =0;
   polyFirUVCfg->horzD    =POLYFIR_U_H_DEN;
   polyFirUVCfg->horzN    =POLYFIR_U_H_NUM;
   polyFirUVCfg->vertD    =POLYFIR_U_V_DEN;
   polyFirUVCfg->vertN    =POLYFIR_U_V_NUM;
   polyFirUVCfg->horzCoefs=(UInt8*)hCoefs;
   polyFirUVCfg->vertCoefs=(UInt8*)vCoefs;
   dmaInCfg->ddrAddr    = (UInt32)&iBufA;
   dmaOutYCfg->ddrAddr  = (UInt32)&oBufA;
   dmaOutUVCfg->ddrAddr = (UInt32)&oBufA[POLY_Y_IMAGE_WIDTH * POLY_Y_IMAGE_HEIGHT];

   sippFinalizePipeline (plA->sippFwHandle);

   if (sippPipeGetErrorStatus (plA->sippFwHandle))
   {
        u32 errNum;
        printf ("Pipeline-A finalization failed\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
    }
}

//###################################################################################################
// Build the pipeline-B
void buildConvPipe ( )
{
   // create new pipeline
   ConcurrencyAppPipe * plB = &concurrencyPipeList[1];
   plB->sippFwHandle        =  sippCreatePipeline(5, 6, SIPP_MBIN(mbinImgSipp));

   // create filters
   plB->dmaIn        =  sippCreateFilter(plB->sippFwHandle, 0x00, SIPP_CONV_PIPE_BUFFER_WIDTH, SIPP_CONV_PIPE_BUFFER_HEIGHT, N_PL(1), SZ(UInt8), SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID,  0);
   plB->convMa2x5x0  =  sippCreateFilter(plB->sippFwHandle, 0x0,  SIPP_CONV_PIPE_BUFFER_WIDTH, SIPP_CONV_PIPE_BUFFER_HEIGHT, N_PL(1), SZ(UInt8), SIPP_AUTO, (FnSvuRun)SIPP_CONV_ID, 0);
   plB->dmaOut       =  sippCreateFilter(plB->sippFwHandle, 0x00, SIPP_CONV_PIPE_BUFFER_WIDTH, SIPP_CONV_PIPE_BUFFER_HEIGHT, N_PL(1), SZ(UInt8), SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID,  0);

   // link filters
   sippLinkFilter(plB->convMa2x5x0, plB->dmaIn,       3,3);
   sippLinkFilter(plB->dmaOut,      plB->convMa2x5x0, 1,1);

   if (sippPipeGetErrorStatus (plB->sippFwHandle))
   {
        u32 errNum;
        printf ("Pipeline-B creation error\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
    }

   // Register callback
   sippRegisterEventCallback (plB->sippFwHandle, appSippCallback);

   // Set a flag to enable consideration of oPipe runtime (that which we wish to use here)
   plB->sippFwHandle->flags |= PLF_CONSIDER_OPIPE_RT;
}

void configConvPipe ( )
{
  ConcurrencyAppPipe * plB    = &concurrencyPipeList[1];
   DmaParam  *dmaIn0Cfg       = (DmaParam  *)plB->dmaIn->params;
   ConvParam *convMa2x5x0Cfg  = (ConvParam *)plB->convMa2x5x0->params;
   DmaParam  *dmaOut0Cfg      = (DmaParam  *)plB->dmaOut->params;

   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   // here go through all the filter instances and assign the value for all parameter
   // fields as defined in the filter property window
   convMa2x5x0Cfg->cfg=CONV_CFG(3, 1, 1, 0,1, 0,  0x3646, 1, 0);
   convMa2x5x0Cfg->kernel[0]=0x0;
   convMa2x5x0Cfg->kernel[1]= 0x0;
   convMa2x5x0Cfg->kernel[2]= 0x0;
   convMa2x5x0Cfg->kernel[3]= 0x0;
   convMa2x5x0Cfg->kernel[4]= 0x00003000;
   convMa2x5x0Cfg->kernel[5]= 0x0;
   convMa2x5x0Cfg->kernel[6]= 0x30000000;
   convMa2x5x0Cfg->kernel[7]= 0x30003800;
   convMa2x5x0Cfg->kernel[8]= 0x0;
   convMa2x5x0Cfg->kernel[9]= 0x0;
   convMa2x5x0Cfg->kernel[10]= 0x00003000;
   convMa2x5x0Cfg->kernel[11]= 0x0;
   convMa2x5x0Cfg->kernel[12]= 0x0;
   convMa2x5x0Cfg->kernel[13]= 0x0;
   convMa2x5x0Cfg->kernel[14]= 0x0;
   convMa2x5x0Cfg->shadowKernel[0]=0x0;
   convMa2x5x0Cfg->shadowKernel[1]= 0x0;
   convMa2x5x0Cfg->shadowKernel[2]= 0x0;
   convMa2x5x0Cfg->shadowKernel[3]= 0x0;
   convMa2x5x0Cfg->shadowKernel[4]= 0x00003000;
   convMa2x5x0Cfg->shadowKernel[5]= 0x0;
   convMa2x5x0Cfg->shadowKernel[6]= 0x30000000;
   convMa2x5x0Cfg->shadowKernel[7]= 0x30003800;
   convMa2x5x0Cfg->shadowKernel[8]= 0x0;
   convMa2x5x0Cfg->shadowKernel[9]= 0x0;
   convMa2x5x0Cfg->shadowKernel[10]= 0x00003000;
   convMa2x5x0Cfg->shadowKernel[11]= 0x0;
   convMa2x5x0Cfg->shadowKernel[12]= 0x0;
   convMa2x5x0Cfg->shadowKernel[13]= 0x0;
   convMa2x5x0Cfg->shadowKernel[14]= 0x0;
   dmaIn0Cfg->ddrAddr   = (UInt32)&iBufB;
   dmaOut0Cfg->ddrAddr  = (UInt32)&oBufB;

   sippFinalizePipeline (plB->sippFwHandle);

   if (sippPipeGetErrorStatus (plB->sippFwHandle))
   {
        u32 errNum;
        printf ("Pipeline-B finalization failed\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
    }
}

void appLogEvent (ConcurrencyAppPipe * pipe,
                  CC_APP_PIPE_EVENT    event)
{
    #ifndef SIPP_PC
    // Use critical sections to future proof against this happening in an RTOS thread
    u32 storedIL = swcLeonGetPIL ();
    swcLeonSetPIL (15);
    #endif

    pipe->eventList[pipe->eventListWrPtr++] = event;
    if (pipe->eventListWrPtr == CC_APP_EVENT_HISTORY_SIZE) pipe->eventListWrPtr = 0;

    #ifndef SIPP_PC
    swcLeonSetPIL (storedIL);
    #endif
}

CC_APP_PIPE_EVENT appProcessEvent (ConcurrencyAppPipe * pipe)
{
    CC_APP_PIPE_EVENT retVal = CC_APP_PIPE_EVENT_NONE;

    if (pipe->eventListWrPtr != pipe->eventListRdPtr )
    {
        retVal = pipe->eventList[pipe->eventListRdPtr++];
        if (pipe->eventListRdPtr == CC_APP_EVENT_HISTORY_SIZE) pipe->eventListRdPtr = 0;

    }

    return retVal;
}

u32 appLookupHandle (SippPipeline * pPipeline)
{
    u32 idx;

    for (idx = 0; idx < SIPP_NUM_PIPELINES; idx++)
    {
        ConcurrencyAppPipe * pipe = &concurrencyPipeList[idx];

        if (pipe->sippFwHandle == pPipeline)
        {
            return idx;
        }
    }

    return 0xFF;
}

void appSippCallback (SippPipeline *             pPipeline,
                      eSIPP_PIPELINE_EVENT       eEvent,
                      SIPP_PIPELINE_EVENT_DATA * ptEventData)
{
    // Find pipeline
    u32                  pipeIdx = appLookupHandle (pPipeline);
    ConcurrencyAppPipe * pipe    = &concurrencyPipeList[pipeIdx];

    UNUSED(ptEventData);

    if (eEvent == eSIPP_PIPELINE_FRAME_DONE)
    {
        printf ("Pipe %lu completes frame\n", pipeIdx);
        appLogEvent (pipe,
                     CC_APP_PIPE_EVENT_FRAME_DONE);
    }

}

u32 appCheckComplete ( )
{
    u32 retVal = 0x1;
    u32 idx;

    for (idx = 0; idx < SIPP_NUM_PIPELINES; idx++ )
    {
        ConcurrencyAppPipe * pipe = &concurrencyPipeList[idx];
        if (pipe->eState != CC_APP_PIPE_STATE_COMPLETE)
        {
            retVal = 0x0;
            break;
        }
    }

    return retVal;
}

void appTask ()
{
    u32 pipeIdx;

    while (testComplete == 0)
    {
        for (pipeIdx = 0x0; pipeIdx < SIPP_NUM_PIPELINES; pipeIdx++)
        {
            ConcurrencyAppPipe * pipe = &concurrencyPipeList[pipeIdx];

            switch (pipe->eState)
            {
                case CC_APP_PIPE_STATE_UNINIT   : {
                                                      pipe->eState = CC_APP_PIPE_STATE_IDLE;
                                                  } break;
                case CC_APP_PIPE_STATE_IDLE     : {
                                                      if (pipe->framesComplete < SIPP_TEST_NUM_FRAMES)
                                                      {
                                                          printf("Pipe %lu call process frame async\n", pipeIdx);
                                                          sippProcessFrameNB (pipe->sippFwHandle);
                                                          pipe->eState = CC_APP_PIPE_STATE_RUNNING;

                                                      }
                                                      else
                                                      {
                                                          printf ("Pipe %lu completed run of %lu images\n", pipeIdx, pipe->framesComplete);
                                                          sippFrameworkRun ();
                                                          pipe->eState = CC_APP_PIPE_STATE_COMPLETE;
                                                      }

                                                  } break;
                case CC_APP_PIPE_STATE_RUNNING  : {
                                                      CC_APP_PIPE_EVENT event;
                                                      while ((event = appProcessEvent (pipe)) != CC_APP_PIPE_EVENT_NONE)
                                                      {
                                                          switch (event)
                                                          {
                                                              case CC_APP_PIPE_EVENT_FRAME_DONE : {
                                                                                                      pipe->framesComplete++;
                                                                                                      pipe->eState = CC_APP_PIPE_STATE_IDLE;
                                                                                                  } break;
                                                              default : break;
                                                          }
                                                      }
                                                  } break;
                case CC_APP_PIPE_STATE_COMPLETE : {
                                                      if (appCheckComplete())
                                                      {
                                                          testComplete = 1;
                                                      }
                                                  } break;
                default                         : {
                                                      printf ("Unknown state on pipe %lu - Terminating early\n", pipeIdx);
                                                      exit (0x0);
                                                  } break;
            }
        }
    }
}

int main (int argc, char *argv[])
{
   UNUSED (argc);
   UNUSED (argv);

   sippPlatformInit();
   sippPlatformInitAsync ();

   unitTestInit ();
   DrvTimerInit ();
   DrvDdrInitialise (NULL);

   buildISPPipe ( );
   sippRdFileU8((UInt8*)iBufA, SIPP_ISP_PIPE_BUFFER_WIDTH * SIPP_ISP_PIPE_BUFFER_HEIGHT * sizeof(UInt16), "../../../../../../../resources/lowLight_512x128.raw");
   ISPPipeCreateParams ();
   configISPPipe ( );

   buildConvPipe ( );
   sippRdFileU8(iBufB, SIPP_CONV_PIPE_BUFFER_WIDTH * SIPP_CONV_PIPE_BUFFER_HEIGHT, "../../../../../../../resources/lena_512x512_luma.raw");
   configConvPipe ( );

   appTask ();

   sippWrFileU8(oBufA, SIPP_ISP_PIPE_BUFFER_WIDTH * SIPP_ISP_PIPE_BUFFER_HEIGHT * 3,  "outA_512x128_P420.yuv");
   sippWrFileU8(oBufB, SIPP_CONV_PIPE_BUFFER_WIDTH * SIPP_CONV_PIPE_BUFFER_HEIGHT,  "outB_512x512.raw");

   #if defined(MA2450)
   sippRdFileU8 ((UInt8*)iBufA, SIPP_ISP_PIPE_BUFFER_WIDTH * SIPP_ISP_PIPE_BUFFER_HEIGHT * 3/2, "../../../../../../../resources/refA_multiStream_ma2450_512x128_P420.yuv");
   sippDbgCompareU8((UInt8*)iBufA, oBufA, SIPP_ISP_PIPE_BUFFER_WIDTH * SIPP_ISP_PIPE_BUFFER_HEIGHT * 3/2);
   #endif

   sippRdFileU8 (iBufB, SIPP_CONV_PIPE_BUFFER_WIDTH * SIPP_CONV_PIPE_BUFFER_HEIGHT, "../../../../../../../resources/refB_multiStream_512x512.raw");
   sippDbgCompareU8(iBufB, oBufB, SIPP_CONV_PIPE_BUFFER_WIDTH * SIPP_CONV_PIPE_BUFFER_HEIGHT);

   unitTestFinalReport();
   return 0;
}
