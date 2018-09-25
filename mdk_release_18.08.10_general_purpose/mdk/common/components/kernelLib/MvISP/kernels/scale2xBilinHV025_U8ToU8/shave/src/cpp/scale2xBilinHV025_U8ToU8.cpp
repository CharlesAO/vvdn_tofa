///
/// @file      scale2xBilinHV025_U8ToU8.cpp
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.tit
///
/// @brief     Up scale by 2 in each direction with bilinear (sw) kernel.
///            U8 in and out
///

#include <scale2xBilinHV025_U8ToU8.h>

/// scale2xBilinHV025_U8ToU8 kernel
/// @param[in] in         	- array of pointers to input lines
/// @param[in] out        	- pointer to output line
/// @param[in] inWidth		- width of the output line
void mvispScale2xBilinHV025_U8ToU8(u8** in, u8** out, u32 inWidth)
{
	u8 *output = out[0];
	int j;	
	
	float weight[2][4] = {
			{1.0/16, 3.0/16, 3.0/16, 9.0/16},
			{3.0/16, 1.0/16, 9.0/16, 3.0/16},		
	};
	
	for (j = 0; j < (int)inWidth; j+=2) 
	{
		output[j] = (u8)((float)in[0][j/2 - 1]*weight[0][0] + (float)in[0][j/2]*weight[0][1] + (float)in[1][j/2-1]*weight[0][2] + (float)in[1][j/2]*weight[0][3]);
		output[j+1] = (u8)((float)in[0][j/2]*weight[1][0] + (float)in[0][j/2+1]*weight[1][1] + (float)in[1][j/2]*weight[1][2] + (float)in[1][j/2+1]*weight[1][3]);
    }
	
    return;
}
