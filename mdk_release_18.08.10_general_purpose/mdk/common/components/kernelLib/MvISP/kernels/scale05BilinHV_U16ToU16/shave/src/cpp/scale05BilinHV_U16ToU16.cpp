///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Downscale to half resolution in each directionwith bilinear (sw) kernel; 
///            U16 in / U16 out
///

#include "scale05BilinHV_U16ToU16.h"
#include <stdio.h>

/// scale05BilinHV_U16ToU16 kernel
/// @param[in] in         	- array of pointers to input lines
/// @param[in] out        	- pointer to output line
/// @param[in] inWidth		- width of input line
void mvispScale05BilinHV_U16ToU16(u16** in, u16* out, u32 inWidth)
{
    int i;	

    for (i = 0; i < (int)inWidth; i++) 
	{
        out[i] = ( ((in[0][i*2] + in[0][i*2+1] + 1)>>1) + ((in[1][i*2] + in[1][i*2+1] + 1)>>1 ) + 1) >> 1;
		
    }
	
    return;
}
