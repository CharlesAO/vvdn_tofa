#ifndef __MEDIANFILTER7X7_H__
#define __MEDIANFILTER7X7_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel applies median blur filter with 7x7 size, histogram based method for median calculation
/// @param[in] widthLine - Width of input line
/// @param[out] outLine  - Output line
/// @param[in] inLine    - Input lines

MVCV_FUNC(void, mvcvMedianFilter7x7, u32 widthLine, u8 **outLine, u8 ** inLine)
//!@}

#endif //__MEDIANFILTER7X7_H__