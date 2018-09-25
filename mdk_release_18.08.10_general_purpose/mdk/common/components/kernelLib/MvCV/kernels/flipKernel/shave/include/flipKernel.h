#ifndef __FLIPKERNEL_H__
#define __FLIPKERNEL_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// mvcvFlipKernel - flips elements of a given line (first element becames last)
/// @param[in] in             - pointer to input line
/// @param[out] out           - pointer to output line
/// @param[in] width          - width of the input line
/// @return    Nothing

MVCV_FUNC(void, mvcvFlipKernel, u8 *in, u8 *out, u32 width)
//!@}
    
#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvFlipKernel_opt(u8 *in, u8 *out, u32 width);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__FLIPKERNEL_H__
