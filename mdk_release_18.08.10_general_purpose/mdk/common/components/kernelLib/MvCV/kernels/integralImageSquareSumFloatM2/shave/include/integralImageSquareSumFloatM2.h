#ifndef __INTEGRALIMAGESQUARESUMFLOAT_M2_H__
#define __INTEGRALIMAGESQUARESUMFLOAT_M2_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel makes the sum of all squared pixels before current pixel 
///( columns to the left and rows above ) - output is fp32 format
/// @param[in] in         - Input lines      
/// @param[out] out       - Output lines    
/// @param[in] sum        - Sum of previous pixels - for this parameter we must have an array of fp32 declared as global and having the width of the line
/// @param[in] width      - Width of input line

MVCV_FUNC(void, mvcvIntegralimage_sqsum_f32_M2, float* out, u8* in, u32 runNr, float** prevPointer, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
#include "moviVectorUtils.h"
extern "C" {
	void mvcvIntegralimage_sqsum_f32_M2_opt(float* out, u8* in, u32 runNr, float** prevPointer, u32 width);
}
#endif //MOVICOMPILE_OPTIMIZED

#endif //__INTEGRALIMAGESQUARESUMFLOAT_H__
