#ifndef __CENSUSTRANSFORM5x5_H__
#define __CENSUSTRANSFORM5x5_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// censusTransform5x5 kernel 
/// @param[in] in         - array of pointers to input lines
/// @param[in] out        - pointer to output line
/// @param[in] inWidth    - width of input line

MVCV_FUNC(void, mvcvCensusTransform5x5, u8** in, u32* out, u32 width)
//!@}

#endif //__CENSUSTRANSFORM5x5_H__
