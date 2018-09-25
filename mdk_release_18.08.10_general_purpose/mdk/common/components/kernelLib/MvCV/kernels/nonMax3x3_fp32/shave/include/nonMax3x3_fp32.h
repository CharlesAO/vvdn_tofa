#ifndef NONMAX_3X3_FP32_H_
#define NONMAX_3X3_FP32_H_

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// nonMax3x3_fp32
/// @param[in] candPos 			- X coordinate of the candidates
/// @param[in] candCount  		- Number of candidates
/// @param[in] corners1			- Input line (N-1 lines)
/// @param[in] corners2			- Input line (N lines)
/// @param[in] corners3			- Input line (N+1 lines)
/// @param[out] candOutPos		- X coordinate of selected elements
/// @param[out] candOutCount	- Output elements

MVCV_FUNC(void, mvcvNonMax3x3_fp32,u16* candPos, u32 candCount, fp32* corners1, fp32* corners2, fp32* corners3, u16* candOutPos, u32 *candOutCount)
//!@}
#endif //NONMAX_3X3_FP32_H_ 
