///
/// @file awbStatsSaturated.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Calculate saturated pixels in a paxel, accumulated for a paxel region
///

#ifndef __statsAWBSatPixelsU32_H__
#define __statsAWBSatPixelsU32_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
/// awbStats kernel - calculate saturated pixels statistics - on a line, accumulate on paxels
/// @param[in]  output 	    - pointer to statistics outputs buffer
/// @param[Out] iline  	    - input pixels line
/// @param[in]  nPaxelX     - number of paxels (a paxel is patch) in current line on which to calculate statistics
/// @param[in]  gapPaxelX   - gap between 2 paxels start, in pixels
/// @param[in]  widthPaxelX - paxel width in pixels
/// @param[in]  satPaxelX   - saturation threshold
/// @return     Nothing

MVCV_FUNC(void, mvispStatsAWBSatPixelsU32, u32 *output, u16 *iline, u32 nPaxelX, u32 gapPaxelX, u32 widthPaxelX, u32 satPaxelX) 
//!@}

#endif //__statsAWBSatPixelsU32_H__
