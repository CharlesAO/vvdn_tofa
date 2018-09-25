///
/// @file      pixelPacker10b.cpp
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Takes 16 bits(10bpp) value as input and compress the data by splitting 
///            into  two components, 8bits and 2bits, store components in separate buffers
///

#include <stdlib.h>
#include <pixelPacker10b.h>

/// @brief pixelPacker 16b -> 10b; takes 16bits(10bpp) as input and in order to compress the data split a pixel in two components

///@param[in]	srcAddr		- array of pointers to input line (16b)
///@param[out]	ls8b		- array of pointers to the 8 LSBs line
///@param[out]	ms2b		- array of pointers to the 2 MSBs line
///@param[in] 	width		- width of input line (number of pixels)

void mvispPixelPacker10b(u16* srcAddr, u32* ls8b, u8 *ms2b, u32 width)
{
	int msb_count = 0;
	int lsb_count = 0;
	int mask = 0x0300;
	int mask2 = 0x00ff;
	u16 in;
	u16 in1_msb, in2_msb, in3_msb, in4_msb, in5_msb, in6_msb, in7_msb, in8_msb;
	u8 *out8;
	u8 *ls8b_out = (u8*)ls8b;
	
	for(int i = 0; i < (int)width; i = i + 8)
	{		
		in = srcAddr[i + 0] & mask2;
		out8 = (u8*)&in;
		ls8b_out[lsb_count++] = *out8;
		
		in = srcAddr[i + 1] & mask2;
		out8 = (u8*)&in;
		ls8b_out[lsb_count++] = *out8;
		
		in = srcAddr[i + 2] & mask2;
		out8 = (u8*)&in;
		ls8b_out[lsb_count++] = *out8;
		
		in = srcAddr[i + 3] & mask2;
		out8 = (u8*)&in;
		ls8b_out[lsb_count++] = *out8;
		
		in = srcAddr[i + 4] & mask2;
		out8 = (u8*)&in;
		ls8b_out[lsb_count++] = *out8;
		
		in = srcAddr[i + 5] & mask2;
		out8 = (u8*)&in;
		ls8b_out[lsb_count++] = *out8;
		
		in = srcAddr[i + 6] & mask2;
		out8 = (u8*)&in;
		ls8b_out[lsb_count++] = *out8;
		
		in = srcAddr[i + 7] & mask2;
		out8 = (u8*)&in;
		ls8b_out[lsb_count++] = *out8;
		
		in1_msb = (srcAddr[i + 0] & mask) >> 2;
		in2_msb = (srcAddr[i + 1] & mask) >> 2;
		in3_msb = (srcAddr[i + 2] & mask) >> 4;
		in4_msb = (srcAddr[i + 3] & mask) >> 4;
		in5_msb = (srcAddr[i + 4] & mask) >> 6;
		in6_msb = (srcAddr[i + 5] & mask) >> 6;
		in7_msb = (srcAddr[i + 6] & mask) >> 8;
		in8_msb = (srcAddr[i + 7] & mask) >> 8;
	
		in1_msb = in1_msb | in3_msb | in5_msb | in7_msb;
		in2_msb = in2_msb | in4_msb | in6_msb | in8_msb;
		
		out8 = (u8*)&in1_msb;
		ms2b[msb_count++] = *out8;			
		out8 = (u8*)&in2_msb;
		ms2b[msb_count++] = *out8;
	}
    return;
}
