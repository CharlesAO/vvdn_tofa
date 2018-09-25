#ifndef __ARITHMETICADD_H__
#define __ARITHMETICADD_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// The kernel performs addition of two input images
///@param[in]	src1Addr	- Input lines from the first image
///@param[in]	src2Addr	- Input lines from the second image
///@param[out]	destAddr	- Output line
///@param[in] 	width		- Width of input line

MVCV_FUNC    (void, mvcvArithmeticAdd, u8** src1, u8** src2, u8** dst, u32 inWidth)
//!@}

#endif //__ARITHMETICADD_H__
