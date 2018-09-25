#ifndef __SUMOFSQUAREDDIFF5x5_H__
#define __SUMOFSQUAREDDIFF5x5_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This filter performs sum of squared differences (SSD), the differences are
/// squared and aggregated within a square window (5x5 in this case).
/// @param[in] in1		- Input lines from the first image
/// @param[in] in2		- Input lines from the second image
/// @param[out] out		- Output line
/// @param[in] width    - Width of input line

MVCV_FUNC(void, mvcvSumOfSquaredDiff5x5, u8** in1, u8** in2, u8** out, u32 width)
//!@}

#endif //__SUMOFSQUAREDDIFF5x5_H__