#ifndef __CORNERMINEIGENVALPATCHED_H__
#define __CORNERMINEIGENVALPATCHED_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel calculates MinEigenVal - kernel size is 5x5 
/// @param[in]  in_lines       - Input lines
/// @param[in]  posx           - Position on line
/// @param[out] out_pix        - Output pixel    
/// @param[in]  width          - Width of line

MVCV_FUNC(void, mvcvCornerMinEigenVal_patched, u8 **in_lines, int posx, u8 *out_pix, u32 width)
//!@}	

#endif //__CORNERMINEIGENVALPATCHED_H__
