#ifndef __CORNERMINEIGENVAL_H__
#define __CORNERMINEIGENVAL_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel calculates MinEigenVal - kernel size is 5x5 
/// @param[in]   input_lines   - Input lines
/// @param[out]  output_line   - Output line
/// @param[in]   width         - Width of line

MVCV_FUNC(void, mvcvCornerMinEigenVal, u8 **in_lines, u8 **out_line, u8 * buffer, u32 width)
//!@}

#ifdef MOVICOMPILE_OPTIMIZED
extern "C" 
{
	void mvcvCornerMinEigenVal_opt(u8 **in_lines, u8 **out_line, u8 *buffer, u32 width);
}
#endif

#endif //__CORNERMINEIGENVAL_H__
