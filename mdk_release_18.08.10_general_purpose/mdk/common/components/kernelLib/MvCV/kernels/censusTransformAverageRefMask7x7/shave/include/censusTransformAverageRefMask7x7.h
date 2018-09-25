#ifndef __CENSUSTRANSFORMAVERAGEREFMASK7X7_H__
#define __CENSUSTRANSFORMAVERAGEREFMASK7X7_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// CENSUSTRANSFORMAVERAGEREF7X7 kernel - this kernel performs comparision between the average of windows size of (7x7)
/// and pixels in windows of 7x7 based on a mask that takes in consideration only the odd pixels
/// @param[in] in         - array of pointers to input lines
/// @param[in] out        - pointer to output line
/// @param[in] inWidth    - width of input line

MVCV_FUNC(void, mvcvCensusTransformAverageRefMask7x7, u8** in, u32* out, u32 width)
//!@}

#endif //__CENSUSTRANSFORMAVERAGEREFMASK7X7_H__
