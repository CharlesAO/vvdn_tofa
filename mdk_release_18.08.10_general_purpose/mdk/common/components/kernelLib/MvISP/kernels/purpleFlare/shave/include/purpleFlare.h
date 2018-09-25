///
/// @file      purpleFlare.h
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Performs a restricted sharpening of the Blue channel, to reduce 
///            purple flare seen around bright objects
///

#ifndef __PURPLEFLARE_H__
#define __PURPLEFLARE_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{
/// 
/// The "purple flare" effect shows up most strongly on a coke can - the
/// white letters are surrounded by a purple halo (the red areas are affected).
/// One of the causes is longitudinal chromatic aberration - the blue
/// channel is out of focus with respect to the green channel, and the
/// blue spreads to the surrounding areas.  Could also happen with the
/// red channel but it hasn't been observed to cause noticeable artifacts.
/// The lighting is likely also a factor - would expect to see it with
/// "blue" light (it has been observed strongly under D50).
/// We apply a modified unsharp mask on the blue channel only to reduce it.
///
/// @param[out] outBlue		- pointer to output line, on blue plane
/// @param[in] 	inGreen		- pointer to input line, on green plane
/// @param[in] 	blue		- array of pointers to three input lines, on blue plane
/// @param[in] 	strength	- strength value
/// @param[in] 	width		- width of the lines
/// @return    	Nothing

MVCV_FUNC(void, mvispPurpleFlare, u16 *inGreen, u16 *blue[3], u16 *outBlue, u8 strength, u32 width)
//!@}

#endif //__PURPLEFLARE_H__

