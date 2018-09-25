///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Downscale to half resolution in each direction with bilinear (sw) kernel; 
///            Fp16 in / U8 out
///

#include "scale05BilinHV_Fp16U8.h"

/// mvispScale05BilinHV_fp16U8 kernel
/// @param[in] in         	- array of pointers to input lines
/// @param[in] out        	- array of pointers to output lines
/// @param[in] inWidth		- width of input line
void mvispScale05BilinHV_Fp16U8(half** in, u8** out, u32 inWidth)
{
    int i;
    half tmp;

    //Go on the whole line
    for (i = 0; i < (int)inWidth; i++){
		tmp = ((in[0][i*2] + in[0][i*2+1] + 
			    in[1][i*2] + in[1][i*2+1]) * (255.0f/4.0f));
		
		//half tmp2 = (half)(tmp); 
        if (tmp >= (half)255)
            tmp = (half)255;
        if (tmp <= (half)0)
            tmp = (half)0;
        out[0][i] = (u8)(tmp);
    }
    return;
}
