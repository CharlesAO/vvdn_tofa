#ifndef __BITWISENOT_H__
#define __BITWISENOT_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{ 
///This kernel performs per-element bit-wise NOT
///@param[in]	srcAddr		- Input lines
///@param[out]	destAddr	- Output lines
///@param[in] 	width		- Width of input line

MVCV_FUNC(void, mvcvBitwiseNot, u8** srcAddr, u8** dstAddr, u32 width)

#endif //__BITWISENOT_H__