#ifndef __SUMOFABSDIFF5X5_H__
#define __SUMOFABSDIFF5X5_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// SAD (sum of absolute differences) (5x5 size)
/// @param[in] in1		- Input lines from the first image
/// @param[in] in2		- Input lines from the second image
/// @param[out] out		- Output line
/// @param[in] width    - Width of input line

MVCV_FUNC(void, mvcvSumOfAbsDiff5x5, u8** in1, u8** in2, u8** out, u32 width)

//!@}

#endif // __SUMOFABSDIFF5X5_H__