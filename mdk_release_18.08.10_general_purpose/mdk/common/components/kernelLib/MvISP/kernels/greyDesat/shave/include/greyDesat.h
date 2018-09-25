///
/// @file	greyDesat.h
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Desaturates areas which are already very close to grey.  
///            It removes residual low-frequency color noise, which is most 
///            noticeable in grey areas.  This filter calculates a metric which 
///            indicates how close a pixel is to grey (the smaller the value 
///            the closer the pixel is to grey.  A blend factor is calculated which 
///            is used to interpolate between the pixel's chrominance values, and 
///            the chrominance values of a grey pixel
///
#ifndef __GREYDESAT_H__
#define __GREYDESAT_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{	
 /// greyDesat kernel 
/// @param[in]  in   	- pointer to the first lines from input  	 
/// @param[Out] out    	- pointer to the first lines from output 	 
/// @param[in]  offset  - offset value
/// @param[in]  slope   - slope value
/// @param[in]  grey    - grey values
/// @param[in]  width   - line width in pixels
/// @return     Nothing

MVCV_FUNC(void, mvispGreyDesat, u8* in[3], u8* out[3], int offset, int slope, int grey[3], u32 width)
//!@}

#endif //__GREYDESAT_H__
