///
/// @file	   pixelPacker10b.h
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Takes 16 bits(10bpp) value as input and compress the data by splitting 
///            into  two components, 8bits and 2bits, store components in separate buffers
///

#ifndef __PIXELPACKER10b_H__
#define __PIXELPACKER10b_H__
#include <mv_types.h>
#include <mvcv_macro.h>

//!@{ 
/// @brief pixelPacker 16b -> 10b; takes 16bits(10bpp) as input and, in order to compress the data, split a pixel in two components
/// @detail
/// first component(ls8b) contains the last significant 8 bits and the second(ms2b) contains the most significant 2 bits
/// at each iteration this kernel can process 8 pixels (8 values of 16b)
/// the ls8b will have 8bits * 8pixels => 64bits (2 * u32)
/// the ms2b will have 2bits * 8pixels => 16bits (2 * u8)

///input:	000000a9a8A7A6A5A4A3A2A1A0 000000b9b8B7B6B5B4B3B2B1B0 000000c9c8C7C6C5C4C3C2C1C0 ... 
///ls8b:	A7A6A5A4A3A2A1A0B7B6B5B4B3B2B1B0C7C6C5C4C3C2C1C0D7D6D5D4D3D2D1D0 ... 
///ms2b:	a9a8c9c8e9e8g9g8 b9b8d9d8f9f8h9h8 

///@param[in]	srcAddr		- Input line (16b)
///@param[out]	ls8b		- 8 LSBs
///@param[out]	ms2b		- 2 MSBs
///@param[in] 	width		- Width of input line (number of pixels)

MVCV_FUNC(void, mvispPixelPacker10b, u16* srcAddr, u32* ls8b, u8* ms2b, u32 width)

//!@}

#endif // __PIXELPACKER10b_H__
