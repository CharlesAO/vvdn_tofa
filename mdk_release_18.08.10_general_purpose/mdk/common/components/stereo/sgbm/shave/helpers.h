///
/// @file Helpers.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Helpers Interface
///
#ifndef HELPERS_FUNCTIONS_H
#define HELPERS_FUNCTIONS_H

#include <mv_types.h>
#include <swcCdma.h>
#include <svuCommonShave.h>

#include <disparityMapApiDefines.h>

#ifndef __MOVICOMPILE__

#define MPRINTF(...) printf(__VA_ARGS__)

#else

#include <svuCommonShave.h>

#define MPRINTF(...)                \
    scMutexRequest(SGBM_MUTEX); \
    printf(__VA_ARGS__);            \
    scMutexRelease(SGBM_MUTEX)

#endif

/******************************************************************************
 @brief     : Add padding for one line of the image

 @param     : pointer           : pointer to image line
 @param     : width             : line size
 @param     : paddingSize       : padding size
 @return    : void
******************************************************************************/
void addPadding(u8 *pointer, u32 width, u32 paddingSize);

void addPaddingLeft(u8* firstElem, u32 paddingSize);

void addPaddingRight(u8* lastElem, u32 width, u32 paddingSize);


/******************************************************************************
 @brief     : Align line to be multiple of paddingSize

 @param     : pointer           : pointer to image line
 @param     : width             : line size
 @param     : paddingSize       : padding size
 @return    : void
******************************************************************************/
void alignLine(u8 *pointer, u32 width, u32 paddingSize);


/******************************************************************************
 @brief     : Align u32 line to be multiple of paddingSize

 @param     : pointer           : pointer to image line
 @param     : width             : line size
 @param     : paddingSize       : padding size
 @return    : void
******************************************************************************/
void alignLineU32(u32 *pointer, u32 width, u32 paddingSize);


/******************************************************************************
 @brief     : Rotate buffers

 @param     : ptrs              : pointer to line buffers
 @return    : void
******************************************************************************/
void rotateBuffers(u8* ptrs[]);


/******************************************************************************
 @brief     : Rotate buffers

 @param     : ptrs              : pointer to line buffers
 @param     : num               : number of line buffers
 @return    : void
******************************************************************************/
void rotateBuffersOnePosition(u8* ptrs[], u32 num);


/******************************************************************************
 @brief     : DMA transfer the first 5 lines

 @param		: tileCfg				: input images set
 @param     : dstLeft               : pointer to destination left image lines
 @param     : dstRight              : pointer to destination right image lines
 @param     : width                 : line size
 @param     : algoCfg               : pointer to AlgorithmConfig structure
 @return    : void
******************************************************************************/
void dmaInFirstImageLines(u32 svuIdx,
    stereoUserConfig_t* userCfg, stereoTileConfig_t* tileCfg,
    u8** dstLeft, u8** dstRight);


/******************************************************************************
 @brief     : Add padding for CensusMatching and align lines

 @param     : censusLeft                : pointer to census left values
 @param     : censusRight               : pointer to census right values
 @param     : width                     : line size
 @param     : widthPadding              : padding for line
 @param     : disparities               : disparities
 @return    : void
******************************************************************************/
void prerequisitesCensus(u32* censusRight, u32 disparities);
void prerequisitesCensusU64(u64* censusRight, u32 disparities);


/******************************************************************************
 @brief     : Add padding for AD computation

 @param     : imgRightInput             : pointer to input image
 @param     : imgRightOutput            : pointer to output image
 @param     : width                     : line size
 @param     : disparities               : disparities
 @return    : void
******************************************************************************/
void prerequisitesAD(u8* imgRightOutput, u32 disparities);


/******************************************************************************
 @brief     : Compute disparity from current census min and previous disparity

 @param     : out                       : pointer to census min
 @param     : in                        : pointer to previous disparity
 @param     : width                     : line size
 @return    : void
******************************************************************************/
void computeDisparity(u8* out, u8* in, u32 width);
void floatComputeDisparity(disparityType* out, u8* in, u32 width);

#ifdef __MOVICOMPILE__
static inline void barrier(const u32 svuIdx, const u32 svuList[], const u32 svuCnt)
{
    const u32 GO = 1;
    const u32 master = svuList[0];

    // First one is master, all others slaves
    if (svuIdx == master)
    {
        const u32 doneMask = (1 << svuCnt) - 1;
        // First one (master) is implicitly done once here
        u32 mask = (0x0 | (1 << master));

        // Shaves, cores born to serve, to the pharaoh
        while (mask != doneMask)
        {
            while (scFifoIsEmpty(master))
                ;
            mask |= (1 << scFifoRead());
        }

        // Now, let my people go
        for (u32 i = 0; i < svuCnt; i++)
            if (svuList[i] != master)
                scFifoWrite(svuList[i], GO);
    }
    else
    {
        // So let it be written
        scFifoWrite(master, svuIdx);
        // So let it be done
        do
        {
            while (scFifoIsEmpty(svuList[svuIdx]))
                ;
        }
        while (scFifoRead() != GO);
    }
};

// Translate virtual data address to absolute address
template <typename T>
static u32 absoluteDataAddr(T virtAddr)
{
    const u32 dataWin = swcReadOwnWindowRegister(0);
    const u32 addr = (u32)virtAddr;
    const u32 mask = 0x00FFFFFF;

    return (addr & mask) | dataWin;
}
#endif // __MOVICOMPILE__
#endif // HELPERS_FUNCTIONS_H
