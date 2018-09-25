#ifndef __ABSOLUTEDIFF_H__
#define __ABSOLUTEDIFF_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// AbsoluteDiff kernel computes the absolute difference of two images given as parameters(used to estimate motion)
/// @param[in] in1             - Input lines of the first image
/// @param[in] in2             - Input lines of the second image
/// @param[out] out            - Output line
/// @param[in] width           - Width of the input lines
/// @return    Nothing

MVCV_FUNC(void, mvcvAbsoluteDiff, u8** in1, u8** in2, u8** out, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvAbsoluteDiff_opt(u8** in1, u8** in2, u8** out, u32 width);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__ABSOLUTEDIFF_H__
