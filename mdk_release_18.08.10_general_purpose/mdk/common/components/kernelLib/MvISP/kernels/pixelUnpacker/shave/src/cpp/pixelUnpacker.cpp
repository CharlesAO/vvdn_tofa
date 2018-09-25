///
/// @file      pixelUnpacker.cpp
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Reverse of pixelPacker kernel.
///            Takes as input 2 lines, containing 8 bits and most significant 2 bits 
///            and packs into a 10bpp value 
///

#include <stdlib.h>
#include "pixelUnpacker.h"
#include <stdio.h>

/// pixelUnpacker 10b -> 16b/15b/14b/13b/12b/11b/10b depending on the shift value
/// @param[in]	ls8b		- array of pointers to the 8 LSBs
/// @param[in]	ms2b		- array of pointers to the 2 MSBs
/// @param[out]	output		- array of pointers for output line
/// @param[in] 	width		- width of output line (number of processed pixels)
/// @param[in] 	shift		- number of bits for shifting the result to left

void mvispPixelUnpacker(u32 *ls8b, u8 *ms2b, u16 *output, u32 width, u8 shift)
{  
	u8 *ls8b_in = (u8*)ls8b;
	u8 *out = (u8*)output;
	u16 *out_shift = (u16*)output;
	int ls8b_count = 0;
	int ms2b_count = 0;
	u8 ms2b_1, ms2b_2, ms2b_3, ms2b_4, ms2b_5, ms2b_6, ms2b_7, ms2b_8;
	u8 mask1 = 0xC0;
	u8 mask2 = 0x30;
	u8 mask3 = 0x0C;
	u8 mask4 = 0x03;
	
	for(int i = 0; i < (int)width * 2; i = i + 16)
	{
		out[i + 0] 	= ls8b_in[ls8b_count++];
		out[i + 2] 	= ls8b_in[ls8b_count++];
		out[i + 4] 	= ls8b_in[ls8b_count++];
		out[i + 6] 	= ls8b_in[ls8b_count++];
		out[i + 8] 	= ls8b_in[ls8b_count++];
		out[i + 10] = ls8b_in[ls8b_count++];
		out[i + 12] = ls8b_in[ls8b_count++];
		out[i + 14] = ls8b_in[ls8b_count++];
		
		ms2b_1 = (ms2b[ms2b_count] & mask1) >> 6;
		ms2b_2 = (ms2b[ms2b_count] & mask2)	>> 4;
		ms2b_3 = (ms2b[ms2b_count] & mask3) >> 2;
		ms2b_4 = ms2b[ms2b_count++] & mask4;
		
		ms2b_5 = (ms2b[ms2b_count] & mask1)	>> 6;
		ms2b_6 = (ms2b[ms2b_count] & mask2)	>> 4;
		ms2b_7 = (ms2b[ms2b_count] & mask3) >> 2;
		ms2b_8 = ms2b[ms2b_count++] & mask4;
		
		out[i + 1] 	= ms2b_1;
		out[i + 5] 	= ms2b_2;
		out[i + 9] 	= ms2b_3;
		out[i + 13] = ms2b_4;		
		out[i + 3] 	= ms2b_5;
		out[i + 7] 	= ms2b_6;
		out[i + 11] = ms2b_7;
		out[i + 15] = ms2b_8;		
		
	}
		
	for(int i = 0; i < (int)width; i++)
		out_shift[i] = out_shift[i] << shift;
}
