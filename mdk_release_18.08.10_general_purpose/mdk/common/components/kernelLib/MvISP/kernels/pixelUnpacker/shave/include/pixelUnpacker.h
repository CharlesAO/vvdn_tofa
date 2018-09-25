///
/// @file      pixelUnpacker.h
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Reverse of pixelPacker kernel.
///            Takes as input 2 lines, containing 8 bits and most significant 2 bits 
///            and packs into a 10bpp value 
///

#ifndef __PIXELUNPACKER_H__
#define __PIXELUNPACKER_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{ 
/// @brief pixelUnpacker 10b -> 16b/15b/14b/13b/12b/11b/10b depending on the shift value
/// @detail
/// takes as input 2 lines, first contains the last significant 8 bits (src32) and the second (src8) contains the most significant 2 bits of a 10bpp  
/// input of this kernel need to be the output of the pixelPacker10b kernel
/// the output will be a line of 16 bits values representing the 10bpp value shifted to the left with the 'shift' amount 

///ms2b_line:							a9a8c9c8e9e8g9g8 b9b8d9d8f9f8h9h8 
///ls8b_line:							A7A6A5A4A3A2A1A0B7B6B5B4B3B2B1B0C7C6C5C4C3C2C1C0D7D6D5D4D3D2D1D0 ... 	
///outputUnpacker(shift0):	000000a9a8A7A6A5A4A3A2A1A0 000000b9b8B7B6B5B4B3B2B1B0 000000c9c8C7C6C5C4C3C2C1C0 ... 
///outputUnpacker(shift4):	00a9a8A7A6A5A4A3A2A1A00000 00b9b8B7B6B5B4B3B2B1B00000 00c9c8C7C6C5C4C3C2C1C00000 ...

/// @param[in]	ls8b		- 8 LSBs
/// @param[in]	ms2b		- 2 MSBs
/// @param[out]	output		- Output line
/// @param[in] 	width		- Width of output line (number of processed pixels)
/// @param[in] 	shift		- Number of bits for shifting the result to left

MVCV_FUNC(void, mvispPixelUnpacker, u32 *ms8b, u8 *ls2b, u16 *output, u32 width, u8 shift)
//!@}


#endif // __PIXELUNPACKER_H__
