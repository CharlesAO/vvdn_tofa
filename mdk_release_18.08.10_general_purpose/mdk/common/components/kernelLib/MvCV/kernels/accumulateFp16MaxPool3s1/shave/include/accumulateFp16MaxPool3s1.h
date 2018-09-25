#ifndef __ACCUMULATEFP16MAXPOOL3S1_H__
#define __ACCUMULATEFP16MAXPOOL3S1_H__
#include <mv_types.h>
#include <mvcv_macro.h>
    
//!@{
/// mvcvaccumulateFp16MaxPool3s1 kernel computes does an addition between elements in *dst and *input, and for the output takes the maximum in range of three
/// s1 - provides one output pixel at each input pixels
/// @param[in][out] dst        - Input line that contains elements to be added and also destination for new elements
/// @param[in] input           - Input line that contains elements to be added
/// @param[in] width           - Width of the input lines
/// @return    Nothing

MVCV_FUNC(void, mvcvAccumulateFp16MaxPool3s1, half** dst, half** input, u32 width)
//!@}


#endif //__ACCUMULATEFP16MAXPOOL3S1_H__