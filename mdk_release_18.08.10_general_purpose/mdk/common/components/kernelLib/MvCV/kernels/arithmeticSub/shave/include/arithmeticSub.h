#ifndef __ARITHMETICSUB_H__
#define __ARITHMETICSUB_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs subtraction for two input images
///@param[in]	src1Addr	- Input lines from the first image
///@param[in]	src2Addr	- Input lines from the second image
///@param[out]	destAddr	- Output line
///@param[in] 	width		- Width of input line

MVCV_FUNC(void, mvcvArithmeticSub, u8** src1, u8** src2, u8** dst, u32 inWidth)
//!@}

#endif //__ARITHMETICSUB_H__
