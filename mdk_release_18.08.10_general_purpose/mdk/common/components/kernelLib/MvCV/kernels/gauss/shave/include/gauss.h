#ifndef __GAUSS_H__
#define __GAUSS_H__

#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel apply a gaussian blur 
///( see http://softwarebydefault.files.wordpress.com/2013/06/guassianblur5x5kernel.jpg )
/// @param[in] inLine     - Input lines
/// @param[out] out       - Output line
/// @param[in] width      - Width of input line

MVCV_FUNC(void, mvcvGauss, u8 **inLine,u8 **out, u32 width)
//!@}

#endif //__GAUSS_H__