///
/// @file      scale05BilinHV_U8ToU8.cpp
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Down scale by half in each direction wil bilinear (sw) kernel.
///            U8 in and out
///

#include <scale05BilinHV_U8ToU8.h>

/// scale05BilinHV_U8ToU8 kernel
/// @param[in] in         	- array of pointers to input lines
/// @param[in] out        	- pointer to output line
/// @param[in] inWidth		- width of input line
void mvispScale05BilinHV_U8ToU8(u8** in, u8* out, u32 inWidth)
{
    int i;	

    for (i = 0; i < (int)inWidth; i++) 
	{
        out[i] = (in[0][i*2] + in[0][i*2+1] + in[1][i*2] + in[1][i*2+1]) >> 2;
    }
	
    return;
}
