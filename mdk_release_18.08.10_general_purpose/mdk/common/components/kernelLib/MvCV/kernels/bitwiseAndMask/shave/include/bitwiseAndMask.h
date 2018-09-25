#ifndef __BITWISEANDMASK_H__
#define __BITWISEANDMASK_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{ 
/// This filter performs per-element bit-wise logical conjunction(AND) for two input images, depending on the mask value
///@param[in]	src1Addr	- Input lines from the first image
///@param[in]	src2Addr	- Input lines from the second image
///@param[in]	maskAddr	- Operation mask
///@param[out]	destAddr	- Output line
///@param[in] 	width		- Width of input line

MVCV_FUNC(void, mvcvBitwiseAndMask, u8** src1Addr, u8** src2Addr, u8** destAddr, u8** maskAddr, u32 width)
//!@}

#endif //__BITWISEANDMASK_H__