#ifndef __BITWISEAND_H__
#define __BITWISEAND_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{ 
/// This kernel performs per-element bit-wise logical conjunction(AND) for two input images
///@param[in]	src1Addr	- Input lines from the first image
///@param[in]	src2Addr	- Input lines from the second image
///@param[out]	destAddr	- Output line
///@param[in] 	width		- Width of input line

MVCV_FUNC(void, mvcvBitwiseAnd, u8** src1Addr, u8** src2Addr, u8** dstAddr, u32 width)
//!@}
#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvBitwiseAnd_opt(u8** src1Addr, u8** src2Addr, u8** dstAddr, u32 width);
}
#endif //MOVICOMPILE_OPTIMIZED
#endif //__BITWISEAND_H__
