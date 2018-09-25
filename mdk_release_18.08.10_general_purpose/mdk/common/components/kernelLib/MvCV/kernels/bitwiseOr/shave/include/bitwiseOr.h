#ifndef __BITWISEOR_H__
#define __BITWISEOR_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{ 
///This kernel performs per-element bit-wise logical disjunction(OR) for two input images
///@param[in]	src1Addr	- Input lines from the first image
///@param[in]	src2Addr	- Input lines from the second image
///@param[out]	destAddr	- Output line
///@param[in] 	width		- Width of input line

MVCV_FUNC(void, mvcvBitwiseOr, u8** src1Addr, u8** src2Addr, u8** dstAddr, u32 width)
//!@}

#endif //__BITWISEOR_H__