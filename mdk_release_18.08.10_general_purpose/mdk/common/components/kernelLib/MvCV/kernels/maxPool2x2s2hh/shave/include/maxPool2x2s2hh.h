#ifndef __MAX_POOL_2x2_s2_hh_H__
#define __MAX_POOL_2x2_s2_hh_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel compute maximum value on a 2x2 pool using an output stride of 2
///@param[in]   src     - Input lines
///@param[out]  dest    - Output line
///@param[in]   width   - Width of output line

MVCV_FUNC(void, mvcvMaxPool2x2s2hh, half** src, half** dst, u32 outputWidth)
//!@}

#endif //__MAX_POOL_2x2_s2_hh_H__
