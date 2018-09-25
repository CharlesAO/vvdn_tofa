///
/// @file Helpers.cpp
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Helpers Functions
///

#include "helpers.h"
#include <Defines.h>

#include <scCmxDma.h>
#include <svuCommonShave.h>

#include <string.h>
#include <stdio.h>

static ScCmxDmaTransaction listLeft, listRight;

#define DMA_USAGE

//----------------------------------------------------------------------------------//
// addPadding - helpers function to add padding to input lines for the MvCV kernels //
//----------------------------------------------------------------------------------//
void addPadding(u8 *pointer, u32 width, u32 paddingSize)
{
    u32 widthAux = width - 1;

    for (u32 i = 1; i <= paddingSize; i++)
    {
        *(pointer - i) = *pointer;
        *(pointer + widthAux + i) = *(pointer + widthAux);
    }
}

void addPaddingLeft(u8* firstElem, u32 paddingSize)
{
    for (u32 i = 1; i <= paddingSize; i++)
        *(firstElem - i) = *firstElem;
}

void addPaddingRight(u8* lastElem, u32 width, u32 paddingSize)
{
    for (u32 i = 1; i <= paddingSize; i++)
    {
        *(lastElem + (width - 1) + i) = *lastElem;
        //*(lastElem  + i) = *lastElem;
    }
}

//---------------------------------------------------------------------//
// alignLine - helpers function to align each u8 line to multiple of x //
//---------------------------------------------------------------------//
void alignLine(u8 *pointer, u32 width, u32 paddingSize)
{
    for (u32 i = 0; i < paddingSize; i++)
    {
        *(pointer + width - 1 + i) = *(pointer + width -1);
    }
}

//----------------------------------------------------------------------//
// alignLine - helpers function to align each u32 line to multiple of x //
//----------------------------------------------------------------------//
void alignLineU32(u32 *pointer, u32 width, u32 paddingSize)
{
    for (u32 i = 0; i < paddingSize; i++)
    {
        *(pointer + width + i) = *(pointer + width -1);
    }
}
void alignLineU64(u64 *pointer, u32 width, u32 paddingSize)
{
    for (u32 i = 0; i < paddingSize; i++)
    {
        *(pointer + width + i) = *(pointer + width -1);
    }
}

//---------------------------------------------------------------//
// rotateBuffersOnePosition - helpers function to rotate buffers //
//---------------------------------------------------------------//
void rotateBuffersOnePosition(u8* ptrs[], u32 num)
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

//---------------------------------------------------------------------//
// rotateBuffers - helpers function to rotate buffers with 2 positions //
//---------------------------------------------------------------------//
void rotateBuffers(u8* ptrs[])
{
    // Keep the first pointers because they will be overwritten
    u8* tmp1 = ptrs[0];
    u8* tmp2 = ptrs[1];

    // Shift all pointers to the left location
    ptrs[0] = ptrs[2];
    ptrs[1] = ptrs[3];
    ptrs[2] = ptrs[4];

    ptrs[CENSUS_KERNEL_SIZE-2] = tmp1;
    ptrs[CENSUS_KERNEL_SIZE-1] = tmp2;
}

//--------------------------------------------------------------------------//
// dmaInFirstImageLines - dma transfer first lines needed for Census kernel //
//--------------------------------------------------------------------------//
void dmaInFirstImageLines(u32 svuIdx,
        stereoUserConfig_t* userCfg, stereoTileConfig_t* tileCfg,
        u8** dstLeft, u8** dstRight)
{
    ScCmxDmaTransactionHnd hnd1, hnd2;

    const u32 svuCnt                = COUNT_OF(userCfg->list_of_shaves_thread_1);
    const u32 cfgLinePadding        = tileCfg->censusKernelSize / 2;
    const u32 cfgCensusKernelSize   = tileCfg->censusKernelSize;
    const u32 halfKernel            = (s32)(cfgCensusKernelSize / 2);

    const u32 widthLocal            = tileCfg->leftImage.spec.width / svuCnt;
    const u32 step                  = tileCfg->leftImage.spec.stride;

    const u8* srcLeftPtrLocal = tileCfg->leftImage.p1 + svuIdx * widthLocal;
    const u8* srcRightPtrLocal = tileCfg->rightImage.p1 + svuIdx * widthLocal;

    s32 startOffset = 0;
    s32 endOffset = 0;
    u32 linesOffset = 0;
    for (u32 i = 0; i < cfgCensusKernelSize; i++)
    {
#ifdef __PC__
        // PC processes entire line so life is simple here
        dstLeft[i] += cfgLinePadding;
        dstRight[i] += cfgLinePadding;
#else
        if (svuIdx == 0)
        {
            // first SHAVE needs padding to the left and 3 extra pixels for CT to the right
            dstLeft[i] += cfgLinePadding;
            dstRight[i] += cfgLinePadding;
            endOffset = halfKernel; // extra size: dma 3 pixels to the right; padd 3 pixels to the left
            linesOffset = EXTRA_PIXELS_ADPYR;
        }
        else if (svuIdx == (svuCnt - 1))
        {
            // last SHAVE needs padding to the right and 3 extra pixels for CT to the left
            startOffset = -(MAX_CENSUS_PYR_OFFSET + cfgLinePadding);
            endOffset = MAX_CENSUS_PYR_OFFSET + cfgLinePadding; // extra size: dma 28 pixels to the left; padd 3 pixels to the right
        }
        else
        {
            // middle SHAVEs need 3 extra pixels to the left and 3 extra pixels to the right for CT
            startOffset = -(MAX_CENSUS_PYR_OFFSET + cfgLinePadding);
            endOffset = MAX_CENSUS_PYR_OFFSET + cfgLinePadding + cfgLinePadding; // extra size: dma 28 pixels to the left; dma 3 pixels to the right
            linesOffset = EXTRA_PIXELS_ADPYR;
        }
#endif
        if (i <= halfKernel)
        {
            ScCmxDmaCreateTransaction(&hnd1, &listLeft,
                        (u8*)srcLeftPtrLocal + startOffset,
                        (u8*)dstLeft[i],
                        widthLocal + endOffset + linesOffset);
            ScCmxDmaCreateTransaction(&hnd2, &listRight,
                        (u8*)srcRightPtrLocal + startOffset,
                        (u8*)dstRight[i],
                        widthLocal + endOffset + linesOffset);
        }
        else
        {
            ScCmxDmaCreateTransaction(&hnd1, &listLeft,
                        (u8*)(srcLeftPtrLocal + (i - halfKernel) * step) + startOffset,
                        (u8*)dstLeft[i],
                        widthLocal + endOffset + linesOffset);
            ScCmxDmaCreateTransaction(&hnd2, &listRight,
                        (u8*)(srcRightPtrLocal + (i - halfKernel) * step) + startOffset,
                        (u8*)dstRight[i],
                        widthLocal + endOffset + linesOffset);
        }

        ScCmxDmaLinkTransactions(&hnd1, 2, &hnd1, &hnd2);
        ScCmxDmaStartTransfer(&hnd1);
        ScCmxDmaWaitTransaction(&hnd1);

#ifdef __PC__
        addPaddingLeft(dstLeft[i], cfgLinePadding);
        addPaddingLeft(dstRight[i], cfgLinePadding);
        addPaddingRight(dstLeft[i], widthLocal, cfgLinePadding);
        addPaddingRight(dstRight[i], widthLocal, cfgLinePadding);
#else
        // Only left and right need to do padding, the inner stripes just
        // copied extra pixels at the beginning and the end of the chunk
        if (svuIdx == 0)
        {
            // add padding to the left for first SHAVE
            addPaddingLeft(dstLeft[i], cfgLinePadding);
            addPaddingLeft(dstRight[i], cfgLinePadding);
        }
        else if (svuIdx == (svuCnt - 1))
        {
            // shift pointers for last SHAVE that transfered extra pixels to the left + add padding to the right
            dstLeft[i] += cfgLinePadding;
            dstRight[i] += cfgLinePadding;
            addPaddingRight(dstLeft[i], widthLocal + cfgLinePadding, cfgLinePadding);
            addPaddingRight(dstRight[i], widthLocal + cfgLinePadding, cfgLinePadding);
        }
        else
        {
            // shift pointers for middle SHAVEs that transfered extra pixels to the left/right
            dstLeft[i] += cfgLinePadding;
            dstRight[i] += cfgLinePadding;
        }
#endif
    }
}

//--------------------------------------------------------------------------//
// prerequisitesCensus - handle needed prerequisites for census matching    //
//--------------------------------------------------------------------------//
void prerequisitesCensus(u32* censusRight, u32 disparities)
{
    // padd census right values
    for (u32 j = 0; j < disparities; j++)
    {
        censusRight[j] = censusRight[disparities];
    }
}
//--------------------------------------------------------------------------//
// prerequisitesCensus - handle needed prerequisites for census matching    //
//--------------------------------------------------------------------------//
void prerequisitesCensusU64(u64* censusRight, u32 disparities)
{
    // padd census right values
    for (u32 j = 0; j < disparities; j++)
    {
        censusRight[j] = censusRight[disparities];
    }
}

//--------------------------------------------------------------------------//
// prerequisitesAD - handle needed prerequisites for absolute differences   //
//--------------------------------------------------------------------------//
void prerequisitesAD(u8* imgRightOutput, u32 disparities)
{
    // padd image right values to the left
    for (u32 j = 0; j < disparities; j++)
    {
        imgRightOutput[j] = imgRightOutput[disparities];
    }
}

//----------------------------------------------------------------------------------------//
// computeDisparity - compute current disparity from census min and previous disparity    //
//----------------------------------------------------------------------------------------//
void computeDisparity(u8* out, u8* in, u32 width)
{
#ifdef __PC__
    for(u32 idx = 0; idx < width; idx++)
    {
        out[idx] = (u8)(out[idx] - 1 + 2*in[(u32)(idx/2)]);
    }
#else
    uchar16* vMin = (uchar16*)out;
    uchar8* pDisp = (uchar8*)in;

    u32 widthScaled = width / 16;
    u32 widthRest = width % 16;

    uchar16 pDispInterlevead;

    for (u32 idx = 0; idx < widthScaled; idx++)
    {
        vMin[idx] = vMin[idx] - 1;
        __asm volatile(\
            "CMU.VILV.X8 %[bufOut] %[bufIn] %[bufIn] %[bufIn]\n"\
            :[bufOut] "=&r"(pDispInterlevead)\
            :[bufIn] "r"(pDisp[idx]));
        pDispInterlevead = pDispInterlevead * 2;
        vMin[idx] = vMin[idx] + pDispInterlevead;
    }

    u8* vMin8 = out + width - widthRest;
    u8* pDistRest = in + width/2 - widthRest/2;
    for(u32 idx = 0; idx < widthRest; idx++)
    {
        vMin8[idx] = (u8)(vMin8[idx] - 1 + 2*pDistRest[(u32)(idx/2)]);
    }
#endif
}

void floatComputeDisparity(disparityType* out, u8* in, u32 width)
{

#ifdef __PC__
    u32 wta_window = 3;
    u32 toSubtract = wta_window / 2;
    for(u32 idx = 0; idx < width; idx++)
    {
        disparityType d_qvga = 2*in[(u32)(idx/2)];
        out[idx] = (out[idx] - toSubtract + (disparityType)d_qvga);

        if (out[idx] < 0)
            out[idx] = 0;
    }
#else
    u32 wta_window = 3;
    u32 toSubtract = wta_window / 2;
    half8* vMin = (half8*)out;
    uchar8* pDisp = (uchar8*)in;

    u32 widthScaled = width / 16;
    u32 widthRest = width % 16;

    uchar16 pDispInterlevead;
    uchar16 local_var_tmpAddr;
    half8 pDispInterleavedHalf;
    for (u32 idx = 0; idx < widthScaled; idx++)
    {
        vMin[idx*2] = vMin[idx*2] - toSubtract;
        vMin[idx*2+1] = vMin[idx*2+1] - toSubtract;
        __asm volatile(\
                "CMU.VILV.X8 %[bufOut] %[bufIn] %[bufIn] %[bufIn]\n"\
                :[bufOut] "=&r"(pDispInterlevead)\
                 :[bufIn] "r"(pDisp[idx]));
        pDispInterlevead = pDispInterlevead * 2;

        __asm volatile(\
                "CMU.vszm.word %[tmpAddr] %[bufIn] [3210] \n"\
                "CMU.cpvv.i8.f16 %[bufOut] %[tmpAddr] \n"\
                :[bufOut] "=&r"(pDispInterleavedHalf),\
                  [tmpAddr] "=&r" (local_var_tmpAddr) \
                 :[bufIn] "r"(pDispInterlevead));
        vMin[idx*2] = vMin[idx*2] + pDispInterleavedHalf;


        __asm volatile(\
                "CMU.vszm.word %[tmpAddr] %[bufIn] [0132] \n"\
                "CMU.cpvv.i8.f16 %[bufOut] %[tmpAddr] \n"\
                :[bufOut] "=&r"(pDispInterleavedHalf),\
                  [tmpAddr] "=&r" (local_var_tmpAddr) \
                 :[bufIn] "r"(pDispInterlevead));
        vMin[idx*2+1] = vMin[idx*2+1] + pDispInterleavedHalf;
}

    half* vMin8 = out + width - widthRest;
    u8* pDistRest = in + width/2 - widthRest/2;
    for(u32 idx = 0; idx < widthRest; idx++)
    {
        vMin8[idx] = (half)(vMin8[idx] - toSubtract + 2*pDistRest[(u32)(idx/2)]);
    }
#endif
}
