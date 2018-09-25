#ifndef __CENSUSTRANSFORM11x11U8_H__
#define __CENSUSTRANSFORM11x11U8_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// censusTransform11x11u8 kernel - The centre pixel’s intensity value is replaced by the bit string composed of set of boolean comparisons such that in a square window, moving left to right
///                               - it has u8 pixel input, works only on multiple of 16
/// @param[in] in         - array of pointers to input lines
/// @param[in] out        - pointer to output line
/// @param[in] inWidth    - width of input line

MVCV_FUNC(void, mvcvCensusTransform11x11u8, u8** in, u8* out, u32 width)
//!@}

#endif //__CENSUSTRANSFORM11x11u8_H__
