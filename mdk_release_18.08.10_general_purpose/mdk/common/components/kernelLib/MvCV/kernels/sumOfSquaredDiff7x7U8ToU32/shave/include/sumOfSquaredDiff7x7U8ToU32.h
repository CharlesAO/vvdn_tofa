#ifndef __SUMOFSQUAREDDIFF7x7U8TOU32_H__
#define __SUMOFSQUAREDDIFF7x7U8TOU32_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs sum of squared differences (SSD) to a U32 output array,
/// the differences are squared and aggregated within a square window (7x7 in
/// this case)
/// @param[in] in1		- Input lines from the first image
/// @param[in] in2		- Input lines from the second image
/// @param[out] out	    - Output line
/// @param[in] width    - Width of input line

MVCV_FUNC(void, mvcvSumOfSquaredDiff7x7U8ToU32, u8** in1, u8** in2, u32** out, u32 width)
//!@}

#endif //__SUMOFSQUAREDDIFF7x7U8TOU32_H__
