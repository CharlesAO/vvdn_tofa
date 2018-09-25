#ifndef __CENSUS_MIN7_H__
#define __CENSUS_MIN7_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvCensusMin - computes minimum of 7 disparity costs values
/// @param[in] in             - pointer to disparity costs
/// @param[out] out           - array of disparity cost
/// @param[in] width          - width of the input lines
/// @return    Nothing

#define DISPARITIES 7

MVCV_FUNC(void, mvcvCensusMin7, u8 *in, u8 *out, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvCensusMin7_opt(u8 *in, u8 *out, u32 width);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__CENSUS_MIN_H__
