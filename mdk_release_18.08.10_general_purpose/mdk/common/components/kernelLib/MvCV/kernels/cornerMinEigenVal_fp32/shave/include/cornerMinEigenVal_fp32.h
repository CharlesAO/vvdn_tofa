#ifndef __CORNERMINEIGENVAL_FP32_H__
#define __CORNERMINEIGENVAL_FP32_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel calculates MinEigenVal - kernel size is 5x5
/// @param[in]   input_lines   - Input lines
/// @param[out]  output_line   - Output line
/// @param[in]   width         - Width of line

MVCV_FUNC(void, mvcvCornerMinEigenVal_fp32, u8 **in_lines, fp32 **out_line, fp32 * buffer, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
extern "C"
{
void mvcvCornerMinEigenVal_fp32_opt(u8 **in_lines, fp32 **out_line, fp32 * buffer, u32 width);
}
#endif

#endif //__CORNERMINEIGENVAL_FP32_H__
