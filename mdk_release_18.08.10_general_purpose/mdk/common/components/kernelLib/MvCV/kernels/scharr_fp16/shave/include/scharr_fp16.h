#ifndef __SCHARR_H__
#define __SCHARR_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel performs scharr edge detection operator
/// @param[in] in     - Input lines
/// @param[out] out   - Output line
/// @param[in] width  - Width of input line

MVCV_FUNC(void, mvcvScharr_fp16, half** in, half** out, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvScharr_fp16_opt(half** in, half** out, u32 width);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__SCHARR_H__
