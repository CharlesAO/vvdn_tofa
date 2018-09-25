///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Raw filter unit test modified from C model test...
///            /lookuptabletest/test.cpp

/////////////////////////////////////////////////////
// Include files
/////////////////////////////////////////////////////

#include <sipp.h>
#include <sippTestCommon.h>

#if defined(SIPP_VCS)
#include <UnitTestApi.h>
#include <DrvTimer.h>
#include <DrvDdr.h>
#endif

#include "testHwLut.h"

/////////////////////////////////////////////////////
// Test Macros
/////////////////////////////////////////////////////

#define SIPP_TEST_SYNC_API  // Set to use the synchronous API
//#define SIPP_USE_PRECALC_SCHEDULE

#ifdef SIPP_PC
//#define SIPP_LUT_TEST_GEN_INPUT
//#define SIPP_LUT_TEST_CREATE_LUT
#endif

typedef fp16                lut_itype;
typedef fp16                lut_ltype;
#define INCONV(x)           (x)*(1.0f/255)
#define LUTCONV(x)          (x)*(1.0f/255)
#define APBCONV(x)          x.getPackedValue()
#define OUTCONV(x)          (uint8_t)x

// Input buffer is arranged in planes and then within that channels
// each channel has a resolution of width x height


/////////////////////////////////////////////////////
// Global variables
/////////////////////////////////////////////////////

TestHwLut testLut;

#ifndef SIPP_TEST_SYNC_API
volatile u32 testComplete = 0;
#endif

// Gamma curve
const uint16_t tonal_curve[256] = {
  0,   3,   5,   8,  10,  13,  15,  17,
 20,  22,  25,  27,  30,  32,  34,  37,
 39,  41,  44,  46,  48,  51,  53,  55,
 57,  59,  62,  64,  66,  68,  70,  72,
 75,  77,  79,  81,  83,  85,  87,  89,
 91,  93,  95,  97,  99, 101, 103, 105,
106, 108, 110, 112, 114, 116, 117, 119,
121, 123, 124, 126, 128, 129, 131, 133,
134, 136, 137, 139, 141, 142, 144, 145,
147, 148, 150, 151, 153, 154, 155, 157,
158, 159, 161, 162, 164, 165, 166, 167,
169, 170, 171, 172, 174, 175, 176, 177,
178, 179, 181, 182, 183, 184, 185, 186,
187, 188, 189, 190, 191, 192, 193, 194,
195, 196, 197, 198, 199, 200, 201, 201,
202, 203, 204, 205, 206, 206, 207, 208,
209, 210, 210, 211, 212, 213, 213, 214,
215, 215, 216, 217, 217, 218, 219, 219,
220, 221, 221, 222, 222, 223, 224, 224,
225, 225, 226, 226, 227, 227, 228, 229,
229, 230, 230, 231, 231, 231, 232, 232,
233, 233, 234, 234, 235, 235, 235, 236,
236, 237, 237, 237, 238, 238, 239, 239,
239, 240, 240, 240, 241, 241, 241, 242,
242, 242, 243, 243, 243, 244, 244, 244,
244, 245, 245, 245, 245, 246, 246, 246,
247, 247, 247, 247, 247, 248, 248, 248,
248, 249, 249, 249, 249, 249, 250, 250,
250, 250, 250, 251, 251, 251, 251, 251,
252, 252, 252, 252, 252, 252, 253, 253,
253, 253, 253, 253, 254, 254, 254, 254,
254, 254, 254, 254, 255, 255, 255, 255,
};

static uint8_t  DDR_DATA SippHwLut_Ref_buffer[SIPP_HW_LUT_OUTPUT_BUFFER_SIZE];

#ifndef SIPP_LUT_TEST_CREATE_LUT
#include "testLut.h"
#endif

/////////////////////////////////////////////////////
// Code
/////////////////////////////////////////////////////

#ifdef SIPP_USE_PRECALC_SCHEDULE
#include "pcDumpSchedule.hh"
#endif

extern void * sippPalMemcpy (void *pDest, const void *pSrc, u32 uSize);

//#############################################################################
// Build the pipeline that's specific to our application

#ifdef SIPP_LUT_TEST_CREATE_LUT

void appCreateLut (lut_ltype * lbuf)
{
    int cnt1, cnt2, cnt3, cnt4;

    for (cnt1 = 0; cnt1 < LUTS_NO; cnt1++)
    {
        for (cnt2 = 0; cnt2 < 256; cnt2++)
        {
            for (cnt3 = 0; cnt3 < CHANNELS_NO; cnt3++)
            {
                *lbuf++ = LUTCONV(CLIP((tonal_curve[cnt2] - cnt3 + 10*cnt1), 0xFF));
            }
            for(cnt4 = cnt3; cnt4 < LUT_ALLOCATE; cnt4++)
            {
                *lbuf++ = LUTCONV(0);
            }
        }
    }

}
#endif

#ifdef SIPP_PC
void appPrintLut (lut_ltype * lbuf)
{
    FILE * lutFp = fopen ("testLut.h", "w");
    int    cnt1;

    fprintf(lutFp, "u16 testLUTBuf[] ALIGNED(8) = {\n");

    for  (cnt1 = 0; cnt1 < LUT_SIZE; cnt1 += 4)
    {
        fprintf(lutFp, "0x%04x,0x%04x,0x%04x,0x%04x,\n",
                       *((uint16_t*)&lbuf[cnt1 + 0x0]),
                       *((uint16_t*)&lbuf[cnt1 + 0x1]),
                       *((uint16_t*)&lbuf[cnt1 + 0x2]),
                       *((uint16_t*)&lbuf[cnt1 + 0x3]));
    }

    fprintf(lutFp, "};\n\n");
    fclose (lutFp);
}

#ifdef SIPP_LUT_TEST_GEN_INPUT

uint32_t appGenerateFp16LUT (u32    numIPlanes,
                             u32    numChannels,
                             u32    width,
                             u32    height,
                             FILE * lutFp,
                             FILE * outLutFp)
{
    uint8_t * convbuf = new uint8_t [width*height];
    lut_itype inputPix;
    lut_itype * pixPtr  = &inputPix;
    uint32_t  idx = 0;

    for(int pl = 0; pl < numIPlanes; pl++)
    {
        for(int ch = 0; ch < numChannels; ch++)
        {
            fread(convbuf, sizeof(*convbuf), width*height, lutFp);

            for(int pix = 0; pix < width * height; pix++, idx++)
            {
                u8 inputPix8 = convbuf[pix];
                *pixPtr  = INCONV(inputPix8);
                SippHwLut_dmaIn0_buffer[idx] = inputPix.getPackedValue();
                fwrite(&SippHwLut_dmaIn0_buffer[idx], sizeof(uint16_t), 0x1, outLutFp);
            }
        }
    }

    return idx;
}

#endif
#endif

//#############################################################################
// Build the pipeline that's specific to our application

#ifndef SIPP_TEST_SYNC_API
void appSippCallback ( SippPipeline *             pPipeline,
                       eSIPP_PIPELINE_EVENT       eEvent,
                       SIPP_PIPELINE_EVENT_DATA * ptEventData
                     )
{
    if (eEvent == eSIPP_PIPELINE_FRAME_DONE)
    {
        printf ("appSippCallback : Frame done event received : Test proceeding to checks and termination\n");
        testComplete = 1;
    }

}

#endif

fp16 appLUT[LUT_SIZE] ALIGNED(8);

int main (int argc, char *argv[])
{
    UNUSED (argc);
    UNUSED (argv);
    u32    sippErrorLog[SIPP_ERROR_HISTORY_SIZE];

    sippPlatformInit ();

    #ifndef SIPP_PC
    if (PROCESS_LEON_OS == swcWhoAmI())
    {
        printf ("App Starting on LEON OS\n");

        #ifndef SIPP_TEST_SYNC_API
        printf ("Please make sure IRQ sources for dynamically route between LeonOS and LeonRT are set\n");
        #endif
    }
    else
    {
        printf ("App Starting on LEON RT\n");
    }
    #endif // SIPP_PC

    #ifndef SIPP_TEST_SYNC_API
    sippPlatformInitAsync ();
    #endif

    #ifdef SIPP_VCS
    unitTestInit ();
    DrvTimerInit ();
    DrvDdrInitialise (NULL);
    #endif

    buildTestHwLut (&testLut);

    if (sippPipeGetErrorStatus (testLut.pl))
    {
        u32 errNum;
        printf ("Pipeline creation error\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
    }


    #ifndef SIPP_TEST_SYNC_API
    // Register callback for async API
    sippRegisterEventCallback (testLut.pl,
                               appSippCallback);
    #endif

    configTestHwLut (&testLut);
	#ifdef SIPP_LUT_TEST_CREATE_LUT
    appCreateLut (appLUT);
    appPrintLut (appLUT);
	#else
    sippPalMemcpy ((void *)appLUT, (void *)testLUTBuf, sizeof(lut_ltype) * LUT_SIZE);
	#endif

    #ifdef SIPP_LUT_TEST_GEN_INPUT

    FILE * lutFp    = fopen("../../../../../../../resources/OCT_384x290_I_gamma_8bpp.raw", "r");
    FILE * outLutFp = fopen("../../../../../../../resources/OCT_384x290_I_gamma_16bpp.raw", "wb");

    uint32_t uNumILines = appGenerateFp16LUT (SIPP_HW_LUT_BUFFER_NUM_IN_PLANES,
                          CHANNELS_NO,
                          SIPP_HW_LUT_BUFFER_WIDTH,
                          SIPP_HW_LUT_BUFFER_HEIGHT,
                          lutFp,
                          outLutFp);

    printf("uNumILines = %d\n", uNumILines);
    fclose (lutFp);
    fclose (outLutFp);
    #else
    readTestHwLutInput(&testLut);
    #endif

    #ifdef SIPP_USE_PRECALC_SCHEDULE
    // Read offline schedule decisions
    dbgSchedInit (testLut.pl);
    #endif

    #ifdef SIPP_TEST_SYNC_API
    sippProcessFrame(testLut.pl);

    if (sippPipeGetErrorStatus (testLut.pl))
    {
        u32 errNum;
        printf ("Pipeline runtime error\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
    }

    #else
    sippProcessFrameNB(testLut.pl);

    if (sippPipeGetErrorStatus (testLut.pl))
    {
        u32 errNum;
        printf ("Pipeline runtime error\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
    }

    while ( testComplete == 0x0 )                // Move below 8 lines or so into testcommon so this becomes ne function call
    {

    }
    #ifdef  SIPP_PC
    sippGenericRuntimeWaitIsrThreadTerm ();
    #endif
    #endif

    writeTestHwLutOutput(&testLut);
    sippRdFileU8 ((u8*)SippHwLut_Ref_buffer, SIPP_HW_LUT_OUTPUT_BUFFER_SIZE, "../../../../../../../resources/Ref_lutTestOutput_384x290_8bpp.raw");
    sippDbgCompareU8 ((u8*)TestHwLut_dmaOut0_buffer, SippHwLut_Ref_buffer, SIPP_HW_LUT_OUTPUT_BUFFER_SIZE);

    #ifdef SIPP_VCS
    unitTestFinalReport();
    #endif

    return 0;
}
