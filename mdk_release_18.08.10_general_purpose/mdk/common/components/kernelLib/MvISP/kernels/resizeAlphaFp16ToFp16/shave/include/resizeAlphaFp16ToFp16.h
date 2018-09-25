#ifndef __ResizeAlphaFp16ToFp16_H__
#define __ResizeAlphaFp16ToFp16_H__
#include <mv_types.h>
#include <mvcv_macro.h>


//!@{
/// This kernel performs a biliniar rescaling
/// @param[in] in          - Input lines, 16-bits floating point
/// @param[out] out        - Output line, 16-bits floating point
/// @param[in] inWidth    - Width of input line
	
MVCV_FUNC(void, mvispResizeAlphaFp16ToFp16, half** alpha, half* output, u32 width)
//!@}

#endif //__ResizeAlphaFp16ToFp16_H__