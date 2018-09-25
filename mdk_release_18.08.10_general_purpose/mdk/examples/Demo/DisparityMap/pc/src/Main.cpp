///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief  This is the main file of the app disparity map computation
///


#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS


//---------------------------------------------------------------------------//
//------------------------------- INCLUDES ----------------------------------//
//---------------------------------------------------------------------------//

// VS and Win
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

// Local Modules
#include "DisparityMapApi.h"
#include <Buffers.h>
#include <swcCrc.h>

// Max number of disparity
const int MAX_DISPARITY = 64;

// Buffers
u8 frameBufferDisparitiesPatches[MAX_WIDTH * (MAX_HEIGHT + MAX_PATCHES * 2*PATCH_H_OVERLAP)];
u8 frameBufferDisparities[MAX_WIDTH * MAX_HEIGHT];	// buffer for disparity buffer before applying median filtering
u8 disparityMedian[MAX_WIDTH * MAX_HEIGHT];			// buffer for disparity buffer after applying median filtering

//---------------------------------------------------------------------------//
//------------------------------- Functions ---------------------------------//
//---------------------------------------------------------------------------//

void setupConfiguration(DisparityConfig* dispCfg, AlgorithmConfig* algoCfg, u8* imgL, u8* imgR, u8* disparities, u32 width, u32 height, u32 bpp)
{  
    algoCfg->cfgCensusKernelSize = CENSUS_KERNEL_SIZE;
    algoCfg->cfgMedianKernelSize = MEDIAN_KERNEL_SIZE;
    algoCfg->cfgLinePadding = LINE_PADDING;
    algoCfg->cfgMedianPadding = MEDIAN_PADDING;
    algoCfg->cfgMaxDisparities = MAX_DISPARITIES;

    // left image
    dispCfg->leftImage.p1 = imgL;
    dispCfg->leftImage.spec.type = YUV400p;
    dispCfg->leftImage.spec.width = width;
    dispCfg->leftImage.spec.height = height;
    dispCfg->leftImage.spec.bytesPP = bpp;

    // right image
    dispCfg->rightImage.p1 = imgR;
    dispCfg->rightImage.spec.type = YUV400p;
    dispCfg->rightImage.spec.width = width;
    dispCfg->rightImage.spec.height = height;
    dispCfg->rightImage.spec.bytesPP = bpp;
    
    // output disparity map
    dispCfg->disparityMap = disparities;
}


int main(int argc, char* argv[])
{
    // Read input images
    FILE* inputDataLeft = fopen("../../myriad/Input/inputFrameLeft_640x480.bin", "rb");
    FILE* inputDataRight = fopen("../../myriad/Input/inputFrameRight_640x480.bin", "rb");
    
    int width = MAX_WIDTH;
    int height = MAX_HEIGHT;
    unsigned char* imgLeft = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
    unsigned char* imgRight = (unsigned char*)malloc(sizeof(unsigned char) * width * height);
    
    size_t bytes;
    bytes=fread(imgLeft, width * height, 1, inputDataLeft);
    if(bytes==0){printf("Error\n");return 1;}
    bytes=fread(imgRight, width * height, 1, inputDataRight);
    if(bytes==0){printf("Error\n");return 1;}
    fclose(inputDataLeft);
    fclose(inputDataRight);

    // Setup configuration
    DisparityConfig dispCfg[MAX_PATCHES];
    AlgorithmConfig algoCfg;
    int patchNo = 0;
    int iyDisparities = 0;
    int ixDisparities = 0;
    int end_ix = MAX_WIDTH - PATCH_W_OVERLAP;
    int end_iy = MAX_HEIGHT - PATCH_H_OVERLAP;
    int patchWidth = PATCH_WIDTH;
    int patchHeight = PATCH_HEIGHT;

    for (int iy = 0; iy < end_iy; iy += (patchHeight - PATCH_H_OVERLAP))
    {
        patchHeight = ((height - iy) < PATCH_HEIGHT) ? (height - iy) : PATCH_HEIGHT;
        ixDisparities = 0;
        for (int ix = 0; ix < end_ix; ix += (patchWidth - PATCH_W_OVERLAP))
        {
            patchWidth = PATCH_WIDTH;

            printf("PATCH %d: ix = %4d, iy = %4d, width = %4d, height = %4d\n", patchNo, ix, iy, patchWidth, patchHeight);

            setupConfiguration(&dispCfg[patchNo], &algoCfg, &imgLeft[MAX_WIDTH * iy + ix], &imgRight[MAX_WIDTH * iy + ix], 
                            &frameBufferDisparitiesPatches[MAX_WIDTH * iyDisparities + ixDisparities], patchWidth, patchHeight, 1);

            char filename00[100];
            sprintf(filename00, "patch%d.bin", patchNo);
            FILE* f = fopen(filename00, "wb");
            fwrite(&imgLeft[MAX_WIDTH * iy + ix], patchWidth*patchHeight, 1, f);
            fclose(f);

            patchNo++;
            ixDisparities += patchWidth;
        }
        iyDisparities += patchHeight;
    }

    // Allocate buffers
	disparityMapInit(&algoCfg, PATCH_WIDTH);

    // Compute Disparity Map
    for (int patchNo = 0; patchNo < MAX_PATCHES; patchNo++)
    {
        disparityMapRunPatches(&algoCfg, &dispCfg[patchNo], dispCfg[patchNo].leftImage.spec.width, dispCfg[patchNo].leftImage.spec.height);
    }

    // Reconstruct disparity map from patches
    FILE* disparityMapFile = fopen("disparityMap.yuv", "wb");
    int cnt = 0;
    u32 sizeOfPatchToSave = 0;
    u8* frameBufferDisparitiesPtr = &frameBufferDisparities[0];
    for (int patchNo = 0; patchNo < MAX_PATCHES; patchNo++)
    {
        if (patchNo == 0)
        {
            sizeOfPatchToSave = dispCfg[patchNo].leftImage.spec.width * (dispCfg[patchNo].leftImage.spec.height - PATCH_H_OVERLAP/2);
            fwrite(dispCfg[patchNo].disparityMap, sizeOfPatchToSave, 1, disparityMapFile);
            memcpy(frameBufferDisparitiesPtr, dispCfg[patchNo].disparityMap, sizeOfPatchToSave);
            frameBufferDisparitiesPtr += sizeOfPatchToSave;
        }
        else
            if (patchNo == MAX_PATCHES - 1)
            {
                sizeOfPatchToSave = dispCfg[patchNo].leftImage.spec.width * (dispCfg[patchNo].leftImage.spec.height - PATCH_H_OVERLAP/2);
                fwrite(dispCfg[patchNo].disparityMap + dispCfg[patchNo].leftImage.spec.width * PATCH_H_OVERLAP/2, sizeOfPatchToSave, 1, disparityMapFile);
                memcpy(frameBufferDisparitiesPtr, dispCfg[patchNo].disparityMap + dispCfg[patchNo].leftImage.spec.width * PATCH_H_OVERLAP/2, sizeOfPatchToSave);
                frameBufferDisparitiesPtr += sizeOfPatchToSave;
            }
        else
        {
            sizeOfPatchToSave = dispCfg[patchNo].leftImage.spec.width * (dispCfg[patchNo].leftImage.spec.height - PATCH_H_OVERLAP);
            fwrite(dispCfg[patchNo].disparityMap + dispCfg[patchNo].leftImage.spec.width * PATCH_H_OVERLAP/2, sizeOfPatchToSave, 1, disparityMapFile);
            memcpy(frameBufferDisparitiesPtr, dispCfg[patchNo].disparityMap + dispCfg[patchNo].leftImage.spec.width * PATCH_H_OVERLAP/2, sizeOfPatchToSave);
            frameBufferDisparitiesPtr += sizeOfPatchToSave;
        }
        cnt++;
    }
    fclose(disparityMapFile);

	medianFiltering(&algoCfg, &frameBufferDisparities[0], disparityMedian, MAX_WIDTH, MAX_HEIGHT);
	FILE* f = fopen("disparityMapMedian.yuv", "wb");
	fwrite(disparityMedian, MAX_WIDTH * MAX_HEIGHT, 1, f);
	fclose(f);

	// Unit test based on CRC
	pointer_type ptr= (pointer_type)0;
	u32 size = sizeof(unsigned char) * width * height;
	u8* disparityMedianPtr = &disparityMedian[0];
	u32 crcUnitTest = swcCalcCrc32(disparityMedianPtr, size, ptr);

	if (crcUnitTest == GOLDEN_CRC)
		printf("PC unit test: PASSED\n");

    // Release allocated memory
    free(imgLeft);
    free(imgRight);
    
    printf("Algorithm finished successfully!\n");
    return 0;
}

