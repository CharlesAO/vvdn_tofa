///
/// @file setup.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Module for setup configuration
///

#include "setup.h"
#include <CameraConfig.h>

#include <stdio.h>
#include <string.h>

// Initialize all output buffers to random value
void setBuffers(u8* frameBufferDisparitiesMedian, u8* frameBufferDisparities, u8** disparityMap, u8** usbBufPtr)
{
	u32 i;

	memset(frameBufferDisparitiesMedian, 0xAA, MAX_WIDTH * MAX_HEIGHT * USB_BPP);
    memset(frameBufferDisparities, 0xAA, MAX_WIDTH * MAX_HEIGHT);

	for(i = 0; i < MAX_USED_BUF; i++)
	{
		memset(disparityMap[i], 0xAA, PATCH_WIDTH*PATCH_HEIGHT);
		memset(usbBufPtr[i], 0xAA, USB_FRAME_SIZE_BYTES + PAYLOAD_HEADER_OFFSET);
	}
}

// Configuration of algorithm parameters and buffers
void setupConfiguration(DisparityConfig* pyrCfg, AlgorithmConfig* algoCfg, u8* imgL, u8* imgR, u8* disparities, u32 width, u32 height, u32 bpp)
{
    algoCfg->cfgCensusKernelSize = CENSUS_KERNEL_SIZE;
    algoCfg->cfgMedianKernelSize = MEDIAN_KERNEL_SIZE;
    algoCfg->cfgLinePadding = LINE_PADDING;
    algoCfg->cfgMedianPadding = MEDIAN_PADDING;
    algoCfg->cfgMaxDisparities = MAX_DISPARITIES;


    // left image
    pyrCfg->leftImage.p1 = imgL;
    pyrCfg->leftImage.spec.type = YUV400p;
    pyrCfg->leftImage.spec.width = width;
    pyrCfg->leftImage.spec.height = height;
    pyrCfg->leftImage.spec.bytesPP = bpp;

    // right image
    pyrCfg->rightImage.p1 = imgR;
    pyrCfg->rightImage.spec.type = YUV400p;
    pyrCfg->rightImage.spec.width = width;
    pyrCfg->rightImage.spec.height = height;
    pyrCfg->rightImage.spec.bytesPP = bpp;

    pyrCfg->disparityMap = disparities;
}

// Split image into patches
void computePatches(u8* originalFrame, u8* frameRectified, u8** disparity, AlgorithmConfig* algoCfg, DisparityConfig* pyrCfg, u32 width, u32 height)
{
    (void) width;// "use" the variables to hush the compiler warning.

	u32 iy, ix;
    u32 patchNo = 0;
    u32 iyDisparities = 0;
    u32 ixDisparities = 0;

	u32 patchWidth = PATCH_WIDTH;
    u32 patchHeight = PATCH_HEIGHT;

    u32 end_ix = MAX_WIDTH - PATCH_W_OVERLAP;
    u32 end_iy = MAX_HEIGHT - PATCH_H_OVERLAP;

	for (iy = 0; iy < end_iy ; iy += (patchHeight - PATCH_H_OVERLAP))
    {
        patchHeight = ((height - iy) < PATCH_HEIGHT) ? (height - iy) : PATCH_HEIGHT;
        ixDisparities = 0;
        for (ix = 0; ix < end_ix ; ix += (patchWidth - PATCH_W_OVERLAP))
        {
            patchWidth = PATCH_WIDTH;
            setupConfiguration(&pyrCfg[patchNo], algoCfg, &originalFrame[MAX_WIDTH * iy + ix], &frameRectified[MAX_WIDTH * iy + ix], disparity[patchNo], patchWidth, patchHeight, 1);

            patchNo++;
            ixDisparities += patchWidth;
        }
        iyDisparities += patchHeight;
    }
}

// Reconstruct image from patches
void recomputeImageFromPatches(u8* frameBufferDisparities, DisparityConfig* pyrCfg)
{
	u32 patchNo = 0;
	u32 sizeOfPatchToSave = 0;
	u8* frameBufferDisparitiesPtr = &frameBufferDisparities[0];

    for (patchNo = 0; patchNo < MAX_PATCHES; patchNo++)
    {
        if (patchNo == 0)
        {
            sizeOfPatchToSave = pyrCfg[patchNo].leftImage.spec.width * (pyrCfg[patchNo].leftImage.spec.height - PATCH_H_OVERLAP/2);
            memcpy(frameBufferDisparitiesPtr, pyrCfg[patchNo].disparityMap, sizeOfPatchToSave);
            frameBufferDisparitiesPtr += sizeOfPatchToSave;
        }
        else
            if (patchNo == MAX_PATCHES - 1)
            {
                sizeOfPatchToSave = pyrCfg[patchNo].leftImage.spec.width * (pyrCfg[patchNo].leftImage.spec.height - PATCH_H_OVERLAP/2);
                memcpy(frameBufferDisparitiesPtr, pyrCfg[patchNo].disparityMap + pyrCfg[patchNo].leftImage.spec.width * PATCH_H_OVERLAP/2, sizeOfPatchToSave);
                frameBufferDisparitiesPtr += sizeOfPatchToSave;
            }
            else
            {
                sizeOfPatchToSave = pyrCfg[patchNo].leftImage.spec.width * (pyrCfg[patchNo].leftImage.spec.height - PATCH_H_OVERLAP);
                memcpy(frameBufferDisparitiesPtr, pyrCfg[patchNo].disparityMap + pyrCfg[patchNo].leftImage.spec.width * PATCH_H_OVERLAP/2, sizeOfPatchToSave);
                frameBufferDisparitiesPtr += sizeOfPatchToSave;
            }
    }
}
