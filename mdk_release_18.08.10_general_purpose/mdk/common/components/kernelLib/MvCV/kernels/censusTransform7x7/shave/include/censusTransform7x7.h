#ifndef __CENSUSTRANSFORM7x7_H__
#define __CENSUSTRANSFORM7x7_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// censusTransform7x7 kernel 
/// @param[in] in         - array of pointers to input lines
/// @param[in] out        - pointer to output line
/// @param[in] inWidth    - width of input line

MVCV_FUNC(void, mvcvCensusTransform7x7, u8** in, u64* out, u32 width)
//!@}

#endif //__CENSUSTRANSFORM7x7_H__
