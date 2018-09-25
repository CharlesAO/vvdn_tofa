///
/// @file DisparityMap.cpp
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Module used to compute dense disparity map; Disparity Map Source File
///

//---------------------------------------------------------------------------//
//------------------------------- INCLUDES ----------------------------------//
//---------------------------------------------------------------------------//

// C++ includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Local Modules
#include "DisparityMapApi.h"
#include "Sections.h"
#include "Defines.h"
#include "Helpers.h"

// MvCV
#include <censusTransform5x5.h>
#include <censusMatching64.h>
#include <censusMin64.h>

// DMA
#include "swcCdma.h"
#include <svuCommonShave.h>

//---------------------------------------------------------------------------//
//------------------------------- Variables ---------------------------------//
//---------------------------------------------------------------------------//

// Debug
//#define DM_Debug printf
#define DM_Debug(string)
#define LEFT_MATCHING 0

// MvCV kernels
#ifdef __PC__
#define censusTransform mvcvCensusTransform5x5
#define mvcvCensusMatching64 mvcvCensusMatching64
#define mvcvCensusMin64 mvcvCensusMin64
#else
#define censusTransform mvcvCensusTransform5x5_asm
#define mvcvCensusMatching64 mvcvCensusMatching64_asm
#define mvcvCensusMin64 mvcvCensusMin64_asm
#endif

// Local buffers
static u32 censusValueLeft[(MAX_WIDTH + MAX_PADDING_8)*sizeof(u32)];
static u32 censusValueRight[(MAX_WIDTH + MAX_PADDING_8 + MAX_DISPARITIES)*sizeof(u32)];
static u8 disparityCost[MAX_WIDTH * MAX_DISPARITIES];
static u8 *disparityMin[MEDIAN_KERNEL_SIZE];
static u8 *linesPointerLeft[CENSUS_KERNEL_SIZE + 1];
static u8 *linesPointerRight[CENSUS_KERNEL_SIZE + 1];
static u8 *predictedDisp[2];
static u8 *result[2];

static u8 lineLeftHeapBuffer[(MAX_WIDTH + 2*LINE_PADDING + MAX_PADDING_16)*(CENSUS_KERNEL_SIZE + 1)];
static intptr_t imageLineLeft[CENSUS_KERNEL_SIZE + 1];
static intptr_t lineLeftHeap;

static u8 lineRightHeapBuffer[(MAX_WIDTH + 2*LINE_PADDING + MAX_PADDING_16)*(CENSUS_KERNEL_SIZE + 1)];
static intptr_t imageLineRight[CENSUS_KERNEL_SIZE + 1];
static intptr_t lineRightHeap;

static u8 resultLineBuffer[MAX_WIDTH*2] ALIGNED(8);
static intptr_t resultLine[2] ALIGNED(8);
static intptr_t resultAddrHeap ALIGNED(8);

static u8 dispMinLineBuffer[BASE_BUFFER_DISPARITIES_SIZE*MEDIAN_KERNEL_SIZE] ALIGNED(8);
static intptr_t dispMinLine[MEDIAN_KERNEL_SIZE] ALIGNED(8);
static intptr_t dispMinHeap ALIGNED(8);

static u8 predictedDispLineBuffer[(MAX_WIDTH + 2*MEDIAN_PADDING)*MEDIAN_KERNEL_SIZE];
static intptr_t predictedDispLine[MEDIAN_KERNEL_SIZE];
static intptr_t predictedDispHeap;

u32 widthPaddingTo16 = 0;
u32 widthPaddingTo8 = 0;
u32 widthPaddingTo4 = 0;
u32 widthMod16 = 0;

// Debug variables
u32 dbg_init = 0;
u32 dbg_init_pyr = 0;
u32 dbg_base = 0;
u32 dbg_pyr = 0;
u32 dbg_run = 0;

dmaTransactionList_t list1, list2, list3, list4, list5, list6, list7, list8, list9, list10;

//---------------------------------------------------------------------------//
//------------------------------- Functions ---------------------------------//
//---------------------------------------------------------------------------//

//------------------------------------------------//
// disparityMapInit - allocate all buffers needed //
//------------------------------------------------//
extern "C" void disparityMapInit(AlgorithmConfig* algoCfg, u32 width)
{

    // read algo parameters

    u32 cfgCensusKernelSize = algoCfg->cfgCensusKernelSize;
    u32 cfgLinePadding      = algoCfg->cfgLinePadding;
#ifdef __PC__
    u32 cfgMedianPadding    = algoCfg->cfgMedianPadding;
#endif
    u32 cfgMedianKernelSize = algoCfg->cfgMedianKernelSize;

    // Line padding when using the HW filter (each line must be aligned to 8)
    widthPaddingTo8 = (width%8 == 0) ? 0 : (8 - width%8);
    widthPaddingTo16 = (width%16 == 0) ? 0 : (16 - width%16);

    // Buffers starting addresses
    lineLeftHeap      = (intptr_t)&lineLeftHeapBuffer[0];
    lineRightHeap     = (intptr_t)&lineRightHeapBuffer[0];
    resultAddrHeap    = (intptr_t)&resultLineBuffer[0];
    dispMinHeap       = (intptr_t)&dispMinLineBuffer[0];
    predictedDispHeap = (intptr_t)&predictedDispLineBuffer[0];

    // Allocate memory for lines from left image
    for (u32 i = 0; i < cfgCensusKernelSize + 1; i++)
    {
        imageLineLeft[i] = (u32)lineLeftHeap;
        lineLeftHeap += width + 2*cfgLinePadding + widthPaddingTo16;
    }

    // Allocate memory for lines from right image
    for (u32 i = 0; i < cfgCensusKernelSize + 1; i++)
    {
        imageLineRight[i] = (u32)lineRightHeap;
        lineRightHeap += width + 2*cfgLinePadding + widthPaddingTo16;
    }

    // Allocate memory for result lines
    for (u32 i = 0; i < 2; i++)
    {
        resultLine[i] = (u32)resultAddrHeap;
        resultAddrHeap += width;
    }

    // Allocate **
    for (u32 i = 0; i < cfgCensusKernelSize + 1; i++)
    {
        linesPointerLeft[i] = (u8*)imageLineLeft[i];
        linesPointerRight[i] = (u8*)imageLineRight[i];
        if (i < 2)
            result[i] = (u8*)resultLine[i];
    }

    // Allocate memory for disparityMin[0] lines
    for (u32 i = 0; i < cfgCensusKernelSize + 1; i++)
    {
        dispMinLine[i] = (u32)dispMinHeap;
#ifdef __PC__
        dispMinHeap += width + 2*cfgMedianPadding;
#else
        dispMinHeap += width;
#endif
    }

    // Allocate **
    for (u32 i = 0; i < cfgMedianKernelSize; i++)
    {
        disparityMin[i] = (u8*)dispMinLine[i];
    }

    // Allocate memory for disparityMin[0] lines
    for (s32 i = 0; i < 2; i++)
    {
        predictedDispLine[i] = (u32)predictedDispHeap;
        predictedDispHeap += (width + 1)/2;
    }

    // Allocate **
    for (s32 i = 0; i < 2; i++)
    {
        predictedDisp[i] = (u8*)predictedDispLine[i];
    }

    SHAVE_HALT;
}
u8 *p1Src, *p1Dst, *p2Src, *p2Dst, *p3Src, *p3Dst, *p4Src, *p4Dst;
//------------------------------------------------//
// disparityMapBase - Compute dense disparity map //
//------------------------------------------------//
extern "C" void disparityMapBase(AlgorithmConfig* algoCfg,
                                 u8 *leftImage,
                                 u8 *rightImage,
                                 u8 *disparity,
                                 u32 width,
                                 u32 height)
{
    dmaTransactionList_t *ref1, *ref2, *ref3, *ref4, *ref5, *ref6;
    dmaRequesterId id1 = dmaInitRequester(3);

    // read algo parameters
    u32 numberOfDisparities = algoCfg->cfgMaxDisparities;
    u32 cfgCensusKernelSize = algoCfg->cfgCensusKernelSize;
    u32 cfgLinePadding      = algoCfg->cfgLinePadding;
    u32 cfgMedianKernelSize = algoCfg->cfgMedianKernelSize;

    u32 incrementPointers = 0;//cfgMedianPadding;

    u32 halfKernel = (u32)(cfgCensusKernelSize / 2);

    u32 line;

    ///////////////////////////////////////////////////////////////////////////
    //---------------------------- Prepare Data -----------------------------//
    ///////////////////////////////////////////////////////////////////////////
    for (u32 i = 0; i < cfgCensusKernelSize; i++)
    {
        // left-right image
        linesPointerLeft[i] += cfgLinePadding;
        linesPointerRight[i] += cfgLinePadding;
        if (i <= halfKernel)
        {
            // lines for the first half of the kernel (included median line); e.g. Kernel size 7 ==> lines 0, 1, 2, 3
            ref1 = dmaCreateTransaction(id1, &list1, (u8*)leftImage, (u8*)linesPointerLeft[i], width);
            ref2 = dmaCreateTransaction(id1, &list2, (u8*)rightImage, (u8*)linesPointerRight[i], width);
        }
        else
        {
            // lines for the second half of the kernel; e.g. Kernel size 7 ==> 4, 5, 6
            ref1 = dmaCreateTransaction(id1, &list1, (u8*)(leftImage + (u32)(i - halfKernel) * width), (u8*)linesPointerLeft[i], width);
            ref2 = dmaCreateTransaction(id1, &list2, (u8*)(rightImage + (u32)(i - halfKernel) * width), (u8*)linesPointerRight[i], width);
        }

        dmaLinkTasks(ref1, 1, ref2);
        dmaStartListTask(ref1);
        dmaWaitTask(ref1);

        // Add padding
        addPadding(linesPointerLeft[i], width, cfgLinePadding);
        addPadding(linesPointerRight[i], width, cfgLinePadding);

        // censusTransform kernel requires each line to be multiple of 16
        alignLine(linesPointerLeft[i], width + cfgLinePadding, widthPaddingTo16);
        alignLine(linesPointerRight[i], width + cfgLinePadding, widthPaddingTo16);
    }

    ///////////////////////////////////////////////////////////////////////////
    //---------------------- Compute Disparity Map --------------------------//
    ///////////////////////////////////////////////////////////////////////////

    // set pointers for the last line of the kernel and results
    linesPointerLeft[cfgCensusKernelSize] += cfgLinePadding;
    linesPointerRight[cfgCensusKernelSize] += cfgLinePadding;
    for (u32 i = 0; i < cfgMedianKernelSize; i++)
        disparityMin[i] += incrementPointers;

    u32 offsetBuffer = halfKernel + 1;

    // Iterate over all lines of the image
    for (u32 i = 0; i < height; i++)
    {
        // Set last Left and Right line for the Kernel
        if ((i + offsetBuffer) >= height)
            line = height - 1;
        else
            line = i + offsetBuffer;

        ref3 = dmaCreateTransaction(id1, &list3, (u8*)(leftImage + line * width), (u8*)linesPointerLeft[cfgCensusKernelSize], width);
        ref4 = dmaCreateTransaction(id1, &list4, (u8*)(rightImage + line * width), (u8*)linesPointerRight[cfgCensusKernelSize], width);
        ref5 = 0;
        if (i >= 1)
        {
            ref5 = dmaCreateTransaction(id1, &list5, (u8*)result[0], (u8*)(disparity + (i - 1) * width), width );
        }

        dmaLinkTasks(ref3, 2, ref4, ref5);
        dmaStartListTask(ref3);

        // Step 1: Compute Census Transform
        DM_Debug("Step 1 - Compute Census \n");
        censusTransform(linesPointerLeft, censusValueLeft, width + widthPaddingTo16);
        censusTransform(linesPointerRight, &censusValueRight[numberOfDisparities], width + widthPaddingTo16);

        // Step 2: Compute Cost Map
        DM_Debug("Step 2 - Compute Cost Map \n");
        for (u32 j = 0; j < numberOfDisparities; j++)
            censusValueRight[j] = censusValueRight[numberOfDisparities];

        // censusMatching kernel requires each line to be multiple of 4
        alignLineU32(censusValueLeft, width, widthPaddingTo4);
        alignLineU32(censusValueRight, width + numberOfDisparities, widthPaddingTo4);

        mvcvCensusMatching64(censusValueLeft, &censusValueRight[numberOfDisparities], disparityCost, width + widthPaddingTo4, LEFT_MATCHING);

        dmaWaitTask(ref3);

        // Step 3: Compute Disparity Map
        DM_Debug("Step 3 - Compute Disp \n");
        mvcvCensusMin64(disparityCost, result[0], result[1], width);

        // Add padding
        addPadding(linesPointerLeft[cfgCensusKernelSize], width, cfgLinePadding);
        addPadding(linesPointerRight[cfgCensusKernelSize], width, cfgLinePadding);

        // ROL line pointers
        rotateBuffersOnePosition(linesPointerLeft, cfgCensusKernelSize);
        rotateBuffersOnePosition(linesPointerRight, cfgCensusKernelSize);
    }

    ref6 = dmaCreateTransaction(id1, &list6, (u8*)result[0], (u8*)(disparity + (height - 1) * width), width );
    dmaLinkTasks(ref6, 0);
    dmaStartListTask(ref6);
    dmaWaitTask(ref6);

    // set pointers back to the begining of the mem space
    for (u32 i = 0; i < cfgCensusKernelSize + 1; i++)
    {
        linesPointerLeft[i]  -= cfgLinePadding;
        linesPointerRight[i] -= cfgLinePadding;
    }
    for (u32 i = 0; i < cfgMedianKernelSize; i++)
        disparityMin[i] -= incrementPointers;
}

//-----------------------------------------------------------------------//
// medianFilter5x5 - apply SW median filtering for one line of the image //
//-----------------------------------------------------------------------//
void medianFilter5x5(u32 widthLine, u8 **outLine, u8 ** inLine)
{
    u32 i = 0;
    u8 *out;
    u32 j = 0;
    u32 histogram[256];
    u32 e = 0;
    const u32 MED_HEIGHT = 5;
    const u32 MED_WIDTH  = 5;
    const u32 MED_LIMIT = (MED_WIDTH*MED_HEIGHT)/2 + 1;

    out = *outLine;

    for (i=0; i<256; i++) histogram[i] = 0;
    // build initial histogram
    for (i=0; i<MED_HEIGHT; i++)
    {
        for (j=0; j<MED_WIDTH; j++)
        {
            // inLine is always passed to this function as the pointer to the begining of an array
            // This function is only called after at least 2 items had been analyzed starting from the position
            // of the element that inLine[0] points to, thus the following negative operation is valid
            e = inLine[i][(s32)(j-(MED_WIDTH>>1))];
            histogram[e]++;
        }
    }
    for (i=0; i<widthLine; i++)
    {
        e = 0;
        j = 0;
        // use histogram
        while (j<256)
        {
            e+=histogram[j];
            if (e<MED_LIMIT)
            {
                j++;
            } else
            {
                out[i] = j;
                j = 256;
            }
        }
        // substract previous values from histogram
        for (j=0; j<MED_HEIGHT; j++)
        {
            // inLine is always passed to this function as the pointer to the begining of an array
            // This function is only called after at least 2 items had been analyzed starting from the position
            // of the element that inLine[0] points to, thus the following negative operation is valid
            e = inLine[j][(s32)(i-(MED_WIDTH>>1))];
            histogram[e]--;
        }
        // add next values to histogram
        for (j=0; j<MED_HEIGHT; j++)
        {
            e = inLine[j][i+1+(MED_WIDTH>>1)];
            histogram[e]++;
        }
    }
    return;
}
//---------------------------------------------------------------//
// medianFiltering - apply median filtering on the disparity map //
//---------------------------------------------------------------//
void medianFiltering(AlgorithmConfig* algoCfg,
                    u8 *disparity,
                    u8* disparityMedian,
                    u32 width,
                    u32 height)
{
    dmaTransactionList_t *ref3, *ref4, *ref5, *ref6;
    dmaRequesterId id1 = dmaInitRequester(3);

    // read algo parameters
    u32 cfgMedianPadding    = algoCfg->cfgMedianPadding;
    u32 cfgMedianKernelSize = algoCfg->cfgMedianKernelSize;

    // reuse "linesPointerLeft" and "result" buffers
    for (u32 i = 0; i < height; i++)
    {
        ref3 = dmaCreateTransaction(id1, &list3, (u8*)(disparity + i * width), (u8*)linesPointerLeft[i % cfgMedianKernelSize], width);
        dmaStartListTask(ref3);
        dmaWaitTask(ref3);

        addPadding(linesPointerLeft[i % cfgMedianKernelSize], width, cfgMedianPadding);

        if (i == 0)
        {
            memcpy(linesPointerLeft[3]-cfgMedianPadding, linesPointerLeft[0]-cfgMedianPadding, width+2*cfgMedianPadding);
            memcpy(linesPointerLeft[4]-cfgMedianPadding, linesPointerLeft[0]-cfgMedianPadding, width+2*cfgMedianPadding);
        }
        else if (i >= 2)
        { // enough info for median filter
            medianFilter5x5(width, &result[0], linesPointerLeft);
            ref4 = dmaCreateTransaction(id1, &list5, (u8*)result[0], (u8*)(disparityMedian + (i - 2) * width), width);
            dmaStartListTask(ref4);
            dmaWaitTask(ref4);
        }
    }
    memcpy( linesPointerLeft[height % cfgMedianKernelSize]-cfgMedianPadding,
            linesPointerLeft[(height-1) % cfgMedianKernelSize]-cfgMedianPadding,
            width+ 2*cfgMedianPadding);
    medianFilter5x5(width, &result[0], linesPointerLeft);
    ref5 = dmaCreateTransaction(id1, &list5, (u8*)result[0], (u8*)(disparityMedian + (height - 2) * width), width);
    dmaStartListTask(ref5);
    dmaWaitTask(ref5);

    memcpy( linesPointerLeft[(height+1) % cfgMedianKernelSize]-cfgMedianPadding,
            linesPointerLeft[(height-1) % cfgMedianKernelSize]-cfgMedianPadding,
            width+ 2*cfgMedianPadding);
    medianFilter5x5(width, &result[0], linesPointerLeft);
    ref6 = dmaCreateTransaction(id1, &list6, (u8*)result[0], (u8*)(disparityMedian + (height - 1) * width), width);
    dmaStartListTask(ref6);
    dmaWaitTask(ref6);
}

//--------------------------------------------------------------------------------------//
// disparityMapRun - extern function that calls the computation for dense disparity map //
//--------------------------------------------------------------------------------------//
void disparityMapRun(AlgorithmConfig* algoCfg,
                        DisparityConfig* pyrCfg,
                        u32 widthL0,
                        u32 heightL0)
{
    disparityMapBase(algoCfg, pyrCfg->leftImage.p1, pyrCfg->rightImage.p1, pyrCfg->disparityMap, widthL0, heightL0);
}

//--------------------------------------------------------------------------------------//
// disparityMapRun - extern function that calls the computation for dense disparity map //
//--------------------------------------------------------------------------------------//
extern "C" void disparityMapRunPatches(AlgorithmConfig* algoCfg,
                                        DisparityConfig* pyrCfg,
                                        u32 widthL0,
                                        u32 heightL0)
{
    UNUSED(widthL0);
    UNUSED(heightL0);
    disparityMapRun(algoCfg, pyrCfg, pyrCfg->leftImage.spec.width, pyrCfg->leftImage.spec.height);

    SHAVE_HALT;
}
