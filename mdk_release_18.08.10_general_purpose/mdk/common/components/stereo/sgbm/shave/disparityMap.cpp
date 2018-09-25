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
#include <limits.h>

// Local Modules
#include "disparityMapApi.h"
#include "kernelsCommonDefines.h"
#include "Defines.h"
#include "helpers.h"
#include "sgbm.h"

// DMA
#include <scCmxDma.h>
#include <svuCommonShave.h>
#define DMA_USAGE

// Line buffers used at both pyramid levels
static u32 ALIGNED(16) censusValueLeft[(MAX_WIDTH / SGBM_SHAVES_USED + MAX_CENSUS_PYR_OFFSET + LINE_PADDING + MAX_PADDING_16 )] BSS;
static u32 ALIGNED(16) censusValueRight[(MAX_WIDTH / SGBM_SHAVES_USED + MAX_CENSUS_PYR_OFFSET + LINE_PADDING + MAX_PADDING_16  + MAX_DISPARITIES_LEVEL1)] BSS;

static u8 *linesPointerLeft[CENSUS_KERNEL_SIZE + 1];
static u8 *linesPointerRight[CENSUS_KERNEL_SIZE + 1];
static u8 *predictedDisparity[2];
static u8* predictedConfidence[2];

static u8 ALIGNED(16) lineLeftHeapBuffer[(MAX_WIDTH + 4*LINE_PADDING + MAX_PADDING_16 + ALIGNMENT_PADDING)*(CENSUS_KERNEL_SIZE + 1)] BSS;
static u32 imageLineLeft[CENSUS_KERNEL_SIZE + 1];
static u32 lineLeftHeap;

static u8 ALIGNED(16) lineRightHeapBuffer[(MAX_WIDTH / SGBM_SHAVES_USED + MAX_CENSUS_PYR_OFFSET + 5*LINE_PADDING + MAX_PADDING_16 + ALIGNMENT_PADDING)*(CENSUS_KERNEL_SIZE + 1)] BSS;
static u32 imageLineRight[CENSUS_KERNEL_SIZE + 1];
static u32 lineRightHeap;

static u8 ALIGNED(16) predictedDispLineBuffer[(MAX_WIDTH / SCALE_FACTOR / SGBM_SHAVES_USED + 2*LINE_PADDING + ALIGNMENT_PADDING)*2] BSS;
static u32 predictedDispLine[2];
static u32 predictedDispHeap;

static u8 ALIGNED(16) predictedConfLineBuffer[(MAX_WIDTH / SCALE_FACTOR / SGBM_SHAVES_USED + 2*LINE_PADDING + ALIGNMENT_PADDING)*2] BSS;
static u32 predictedConfLine[2] ALIGNED(8);
static u32 predictedConfHeap ALIGNED(8);

static disparityType disparityFloatLine[MAX_WIDTH / SGBM_SHAVES_USED];

// SGBM aggregation buffers - SGBM aggregation is done at QVGA level, where the patch size is MAX_PATCH_WIDTH/SCALE_FACTOR
static u8 ALIGNED(16) costBuff[AGGREGATION_PATH_BUF_SIZE * 2] BSS;
static u8* cost[2];
static u8 ALIGNED(16) aggCostAllPaths[AGGREGATION_PATH_BUF_SIZE] BSS;

#ifdef __PC__
u8 ALIGNED(16) aggCostBuff[(AGGREGATION_PATH_BUF_SIZE + 2 * DISP / SCALE_FACTOR )* 2 * SGBM_NUMBER_OF_PATHS] BSS;
u8 ALIGNED(16) aggCostBuff2[(AGGREGATION_PATH_BUF_SIZE + 2 * DISP / SCALE_FACTOR )* 2 * SGBM_NUMBER_OF_PATHS] BSS;
u8* test[2][5];
u8 ddd[MAX_WIDTH / SCALE_FACTOR * MAX_HEIGHT / SCALE_FACTOR + 25];
outputType eee[MAX_WIDTH * MAX_HEIGHT + 50];
#else
static u8* test[2][5];
static u8 ALIGNED(16) aggCostBuff[AGGREGATION_PATH_BUF_SIZE * 2] BSS;
#endif
static u8* aggCost[2];

static u32 ALIGNED(16) penaltyTableP1[256];
static u32 ALIGNED(16) penaltyTableP2[256];

static u32 widthPaddingTo16 = 0;
static u32 widthPaddingTo8 = 0;
static u32 widthPaddingTo4 = 0;
static u32 functionTableIdx = 0;

static ScCmxDmaTransaction list1, list2, list3, list4;
static u32 countInvalidatedPixels = 0;
static u32 svuLocalIdx = INT_MAX;
static u32 svuGlobalCnt = SGBM_SHAVES_USED;

//--------------------------------------------------------------------------------------//
// generateLutPenalty - precompute the division with P2 needed later in SGBM module     //
//--------------------------------------------------------------------------------------//
void generateLutPenaltyP2(void)
{
    u32 i;

    // Look-up table for penalty P2
    for (i = 0; i < 256; i++)
    {
        if (i == 0){
            penaltyTableP2[i] = ref_P2;
        }
        else{
            penaltyTableP2[i] = (ref_P2/i);
        }
    }
}

//--------------------------------------------------------------------------------------//
// generateLutPenalty - precompute the division with P2 needed later in SGBM module     //
//--------------------------------------------------------------------------------------//
void generateLutPenaltyP1(void)
{
    u32 i;

    // Look-up table for penalty P1
    for (i = 0; i < 256; i++)
    {
        if (i == 0){
            penaltyTableP1[i] = ref_P1;
        }
        else{
            penaltyTableP1[i] = (ref_P1/i);
        }
    }
}

half local_interpolateF(const unsigned char *buff, half minVal, half maxVal)
{
    const unsigned char f0 = buff[0];
    const unsigned char f1 = buff[1];
    const unsigned char f2 = buff[2];

    unsigned short a = f0 + f2 - 2 * f1;  // assuming here f1 <= f0 && f1<= f2
    if (a == 0)
    {
        return 0.f;
    }

    int denom = 2 * a;
    int numer = f0 - f2;

    half x = (half)numer / (half)denom;
    if (minVal > x)
        x = minVal;
    if (x > maxVal)
        x = maxVal;

    return x;
}

void mvcvCensusMinConfidence64Subpixel_local(u8 *input, half *minimumPosition, u8* minimumValue, u32 width)
{
    //u32 j;
    u32 disparities = 64;
    u32 scaleFactor = 255;

    u8 minCost = 255, preCost, postCost;
    u8 secondMinCost = 255;
    u8 minPos = 0;
    u32 disp;

    u8 costInterp[3];

    for (u32 i = 0; i < width; i++)
    {
        secondMinCost = 255;
        // compute minimum value and minimum position for first disparity
        disp = 0;
        minCost = input[i * disparities];
        preCost = input[i * disparities];
        postCost = input[i * disparities + 1];
        minPos = 0;

        // compute minimum value and minimum position
        for (disp = 1; disp < (disparities-1); disp++)
        {
            if (input[i * disparities + disp] < minCost)
            {
                minCost = input[i * disparities + disp];
                preCost = input[i * disparities + disp - 1];
                postCost = input[i * disparities + disp + 1];
                minPos = disp;
            }
        }

        // compute minimum value and minimum position for last disparity
        disp = disparities-1;
        if (input[i * disparities + disp] < minCost)
        {
            minCost = input[i * disparities + disp];
            preCost = input[i * disparities + disp - 1];
            postCost = input[i * disparities + disp];
            minPos = disp;
        }
        ////
        // compute minimum disparity with subpixel precision
        costInterp[0] = preCost;
        costInterp[1] = minCost;
        costInterp[2] = postCost;

        half f = local_interpolateF(&costInterp[0], -0.5f, 0.5f);
        minimumPosition[i] = (half)minPos + f;

        // check if first pixel is second min
        if (input[i * disparities + 0] < input[i * disparities + 1] && // is smaller than the right neighbour
            input[i * disparities + 0] != minCost
            &&  input[i * disparities + 0] < secondMinCost) // is not minimum value already found
        {
            secondMinCost = input[i * disparities + 0];
        }
        // compute second minimum value which is also local minimum (minimum of its two neighbours)
        for (disp = 1; disp < (disparities-1); disp++)
        {
            if (input[i * disparities + disp] < input[i * disparities + disp - 1] && // is smaller than the left neighbour
                input[i * disparities + disp] < input[i * disparities + disp + 1] && // is smaller than the right neighbour
                input[i * disparities + disp] != minCost
                &&  input[i * disparities + disp] < secondMinCost) // is not minimum value already found
            {
                secondMinCost = input[i * disparities + disp];
            }
        }
        // check if last pixel is second min
        if (input[i * disparities + (disparities - 1)] < input[i * disparities + (disparities - 1) - 1] && // is smaller than the left neighbour
            input[i * disparities + (disparities - 1)] != minCost
            &&  input[i * disparities + (disparities - 1)] < secondMinCost) // is not minimum value already found
        {
            secondMinCost = input[i * disparities + (disparities - 1)];
        }

        minimumValue[i] = scaleFactor * minCost / secondMinCost;
    }
}

void thresholdDisparityFp16_qvga(u8* input, half* output, u32* countThresPixels, u32 width, u32 threshold, half setValue)
{
#pragma clang loop unroll(enable) vectorize_width(8)
//#pragma clang loop interleave(enable)
    for(u32 col = 0; col < width; col++)
    {
        if (input[col] > threshold)
        {
            output[col] = (half)setValue;
            (*countThresPixels)++;
        }
    }
}

void pyr_l0(u8* combinedCostCube,
            stereoUserConfig_t* userCfg,
            stereoTileConfig_t* tileCfg,
            stereoSGBMConfig_t* sgbmConfig)
{
    ScCmxDmaTransactionHnd hndIn1, hndIn2;
    ScCmxDmaTransactionHnd hndOut1, hndOut2;

    // Read algo parameters
    const u32 svuCnt        = COUNT_OF(userCfg->list_of_shaves_thread_1);
    const u32 widthGlobal   = tileCfg->leftImage.spec.width;
    const u32 widthLocal    = tileCfg->leftImage.spec.width / svuCnt;
    const u32 heightGlobal  = tileCfg->leftImage.spec.height;

    const u32 disparities         = tileCfg->disparities;
    const u8 confidenceThreshold  = userCfg->confidenceThreshold;
    UNUSED(confidenceThreshold);

    qvgaType* disparity       = (qvgaType*)tileCfg->disparityMap.p1;
    qvgaType* disparityLocal  = disparity + svuLocalIdx * widthLocal;

    u8* confidence      = tileCfg->confidenceMap.p1;
    u8* confidenceLocal = confidence + svuLocalIdx * widthLocal;

    u8* leftImg         = tileCfg->leftImage.p1;

    const u32 combCostLineLenGlobal = disparities * (widthGlobal + 0);
    u8* combCostCubeGlobal          = combinedCostCube;

    // following statement may seem weird but is correct: disp = 64 at L0 means disp = 128 in the ftable (DISP_F_TABLE_IDX_D128)
    u32 fIdx =  ((disparities == 64) ? DISP_F_TABLE_IDX_D128 : DISP_F_TABLE_IDX_D64);

    assert(disparity != NULL);
    assert(confidence != NULL);

    // Make room for padding
    linesPointerLeft[0]++;
    linesPointerLeft[1]++;
    linesPointerLeft[2]++;

#ifdef MEMCPY
    memcpy(cost[0], combCostCubeGlobal + 0 * combCostLineLenGlobal, combCostLineLenGlobal);
    memcpy(linesPointerLeft[0], leftImg + 0 * widthGlobal, widthGlobal);
#else
    // Bring in the first line
    ScCmxDmaCreateTransaction(&hndIn1, &list1,
                    combCostCubeGlobal + 0 * combCostLineLenGlobal,
                    cost[0],
                    combCostLineLenGlobal);
    ScCmxDmaCreateTransaction(&hndIn2, &list2,
                    leftImg + 0 * widthGlobal,
                    linesPointerLeft[0],
                    widthGlobal);
    ScCmxDmaLinkTransactions(&hndIn1, 2, &hndIn1, &hndIn2);
    ScCmxDmaStartTransfer(&hndIn1);
    ScCmxDmaWaitTransaction(&hndIn1);
#endif

    //MPRINTF("SHV: %d %d %d\n", cost[0][0], cost[0][1], cost[0][2] );
    //MPRINTF("CUBE: %d %d %d\n", combCostCubeGlobal[0], combCostCubeGlobal[1], combCostCubeGlobal[2] );
    //asm("bru.rpim 0 \n nop 6 \n nop \n");

    // Duplicate the first line for top padding
    memcpy(linesPointerLeft[1], linesPointerLeft[0], widthGlobal);
    memcpy(linesPointerLeft[2], linesPointerLeft[0], widthGlobal);
    memcpy(cost[1], cost[0], combCostLineLenGlobal);

    // Pad the first line which becomes 'prev' in the first iteration
    paddLine(linesPointerLeft[0] - 1, linesPointerLeft[0]);
    paddLine(linesPointerLeft[0] + widthGlobal,
            linesPointerLeft[0] + (widthGlobal - 1));
#ifdef __MOVICOMPILE__
    memcpy(aggCost[0], cost[0], combCostLineLenGlobal);
    memcpy(aggCost[1], cost[0], combCostLineLenGlobal);
    paddCost(aggCost[0] - disparities, aggCost[0], disparities);
    paddCost(aggCost[0] + widthGlobal * disparities,
            aggCost[0] + (widthGlobal - 1) * disparities, disparities);
    paddCost(aggCost[1] - disparities, aggCost[0], disparities);
    paddCost(aggCost[1] + widthGlobal * disparities,
            aggCost[0] + (widthGlobal - 1) * disparities, disparities);
#else
    for (u32 i = 0; i < SGBM_NUMBER_OF_PATHS; i++)
    {
        memcpy(test[0][i], cost[0], combCostLineLenGlobal);
        memcpy(test[1][i], cost[0], combCostLineLenGlobal);

        paddCost(test[0][i] - disparities, test[0][i], disparities);
        paddCost(test[0][i] + widthGlobal * disparities,
                test[0][i] + (widthGlobal - 1) * disparities, disparities);
        paddCost(test[1][i] - disparities, test[1][i], disparities);
        paddCost(test[1][i] + widthGlobal * disparities,
                test[1][i] + (widthGlobal - 1) * disparities, disparities);
    }
#endif

    for (u32 i = 0; i < heightGlobal; i++)
    {
        semiGlobalBlockMatching(svuLocalIdx, userCfg, sgbmConfig,
                linesPointerLeft, cost, aggCost, aggCostAllPaths, i + 1, test);
#ifndef MEMCPY
         if (i > 0)
             ScCmxDmaWaitTransaction(&hndOut1);
#endif
        // Step 6: Compute Disparity Map - note: reuse censusValueLeft buffer
        // to save disparity and censusValueRight buffer to save confidence
#ifdef V4
        ((censusMinConfidence)fTable[fIdx])(aggCostAllPaths + svuLocalIdx * widthLocal * disparities,
                (u8*)&censusValueLeft[0], (u8*)&censusValueRight[0], widthLocal);
         //mvcvCensusMinConfidence64(aggCostAllPaths + svuLocalIdx * widthLocal * disparities,
           //            (u8*)&censusValueLeft[0], (u8*)&censusValueRight[0], widthLocal);
#else
        mvcvCensusMinConfidence64Subpixel(aggCostAllPaths + svuLocalIdx * widthLocal * disparities,
                         (half*)&censusValueLeft[0], (u8*)&censusValueRight[0], widthLocal);

        thresholdDisparityFp16_qvga((u8*)&censusValueRight[0], (half*)&censusValueLeft[0], &countInvalidatedPixels, widthLocal + widthPaddingTo16, confidenceThreshold, 0.0);
        #ifdef PC_VIEWER_FLAG
                 mvcvConvert_fp16_u8((half*)&censusValueLeft[0], (u8*)&censusValueLeft[0], widthLocal + widthPaddingTo8);
        #else
                 mvcvConvertFp16ToQ4((half*)&censusValueLeft[0], (unsigned short*)&censusValueLeft[0], widthLocal + widthPaddingTo8);
        #endif
#endif

        if (i < (heightGlobal - 2))
        {
#ifdef MEMCPY
            memcpy(cost[(i + 1) % 2], combCostCubeGlobal + (i + 2) * combCostLineLenGlobal, combCostLineLenGlobal);
            memcpy(linesPointerLeft[i % 3], leftImg + (i + 2) * widthGlobal, widthGlobal);
#else
            ScCmxDmaCreateTransaction(&hndIn1, &list1,
                            combCostCubeGlobal + (i + 2) * combCostLineLenGlobal,
                            cost[(i + 1) % 2],
                            combCostLineLenGlobal);
            ScCmxDmaCreateTransaction(&hndIn2, &list2,
                            leftImg + (i + 2) * widthGlobal,
                            linesPointerLeft[i % 3],
                            widthGlobal);
#endif
        }
#ifdef MEMCPY
        memcpy((u8*)(disparityLocal + i * widthGlobal), (u8*)&censusValueLeft[0], widthLocal * sizeof(qvgaType));
        memcpy((u8*)(confidenceLocal + i * widthGlobal), (u8*)&censusValueRight[0],  widthLocal);
#else
        ScCmxDmaCreateTransaction(&hndOut1, &list3,
                (u8*)&censusValueLeft[0],
                (u8*)(disparityLocal + i * widthGlobal),
                widthLocal * sizeof(qvgaType));
        ScCmxDmaCreateTransaction(&hndOut2, &list4,
                (u8*)&censusValueRight[0],
                (u8*)(confidenceLocal + i * widthGlobal),
                widthLocal);
#endif
#ifndef MEMCPY
        if (i < (heightGlobal - 2))
            ScCmxDmaLinkTransactions(&hndOut1, 4, &hndOut1, &hndOut2, &hndIn1, &hndIn2);
        else
            ScCmxDmaLinkTransactions(&hndOut1, 2, &hndOut1, &hndOut2);

        ScCmxDmaStartTransfer(&hndOut1);
#endif
    }

#ifndef MEMCPY
    ScCmxDmaWaitTransaction(&hndOut1);
#endif

#ifdef __PC__
    int w = MAX_WIDTH / SCALE_FACTOR;
    //memset(&ddd[0], 0xAA, (320+25) * 240);
    for (int i = 0; i < MAX_HEIGHT / SCALE_FACTOR; i++)
        for (int j = 0; j < w - 16; j++)
        {
            ddd[i * w + j + 16] = disparityLocal[i * w + j];
        }
    FILE* f = fopen("dispQvga.raw", "wb");
    fwrite(disparityLocal, MAX_WIDTH / SCALE_FACTOR * MAX_HEIGHT / SCALE_FACTOR * sizeof(qvgaType), 1, f);
    fclose(f);
#endif
    // Reset padding
    linesPointerLeft[0]--;
    linesPointerLeft[1]--;
    linesPointerLeft[2]--;
}

//-------------------------------------------------------------------------------------------------//
// disparityMapPyr - Compute dense disparity map for Level 1 taking into account Level 0 disparity //
//-------------------------------------------------------------------------------------------------//
void pyr_l1(stereoUserConfig_t* userCfg,
            stereoTileConfig_t* tileCfg,
            stereoSGBMConfig_t* sgbmConfig)
{
    UNUSED(sgbmConfig);
    ScCmxDmaTransactionHnd hnd1, hnd2;

    // Read algo parameters
#ifdef __PC__
    const u32 svuCnt = 1;
    memset((u8*)tileCfg[1].disparityMap.p1, 0, MAX_WIDTH * MAX_HEIGHT * 2);

#else
    const u32 svuCnt              = COUNT_OF(userCfg->list_of_shaves_thread_1);
#endif
    const u8 confidenceThreshold  = userCfg->confidenceThreshold;
    const u32 cfgCensusKernelSize = tileCfg[1].censusKernelSize;
    const u32 halfKernel          = cfgCensusKernelSize / 2;
    const u32 cfgLinePadding      = cfgCensusKernelSize / 2;

    const u32 widthGlobalL1       = tileCfg[1].leftImage.spec.width;
    const u32 widthLocalL1        = widthGlobalL1 / svuCnt;
    const u32 widthGlobalL0       = tileCfg[0].leftImage.spec.width;
    const u32 widthLocalL0        = widthGlobalL0 / svuCnt;
    const u32 height              = tileCfg[1].leftImage.spec.height;
    const u32 step                = tileCfg[1].leftImage.spec.stride;

    u8* leftImageLocal  = tileCfg[1].leftImage.p1 + svuLocalIdx * widthLocalL1;
    u8* rightImageLocal = tileCfg[1].rightImage.p1 + svuLocalIdx * widthLocalL1;
    outputType* dispLocal = (outputType*)tileCfg[1].disparityMap.p1 + svuLocalIdx * widthLocalL1;
    u8* predDispLocal   = tileCfg[0].disparityMap.p1 + svuLocalIdx * widthLocalL0;
    u8* confLocal       = tileCfg[0].confidenceMap.p1 + svuLocalIdx * widthLocalL0;

    u8* disparityCost   = aggCostAllPaths;
    u32 offsetBuffer    = (cfgCensusKernelSize / 2) + 1;

    u32 idx1, idx2;
    s32 line;
    s32 startOffset = 0;
    s32 endOffset = 0;
    u32 censusOffset = 0;
    u32 linesOffset = 0;
    assert(dispLocal != NULL);
    assert(leftImageLocal != NULL);
    assert(rightImageLocal != NULL);
    assert(confLocal != NULL);
    assert(predDispLocal != NULL);

#ifdef __PC__
    // Set pointers for the last line of the kernel and results
    linesPointerLeft[cfgCensusKernelSize] += cfgLinePadding;
    linesPointerRight[cfgCensusKernelSize] += cfgLinePadding;
#else
    if (svuLocalIdx == 0)
    {
        linesPointerLeft[cfgCensusKernelSize] += cfgLinePadding;
        linesPointerRight[cfgCensusKernelSize] += cfgLinePadding;
        endOffset = halfKernel; // extra size: dma 3 pixels to the right; padd 3 pixels to the left
        linesOffset = EXTRA_PIXELS_ADPYR;
    }
    else if (svuLocalIdx == (svuCnt - 1))
    {
        startOffset = -(MAX_CENSUS_PYR_OFFSET+cfgLinePadding);//+1);
        endOffset = MAX_CENSUS_PYR_OFFSET+cfgLinePadding; // extra size: dma 28 pixels to the left; padd 3 pixels to the right
        censusOffset = MAX_CENSUS_PYR_OFFSET;
    }
    else
    {
        startOffset = -(MAX_CENSUS_PYR_OFFSET+cfgLinePadding);
        endOffset = MAX_CENSUS_PYR_OFFSET+cfgLinePadding+cfgLinePadding; // extra size: dma 28 pixels to the left; dma 3 pixels to the right
        censusOffset = MAX_CENSUS_PYR_OFFSET;
        linesOffset = EXTRA_PIXELS_ADPYR;
    }
#endif

    widthPaddingTo16    = ((widthGlobalL1+censusOffset) % 16 == 0) ? 0 : (16 - widthGlobalL1 % 16);
    widthPaddingTo8     = (widthGlobalL1 % 8 == 0) ? 0 : (8 - widthGlobalL1 % 16);
    widthPaddingTo4     = (widthGlobalL1 % 4 == 0) ? 0 : (4 - widthGlobalL1 % 4);

    // Prepare data: dma in first 5 lines for CensusTransform5x5
    dmaInFirstImageLines(svuLocalIdx, userCfg, &tileCfg[1],
            linesPointerLeft, linesPointerRight);

    // Dma transfer disparity old
    ScCmxDmaCreateTransaction(&hnd1, &list1,
            (u8*)predDispLocal + 0 * widthGlobalL0,
            (u8*)predictedDisparity[0],
            widthLocalL0);
    ScCmxDmaStartTransfer(&hnd1);
    ScCmxDmaWaitTransaction(&hnd1);

    ScCmxDmaCreateTransaction(&hnd1, &list1,
            (u8*)confLocal + 0 * widthGlobalL0,
            (u8*)predictedConfidence[0],
            widthLocalL0);
    ScCmxDmaStartTransfer(&hnd1);
    ScCmxDmaWaitTransaction(&hnd1);

    countInvalidatedPixels = 0;

    // Compute disparity map
    for (u32 i = 0; i < height; i++)
    {
        idx1 = (i / 2) % 2;
        idx2 = (idx1 + 1) % 2;

        // Set last Left and Right line for the Kernel
        if ((i + offsetBuffer) >= height) line = height - 1;
        else line = i + offsetBuffer;

        // Transfer in new image lines for census transform
        ScCmxDmaCreateTransaction(&hnd1, &list1,
                (u8*)(leftImageLocal + line * step) + startOffset,
                (u8*)linesPointerLeft[cfgCensusKernelSize],
                widthLocalL1 + endOffset + linesOffset);
        ScCmxDmaCreateTransaction(&hnd2, &list2,
                (u8*)(rightImageLocal + line * step) + startOffset,
                (u8*)linesPointerRight[cfgCensusKernelSize],
                widthLocalL1 + endOffset + linesOffset);
        ScCmxDmaLinkTransactions(&hnd1, 2, &hnd1, &hnd2);
        ScCmxDmaStartTransfer(&hnd1);
        ScCmxDmaWaitTransaction(&hnd1);

#ifdef __MOVICOMPILE__
        if (svuLocalIdx == 0)
        {
            addPaddingLeft(linesPointerLeft[cfgCensusKernelSize], cfgLinePadding);
            addPaddingLeft(linesPointerRight[cfgCensusKernelSize], cfgLinePadding);
        }
        else if (svuLocalIdx == (svuCnt - 1))
        {
            linesPointerLeft[cfgCensusKernelSize] += cfgLinePadding;
            linesPointerRight[cfgCensusKernelSize] += cfgLinePadding;
            addPaddingRight(linesPointerLeft[cfgCensusKernelSize], widthLocalL1 + cfgLinePadding, cfgLinePadding);
            addPaddingRight(linesPointerRight[cfgCensusKernelSize], widthLocalL1 + cfgLinePadding, cfgLinePadding);
        }
        else
        {
            linesPointerLeft[cfgCensusKernelSize] += cfgLinePadding;
            linesPointerRight[cfgCensusKernelSize] += cfgLinePadding;
        }

#else
        addPaddingLeft(linesPointerLeft[cfgCensusKernelSize], cfgLinePadding);
        addPaddingLeft(linesPointerRight[cfgCensusKernelSize], cfgLinePadding);
        addPaddingRight(linesPointerLeft[cfgCensusKernelSize], widthLocalL1, cfgLinePadding);
        addPaddingRight(linesPointerRight[cfgCensusKernelSize], widthLocalL1, cfgLinePadding);
#endif

        // Transfer out the disparity map
        if (i >= 1)
        {
#ifndef PC_VIEWER_FLAG
            outputType* dstFloatPtr = (dispLocal + (i - 1) * widthGlobalL1);
            outputType* srcFloatPtr = (outputType*)&disparityFloatLine[0];
            ScCmxDmaCreateTransaction(&hnd1, &list1,
                    (u8*)(srcFloatPtr),
                    (u8*)dstFloatPtr,
                    widthLocalL1 * sizeof(outputType));
            ScCmxDmaStartTransfer(&hnd1);
            ScCmxDmaWaitTransaction(&hnd1);

#else
            ScCmxDmaCreateTransaction(&hnd1, &list1,
                    (u8*)&censusValueLeft[0],
                    (u8*)(dispLocal + (i - 1) * widthGlobalL1),
                    widthLocalL1);
            ScCmxDmaStartTransfer(&hnd1);
            ScCmxDmaWaitTransaction(&hnd1);
#endif
        }

        // Transfer in predicted disparity from previous level
        if (i % 2 == 1)
        {
            ScCmxDmaCreateTransaction(&hnd1, &list1,
                    (u8*)(predDispLocal + (i / 2 + 1) * widthGlobalL0),
                    (u8*)predictedDisparity[idx2],
                    widthLocalL0);
            ScCmxDmaStartTransfer(&hnd1);
            ScCmxDmaWaitTransaction(&hnd1);

            ScCmxDmaCreateTransaction(&hnd1, &list1,
                    (u8*)(confLocal + (i / 2 + 1) * widthGlobalL0),
                    (u8*)predictedConfidence[idx2],
                    widthLocalL0);
            ScCmxDmaStartTransfer(&hnd1);
            ScCmxDmaWaitTransaction(&hnd1);
        }

        // Step 1: Compute Census Transform
        censusTransform(linesPointerLeft, censusValueLeft, widthLocalL1 + censusOffset + widthPaddingTo16);
        censusTransform(linesPointerRight, &censusValueRight[COST_WINDOW], widthLocalL1 + censusOffset + widthPaddingTo16);

        // Step 2: Compute Cost Map
#ifdef __PC_
        prerequisitesCensus(censusValueRight, COST_WINDOW);
#else
        if (svuLocalIdx == 0)
            prerequisitesCensus(censusValueRight, COST_WINDOW);
#endif
        mvcvCensusMatchingPyrOnePosWindow(&censusValueLeft[censusOffset], &censusValueRight[COST_WINDOW+censusOffset], predictedDisparity[idx1], disparityCost, widthLocalL1);

        // Step 3: Compute Absolute Differences - note: reuse censusValueLeft buffer to save absolute difference cost
#ifdef __PC__
        prerequisitesAD(linesPointerRight[halfKernel]+censusOffset, COST_WINDOW);
#else
        if (svuLocalIdx == 0)
            prerequisitesAD(linesPointerRight[halfKernel]+censusOffset, COST_WINDOW);
#endif
        mvcvComputeADPyrOnePosWindow(linesPointerLeft[halfKernel]+censusOffset, linesPointerRight[halfKernel]+censusOffset+COST_WINDOW-1, (u8*)&censusValueLeft[0], widthLocalL1 + widthPaddingTo8);

        // Step 4:  Conpute Combined Cost from Census Cost and Absolute Difference
        mvcvComputeCombinedCost3Window(disparityCost, (u8*)&censusValueLeft[0], 5, 1, widthLocalL1 + widthPaddingTo8, 2);

        // Step 5: Compute Disparity Map - note: reuse censusValueLeft buffer to save disparity and censusValueRight buffer to save confidence
        mvcvCensusMin3SubpixelWindow(disparityCost, &disparityFloatLine[0], (u8*)&censusValueRight[0], widthLocalL1 + widthPaddingTo8);
        floatComputeDisparity(disparityFloatLine, predictedDisparity[idx1], widthLocalL1 + widthPaddingTo8);
        mvcvThresholdDisparity(predictedConfidence[idx1], disparityFloatLine, &countInvalidatedPixels, widthLocalL1 + widthPaddingTo16, confidenceThreshold, 0.0);

#ifdef PC_VIEWER_FLAG
        mvcvConvert_fp16_u8(disparityFloatLine, (u8*)&censusValueLeft[0], widthLocalL1 + widthPaddingTo8);
#else
        mvcvConvertFp16ToQ4(disparityFloatLine, (unsigned short*)disparityFloatLine, widthLocalL1 + widthPaddingTo8);
#endif

        // ROL line pointers
        rotateBuffersOnePosition(linesPointerLeft, cfgCensusKernelSize + 1);
        rotateBuffersOnePosition(linesPointerRight, cfgCensusKernelSize + 1);
    }

#ifndef PC_VIEWER_FLAG
    outputType* dstFloatPtr = (dispLocal + (height - 1) * widthGlobalL1);
    outputType* srcFloatPtr = (outputType*)&disparityFloatLine[0];
    ScCmxDmaCreateTransaction(&hnd1, &list1,
            (u8*)(srcFloatPtr),
            (u8*)dstFloatPtr,
            widthLocalL1 * sizeof(outputType));
    ScCmxDmaStartTransfer(&hnd1);
    ScCmxDmaWaitTransaction(&hnd1);
#else
    ScCmxDmaCreateTransaction(&hnd1, &list1,
            (u8*)&censusValueLeft[0],
            (u8*)(dispLocal + (height - 1) * widthGlobalL1),
            widthLocalL1);
    ScCmxDmaStartTransfer(&hnd1);
    ScCmxDmaWaitTransaction(&hnd1);
#endif
    // set pointers back to the begining of the mem space
    for (u32 i = 0; i < cfgCensusKernelSize + 1; i++)
    {
        linesPointerLeft[i]  -= cfgLinePadding;
        linesPointerRight[i] -= cfgLinePadding;
    }
#ifdef __PC__
    int shift = 0;
    for (int i = 0; i < MAX_HEIGHT ; i++)
            for (int j = 0; j < MAX_WIDTH  - shift; j++)
            {
                eee[i * MAX_WIDTH  + j + shift] = dispLocal[i * MAX_WIDTH  + j];
            }
    FILE* f = fopen("dispVga.raw", "wb");
    fwrite(eee, MAX_WIDTH * MAX_HEIGHT * sizeof(half), 1, f);
    fclose(f);
#endif
}

//-------------------------------------------------------------------------------//
// disparityMapRun - function that calls the computation for dense disparity map //
//-------------------------------------------------------------------------------//
void run(u8* combinedCostCube,
         stereoUserConfig_t* userCfg,
         stereoTileConfig_t* pyrCfg)
{
#ifdef __MOVICOMPILE__
    stereoSGBMConfig_t sgbmConfig =
    {
            .penaltyTableP1     = penaltyTableP1,
            .penaltyTableP2     = penaltyTableP2,
            .disparities        = pyrCfg[0].disparities,
            .width              = pyrCfg[0].disparityMap.spec.width
    };
#else
    stereoSGBMConfig_t sgbmConfig{};
            sgbmConfig.penaltyTableP1     = penaltyTableP1;
            sgbmConfig.penaltyTableP2     = penaltyTableP2;
            sgbmConfig.disparities        = pyrCfg[0].disparities;
            sgbmConfig.width              = pyrCfg[0].disparityMap.spec.width;
#endif
    pyr_l0(combinedCostCube, userCfg, &pyrCfg[0], &sgbmConfig);
#ifdef V4
    pyr_l1(userCfg, pyrCfg, &sgbmConfig);
#endif
}

void sharePointers(stereoUserConfig_t* userCfg)
{
    assert(userCfg != NULL);

#ifdef __MOVICOMPILE__
    for (u32 i = 0; i < 2; i++)
{
        userCfg->aggCostPaths[i][svuLocalIdx] = (u8*)absoluteDataAddr(aggCost[i]);
}
    barrier(svuLocalIdx, userCfg->list_of_shaves_thread_1, SGBM_NUMBER_OF_PATHS);
#endif
}

//------------------------------------------------//
// disparityMapInit - allocate all buffers needed //
//------------------------------------------------//
void init(stereoUserConfig_t* userCfg, stereoTileConfig_t* tileCfg)
{
    // read algo parameters
    const u32 cfgCensusKernelSize = tileCfg[1].censusKernelSize;
    const u32 width               = tileCfg[1].leftImage.spec.width;
    const u32 cfgLinePadding      = cfgCensusKernelSize / 2;
    u32 safeRange = 15;

    // Line padding
    widthPaddingTo16 = (width%16 == 0) ? 0 : (16 - width%16);

    // Buffers starting addresses
    lineLeftHeap = (u32)&lineLeftHeapBuffer[0];
    lineRightHeap = (u32)&lineRightHeapBuffer[0];
    predictedDispHeap = (u32)&predictedDispLineBuffer[0];
    predictedConfHeap = (u32)&predictedConfLineBuffer[0];

    if (DISP == 128) { functionTableIdx = DISP_F_TABLE_IDX_D128; }
    else             { functionTableIdx = DISP_F_TABLE_IDX_D64; }

    // Allocate memory for lines from left image
    for (u32 i = 0; i < cfgCensusKernelSize + 1; i++)
    {
        imageLineLeft[i] = (u32)lineLeftHeap;
        lineLeftHeap += width + 4*cfgLinePadding + widthPaddingTo16;
    }

    // Allocate memory for lines from right image
    for (u32 i = 0; i < cfgCensusKernelSize + 1; i++)
    {
        imageLineRight[i] = (u32)lineRightHeap;
        lineRightHeap += width / svuGlobalCnt + 2*cfgLinePadding + MAX_CENSUS_PYR_OFFSET + LINE_PADDING + safeRange;
    }

    // Align line buffers
    for (u32 i = 0; i < cfgCensusKernelSize + 1; i++)
    {
        linesPointerLeft[i] = ALIGN_UP((u8*)imageLineLeft[i], 16);
        linesPointerRight[i] = ALIGN_UP((u8*)imageLineRight[i], 16);
    }

    // Allocate memory for predicted disparity lines
    for (u32 i = 0; i < 2; i++)
    {
        predictedDispLine[i] = ALIGN_UP(predictedDispHeap, 16);
        predictedConfLine[i] = ALIGN_UP(predictedConfHeap, 16);
        predictedDispHeap += (width / svuGlobalCnt )/2;
        predictedConfHeap += (width / svuGlobalCnt )/2;
    }

    // Allocate **
    for (u32 i = 0; i < 2; i++)
    {
        predictedDisparity[i] = ALIGN_UP((u8*)predictedDispLine[i], 16);
        predictedConfidence[i] = ALIGN_UP((u8*)predictedConfLine[i], 16);
    }

    for (u32 i = 0; i < 2; i++)
    {
        const u32 costLen = (tileCfg[0].leftImage.spec.width) *
                tileCfg[0].disparities;
        const u32 offset = (costLen + 16) * i;

        cost[i] = ALIGN_UP(&costBuff[offset], 16);
    }

#ifdef __PC__

    for (u32 j = 0; j < SGBM_NUMBER_OF_PATHS; j++)
    {
        const u32 aggCostPathLen = (tileCfg[0].leftImage.spec.width + 2) *
                tileCfg[0].disparities + tileCfg[0].disparities;
        const u32 offset = (aggCostPathLen + 16) * j;

        userCfg->aggCostPaths[0][j] = &aggCostBuff[offset];
        userCfg->aggCostPaths[0][j] += tileCfg[0].disparities;

        test[0][j] = &aggCostBuff[offset];
        test[0][j] += tileCfg[0].disparities;

        //printf("init: %d userCfg->aggCostPaths[%d][%d] 0x%x\n", offset, 0, j, userCfg->aggCostPaths[0][j]);
    }

    for (u32 j = 0; j < SGBM_NUMBER_OF_PATHS; j++)
    {
        const u32 aggCostPathLen = (tileCfg[0].leftImage.spec.width + 2) *
                tileCfg[0].disparities + tileCfg[0].disparities;
        const u32 offset = (aggCostPathLen + 16) * j;// + (aggCostPathLen + 16) * SGBM_NUMBER_OF_PATHS;

        userCfg->aggCostPaths[1][j] = &aggCostBuff2[offset];
        userCfg->aggCostPaths[1][j] += tileCfg[0].disparities;

        test[1][j] = &aggCostBuff2[offset];
        test[1][j] += tileCfg[0].disparities;

        //printf("init: %d userCfg->aggCostPaths[%d][%d] 0x%x\n", offset, 1, j, userCfg->aggCostPaths[1][j]);
    }
#else
    for (u32 i = 0; i < 2; i++)
    {
        const u32 aggCostPathLen = (tileCfg[0].leftImage.spec.width + 2) *
                tileCfg[0].disparities;
        const u32 offset = (aggCostPathLen + 16) * i;

        aggCost[i] = ALIGN_UP(&aggCostBuff[offset], 16);
        aggCost[i] += tileCfg[0].disparities;
    }
#endif

#ifndef __PC__
    sharePointers(userCfg);
#endif
    // generate LUT with penalties for SGBM
    generateLutPenaltyP1();
    generateLutPenaltyP2();

    ScCmxDmaInitialize(NULL);
}

// Assign a consecutive, zero-based Shave index regardless of the list of Shaves
// assigned at the start of the SGBM app
static u32 getLocalSvuIndex(u32 shaves[])
{
    const u32 svuId = scGetShaveNumber();

    for (u32 i = 0; i < svuGlobalCnt; i++)
        if (svuId == shaves[i])
            return i;

    // Must never get here
    assert(1 && "Shave ID was not found in the Shave list for SGBM");
    return INT_MAX;
}

void prepMedian(unsigned short* dispSubpixelIn,
                u8* dispIntegerOut,
                u32 width,
                u32 height)
{
    ScCmxDmaTransactionHnd hnd1;
    ScCmxDmaInitialize(NULL);

    unsigned short* inputLine = (unsigned short*)&aggCostAllPaths[0];
    u8* outputLine = (u8*)&costBuff[0];
    int step = width * sizeof(unsigned short);

    // reuse "linesPointerLeft" and "result" buffers
    for (u32 i = 0; i < height; i++)
    {
        ScCmxDmaCreateTransaction(&hnd1, &list1, (u8*)(dispSubpixelIn + i * width), (u8*)inputLine, step);
        ScCmxDmaStartTransfer(&hnd1);
        ScCmxDmaWaitTransaction(&hnd1);

//#pragma clang loop unroll(enable) vectorize_width(8)
//#pragma clang vectorize(enable)
//#pragma clang loop interleave(enable)
        for (u32 j = 0; j < width; j++)
        {
            unsigned char real = (unsigned char)(inputLine[j] >> 4);
            outputLine[j] = real;
            //MPRINTF("%d -> %d\n", inputLine[j], real);
        }
        ScCmxDmaCreateTransaction(&hnd1, &list1, (u8*)(outputLine), (u8*)(dispIntegerOut + i * width), width);
        ScCmxDmaStartTransfer(&hnd1);
        ScCmxDmaWaitTransaction(&hnd1);
    }
}

void addFracToMedian(u8* dispMedianInteger,
                    unsigned short* dispMedianSubpixel,
                    u32 width,
                    u32 height)
{
    ScCmxDmaTransactionHnd hnd1;
    ScCmxDmaInitialize(NULL);

    u8* medianInputLinePtr = (u8*)&aggCostAllPaths[0];
    unsigned short* outLinePtr = (unsigned short*)&costBuff[0];
    int step = width * sizeof(unsigned short);

    // reuse "linesPointerLeft" and "result" buffers
    for (u32 i = 0; i < height; i++)
    {
        ScCmxDmaCreateTransaction(&hnd1, &list1, (u8*)(dispMedianInteger + i * width), medianInputLinePtr, width);
        ScCmxDmaStartTransfer(&hnd1);
        ScCmxDmaWaitTransaction(&hnd1);

        ScCmxDmaCreateTransaction(&hnd1, &list1, (u8*)(dispMedianSubpixel + i * width), (u8*)outLinePtr, step);
        ScCmxDmaStartTransfer(&hnd1);
        ScCmxDmaWaitTransaction(&hnd1);

        for (u32 j = 0; j < width; j++)
        {
            unsigned short real_median = (unsigned short)(medianInputLinePtr[j] << 4);
            unsigned short frac_gauss = outLinePtr[j] & 0x000F;
            outLinePtr[j] = real_median + frac_gauss;
        }
        ScCmxDmaCreateTransaction(&hnd1, &list1, (u8*)(outLinePtr), (u8*)(dispMedianSubpixel + i * width), step);
        ScCmxDmaStartTransfer(&hnd1);
        ScCmxDmaWaitTransaction(&hnd1);
    }
}
//---------------------------------------------------------------------------//
// disparityMapRunFrame - SHAVE entrypoint to call disparity map computation //
//---------------------------------------------------------------------------//
extern "C" void sgbm(u8* combinedCostCube,
                    stereoUserConfig_t* userCfg,
                    stereoTileConfig_t* tileCfg,
                    unsigned short* dispMedianSubpixel,
                    u8* dispMedianInteger,
                    u32 flag)
{
    assert(userCfg != NULL);
    assert(tileCfg != NULL);

    // Must initialize the index before anything else
#ifdef __PC__
    svuLocalIdx = 0;
#else
    svuLocalIdx = getLocalSvuIndex(userCfg->list_of_shaves_thread_1);
#endif
    //MPRINTF("%s on Shave %d/%d\n",
    //  __FUNCTION__, svuLocalIdx, svuGlobalCnt);

    init(userCfg, tileCfg);

    if (flag == 0)
    {
        run(combinedCostCube, userCfg, tileCfg);
    }
    else
    {
        if (svuLocalIdx == 0 && flag == 1)
        {
            prepMedian((unsigned short*)tileCfg[1].disparityMap.p1, (u8*)dispMedianSubpixel, tileCfg[1].leftImage.spec.width, tileCfg[1].leftImage.spec.height);
        }
        else
            if (svuLocalIdx == 0 && flag == 2)
            {
                addFracToMedian(dispMedianInteger, dispMedianSubpixel, tileCfg[1].leftImage.spec.width, tileCfg[1].leftImage.spec.height);
            }
    }
}
