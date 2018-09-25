///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application to test running SIPP pipelines concurrently
///            This test uses pipelines which have no conflicting SHAVE or
///            HW filter resource and therefore should be capable of running
///            concurrently

#include <sipp.h>
#include <sippTestCommon.h>
#include <filters/conv5x5/conv5x5.h>
#include <filters/sobel/sobel.h>
#include <filters/erode3x3/erode3x3.h>
#include <filters/dilate3x3/dilate3x3.h>
#include <filters/boxFilter5x5/boxFilter5x5.h>
#include <filters/laplacian5x5/laplacian5x5.h>
#include <filters/genLuma/genLuma.h>
#include <filters/lumaBlur/lumaBlur.h>
#include <filters/genChroma/genChroma.h>
#include <string.h>

#if defined(SIPP_VCS)
#include <UnitTestApi.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#endif

#include "ConcurrencyAppTypes.h"

/////////////////////////////////////////////////////
// Test specific macros
/////////////////////////////////////////////////////

#define CONCURRENCY_APP_NUM_PIPELINES       0x3
#define CONCURRENCY_APP_FRAMES_PER_PIPE     0x3
#define CONCURRENCY_TEST_CREATE_CONFLICT    0            // Set non-zero to create conflicts among the SHAVE resource requests
                                                         // which cause the pipelines to be run serially

// Pipeline 0 Setup
#define CONCURRENCY_APP_PIPE0_WIDTH         522
#define CONCURRENCY_APP_PIPE0_HEIGHT        128
#define CONCURRENCY_APP_PIPE0_NUM_PLANES    1
#define CONCURRENCY_APP_PIPE0_ASYNC         0            // Mark this pipe as sync
typedef u16  t_Pipe0;

// Pipeline 1 Setup
#define CONCURRENCY_APP_PIPE1_WIDTH         1280
#define CONCURRENCY_APP_PIPE1_HEIGHT        720
#define CONCURRENCY_APP_PIPE1_NUM_PLANES    1
#define CONCURRENCY_APP_PIPE1_ASYNC         1
typedef u8  t_Pipe1;

#if CONCURRENCY_APP_NUM_PIPELINES > 0x2
// Pipeline 2 Setup
#define CONCURRENCY_APP_PIPE2_WIDTH         1280
#define CONCURRENCY_APP_PIPE2_HEIGHT        720
#define CONCURRENCY_APP_PIPE2_NUM_PLANES    1
#define CONCURRENCY_APP_PIPE2_ASYNC         1
typedef u8  t_Pipe2;

#endif

#if CONCURRENCY_APP_NUM_PIPELINES > 0x3
// Pipeline 3 Setup
#define CONCURRENCY_APP_PIPE3_WIDTH         1280
#define CONCURRENCY_APP_PIPE3_HEIGHT        720
#define CONCURRENCY_APP_PIPE3_NUM_PLANES    1
#define CONCURRENCY_APP_PIPE3_ASYNC         1
typedef u8  t_Pipe3;

#endif


/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

#include "appParams.h"

u32    sippErrorLog[SIPP_ERROR_HISTORY_SIZE];
volatile u32       testComplete = 0;
ConcurrencyAppPipe concurrencyPipeList[CONCURRENCY_APP_NUM_PIPELINES];

#if CONCURRENCY_APP_NUM_PIPELINES > 0x3
#if CONCURRENCY_TEST_CREATE_CONFLICT > 0
u32                sliceFirst[CONCURRENCY_APP_NUM_PIPELINES] = {0,3,6,9};
u32                sliceLast[CONCURRENCY_APP_NUM_PIPELINES]  = {3,6,9,11};
#else
u32                sliceFirst[CONCURRENCY_APP_NUM_PIPELINES] = {0,3,6,9};
u32                sliceLast[CONCURRENCY_APP_NUM_PIPELINES]  = {2,5,8,11};
#endif
#elif CONCURRENCY_APP_NUM_PIPELINES > 0x2
#if CONCURRENCY_TEST_CREATE_CONFLICT > 0
u32                sliceFirst[CONCURRENCY_APP_NUM_PIPELINES] = {0,4,8};
u32                sliceLast[CONCURRENCY_APP_NUM_PIPELINES]  = {4,8,11};
#else
u32                sliceFirst[CONCURRENCY_APP_NUM_PIPELINES] = {0,4,8};
u32                sliceLast[CONCURRENCY_APP_NUM_PIPELINES]  = {3,7,11};
#endif
#else
#if CONCURRENCY_TEST_CREATE_CONFLICT > 0
u32                sliceFirst[CONCURRENCY_APP_NUM_PIPELINES] = {0,6};
u32                sliceLast[CONCURRENCY_APP_NUM_PIPELINES]  = {6,11};
#else
u32                sliceFirst[CONCURRENCY_APP_NUM_PIPELINES] = {0,6};
u32                sliceLast[CONCURRENCY_APP_NUM_PIPELINES]  = {5,11};
#endif
#endif

u8 *               pmBinImg[CONCURRENCY_APP_NUM_PIPELINES];  // How do we get the different binaries? Or do we use one binary with different entry points?? Programmers Guide...


t_Pipe0 DDR_DATA DMA0_InputBuffer[CONCURRENCY_APP_PIPE0_WIDTH * CONCURRENCY_APP_PIPE0_HEIGHT * CONCURRENCY_APP_PIPE0_NUM_PLANES] ALIGNED(8);
t_Pipe1 DDR_DATA DMA1_InputBuffer[CONCURRENCY_APP_PIPE1_WIDTH * CONCURRENCY_APP_PIPE1_HEIGHT * CONCURRENCY_APP_PIPE1_NUM_PLANES] ALIGNED(8);
#if CONCURRENCY_APP_NUM_PIPELINES > 0x2
t_Pipe2 DDR_DATA DMA2_InputBuffer[CONCURRENCY_APP_PIPE2_WIDTH * CONCURRENCY_APP_PIPE2_HEIGHT * CONCURRENCY_APP_PIPE2_NUM_PLANES] ALIGNED(8);
#endif
#if CONCURRENCY_APP_NUM_PIPELINES > 0x3
t_Pipe3 DDR_DATA DMA3_InputBuffer[CONCURRENCY_APP_PIPE3_WIDTH * CONCURRENCY_APP_PIPE3_HEIGHT * CONCURRENCY_APP_PIPE3_NUM_PLANES] ALIGNED(8);
#endif

t_Pipe0 DDR_DATA DMA0_OutputBuffer[CONCURRENCY_APP_PIPE0_WIDTH * CONCURRENCY_APP_PIPE0_HEIGHT * 3 ] ALIGNED(8);
t_Pipe1 DDR_DATA DMA1_OutputBuffer[CONCURRENCY_APP_PIPE1_WIDTH * CONCURRENCY_APP_PIPE1_HEIGHT * CONCURRENCY_APP_PIPE1_NUM_PLANES] ALIGNED(8);
#if CONCURRENCY_APP_NUM_PIPELINES > 0x2
t_Pipe2 DDR_DATA DMA2_OutputBuffer[CONCURRENCY_APP_PIPE2_WIDTH * CONCURRENCY_APP_PIPE2_HEIGHT * CONCURRENCY_APP_PIPE2_NUM_PLANES] ALIGNED(8);
#endif
#if CONCURRENCY_APP_NUM_PIPELINES > 0x3
t_Pipe3 DDR_DATA DMA3_OutputBuffer[CONCURRENCY_APP_PIPE3_WIDTH * CONCURRENCY_APP_PIPE3_HEIGHT * CONCURRENCY_APP_PIPE3_NUM_PLANES] ALIGNED(8);
#endif

// This will be linked with makefile and represent the start address for test input image
extern UInt8 test_image;
u16 conv5x50Cfg_cMat[CONCURRENCY_APP_NUM_PIPELINES][25];

#ifdef SIPP_VCS
tyAuxClkDividerCfg auxClk[] =
{
    {0, 0, 0, 0} // Null Terminated List
};

tySocClockConfig clockConfig =
{
    .refClk0InputKhz         = 12000,           // Default 12Mhz input clock
    .refClk1InputKhz         = 0,               // Assume no secondary oscillator for now
    .targetPll0FreqKhz       = 500000,
    .targetPll1FreqKhz       = 0,               // will be set by DDR driver
    .clkSrcPll1              = CLK_SRC_REFCLK0, // Supply both PLLS from REFCLK0
    .masterClkDivNumerator   = 1,
    .masterClkDivDenominator = 1,
    .cssDssClockEnableMask   = DEFAULT_CORE_CSS_DSS_CLOCKS,
    .mssClockEnableMask      = -1,                            // Not enabling any MSS clocks for now
    .upaClockEnableMask      = DEFAULT_UPA_CLOCKS,
    .pAuxClkCfg              = auxClk
};

#endif

/////////////////////////////////////////////////////
// Function prototypes
/////////////////////////////////////////////////////

void initConvMat (SippFilter * conv5x5, u32 pipeId, u32 ks);
void appSippCallback (SippPipeline *             pPipeline,
                      eSIPP_PIPELINE_EVENT       eEvent,
                      SIPP_PIPELINE_EVENT_DATA * ptEventData);

/////////////////////////////////////////////////////
// Code
/////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appLookupHandle                                              //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void appInit ()
{
    memset (concurrencyPipeList, 0x0, sizeof(concurrencyPipeList));

    concurrencyPipeList[0].runAsync = CONCURRENCY_APP_PIPE0_ASYNC;
    concurrencyPipeList[1].runAsync = CONCURRENCY_APP_PIPE1_ASYNC;
    #if CONCURRENCY_APP_NUM_PIPELINES > 0x2
    concurrencyPipeList[2].runAsync = CONCURRENCY_APP_PIPE2_ASYNC;
    #endif
    #if CONCURRENCY_APP_NUM_PIPELINES > 0x3
    concurrencyPipeList[3].runAsync = CONCURRENCY_APP_PIPE3_ASYNC;
    #endif

    sippRdFileU8((UInt8*)DMA0_InputBuffer,CONCURRENCY_APP_PIPE0_WIDTH * CONCURRENCY_APP_PIPE0_HEIGHT * CONCURRENCY_APP_PIPE0_NUM_PLANES * sizeof(UInt16),"../../../../../../../resources/test_522x128.raw");
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appLookupHandle                                              //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

u32 appLookupHandle (SippPipeline * pPipeline)
{
    u32 idx;

    for (idx = 0; idx < CONCURRENCY_APP_NUM_PIPELINES; idx++)
    {
        ConcurrencyAppPipe * pipe = &concurrencyPipeList[idx];

        if (pipe->sippFwHandle == pPipeline)
        {
            return idx;
        }
    }

    return 0xFF;
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appCreatePipelines                                           //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void appCreatePipelines ( )
{
    u32 idx;
    for (idx = 0; idx < CONCURRENCY_APP_NUM_PIPELINES; idx++ )
    {
        ConcurrencyAppPipe * pipe = &concurrencyPipeList[idx];

        pipe->sippFwHandle = sippCreatePipeline (sliceFirst[idx],
                                                 sliceLast[idx],
                                                 SIPP_MBIN(mbinImgSipp));

        if (sippPipeGetErrorStatus (pipe->sippFwHandle))
        {
            u32 errNum;
            printf ("Pipeline creation error\nError codes:\n");
            errNum = sippGetErrorHistory (sippErrorLog);
            while (errNum)
            {
                printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
                errNum--;
            }
            exit (0x0);
        }

        // Register callback for async API
        sippRegisterEventCallback (pipe->sippFwHandle,
                                   appSippCallback);
    }
}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appConfigurePipelines                                        //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void appConfigurePipelines ( )
{
    ConcurrencyAppPipe * pipe;
    DmaParam           * dmaInCfg;
    DmaParam           * dmaOutCfg;

    // Pipeline 0 - create and config filters
    pipe           = &concurrencyPipeList[0];

    pipe->dmaIn           =  sippCreateFilter(pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(1), SZ(UInt16), SIPP_AUTO,       (FnSvuRun)SIPP_DMA_ID,        0);
    pipe->lscMa2x5x0      =  sippCreateFilter(pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(1), SZ(UInt16), SIPP_AUTO,          (FnSvuRun)SIPP_LSC_ID,        0);
    pipe->rawMa2x5x0      =  sippCreateFilter(pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(1), SZ(UInt8), SIPP_AUTO,          (FnSvuRun)SIPP_RAW_ID,        0);
    pipe->debayerMa2x5x0  =  sippCreateFilter(pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(3),  SZ(UInt8), SIPP_AUTO,          (FnSvuRun)SIPP_DBYR_ID,       0);
    sippFilterAddOBuf(pipe->debayerMa2x5x0, 1, sizeof(UInt8));
    pipe->genLuma0        =  sippCreateFilter(pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(1),  SZ(UInt8), 0,                  SVU_SYM(svuGenLuma),          0);
    pipe->lumaBlur0       =  sippCreateFilter(pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(1),  SZ(UInt8), 0,                  SVU_SYM(svuLumaBlur),         0);
    pipe->genChroma0      =  sippCreateFilter(pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(3),  SZ(UInt8), SZ(ChrGenParam),    SVU_SYM(svuGenChroma),        0);
    pipe->sharpenMa2x5x1  =  sippCreateFilter(pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(1),  SZ(UInt8), SIPP_AUTO,          (FnSvuRun)SIPP_SHARPEN_ID,    0);
    pipe->ydnsMa2x5x1     =  sippCreateFilter(pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(1),  SZ(fp16), SIPP_AUTO,          (FnSvuRun)SIPP_LUMA_ID,       0);
    pipe->polyphasefirMa2x5x1 =  sippCreateFilter(pipe->sippFwHandle, SIPP_RESIZE, CONCURRENCY_APP_PIPE0_WIDTH/2, CONCURRENCY_APP_PIPE0_HEIGHT/2, N_PL(3),  SZ(UInt8), SIPP_AUTO,          (FnSvuRun)SIPP_UPFIRDN_ID,    0);
    pipe->medianMa2x5x1    =  sippCreateFilter(pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH/2, CONCURRENCY_APP_PIPE0_HEIGHT/2, N_PL(3),  SZ(UInt8), SIPP_AUTO,          (FnSvuRun)SIPP_MED_ID,        0);
    pipe->chromadnsMa2x5x1 =  sippCreateFilter(pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH/2, CONCURRENCY_APP_PIPE0_HEIGHT/2, N_PL(3),  SZ(UInt8), SIPP_AUTO,          (FnSvuRun)SIPP_CHROMA_ID,     0);
    pipe->colorcombMa2x5x1 =  sippCreateFilter(pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(3),  SZ(half), SIPP_AUTO,          (FnSvuRun)SIPP_CC_ID,         0);
    pipe->lutMa2x5x1       =  sippCreateFilter(pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(3),  SZ(UInt8), SIPP_AUTO,          (FnSvuRun)SIPP_LUT_ID,        0);
    pipe->dmaOut           =  sippCreateFilter(pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE0_WIDTH, CONCURRENCY_APP_PIPE0_HEIGHT, N_PL(3),  SZ(UInt8), SIPP_AUTO,          (FnSvuRun)SIPP_DMA_ID,        0);


    sippLinkFilter(pipe->lscMa2x5x0,       pipe->dmaIn ,              1,0    );
    sippLinkFilter(pipe->rawMa2x5x0,       pipe->lscMa2x5x0,          5,0    );
    sippLinkFilter(pipe->debayerMa2x5x0,   pipe->rawMa2x5x0,          11,11  );
    sippLinkFilter(pipe->genLuma0,         pipe->debayerMa2x5x0,      1,0    );
    sippLinkFilter(pipe->lumaBlur0,        pipe->genLuma0,            3,3    );
    sippLinkFilter(pipe->genChroma0,       pipe->debayerMa2x5x0,      1,1    );
    sippLinkFilter(pipe->genChroma0,       pipe->lumaBlur0,           1,1    );
    sippLinkFilter(pipe->sharpenMa2x5x1,   pipe->genLuma0,            7,7    );
    sippLinkFilter(pipe->ydnsMa2x5x1,      pipe->sharpenMa2x5x1,      11,11  );
    sippLinkFilter(pipe->polyphasefirMa2x5x1, pipe->genChroma0,       7,7    );
    sippLinkFilter(pipe->medianMa2x5x1,    pipe->polyphasefirMa2x5x1, 3,3    );
    sippLinkFilter(pipe->chromadnsMa2x5x1, pipe->medianMa2x5x1,       3,3    );
    sippLinkFilter(pipe->colorcombMa2x5x1, pipe->ydnsMa2x5x1,         1,1    );
    sippLinkFilter(pipe->colorcombMa2x5x1, pipe->chromadnsMa2x5x1,    5,5    );
    sippLinkFilter(pipe->lutMa2x5x1,       pipe->colorcombMa2x5x1,    1,1    );
    sippLinkFilter(pipe->dmaOut,           pipe->debayerMa2x5x0,      1,1    );

    if (sippPipeGetErrorStatus (pipe->sippFwHandle))
    {
        u32 errNum;
        printf ("Pipeline 0 creation error\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
        exit (0x0);
    }

    DmaParam            *dmaIn0Cfg          = (DmaParam         *)pipe->dmaIn->params;
    LscParam            *lscMa2x5x0Cfg      = (LscParam         *)pipe->lscMa2x5x0->params;
    RawParam            *rawMa2x5x0Cfg      = (RawParam         *)pipe->rawMa2x5x0->params;
    DbyrParam           *debayerMa2x5x0Cfg  = (DbyrParam        *)pipe->debayerMa2x5x0->params;
    ChrGenParam         *genChroma0Cfg      = (ChrGenParam      *)pipe->genChroma0->params;
    UsmParam            *sharpenMa2x5x1Cfg  = (UsmParam         *)pipe->sharpenMa2x5x1->params;
    YDnsParam           *ydnsMa2x5x1Cfg     = (YDnsParam        *)pipe->ydnsMa2x5x1->params;
    PolyFirParam        *polyphasefirMa2x5x1Cfg= (PolyFirParam     *)pipe->polyphasefirMa2x5x1->params;
    MedParam            *medianMa2x5x1Cfg   = (MedParam         *)pipe->medianMa2x5x1->params;
    ChrDnsParam         *chromadnsMa2x5x1Cfg= (ChrDnsParam      *)pipe->chromadnsMa2x5x1->params;
    ColCombParam        *colorcombMa2x5x1Cfg= (ColCombParam     *)pipe->colorcombMa2x5x1->params;
    LutParam            *lutMa2x5x1Cfg      = (LutParam         *)pipe->lutMa2x5x1->params;
    DmaParam            *dmaOut1Cfg         = (DmaParam         *)pipe->dmaOut->params;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // here go through all the filter instances and assign the value for all parameter
    // fields as defined in the filter property window
    lscMa2x5x0Cfg->gmBase=   (UInt16*)lscMesh;
    lscMa2x5x0Cfg->gmWidth=  LSC_GAIN_MAP_W;
    lscMa2x5x0Cfg->gmHeight= LSC_GAIN_MAP_H;
    lscMa2x5x0Cfg->dataFormat=BAYER;
    lscMa2x5x0Cfg->dataWidth=BAYER_BITS;
    rawMa2x5x0Cfg->grgbPlat= RAW_GRGB_PLATO(GRGB_IMBAL_PLAT_DARK,GRGB_IMBAL_PLAT_BRIGHT);
    rawMa2x5x0Cfg->grgbDecay=RAW_GRGB_DECAY(GRGB_IMBAL_DECAY_DARK,GRGB_IMBAL_DECAY_BRIGHT);
    rawMa2x5x0Cfg->badPixCfg=RAW_BAD_PIX_CFG(BAD_PIX_NOISE_LEVEL, BAD_PIX_ALPHA_G_HOT, BAD_PIX_ALPHA_G_COLD, BAD_PIX_ALPHA_RB_HOT, BAD_PIX_ALPHA_RB_COLD);
    rawMa2x5x0Cfg->cfg=      RAW_CFG(BAYER,GRBG,0,0,0,0,(BAYER_BITS - 1),(GRGB_IMBAL_THRESHOLD << 16),0,0,1,0,1);
    rawMa2x5x0Cfg->gainSat[0]=( DIGI_GAIN_GR | (DIGI_SAT_GR << 16));
    rawMa2x5x0Cfg->gainSat[1]=(DIGI_GAIN_R  | (DIGI_SAT_R  << 16));
    rawMa2x5x0Cfg->gainSat[2]=(DIGI_GAIN_B  | (DIGI_SAT_B  << 16));
    rawMa2x5x0Cfg->gainSat[3]=(DIGI_GAIN_GB | (DIGI_SAT_GB << 16));
    rawMa2x5x0Cfg->statsBase=0;
    rawMa2x5x0Cfg->statsPlanes=0;
    rawMa2x5x0Cfg->statsPatchCfg=0;
    rawMa2x5x0Cfg->statsPatchStart=0;
    rawMa2x5x0Cfg->statsPatchSkip=0;
    rawMa2x5x0Cfg->statsThresh=0;
    rawMa2x5x0Cfg->afF1coefs[0]=0;  rawMa2x5x0Cfg->afF1coefs[1]=0;  rawMa2x5x0Cfg->afF1coefs[2]=0;
    rawMa2x5x0Cfg->afF1coefs[3]=0;  rawMa2x5x0Cfg->afF1coefs[4]=0;  rawMa2x5x0Cfg->afF1coefs[5]=0;
    rawMa2x5x0Cfg->afF1coefs[6]=0;  rawMa2x5x0Cfg->afF1coefs[7]=0;  rawMa2x5x0Cfg->afF1coefs[8]=0;
    rawMa2x5x0Cfg->afF1coefs[9]=0;  rawMa2x5x0Cfg->afF1coefs[10]=0; rawMa2x5x0Cfg->afF2coefs[0]=0;
    rawMa2x5x0Cfg->afF2coefs[1]=0;  rawMa2x5x0Cfg->afF2coefs[2]=0;  rawMa2x5x0Cfg->afF2coefs[3]=0;
    rawMa2x5x0Cfg->afF2coefs[4]=0;  rawMa2x5x0Cfg->afF2coefs[5]=0;  rawMa2x5x0Cfg->afF2coefs[6]=0;
    rawMa2x5x0Cfg->afF2coefs[7]=0;  rawMa2x5x0Cfg->afF2coefs[8]=0;  rawMa2x5x0Cfg->afF2coefs[9]=0;
    rawMa2x5x0Cfg->afF2coefs[10]=0;
    rawMa2x5x0Cfg->afMinThresh=0;
    rawMa2x5x0Cfg->afSubtract=0;
    rawMa2x5x0Cfg->afPatchCfg=0;
    rawMa2x5x0Cfg->afPatchStart=0;
    rawMa2x5x0Cfg->afStatsBase=0;
    rawMa2x5x0Cfg->histLumaBase=0;
    rawMa2x5x0Cfg->histRgbBase=0;
    debayerMa2x5x0Cfg->cfg=  DEBAYER_CFG(GRBG,0 ,0,8,8,P_RGB,3,0);
    debayerMa2x5x0Cfg->thresh=DEBAYER_THRESH(0,0,1,0,0,0);
    debayerMa2x5x0Cfg->dewormCfg=0;
    debayerMa2x5x0Cfg->lumaWeight=0;
    genChroma0Cfg->epsilon=  1;
    sharpenMa2x5x1Cfg->cfg=  SHARPEN_CFG(SHARP_MIN_THR,7,1,0,0);
    sharpenMa2x5x1Cfg->strength=SHARPEN_STRENGTH_CFG(SHARP_STRENGTH, SHARP_STRENGTH);
    sharpenMa2x5x1Cfg->clip= SHARP_ALPHA;
    sharpenMa2x5x1Cfg->limit=SHARPEN_LIMITS_CFG(SHARP_UNDERSHOOT,SHARP_OVERSHOOT);
    sharpenMa2x5x1Cfg->rgnStop01=SHARPEN_RANGESTOP01_CFG(SHARP_RANGE_STOP_0, SHARP_RANGE_STOP_1);
    sharpenMa2x5x1Cfg->rgnStop23=SHARPEN_RANGESTOP23_CFG(SHARP_RANGE_STOP_2, SHARP_RANGE_STOP_3);
    sharpenMa2x5x1Cfg->coef01=SHARPEN_COEFS01_CFG(sharpen_coeffs[0],sharpen_coeffs[1]);
    sharpenMa2x5x1Cfg->coef23=SHARPEN_COEFS23_CFG(sharpen_coeffs[2],sharpen_coeffs[3]);
    ydnsMa2x5x1Cfg->cfg=     LUMA_DNS_CFG (0, 0, LUMA_DNS_ALPHA, bitpos);
    ydnsMa2x5x1Cfg->gaussLut[0]=gaussLut[0];  ydnsMa2x5x1Cfg->gaussLut[1]=gaussLut[1];
    ydnsMa2x5x1Cfg->gaussLut[2]=gaussLut[2];  ydnsMa2x5x1Cfg->gaussLut[3]=gaussLut[3];
    ydnsMa2x5x1Cfg->f2=      LUMA_DNS_F2;
    ydnsMa2x5x1Cfg->gammaLut[0]=gammaLut[0];  ydnsMa2x5x1Cfg->gammaLut[1]=gammaLut[1];
    ydnsMa2x5x1Cfg->gammaLut[2]=gammaLut[2];  ydnsMa2x5x1Cfg->gammaLut[3]=gammaLut[3];
    ydnsMa2x5x1Cfg->gammaLut[4]=gammaLut[4];
    ydnsMa2x5x1Cfg->distCfg= (UInt32)&distLUT[0];
    ydnsMa2x5x1Cfg->distOffsets=LUMA_DNS_X_OFFSET | (LUMA_DNS_Y_OFFSET << 16);
    ydnsMa2x5x1Cfg->fullFrmDim=ISP_PIPE_INPUT_BUFFER_WIDTH | (ISP_PIPE_INPUT_BUFFER_HEIGHT << 16);
    polyphasefirMa2x5x1Cfg->clamp=0;
    polyphasefirMa2x5x1Cfg->horzD=HORZ_D;
    polyphasefirMa2x5x1Cfg->horzN=HORZ_N;
    polyphasefirMa2x5x1Cfg->vertD=VERT_D;
    polyphasefirMa2x5x1Cfg->vertN=VERT_N;
    polyphasefirMa2x5x1Cfg->horzCoefs=horzCoefs;
    polyphasefirMa2x5x1Cfg->vertCoefs=vertCoefs;
    polyphasefirMa2x5x1Cfg->planeMode=POLY_PLANE_ALL;
    medianMa2x5x1Cfg->cfg=   MED_CFG(CHROMA_MEDIAN_SIZE,((CHROMA_MEDIAN_SIZE * CHROMA_MEDIAN_SIZE)>>1),0,0,0,0);
    medianMa2x5x1Cfg->lumaAlpha=0;
    chromadnsMa2x5x1Cfg->cfg= CHROMA_DNS_CFG(CHROMA_DNS_H_ENAB,CHROMA_DNS_LIM,0,0,0,0,0);
    chromadnsMa2x5x1Cfg->thr[0]=(CHROMA_DNS_T1 <<  0) | (CHROMA_DNS_T2 <<  8) | (CHROMA_DNS_T1 << 16) | (CHROMA_DNS_T2 << 24);
    chromadnsMa2x5x1Cfg->thr[1]=0;
    chromadnsMa2x5x1Cfg->greyPt=CHORMA_GREY_PT_SET(0,0,0,1);
    chromadnsMa2x5x1Cfg->chrCoefs=CHROMA_DNS_COEFFS_SET(0x74,0x1c,0x07);
    colorcombMa2x5x1Cfg->cfg=CC_CFG(0,0,1,0,0,3);
    colorcombMa2x5x1Cfg->krgb[0]=(((UInt32)((255.0f*(1.0f/(255.0f/3.0f)))*256))<< 0)|(((UInt32)((255.0f*(1.0f/(255.0f/3.0f)))*256))<<16);
    colorcombMa2x5x1Cfg->krgb[1]=(UInt32)((255.0f*(1.0f/(255.0f/3.0f)))*256);
    colorcombMa2x5x1Cfg->ccm[0]=(CCM_COEF(ccm[0]) | (CCM_COEF(ccm[3])<<16));
    colorcombMa2x5x1Cfg->ccm[1]=(CCM_COEF(ccm[6]) | (CCM_COEF(ccm[1])<<16));
    colorcombMa2x5x1Cfg->ccm[2]=(CCM_COEF(ccm[4]) | (CCM_COEF(ccm[7])<<16));
    colorcombMa2x5x1Cfg->ccm[3]=(CCM_COEF(ccm[2]) | (CCM_COEF(ccm[5])<<16));
    colorcombMa2x5x1Cfg->ccm[4]=CCM_COEF(ccm[8]);
    colorcombMa2x5x1Cfg->ccOffs=0;
    colorcombMa2x5x1Cfg->threeDLut=0;
    colorcombMa2x5x1Cfg->lutFormat=0;
    lutMa2x5x1Cfg->cfg=      LUT_CFG(0, 0, 0, 0, 0, 16, 0,1);
    lutMa2x5x1Cfg->sizeA=    LUT_REGION_0_TO_7_SIZE(0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4);
    lutMa2x5x1Cfg->sizeB=    LUT_REGION_8_TO_15_SIZE(0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4);
    lutMa2x5x1Cfg->lut=      gammaTable;
    lutMa2x5x1Cfg->lutFormat=2;
    lutMa2x5x1Cfg->mat[0]=   0;  lutMa2x5x1Cfg->mat[1]=   0;  lutMa2x5x1Cfg->mat[2]=   0;
    lutMa2x5x1Cfg->mat[3]=   0;  lutMa2x5x1Cfg->mat[4]=   0;  lutMa2x5x1Cfg->mat[5]=   0;
    lutMa2x5x1Cfg->mat[6]=   0;  lutMa2x5x1Cfg->mat[7]=   0;  lutMa2x5x1Cfg->mat[8]=   0;
    lutMa2x5x1Cfg->offset[0]=0;
    lutMa2x5x1Cfg->offset[1]=0;
    lutMa2x5x1Cfg->offset[2]=0;
    dmaIn0Cfg->ddrAddr   = (UInt32)DMA0_InputBuffer;
    dmaOut1Cfg->ddrAddr  = (UInt32)DMA0_OutputBuffer;

    sippFinalizePipeline (pipe->sippFwHandle);

    if (sippPipeGetErrorStatus (pipe->sippFwHandle))
    {
        u32 errNum;
        printf ("Pipeline 0 finalization failed\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
        exit (0x0);
    }

    // Pipeline 1 - create and config filters
    pipe              = &concurrencyPipeList[1];
    pipe->dmaIn       =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE1_WIDTH, CONCURRENCY_APP_PIPE1_HEIGHT, N_PL(CONCURRENCY_APP_PIPE1_NUM_PLANES), SZ(t_Pipe1), SIPP_AUTO,          (FnSvuRun)SIPP_DMA_ID, 0);
    pipe->sobel0      =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE1_WIDTH, CONCURRENCY_APP_PIPE1_HEIGHT, N_PL(CONCURRENCY_APP_PIPE1_NUM_PLANES), SZ(t_Pipe1), 0,                  SVU_SYM(svuSobel),     0);
    pipe->erode3x30   =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE1_WIDTH, CONCURRENCY_APP_PIPE1_HEIGHT, N_PL(CONCURRENCY_APP_PIPE1_NUM_PLANES), SZ(t_Pipe1), SZ(Erode3x3Param),  SVU_SYM(svuErode3x3),  0);
    pipe->dilate3x30  =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE1_WIDTH, CONCURRENCY_APP_PIPE1_HEIGHT, N_PL(CONCURRENCY_APP_PIPE1_NUM_PLANES), SZ(t_Pipe1), SZ(Dilate3x3Param), SVU_SYM(svuDilate3x3), 0);
    pipe->lut0        =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE1_WIDTH, CONCURRENCY_APP_PIPE1_HEIGHT, N_PL(CONCURRENCY_APP_PIPE1_NUM_PLANES), SZ(t_Pipe1), SIPP_AUTO,          (FnSvuRun)SIPP_LUT_ID, 0);
    pipe->dmaOut      =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE1_WIDTH, CONCURRENCY_APP_PIPE1_HEIGHT, N_PL(CONCURRENCY_APP_PIPE1_NUM_PLANES), SZ(t_Pipe1), SIPP_AUTO,          (FnSvuRun)SIPP_DMA_ID, 0);


    sippLinkFilter (pipe->sobel0,     pipe->dmaIn,       3, 3 );
    sippLinkFilter (pipe->erode3x30,  pipe->sobel0,      3, 3 );
    sippLinkFilter (pipe->dilate3x30, pipe->erode3x30,   3, 3 );
    sippLinkFilter (pipe->lut0,       pipe->dilate3x30,  1, 1 );
    sippLinkFilter (pipe->dmaOut,     pipe->lut0,        1, 1 );

    if (sippPipeGetErrorStatus (pipe->sippFwHandle))
    {
        u32 errNum;
        printf ("Pipeline 1 creation error\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
        exit (0x0);
    }

    dmaInCfg           = (DmaParam *)pipe->dmaIn->params;
    dmaOutCfg          = (DmaParam *)pipe->dmaOut->params;
    dmaInCfg->ddrAddr  = (UInt32)&test_image;
    dmaOutCfg->ddrAddr = (UInt32)DMA1_OutputBuffer;

    Erode3x3Param       *erode3x30Cfg       = (Erode3x3Param    *)pipe->erode3x30->params;
    Dilate3x3Param      *dilate3x30Cfg      = (Dilate3x3Param   *)pipe->dilate3x30->params;
    LutParam            *lut0Cfg            = (LutParam         *)pipe->lut0->params;

    erode3x30Cfg->eMat[0]=   &erodeMat[0];
    erode3x30Cfg->eMat[1]=   &erodeMat[3];
    erode3x30Cfg->eMat[2]=   &erodeMat[6];
    dilate3x30Cfg->dMat[0]=  &dilateMat[0];
    dilate3x30Cfg->dMat[1]=  &dilateMat[3];
    dilate3x30Cfg->dMat[2]=  &dilateMat[6];
    lut0Cfg->cfg=            (0      )  | (0  << 1)  | (8  << 3) | (0  << 8);
    lut0Cfg->sizeA=          0x44444444;
    lut0Cfg->sizeB=          0x44444444;
    lut0Cfg->lut=            lutTab;

    sippFinalizePipeline (pipe->sippFwHandle);

    if (sippPipeGetErrorStatus (pipe->sippFwHandle))
    {
        u32 errNum;
        printf ("Pipeline 1 finalization failed\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
        exit (0x0);
    }

    #if CONCURRENCY_APP_NUM_PIPELINES > 2
    // Pipeline 2 - create and config filters
    pipe           = &concurrencyPipeList[2];

    pipe->dmaIn    =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE2_WIDTH, CONCURRENCY_APP_PIPE2_HEIGHT, N_PL(CONCURRENCY_APP_PIPE2_NUM_PLANES), SZ(t_Pipe2), SIPP_AUTO,             (FnSvuRun)SIPP_DMA_ID, 0);
    pipe->conv5x5  =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE2_WIDTH, CONCURRENCY_APP_PIPE2_HEIGHT, N_PL(CONCURRENCY_APP_PIPE2_NUM_PLANES), SZ(t_Pipe2), SZ(Conv5x5Param),      SVU_SYM(svuConv5x5),   0);
    pipe->box5x5   =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE2_WIDTH, CONCURRENCY_APP_PIPE2_HEIGHT, N_PL(CONCURRENCY_APP_PIPE2_NUM_PLANES), SZ(t_Pipe2), SZ(BoxFilter5x5Param), SVU_SYM(svuBoxFilter5x5),0);
    pipe->lap5x5   =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE2_WIDTH, CONCURRENCY_APP_PIPE2_HEIGHT, N_PL(CONCURRENCY_APP_PIPE2_NUM_PLANES), SZ(t_Pipe2), SZ(0),                 SVU_SYM(svuLaplacian5x5), 0);
    pipe->dmaOut   =  sippCreateFilter (pipe->sippFwHandle, 0x0, CONCURRENCY_APP_PIPE2_WIDTH, CONCURRENCY_APP_PIPE2_HEIGHT, N_PL(CONCURRENCY_APP_PIPE2_NUM_PLANES), SZ(t_Pipe2), SIPP_AUTO,             (FnSvuRun)SIPP_DMA_ID, 0);


    sippLinkFilter (pipe->conv5x5, pipe->dmaIn,   5, 5 );
    sippLinkFilter (pipe->box5x5,  pipe->conv5x5, 5, 5 );
    sippLinkFilter (pipe->lap5x5,  pipe->box5x5,  5, 5 );
    sippLinkFilter (pipe->dmaOut,  pipe->lap5x5,  1, 0 );

    if (sippPipeGetErrorStatus (pipe->sippFwHandle))
    {
        u32 errNum;
        printf ("Pipeline 2 creation error\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
        exit (0x0);
    }

    dmaInCfg           = (DmaParam *)pipe->dmaIn->params;
    dmaOutCfg          = (DmaParam *)pipe->dmaOut->params;
    dmaInCfg->ddrAddr  = (UInt32)&test_image;
    dmaOutCfg->ddrAddr = (UInt32)DMA2_OutputBuffer;

    initConvMat (pipe->conv5x5, 0x1, KERNEL_SIZE);

    BoxFilter5x5Param *  boxCfg = (BoxFilter5x5Param *)pipe->box5x5->params;
    boxCfg->normalize  = 0;

    sippFinalizePipeline (pipe->sippFwHandle);

    if (sippPipeGetErrorStatus (pipe->sippFwHandle))
    {
        u32 errNum;
        printf ("Pipeline 2 finalization failed\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
        exit (0x0);
    }

    #endif

    #if CONCURRENCY_APP_NUM_PIPELINES > 3
    // Pipeline 3 - create and config filters
    pipe           = &concurrencyPipeList[3];

    #endif

}

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appLogEvent                                                  //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appProcessEvent                                              //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appSippCallback                                              //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appCheckComplete                                             //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

u32 appCheckComplete ( )
{
    u32 retVal = 0x1;
    u32 idx;

    for (idx = 0; idx < CONCURRENCY_APP_NUM_PIPELINES; idx++ )
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


////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    appTask                                                      //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

void appTask ()
{
    u32 pipeIdx;

    while (testComplete == 0)
    {
        for (pipeIdx = 0x0; pipeIdx < CONCURRENCY_APP_NUM_PIPELINES; pipeIdx++)
        {
            ConcurrencyAppPipe * pipe = &concurrencyPipeList[pipeIdx];

            switch (pipe->eState)
            {
                case CC_APP_PIPE_STATE_UNINIT   : {
                                                      pipe->eState = CC_APP_PIPE_STATE_IDLE;
                                                  } break;
                case CC_APP_PIPE_STATE_IDLE     : {
                                                      if (pipe->framesComplete < CONCURRENCY_APP_FRAMES_PER_PIPE)
                                                      {
                                                          if (pipe->runAsync)
                                                          {
                                                              printf("Pipe %lu call process frame async\n", pipeIdx);
                                                              sippProcessFrameNB (pipe->sippFwHandle);
                                                              pipe->eState = CC_APP_PIPE_STATE_RUNNING;
                                                          }
                                                          else
                                                          {
                                                              u32 errCode      = (u32)E_BLOCK_CALL_REJECTED;
                                                              u32 errMaskIdx   = errCode >> 5;
                                                              u32 errMaskEntry = 0x1 << (errCode & 0x1F);

                                                              // We wish to call the sync API
                                                              sippProcessFrame (pipe->sippFwHandle);

                                                              // Check if the call worked
                                                              // Interrogate the error
                                                              if (pipe->sippFwHandle->errorStatus[errMaskIdx] & errMaskEntry)
                                                              {
                                                                  printf ("Pipe %lu failed in call to blocking API sippProcessFrame due to active and/or pending operations\n", pipeIdx);
                                                                  // Clear the error
                                                                  pipe->sippFwHandle->errorStatus[errMaskIdx] &= (~errMaskEntry);
                                                              }
                                                              else
                                                              {
                                                                  printf ("Pipe %lu successfully called blocking API sippProcessFrame\n", pipeIdx);
                                                                  pipe->framesComplete++;
                                                                  pipe->eState = CC_APP_PIPE_STATE_IDLE;
                                                              }
                                                          }
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

////////////////////////////////////////////////////////////////////////////////
// MAIN

////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    main                                                         //
//                                                                            //
//  DESCRIPTION:                                                              //
//                                                                            //
//  INPUTS:                                                                   //
//                                                                            //
//  OUTPUTS:     None                                                         //
//                                                                            //
//  RETURNS:     None                                                         //
//                                                                            //
//  NOTES:       None                                                         //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

int main (int argc, char *argv[])
{
    UNUSED (argc);
    UNUSED (argv);

    #ifdef SIPP_VCS
    unitTestInit ();
    DrvCprInit();
    DrvDdrInitialise (NULL);
    DrvCprSetupClocks(&clockConfig);
    #endif

    sippPlatformInit ();

    #ifdef SIPP_VCS
    DrvTimerInit ();
    #endif

    #ifndef SIPP_PC
    if (PROCESS_LEON_OS == swcWhoAmI())
    {
        printf ("App Starting on LEON OS\n");
        printf ("Please make sure IRQ sources for dynamically route between LeonOS and LeonRT are set\n");
    }
    else
    {
    	printf ("App Starting on LEON RT\n");
    }
    #endif // SIPP_PC

    sippPlatformInitAsync ();

    appInit ();
    appCreatePipelines ();
    appConfigurePipelines ();

    // Load input data for pipes
    appTask ();

    #ifdef SIPP_VCS
    unitTestFinalReport();
    #endif

   return 0;
}
