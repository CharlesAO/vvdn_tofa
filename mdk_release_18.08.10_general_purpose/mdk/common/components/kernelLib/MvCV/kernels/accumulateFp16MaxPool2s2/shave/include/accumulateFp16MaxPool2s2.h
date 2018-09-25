#ifndef __ACCUMULATEFP16MAXPOOL2S2_H__
#define __ACCUMULATEFP16MAXPOOL2S2_H__
#include <mv_types.h>
#include <mvcv_macro.h>
    
//!@{
/// mvcvaccumulateFp16MaxPool2s2 kernel computes does an addition between elements in *dst and *input, and for the output takes the maximum in range of two
/// s2 - provides one output pixel at each two input pixels
/// @param[in][out] dst        - Input line that contains elements to be added and also destination for new elements
/// @param[in] input           - Input line that contains elements to be added
/// @param[in] width           - Width of the input lines
/// @return    Nothing

MVCV_FUNC(void, mvcvAccumulateFp16MaxPool2s2, half** dst, half** input, u32 width)
//!@}


#endif //__ACCUMULATEFP16MAXPOOL2s2_H__