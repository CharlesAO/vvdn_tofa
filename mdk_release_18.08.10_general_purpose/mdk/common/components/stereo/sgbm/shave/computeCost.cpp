///
/// @file DisparityMap.cpp
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Module used to compute dense disparity map; Disparity Map Source File
///

// C++ includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Local Modules
#include "disparityMapApi.h"
#include "kernelsCommonDefines.h"
#include <censusMin64.h>
#include "Defines.h"
#include "helpers.h"
#include "sgbm.h"

// DMA
#include <scCmxDma.h>
#include <svuCommonShave.h>
#ifndef __PC__
#define DMA_USAGE
#endif
#define DMA_USAGE
// Line buffers used at both pyramid levels
static u32 ALIGNED(16) ccensusValueLeft[(MAX_WIDTH / SCALE_FACTOR)];
static u32 ALIGNED(16) ccensusValueRight[(MAX_WIDTH / SCALE_FACTOR + MAX_DISPARITIES_LEVEL0)];
static u8 ALIGNED(16) cdisparityCost[MAX_WIDTH  / SCALE_FACTOR * MAX_DISPARITIES_LEVEL0 + 2 * MAX_DISPARITIES_LEVEL0];
static u8 ALIGNED(16) cadCost[MAX_WIDTH  / SCALE_FACTOR * MAX_DISPARITIES_LEVEL0];
static u8 ALIGNED(16) clinesRightAD[MAX_WIDTH / SCALE_FACTOR + MAX_DISPARITIES_LEVEL0];

static u8 *clinesPointerLeft[CENSUS_KERNEL_SIZE + 1];
static u8 *clinesPointerRight[CENSUS_KERNEL_SIZE + 1];

static u8 ALIGNED(16) clineLeftHeapBuffer[(MAX_WIDTH / SCALE_FACTOR + 2*LINE_PADDING + MAX_PADDING_16 + ALIGNMENT_PADDING)*(CENSUS_KERNEL_SIZE + 1)];
static u32 cimageLineLeft[CENSUS_KERNEL_SIZE + 1];
static u32 clineLeftHeap;

static u8 ALIGNED(16) clineRightHeapBuffer[(MAX_WIDTH / SCALE_FACTOR + 2*LINE_PADDING + MAX_PADDING_16 + ALIGNMENT_PADDING)*(CENSUS_KERNEL_SIZE + 1)];
static u32 cimageLineRight[CENSUS_KERNEL_SIZE + 1];
static u32 clineRightHeap;

static u32 widthPaddingTo16 = 0;
static u32 widthPaddingTo8 = 0;
static u32 widthPaddingTo4 = 0;
static u32 functionTableIdx = 0;

#ifdef DMA_USAGE
static ScCmxDmaTransaction list1, list2;
#endif

//----------------------------------------------------------------------------------//
// addPadding - helpers function to add padding to input lines for the MvCV kernels //
//----------------------------------------------------------------------------------//
static void addPadding1(u8 *pointer, u32 width, u32 paddingSize)
{
    u32 widthAux = width - 1;

    for (u32 i = 1; i <= paddingSize; i++)
    {
        *(pointer - i) = *pointer;
        *(pointer + widthAux + i) = *(pointer + widthAux);
    }
}

//---------------------------------------------------------------//
// rotateBuffersOnePosition - helpers function to rotate buffers //
//---------------------------------------------------------------//
static void rotateBuffersOnePosition1(u8* ptrs[], u32 num)
{
    // Keep the first pointer because it will be overwritten
    u8* tmp = ptrs[0];

    // Shift all pointers to the left location
    for (u32 i = 0; i < num; i++)
    {
        ptrs[i] = ptrs[i + 1];
    }
    // Last pointer becomes the first
    ptrs[CENSUS_KERNEL_SIZE] = tmp;
}

//--------------------------------------------------------------------------//
// dmaInFirstImageLines - dma transfer first lines needed for Census kernel //
//--------------------------------------------------------------------------//
static void dmaInFirstImageLines1(stereoMatImage_t* srcLeft, stereoMatImage_t* srcRight, u8** dstLeft, u8** dstRight, stereoUserConfig_t* algoCfg)
{
    UNUSED(algoCfg);
#ifdef DMA_USAGE
    ScCmxDmaTransactionHnd hnd1, hnd2;
#endif

    u32 cfgLinePadding      = LINE_PADDING;
    u32 cfgCensusKernelSize = CENSUS_KERNEL_SIZE;
    u32 halfKernel          = (s32)(cfgCensusKernelSize / 2);

    u8* srcLeftPtr = srcLeft->data;
    u8* srcRightPtr = srcRight->data;
    u32 width = srcLeft->w;
    u32 step = srcLeft->step;

    //printf("dmaInFirstImageLines1: %d %d %d %d %d\n", cfgLinePadding, cfgCensusKernelSize, halfKernel, width, step);
    for (u32 i = 0; i < cfgCensusKernelSize; i++)
    {
        // left-right image
        dstLeft[i] += cfgLinePadding;
        dstRight[i] += cfgLinePadding;
        if (i <= halfKernel)
        {
            // lines for the first half of the kernel (included median line); e.g. Kernel size 7 ==> lines 0, 1, 2, 3
#ifdef DMA_USAGE
            ScCmxDmaCreateTransaction(&hnd1, &list1,
                                        (u8*)srcLeftPtr, (u8*)dstLeft[i], width);
            ScCmxDmaCreateTransaction(&hnd2, &list2,
                                        (u8*)srcRightPtr, (u8*)dstRight[i], width);
#else
            memcpy((u8*)dstLeft[i], (u8*)srcLeftPtr, width);
            memcpy((u8*)dstRight[i], (u8*)srcRightPtr, width);
#endif
        }
        else
        {
            // lines for the second half of the kernel; e.g. Kernel size 7 ==> 4, 5, 6
#ifdef DMA_USAGE
            ScCmxDmaCreateTransaction(&hnd1, &list1,
                                        (u8*)(srcLeftPtr + (i - halfKernel) * step), (u8*)dstLeft[i], width);
            ScCmxDmaCreateTransaction(&hnd2, &list2,
                                        (u8*)(srcRightPtr + (i - halfKernel) * step), (u8*)dstRight[i], width);
#else
            memcpy((u8*)dstLeft[i], (u8*)(srcLeftPtr + (i - halfKernel) * step), width);
            memcpy((u8*)dstRight[i],  (u8*)(srcRightPtr + (i - halfKernel) * step), width);
#endif
        }
#ifdef DMA_USAGE
        ScCmxDmaLinkTransactions(&hnd1, 2, &hnd1, &hnd2);
        ScCmxDmaStartTransfer(&hnd1);
        ScCmxDmaWaitTransaction(&hnd1);
#endif
        // Add padding
        addPadding1(dstLeft[i], width, cfgLinePadding);
        addPadding1(dstRight[i], width, cfgLinePadding);
    }
}

//----------------------------------------------------------------------//
// alignLine - helpers function to align each u32 line to multiple of x //
//----------------------------------------------------------------------//
static void alignLineU321(u32 *pointer, u32 width, u32 paddingSize)
{
    for (u32 i = 0; i < paddingSize; i++)
    {
        *(pointer + width + i) = *(pointer + width -1);
    }
}

//--------------------------------------------------------------------------//
// prerequisitesCensus - handle needed prerequisites for census matching    //
//--------------------------------------------------------------------------//
static void prerequisitesCensus1(u32* censusLeft, u32* censusRight, u32 width, u32 widthPadding, u32 disparities)
{
    // padd census right values
    for (u32 j = 0; j < disparities; j++)
    {
        censusRight[j] = censusRight[disparities];
    }

    // censusMatching kernel requires each line to be multiple of 4
    alignLineU321(censusLeft, width, widthPadding);
    alignLineU321(censusRight, width + disparities, widthPadding);
}

//--------------------------------------------------------------------------//
// prerequisitesAD - handle needed prerequisites for absolute differences   //
//--------------------------------------------------------------------------//
static void prerequisitesAD1(u8* imgRightInput, u8* imgRightOutput, u32 width, u32 disparities)
{
    memcpy(&imgRightOutput[disparities], imgRightInput, width);

    // padd image right values to the left
    for (u32 j = 0; j < disparities; j++)
    {
        imgRightOutput[j] = imgRightOutput[disparities];
    }
}

//------------------------------------------------//
// disparityMapInit - allocate all buffers needed //
//------------------------------------------------//
void computeCostInit(u32 width)
{

    // read algo parameters
    u32 cfgCensusKernelSize = CENSUS_KERNEL_SIZE;
    u32 cfgLinePadding      = LINE_PADDING;

    // Line padding
    widthPaddingTo16 = (width%16 == 0) ? 0 : (16 - width%16);
    widthPaddingTo4 = (width%4 == 0) ? 0 : (4 - width%4);

    // Buffers starting addresses
    clineLeftHeap = (u32)&clineLeftHeapBuffer[0];
    clineRightHeap = (u32)&clineRightHeapBuffer[0];

    if (DISP == 128){
        functionTableIdx = COST_F_TABLE_IDX_D128;
    }
    else{
        if (DISP == 64){
            functionTableIdx = COST_F_TABLE_IDX_D64;
        }
    }

    // Allocate memory for lines from left image
    for (u32 i = 0; i < cfgCensusKernelSize + 1; i++)
    {
        cimageLineLeft[i] = (u32)clineLeftHeap;
        clineLeftHeap += width + 2*cfgLinePadding + widthPaddingTo16;
    }

    // Allocate memory for lines from right image
    for (u32 i = 0; i < cfgCensusKernelSize + 1; i++)
    {
        cimageLineRight[i] = (u32)clineRightHeap;
        clineRightHeap += width + 2*cfgLinePadding + widthPaddingTo16;
    }

    // Allocate **
    for (u32 i = 0; i < cfgCensusKernelSize + 1; i++)
    {
        clinesPointerLeft[i] = (u8*)cimageLineLeft[i];//ALIGN_UP((u8*)imageLineLeft[i], 16);
        clinesPointerRight[i] = (u8*)cimageLineRight[i];//ALIGN_UP((u8*)imageLineRight[i], 16);
    }
}

//------------------------------------------------------------//
// computeCost - Compute combined cost for Level 0 //
//------------------------------------------------------------//
void computeCostBase(stereoUserConfig_t* algoCfg,
                        stereoMatImage_t *leftImage,
                        stereoMatImage_t *rightImage,
                        u8* outputCost)
{
#ifdef DMA_USAGE
    ScCmxDmaTransactionHnd hnd1, hnd2;
#endif

    // Read algo parameters
    u32 disparities         = MAX_DISPARITIES_LEVEL0;
    u32 cfgCensusKernelSize = CENSUS_KERNEL_SIZE;
    u32 cfgLinePadding      = LINE_PADDING;

    u8* leftImagePtr        = leftImage->data;
    u8* rightImagePtr       = rightImage->data;
    u32 width               = leftImage->w;
    u32 height              = leftImage->h;
    widthPaddingTo16        = (width%16 == 0) ? 0 : (16 - width%16);
    widthPaddingTo8         = (width%8 == 0) ? 0 : (8 - width%16);
    widthPaddingTo4         = (width%4 == 0) ? 0 : (4 - width%4);
    u32 halfKernel          = (s32)(cfgCensusKernelSize / 2);
    u32 offsetBuffer        = (cfgCensusKernelSize / 2) + 1;
    s32 line;
    u32 fIdx = functionTableIdx;

    // Prepare data: dma in first 5 lines for CensusTransform
    dmaInFirstImageLines1(leftImage, rightImage, clinesPointerLeft, clinesPointerRight, algoCfg);

    // Set pointers for the last line of the kernel and results
    clinesPointerLeft[cfgCensusKernelSize] += cfgLinePadding;
    clinesPointerRight[cfgCensusKernelSize] += cfgLinePadding;

    // Compute disparity map
    for (u32 i = 0; i < height; i++)
    {
        // Set last Left and Right line for the Kernel
        if ((i + offsetBuffer) >= height){
            line = height - 1;
        }
        else{
            line = i + offsetBuffer;
        }

        // Transfer out the result
        if (i >= 1)
        {
#ifdef DMA_USAGE
            // DMA the QVGA level cost
            ScCmxDmaCreateTransaction(&hnd1, &list1,
                    cdisparityCost,
                    (u8*)(outputCost + (i - 1) * width * disparities),
                    width * disparities);
            ScCmxDmaStartTransfer(&hnd1);
            ScCmxDmaWaitTransaction(&hnd1);
#else
            memcpy((u8*)(outputCost + (i - 1) * width * disparities), cdisparityCost, width * disparities);
#endif
             // DEBUG WTA
            //memcpy(outputCost + (i-1) * width, (u8*)&ccensusValueLeft[0], width);
        }
#ifdef DMA_USAGE
        // Transfer in new image lines for census transform
        ScCmxDmaCreateTransaction(&hnd1, &list1,
                    (u8*)(leftImagePtr + line * leftImage->step),
                    (u8*)clinesPointerLeft[cfgCensusKernelSize],
                    width);
        ScCmxDmaCreateTransaction(&hnd2, &list2,
                    (u8*)(rightImagePtr + line * rightImage->step),
                    (u8*)clinesPointerRight[cfgCensusKernelSize],
                    width);

        ScCmxDmaLinkTransactions(&hnd1, 2, &hnd1, &hnd2);
        ScCmxDmaStartTransfer(&hnd1);
        ScCmxDmaWaitTransaction(&hnd1);
#else
        memcpy((u8*)clinesPointerLeft[cfgCensusKernelSize], (u8*)(leftImagePtr + line * leftImage->step), width);
        memcpy((u8*)clinesPointerRight[cfgCensusKernelSize], (u8*)(rightImagePtr + line * rightImage->step), width);
#endif

        // Step 1: Compute Census Transform
        censusTransform(clinesPointerLeft, ccensusValueLeft, width + widthPaddingTo16);
        censusTransform(clinesPointerRight, &ccensusValueRight[disparities], width + widthPaddingTo16);

        // Reset table function index
        fIdx = functionTableIdx;

        // Step 2: Compute Cost Map
        prerequisitesCensus1(ccensusValueLeft, ccensusValueRight, width, widthPaddingTo4, disparities);
        ((censusMatching)fTable[fIdx++])(ccensusValueLeft, &ccensusValueRight[disparities], cdisparityCost, width + widthPaddingTo4, LEFT_MATCHING);

        // Step 3: Compute Absolute Difference - note: reuse aggregatedCost buffer to save absolute difference cost
        prerequisitesAD1(clinesPointerRight[halfKernel], clinesRightAD, width, disparities);
        ((computeAD)fTable[fIdx++])(clinesPointerLeft[halfKernel], &clinesRightAD[disparities], cadCost, width + widthPaddingTo8, LEFT_MATCHING);

        // Step 4: Conpute Combined Cost from Census Cost and Absolute Difference
        ((computeCombinedCost)fTable[fIdx++])(cdisparityCost, cadCost, 5, 1, width, 2);
        //if (i == 0)
          //  printf("COST: %d %d %d\n", cdisparityCost[0], cdisparityCost[1], cdisparityCost[2] );
         // DEBUG WTA
        //mvcvCensusMin64(cdisparityCost, (u8*)&ccensusValueLeft[0], (u8*)&ccensusValueRight[0], width);

        // Add padding
        addPadding1(clinesPointerLeft[cfgCensusKernelSize], width, cfgLinePadding);
        addPadding1(clinesPointerRight[cfgCensusKernelSize], width, cfgLinePadding);

        // ROL line pointers
        rotateBuffersOnePosition1(clinesPointerLeft, cfgCensusKernelSize);
        rotateBuffersOnePosition1(clinesPointerRight, cfgCensusKernelSize);
    }
#ifdef DMA_USAGE
    ScCmxDmaCreateTransaction(&hnd1, &list1,
            cdisparityCost,
            (u8*)(outputCost + (height - 1) * width * disparities),
            width * disparities);
    ScCmxDmaStartTransfer(&hnd1);
    ScCmxDmaWaitTransaction(&hnd1);
#else
    memcpy((u8*)(outputCost + (height - 1) * width * disparities), cdisparityCost, width * disparities);
#endif

    // set pointers back to the begining of the mem space
    for (u32 i = 0; i < cfgCensusKernelSize + 1; i++)
    {
        clinesPointerLeft[i]  -= cfgLinePadding;
        clinesPointerRight[i] -= cfgLinePadding;
    }

}

extern "C" void computeCost(stereoUserConfig_t* algoCfg,
        stereoMatImage_t *leftImage,
        stereoMatImage_t *rightImage,
        u8* outputCost)
{
    computeCostInit(leftImage->w);
#ifdef DMA_USAGE
    ScCmxDmaInitialize(NULL);
#endif

    algoCfg->maxDisparities  = MAX_DISPARITIES_LEVEL0;
    computeCostBase(algoCfg, leftImage, rightImage, outputCost);
    algoCfg->maxDisparities  = MAX_DISPARITIES_LEVEL1;

#ifdef __PC__
    FILE* f = fopen("cost.raw", "wb");
    fwrite(outputCost, 320*240, 1, f);
    fclose(f);
#endif
}
