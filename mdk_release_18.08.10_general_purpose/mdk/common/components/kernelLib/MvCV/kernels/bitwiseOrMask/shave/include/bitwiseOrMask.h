#ifndef __BITWISEORMASK_H__
#define __BITWISEORMASK_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{ 
///This kernel performs per-element bit-wise logical disjunction(OR) for two input images, depending on the mask value
///@param[in]	src1Addr	- Input lines from the first image
///@param[in]	src2Addr	- Input lines from the second image
///@param[in]	maskAddr	- Operation mask
///@param[out]	destAddr	- Output line
///@param[in] 	width		- Width of input line

MVCV_FUNC(void, mvcvBitwiseOrMask, u8** src1Addr, u8** src2Addr, u8** destAddr, u8** maskAddr, u32 width)
//!@}

#endif //__BITWISEORMASK_H__