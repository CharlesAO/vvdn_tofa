#ifndef __CENSUSTRANSFORMAVERAGEREF7X7_H__
#define __CENSUSTRANSFORMAVERAGEREF7X7_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// CENSUSTRANSFORMAVERAGEREF7X7 kernel - this kernel performs comparision between the average of windows size of (7x7)
/// and each pixel in windows of 7x7
/// @param[in] in         - array of pointers to input lines
/// @param[in] out        - pointer to output line
/// @param[in] inWidth    - width of input line

MVCV_FUNC(void, mvcvCensusTransformAverageRef7x7, u8** in, u64* out, u32 width)
//!@}

#endif //__CENSUSTRANSFORMAVERAGEREF7X7_H__
