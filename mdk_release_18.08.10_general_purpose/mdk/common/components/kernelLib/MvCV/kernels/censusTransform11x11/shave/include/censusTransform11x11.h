#ifndef __CENSUSTRANSFORM11x11_H__
#define __CENSUSTRANSFORM11x11_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// censusTransform11x11 kernel - The centre pixel’s intensity value is replaced by the bit string composed of set of boolean comparisons such that in a square window, moving left to right
/// @param[in] in         - array of pointers to input lines
/// @param[in] out        - pointer to output line
/// @param[in] inWidth    - width of input line

MVCV_FUNC(void, mvcvCensusTransform11x11, u16** in, u8* out, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvCensusTransform11x11_opt(u16** in, u8* out, u32 width);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__CENSUSTRANSFORM11x11_H__
