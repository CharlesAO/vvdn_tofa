
#ifndef __ADDV2FP16_H__
#define __ADDV2FP16_H__
#include <mv_types.h>
#include <mvcv_macro.h>
    
//!@{
/// mvcvAddV2Fp16 kernel does an addition between elements in *dst and *input
/// @param[in][out] dst        - Input line that contains elements to be added and also destination for new elements
/// @param[in] input           - Input line that contains elements to be added
/// @param[in] width           - Width of the input lines
/// @return    Nothing

MVCV_FUNC(void, mvcvAddV2Fp16, half** dst, half** input, u32 width)
//!@}


#endif //__ADDV2FP16_H__
