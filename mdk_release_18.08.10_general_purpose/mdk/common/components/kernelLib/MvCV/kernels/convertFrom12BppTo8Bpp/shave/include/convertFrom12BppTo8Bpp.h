#ifndef __CONVERTFROM12BPPTO8BPP_H__
#define __CONVERTFROM12BPPTO8BPP_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{ 
///This kernel converts from 12 bpp to 8 bpp
///@param[in]   in          - Input line
///@param[out]  out         - Output line
///@param[in]   width       - Width of the input line

MVCV_FUNC(void, mvcvConvert12BppTo8Bpp, u8* out, u8* in, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvConvert12BppTo8Bpp_opt(u8* out, u8* in, u32 width);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__CONVERTFROM12BPPTO8BPP_H__
