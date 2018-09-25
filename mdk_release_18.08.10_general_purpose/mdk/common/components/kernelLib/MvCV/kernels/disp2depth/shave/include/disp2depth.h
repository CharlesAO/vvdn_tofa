#ifndef __disp2depth_H__
#define __disp2depth_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// 

MVCV_FUNC(void, mvcvDisp2depth, u8* in, half* out, half* lut_table, u32 flip_disp2depth, u32 width)
//!@}

#endif //__disp2depth_H__
