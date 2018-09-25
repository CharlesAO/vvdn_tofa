///
/// @file  scale05BilinHVFp16.cpp
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Down scale by half in each direction wil bilinear (sw) kernel.
///            Fp16 in and out
///

#include "scale05BilinHVFp16.h"

/// Scale05BilinHVFp16 kernel - downscale filter with 0.5 factor and 2 input lines
/// @param[in] in         	- array of pointers to input lines
/// @param[in] out        	- array of pointers to output lines
/// @param[in] width		- width of input line
void mvispScale05BilinHVFp16(half **in, half **out, u32 width)
{
	int i;
	
	for (i = 0; i < (int)width; i++)
	{
		out[0][i] = (in[0][i * 2] + in[0][i * 2 + 1] + 
			in[1][i * 2] + in[1][i * 2 + 1]) * .25;
    }
}
