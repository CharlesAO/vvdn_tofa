///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file test
///
// 1: Includes
// ----------------------------------------------------------------------------a
// Standard libraries
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// General MDK
#include <mv_types.h>
#include <swcCrc.h>

// Drivers
#include <DrvTimer.h>
#include <DrvShaveL2Cache.h>
#include <DrvCmxDma.h>
#include <DrvCpr.h>

// Components
#include <UnitTestApi.h>
#include <VcsHooksApi.h>
#include <Fp16Convert.h>

// Project local
#include "app_config.h"
#include "mvTensorApi.h"

// Defines
// ----------------------------------------------------------------------------
#define SHAVES_NO 12

#define MAX_INPUT_SIZE   (20*1024*1024)
#define MAX_OUTPUT_SIZE  (1*1024*1024)
#define MAX_TAPS_SIZE    (150*1024)
#define MAX_FRAME_HEIGHT  2048
#define MAX_CHANNEL_NO    256

// Global Data
// ----------------------------------------------------------------------------
fp16 __attribute__((section(".ddr_direct.bss"), aligned (8))) inputFrame[MAX_INPUT_SIZE];
fp16 __attribute__((section(".ddr_direct.bss"), aligned (8))) outputFrame[MAX_OUTPUT_SIZE];
fp16 __attribute__((section(".ddr_direct.bss"), aligned (8))) taps[MAX_TAPS_SIZE];
u8   __attribute__((section(".ddr_direct.data"), aligned (8))) inputBPP = 2;
u8   __attribute__((section(".ddr_direct.data"), aligned (8))) outputBPP = 2;
u8   __attribute__((section(".ddr_direct.data"), aligned (8))) kernelBPP = 2;
u8   __attribute__((section(".ddr_direct.data"), aligned (8))) channelsNo = 64;
u8   __attribute__((section(".ddr_direct.data"), aligned (8))) outputMapsNo = 192;
u32  __attribute__((section(".ddr_direct.data"))) inputWidth = 56;
u32  __attribute__((section(".ddr_direct.data"))) inputHeight = 56;
u32  __attribute__((section(".ddr_direct.data"))) kernelSize = 3;
u32  __attribute__((section(".ddr_direct.data"))) convSize = k3x3;
u32  __attribute__((section(".ddr_direct.data"))) convStride = stride1;
u32  __attribute__((section(".ddr_direct.data"))) convDataType = hhhh;
u32  __attribute__((section(".ddr_direct.data"))) inputStride = 112; // (inputWidth  + PADDING ) *
								     // inputBPP;

u32 __attribute__((section(".ddr_direct.data"))) firstShave;
u32 __attribute__((section(".ddr_direct.data"))) lastShave;

tyTimeStamp timer_data;
u64 cyclesElapsed;

fp32 __attribute__((section(".ddr_direct.data"))) executionTime;
u32 __attribute__((section(".ddr_direct.data"))) fps;
fp32 __attribute__((section(".ddr_direct.data"))) gflops;

// Functions Implementation
// ----------------------------------------------------------------------------
static int checkOutput()
{
    u32 outputStride = convStride + 1;
    if (convStride == stride8)
        outputStride = 8;

    u32 outIndex = 0;
    u32 errorNo = 0;
    s32 firstErrorElementNumber = -1;

    for (int k = 0; k < outputMapsNo; k++)
        for (u32 i = 0; i < inputHeight; i += outputStride)
            for (u32 j = 0; j < inputWidth; j += outputStride)
            {
                if (outputFrame[outIndex] != inputFrame[i * inputWidth + j])
                {
                  errorNo++;

                  if (firstErrorElementNumber == -1)
                  {
                      firstErrorElementNumber = outIndex;

                      printf("%d: 0x%x 0x%x \n\n", (int)outIndex, 
                             outputFrame[outIndex], inputFrame[i * inputWidth + j]);
                      outIndex++;
                      return 0;
                  }
                }
                outIndex++;
            }

    return 1;
}

static void setTensorParams(mvTensorInfo* mvTensorParam, u32 width, u32 height, u32 planesNo)
{
    UNUSED(width);
    UNUSED(height);
    // Adjust the convolution stride from the enum to the real number of pixels to skip
    u32 outputStride = convStride + 1;
    if (convStride == stride8)
        outputStride = 8;

    for (int j = 0; j < channelsNo; j++)
        mvTensorParam->input[j] = (u8*)&inputFrame[inputWidth * inputHeight * j];

    for (int j = 0; j < outputMapsNo; j++)
        mvTensorParam->output[j] = (u8*)&outputFrame[(inputWidth / outputStride) * 
            (inputHeight / outputStride) * j];

    mvTensorParam->convTaps = (u8*)taps;
    mvTensorParam->inputBPP = inputBPP;
    mvTensorParam->outputBPP = outputBPP;
    mvTensorParam->kernelBPP = kernelBPP;
    mvTensorParam->channelsNo = planesNo;
    mvTensorParam->outputMapsNo = outputMapsNo;
    mvTensorParam->inputWidth = inputWidth;
    mvTensorParam->inputHeight = inputHeight;
    mvTensorParam->inputStride = inputStride;
    mvTensorParam->kernelSize = kernelSize;
    mvTensorParam->convSize = convSize;
    mvTensorParam->convStride = convStride;
    mvTensorParam->convDataType = convDataType;
}

static void test(u32 firstShv, u32 lastShv, u32 inputWidth, u32 inputHeight, u32 planesNo)
{
    mvTensorInfo mvTensorParam;

    setTensorParams(&mvTensorParam, inputWidth, inputHeight, planesNo);

    // Clear previous output to start each test clean
    memset(outputFrame, 0, sizeof(outputFrame));

    // Start execution and measure the total time
    DrvTimerStartTicksCount(&timer_data);

    conv4d(&mvTensorParam, firstShv, lastShv);

    DrvTimerGetElapsedTicks(&timer_data, &cyclesElapsed);
    executionTime = DrvTimerTicksToMs(cyclesElapsed);
    fps = (u32)((DrvCprGetClockFreqKhz(SYS_CLK, NULL) * 1000) / cyclesElapsed);
    gflops = (fp32)((kernelSize * kernelSize * 2) * channelsNo * outputMapsNo * (inputWidth * inputHeight) * (fp32)fps / pow(10.0, 9));

    printf("Shaves %2ld: %9ld cycles, %6.2f ms, %2lu fps, %5.2f gflops \n", lastShv - firstShv + 1,
         (u32)cyclesElapsed, executionTime, fps, gflops);

    u32 testPassed = checkOutput();    

    unitTestAssert(testPassed == 1);
}

int main(void)
{
    unitTestInit();

    initClocksAndMemory();

    printf("3D Convolution benchmark: conv3x3 on 64x192x56x56\n");
    printf("System frequency: %d Mhz\n\n", DrvCprGetClockFreqKhz(SYS_CLK, NULL) / 1000);

    // Initialize the convolution taps to a unit kernel (1.0f in the middle)
    bzero(taps, outputMapsNo * channelsNo * kernelSize * kernelSize * kernelBPP);   
    for(int i = 0; i < outputMapsNo; i++)
         taps[kernelSize * kernelSize / 2 + channelsNo * kernelSize * kernelSize * i] = 0x3c00;
     
    // Initialize the input buffers with random data
    float a = 255.0;
    for(u32 i = 0; i < (inputWidth * inputHeight * channelsNo); i++)
    {
        float x = (float)rand() / ((float)RAND_MAX / a);
        inputFrame[i] = f32Tof16(x);
    }

    // Run the test on each number of Shaves
    for (u32 i = 0; i < SHAVES_NO; i++)
        test(0, i, inputWidth, inputHeight, channelsNo);
    
    unitTestFinalReport();

    return 0;
}
