#ifndef __ARITHMETICSUBMASK_H__
#define __ARITHMETICSUBMASK_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// ArithmeticSub with mask for two arrays
///@param[in]	src1Addr	- array of pointers to input lines from the first image
///@param[in]	src2Addr	- array of pointers to input lines from the second image
///@param[out]	destAddr	- array of pointers for output line
///@param[in] 	width		- width of input line

MVCV_FUNC(void, mvcvArithmeticSubmask, u8** src1, u8** src2, u8** dst, u8** mask, u32 width)
//!@}

#endif //__ARITHMETICSUBMASK_H__
