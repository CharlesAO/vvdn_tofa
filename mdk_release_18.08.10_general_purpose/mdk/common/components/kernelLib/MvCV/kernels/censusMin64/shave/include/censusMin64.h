#ifndef __CENSUS_MIN64_H__
#define __CENSUS_MIN64_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvCensusMin - computes minimum of 64 disparity costs values
/// @param[in] in             - pointer to disparity costs
/// @param[out] out           - array of minimum positions
/// @param[out] outm           - array of minimum values
/// @param[in] width          - width of the input lines
/// @return    Nothing

MVCV_FUNC(void, mvcvCensusMin64, u8 *in, u8 *out, u8 *outm, u32 width)
//!@}
	
#endif //__CENSUS_MIN_H__