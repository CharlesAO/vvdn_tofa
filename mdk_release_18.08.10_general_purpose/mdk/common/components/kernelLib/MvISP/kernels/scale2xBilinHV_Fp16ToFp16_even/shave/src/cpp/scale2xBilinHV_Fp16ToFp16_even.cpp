///
/// @file      mvispScale2xBilinHV_Fp16ToFp16_even.cpp
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.tit
///
/// @brief     Up scale by 2 in each direction with bilinear (sw) kernel.
///            Fp16 in and out
///

#include <scale2xBilinHV_Fp16ToFp16_even.h>

void mvispScale2xBilinHV_Fp16ToFp16_even(half** in, half** out, u32 outWidth)
{
    u32 x;

    for(x=0; x<outWidth; x+=2)
    {
        out[0][x+0] = (in[0][x/2]);
        out[0][x+1] = (in[0][x/2] + in[0][x/2+1]) * .50f;
    }
}
