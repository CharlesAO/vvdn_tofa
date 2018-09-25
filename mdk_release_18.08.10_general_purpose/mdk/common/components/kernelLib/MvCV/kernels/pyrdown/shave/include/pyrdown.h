#ifndef __PYRDOWN_H__
#define __PYRDOWN_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel downsample even lines and even columns (monolithic implementation of GaussVx2 + GaussHx2 kernels) 
/// @param[in] inLine     - Input lines
/// @param[out] outLine   - Output lines
/// @param[in] width      - Width of input line; it must be bigger then 48 and multiple of 16

MVCV_FUNC(void, mvcvPyrdown, u8 **inLine, u8 **outLine, int width)

//!@}

#endif //__PYRDOWN_H__
