#ifndef NONMAX_M2_H_
#define NONMAX_M2_H_

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel verifies whether each element from the central line is the maximum within a 3x3 range
/// @param[in] width    		- line width
/// @param[in] corners1			- input N-1 line
/// @param[in] corners2			- input N line
/// @param[in] corners3			- input N+1 line
/// @param[out] candOutPos		- X coordinate of output elements
/// @param[out] candOutCount	- number of output elements

MVCV_FUNC(void, mvcvNonMax3x3_u8, u32 width, u8* corners1, u8* corners2, u8* corners3, u16* candOutPos, u32 *candOutCount)

#endif /* NONMAX_M2_H_ */
