#ifndef __THRESHOLDBINARYU8_H__
#define __THRESHOLDBINARYU8_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel set output to 0 if threshold value is less then input value 
/// and to 0xFF if threshold value is greater then input value
/// @param[in] in			- Input lines    
/// @param[out] out			- Output lines  
/// @param[in] threshold	- Threshold value   
/// @param[in] width		- Width of input line

MVCV_FUNC(void, mvcvThresholdBinaryU8, u8** in, u8** out, u8 threshold,  u32 width)

//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvThresholdBinaryU8_opt(u8** in, u8** out, u8 threshold,  u32 width);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__THRESHOLDBINARYU8_H__
