///
/// @file scale05Lanc6HV.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Downscale with factor 0.5 (half) in each direction with lanczos kernel
///            Kernel size is 6. 
///

#ifndef __SCALE05LANC6HV_H__
#define __SCALE05LANC6HV_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// This kernel applies a lanczos downscale, with factor 0.5, and 6 taps 
/// @param[in] in     - array of pointers to 6 input lines
/// @param[out]out    - pointers to output lines
/// @param[in] width  - width of output line
/// @return    Nothing
MVCV_FUNC(void, mvispScale05Lanc6HV, u8** in, u8* out, unsigned int width)
//!@}
	
#endif //__SCALE05LANC6HV_H__