#ifndef __ARITHMETICADDMASK_H__
#define __ARITHMETICADDMASK_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs addition of two input images depending on the mask value
///@param[in]	src1Addr	- Input lines from the first image
///@param[in]	src2Addr	- Input lines from the second image
///@param[out]	destAddr	- Output line
///@param[in]   mask		- Operation mask
///@param[in] 	width		- Width of input line

MVCV_FUNC(void, mvcvArithmeticAddmask, u8** src1, u8** src2, u8** dst, u8** mask, u32 inWidth)
//!@}


#endif //__ARITHMETICADDMASK_H__
