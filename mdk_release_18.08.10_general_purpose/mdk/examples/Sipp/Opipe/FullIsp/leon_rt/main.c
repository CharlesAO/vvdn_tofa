///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file - oPipe- low-light small resolution real-life params YUV444 output
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <UnitTestApi.h>
#include <VcsHooksApi.h>
#include "mv_types.h"
#include "isp_params_wrap.h"
#include "DrvLeon.h"
#include "DrvLeonL2C.h"
#include "Opipe.h"
#include "OpipeApps.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
//Number of runs, each run uses the unique input buffer, but different output buffer
#define N_RUNS 3

//Image paths
#define INPUT_IMAGE  "../../../../resources/lowLight_512x128.raw"
#define OUTPUT_IMAGE "out_512x128_8bpp_P420.yuv"

//Input resolution
#define IMG_W ISPC_BAYER_W
#define IMG_H ISPC_BAYER_H

//Output resolution
#define O_IMG_W  ((IMG_W * N -1)/D + 1)
#define O_IMG_H  ((IMG_H * N -1)/D + 1)

#define DEBAYER_Y_H_V  20
#define SHARP_Y_H_V 18

//RAW 8BPP
#define INPUT_IMAGE_SIZE  (IMG_W * IMG_H)

//YUV420 8BPP
#define CHROMA_SIZE       (O_IMG_W*O_IMG_H/4)
#define OUTPUT_IMAGE_SIZE ((O_IMG_W * O_IMG_H) + (2*CHROMA_SIZE))

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
//Main full buffers
uint16_t iBuf     [INPUT_IMAGE_SIZE]           SECTION(".shv5.S.init") ALIGNED(8); // ISPC_BAYER_BITS_STILL
uint8_t  yuvBuf   [N_RUNS][OUTPUT_IMAGE_SIZE]  SECTION(".shv5.S.init") ALIGNED(8); // yuv420

//Circular buffers
uint16_t cSigma   [  IMG_W * I_CBUFF_H    ]    SECTION(".shv1.S.init") ALIGNED(8);  //Bayer is 10bit
uint16_t cDbyrY   [  IMG_W * DBYR_Y_H     ]    SECTION(".shv2.S.init") ALIGNED(8);
uint16_t cSharpY  [  IMG_W * SHARP_Y_H    ]    SECTION(".shv3.S.init") ALIGNED(8);
uint8_t  cLut     [  IMG_W * O_CBUFF_H * 3]    SECTION(".shv4.S.init") ALIGNED(8);
uint8_t  cUpfirDn [O_IMG_W * O_CBUFF_H * 3]    SECTION(".shv5.S.init") ALIGNED(8);  //as for yuv444

OpipeMF opF;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
extern void appSetParams(Opipe *p);

// 6: Functions Implementation
// ----------------------------------------------------------------------------
void appBuildPipe()
{
    // Must specify buffers first
    opF.in.cBufSigma.base = (uint32_t)cSigma;   opF.in.cBufSigma.h = I_CBUFF_H;
    opF.cBufDbyrY.base    = (uint32_t)cDbyrY;   opF.cBufDbyrY.h    = DBYR_Y_H;
    opF.cBufSharp.base    = (uint32_t)cSharpY;  opF.cBufSharp.h    = SHARP_Y_H;
    opF.cBufLut.base      = (uint32_t)cLut;     opF.cBufLut.h      = O_CBUFF_H;
    opF.cBufPoly.base     = (uint32_t)cUpfirDn; opF.cBufPoly.h     = O_CBUFF_H;
    OpipeCreateFull(&opF, BPP(2));
}

int main()
{
    uint32_t run;

    OpipeTestInit();
    unitTestInit();

    // Read test image
    loadMemFromFileSimple(INPUT_IMAGE, IMG_W*IMG_H*2, iBuf);

    // Opipe init
    OpipeReset ();
    appBuildPipe();

    for(run=0; run < N_RUNS; run++)
    {
        // Frame based setup:
        appSetParams(&opF.p);
        OpipeSetSizeMF(&opF, IMG_W, IMG_H);
        opF.pIn->ddr.base    = (uint32_t)iBuf;
        opF.pOutY->ddr.base  = (uint32_t)&yuvBuf[run][0];
        opF.pOutUV->ddr.base = (uint32_t)&yuvBuf[run][O_IMG_W*O_IMG_H];
        // Actual run:
        OpipeStart (&opF.p);
        OpipeWait  (&opF.p);
    }

    // Check that 2nd, 3rd, ... runs are identical to 1st run
    // (it should be as input data and params are the same)
    for(run=1; run < N_RUNS; run++)
    {
        unitTestMemCompare((void *)yuvBuf[run], (void *)yuvBuf[0], sizeof(yuvBuf[0]));
    }
    unitTestFinalReport();
    // Dump image to file
    saveMemoryToFile((uint32_t)yuvBuf[0], sizeof(yuvBuf[0]), OUTPUT_IMAGE);

    DrvLeonRTSignalStop();
    return 0;
}
