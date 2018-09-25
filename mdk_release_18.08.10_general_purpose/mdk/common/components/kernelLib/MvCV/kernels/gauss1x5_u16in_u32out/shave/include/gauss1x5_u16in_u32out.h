#ifndef __GAUSS1x5_U16IN_U32OUT_H__
#define __GAUSS1x5_U16IN_U32OUT_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a gauss on the input image using the 1 4 6 4 1 matrix
/// @param[in] in         - Input lines
/// @param[in] out        - Output line
/// @param[in] inWidth    - Width of input line

MVCV_FUNC(void, mvcvGauss1x5_u16in_u32out, u16** in, u32** out, u32 inWidth)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvGauss1x5_u16in_u32out_opt(u16** in, u32** out, u32 inWidth);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__GAUSS1x5_U16IN_U32OUT_H__
