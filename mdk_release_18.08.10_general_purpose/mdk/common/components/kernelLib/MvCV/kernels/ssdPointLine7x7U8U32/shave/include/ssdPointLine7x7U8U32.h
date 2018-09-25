#ifndef __SUMOFSQUAREDDIFF5x5_H__
#define __SUMOFSQUAREDDIFF5x5_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// SSD (Sum of Squared differences) (7x7 in this case)
/// @param[in] in1		- Input lines from the first image, 8-bits unsigned integer
/// @param[in] in2		- Input lines from the second image, 8-bits unsigned integer
/// @param[out] out		- Output line, 32-bits unsigned integer
/// @param[in] width    - Width of input line

MVCV_FUNC(void, mvcvSsdPointLine7x7U8U32, u8** in1, u8** in2, u32** out, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
extern "C"
{
	void mvcvSsdPointLine7x7U8U32_opt(u8** in1, u8** in2, u32** out, u32 width);
}
#endif

#endif //__SUMOFSQUAREDDIFF5x5_H__
