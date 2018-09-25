#ifndef __CENSUS_MIN16_H__
#define __CENSUS_MIN16_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvCensusMin - computes minimum of 16 disparity costs values
/// @param[in] in             - pointer to disparity costs
/// @param[out] out           - array of disparity cost
/// @param[in] width          - width of the input lines
/// @return    Nothing

MVCV_FUNC(void, mvcvCensusMin16, u8 *in, u8 *out, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED

#include "moviVectorUtils.h"
extern "C" {
	void mvcvCensusMin16_opt( u8 *in, u8 *out, u32 width);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__CENSUS_MIN_H__

