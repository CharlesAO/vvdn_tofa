#ifndef __MINMAXPOS_H__
#define __MINMAXPOS_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel computes the minimum and the maximum value of a given input line and their position
/// @param[in] in        - input line
/// @param[in] width     - line`s width(length)
/// @param[in] height    - height of image (defaulted to one line)
/// @param[in] minVal    - stores the minimum value on the line
/// @param[in] maxVal    - stores the maximum value on the line
/// @param[out] minPos   - stores the position occupied by the MIN value within line
/// @param[out] maxPos   - stores the position occupied by the MAX value within line
/// @param[in] maskAddr  - mask filled with 1s and 0s which determines the image area to compute minimum and maximum
/// @return              - Nothing

MVCV_FUNC(void, mvcvMinMaxPos, u8** in, u32 width, u8* minVal, u8* maxVal, u32* minPos, u32* maxPos, u8* maskAddr)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
extern "C"
{
	void mvcvMinMaxPos_opt(u8** in, u32 width, u8* minVal, u8* maxVal, u32* minPos, u32* maxPos, u8* maskAddr);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__MINMAXPOS_H__