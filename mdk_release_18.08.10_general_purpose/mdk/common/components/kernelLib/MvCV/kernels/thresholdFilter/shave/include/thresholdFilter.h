#ifndef __THRESHOLD_FILTER_H__
#define __THRESHOLD_FILTER_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// Threshold filter 
///@param[in] 
///@param[out] 


MVCV_FUNC(void, mvcvThresholdFilter, fp32* cornerScores, fp32 threshold, u32 width, u32 posOffset, u16* candPos, u32* candCount)

//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvThresholdFilter_opt(fp32* cornerScores, fp32 threshold, u32 width, u32 posOffset, u16* candPos, u32* candCount);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif // __THRESHOLD_FILTER_H__
