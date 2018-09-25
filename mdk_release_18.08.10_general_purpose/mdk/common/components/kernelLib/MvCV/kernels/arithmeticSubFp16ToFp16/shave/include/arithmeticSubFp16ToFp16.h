#ifndef __ARITHMETICSUBFP16TOFP16_H__
#define __ARITHMETICSUBFP16TOFP16_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{ 
///This kernel performs subtraction of two fp16 input images
///@param[in]	src1Addr	- Input lines of the first image, 16-bits floating point
///@param[in]	src2Addr	- Input lines of the second image, 16-bits floating point
///@param[out]	destAddr	- Output line, 16-bits floating point
///@param[in] 	width		- Width of input line

MVCV_FUNC(void, mvcvArithmeticSubFp16ToFp16, half** src1Addr, half** src2Addr, half** dstAddr, u32 width)
//!@}


#endif //__ARITHMETICSUBFP16TOFP16_H__