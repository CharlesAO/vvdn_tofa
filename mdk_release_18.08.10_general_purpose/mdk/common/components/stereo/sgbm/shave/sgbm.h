///
/// @file SGBM.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Apply cost aggregation over a census cost (inspired by Semi-Global block Matching); SGBM Interface
///


#ifndef SGBM_H
#define SGBM_H

#include <mv_types.h>
#include <disparityMapApiDefines.h>

#define D_DISPARITIES       63
// SGBM param
const u32 ref_P1 = 500;
const u32 ref_P2 = 1000;

//---------------------------------------------------------------------------//
//-------------------------- Exported Functions -----------------------------//
//---------------------------------------------------------------------------//

/******************************************************************************
 @brief     : PC function for computing cost aggregation by adding the 4 paths costs

 @param     : path0             : cost for path0
 @param     : path1             : cost for path1
 @param     : path2             : cost for path2
 @param     : path3             : cost for path3
 @param     : out               : aggregated cost
 @param     : width             : line size
 @param     : disparities       : number of disparities
 @return    : void
******************************************************************************/
void aggregatePaths(u8* path0,
                    u8* path1,
                    u8* path2,
                    u8* path3,
                    u8* out,
                    u32 width,
                    u32 disparities);

/******************************************************************************
 @brief     : Vectorized function to compute cost aggregation by adding the 4 paths costs

 @param     : path0             : cost for path0
 @param     : path1             : cost for path1
 @param     : path2             : cost for path2
 @param     : path3             : cost for path3
 @param     : out               : aggregated cost
 @param     : width             : line size
 @param     : disparities       : number of disparities
 @return    : void
******************************************************************************/
void aggregatePathsVectorized(u8* path0,
                                u8* path1,
                                u8* path2,
                                u8* path3,
                                u8* out,
                                u32 width,
                                u32 disparities);

/******************************************************************************
 @brief     : Compute aggregated cost for boundary pixels

 @param     : aggregatedCostPath    : pointer to aggregated costs for each path
 @param     : matchingCost          : Census matching cost
 @param     : width                 : line size
 @param     : disparities           : number of disparities
 @param     : line                  : line number
 @return    : void
******************************************************************************/
void aggregateCostForBoundaryPixels(u32 svuIdx,
        u8* aggregatedCostPreviousPath,
        u8* aggregatedCostCurrentPath,
        u8* matchingCost,
        u32 width, u32 disparities, u32 line);

static inline void paddLine(u8* dst, u8* src)
{
    *dst = *src;
}

static inline void paddCost(u8* dst, u8* src, u32 disp)
{
    memcpy(dst, src, disp);
}


/******************************************************************************
 @brief     : Compute aggregated cost using 4 paths

 @param	    : svuIdx				: zero-based Shave index (0-4)
 @param     : userConfig			: pointer to app level configuration
 @param     : sgbmConfig            : pointer to SGBMConfig structure
 @param     : imgLine               : pointer to input image lines
 @param     : aggregatedCostPath    : pointer to aggregated costs for each path
 @param     : disparities           : number of disparities
 @return    : void
******************************************************************************/
void semiGlobalBlockMatching(u32 svuIdx,
                            stereoUserConfig_t* userCfg,
                            stereoSGBMConfig_t* sgbmConfig,
                            u8* imgLine[2],
                            u8* cost[2],
                            u8* aggCostPath[2],
                            u8* aggCostAllPaths,
                            u32 line, u8* test[2][5]);

#endif // SGBM_H
