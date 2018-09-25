///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     JPEG Encoder Test application.
///

// Includes
// ----------------------------------------------------------------------------
#include <string.h>
#include <stdio.h>
#include "mv_types.h"
#include "JpegEncoderApi.h"
#include "app_config.h"
#include "DrvTimer.h"
#include <swcCrc.h>
#include <DrvShaveL2Cache.h>
#include <DrvCmxDma.h>

// Components
#include <UnitTestApi.h>
#include <VcsHooksApi.h>

// Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define MAX_WIDTH 4096
#define MAX_HEIGHT 3072

#define EXPECTED_CRC_IMG_BAYWATCH   (0x28048228)
#define EXPECTED_CRC_IMG_COBKITCHEN (0x38A37F1B)

#define INPUT_BUFF_SIZE_SHAVE 32*1024

                        
//  Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
u8 __attribute__((section(".ddr.bss"))) jpgbuff[MAX_WIDTH * MAX_HEIGHT];
u8 __attribute__((section(".ddr.bss"))) img_yuv[MAX_WIDTH * MAX_HEIGHT * 3];
frameBuffer gimginfo;

// Static Function Prototypes
// ----------------------------------------------------------------------------
static float ConfigureImgFull(int yuvFormat, int width, int height);
static void jpegEncodeAndCheck(int yuvFormat, int width, int height, char *inputFile, char * outFile, u32 expectedCRC);

// Functions Implementation
// ----------------------------------------------------------------------------

int main(void)
{
    unitTestInit();
    initClocksAndMemory();
    DrvCmxDmaInitDefault();

    jpegEncodeAndCheck(JPEG_422_PLANAR, 512, 512 , "../../../resources/Baywatch_512x512_P422.yuv", "Baywatch.jpg"  ,EXPECTED_CRC_IMG_BAYWATCH  );
    jpegEncodeAndCheck(JPEG_420_PLANAR, 1280, 720, "../../../resources/CobKitchen_1280x720.yuv"  , "CobKitchen.jpg",EXPECTED_CRC_IMG_COBKITCHEN);

    unitTestFinalReport();

    return 0;
}

// Set values on gimginfo struct and return imputSizeCoef
static float ConfigureImgFull(int yuvFormat, int width, int height)
{
    int lumaValuesPerChroma = 4;
    float inputSizeCoef = 1.5;
    if(yuvFormat == JPEG_422_PLANAR)
    {
        lumaValuesPerChroma = 2;
        inputSizeCoef = 2;
    }
    if(yuvFormat == JPEG_444_PLANAR)
    {
        lumaValuesPerChroma = 1;
        inputSizeCoef = 3;
    }

    gimginfo.p1 = (u8*) (img_yuv);
    gimginfo.p2 = (u8*) ((img_yuv) + width * height);
    gimginfo.p3 = (u8*) ((img_yuv) + width * height + width * height/lumaValuesPerChroma);

    gimginfo.spec.width = width;
    gimginfo.spec.height = height;

    return inputSizeCoef;
}

static void jpegEncodeAndCheck(int yuvFormat, int width, int height, char *inputFile, char * outFile, u32 expectedCRC)
{
    tyTimeStamp timer_data;
    u64 cyclesElapsed;
    float inputSizeCoef;
    double encodeTimeMs;
    double framesPerSecond;
    u32 outputSize;
    u32 crcTest;

    printf("Starting JPEG Encode for %s\n", inputFile);
    printf("Loading Yuv file, please wait...\n");

    memset(jpgbuff, 0, sizeof(jpgbuff));
    memset(img_yuv, 0, sizeof(img_yuv));
    inputSizeCoef = ConfigureImgFull(yuvFormat, width, height);
    loadMemFromFile(inputFile, 0, 0, width * height* inputSizeCoef, img_yuv);

    DrvTimerStartTicksCount(&timer_data);

    outputSize = JPEG_encode(gimginfo, jpgbuff, SHAVES_USED, INPUT_BUFF_SIZE_SHAVE, yuvFormat);

    DrvTimerGetElapsedTicks(&timer_data,&cyclesElapsed);
    encodeTimeMs = DrvTimerTicksToMs(cyclesElapsed);
    framesPerSecond = 1000.0 / encodeTimeMs;
    printf("Encoded in: %lu cycles (%f ms; %5.0f frames per second)\n", (u32) cyclesElapsed,encodeTimeMs,framesPerSecond);

    saveMemoryToFile((u32)&jpgbuff[0], outputSize,outFile);

    crcTest = swcCalcCrc32(jpgbuff, outputSize, le_pointer);
    unitTestAssert(crcTest == expectedCRC);
    if (crcTest == expectedCRC){
        printf("Encoded Image CRC32 OK [0x%08lX]\n", crcTest);
    } else {
        printf("Encoded Image CRC32 FAIL [Expected:0x%08lX, GOT:0x%08lX\n",expectedCRC, crcTest);
    }

}
