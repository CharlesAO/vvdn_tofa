#ifndef __THRESHOLD_FILTER_S16_H__
#define __THRESHOLD_FILTER_S16_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// Threshold filter
///@param[in] line        - pointer to the input line
///@param[in] threshold   - threshold value
///@param[in] width       - input line width
///@param[out] indexList  - pointer to the output line
///@param[out] count      - pointer to the width of the output line

MVCV_FUNC(void, mvcvThresholdFilterS16, s16* line, u32 threshold, u32 width, u32* indexList, u32* count)

//!@}

#endif // __THRESHOLD_FILTER_H__
