#ifndef __SUMOFSQUAREDDIFF11x11_H__
#define __SUMOFSQUAREDDIFF11x11_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This filter performs sum of squared differences (SSD), the differences are
/// squared and aggregated within a square window (11x11 in this case)
/// @param[in] in1		- Input lines from the first image
/// @param[in] in2		- Input lines from the second image
/// @param[out] out		- Output line
/// @param[in] width    - Width of input line

MVCV_FUNC(void, mvcvSumOfSquaredDiff11x11, u8** in1, u8** in2, u8** out, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C"
{
	void mvcvSumOfSquaredDiff11x11_opt(u8** in1, u8** in2, u8** out, u32 width);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__SUMOFSQUAREDDIFF11x11_H__
