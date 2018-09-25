#include "flipKernel.h"

void mvcvFlipKernel(u8 *in, u8 *out, u32 width)
{
    for (u32 i = 0 ; i < width; i++)
    {
        out[i] = in[(width-1)-i];
    }
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvFlipKernel_opt(u8 *__restrict__ in, u8 * __restrict__ out, u32 width)
{

    uchar8 *__restrict__ inV = (uchar8*) in;
    uchar8 * __restrict__ outV = (uchar8*) (out+width - 8);

    ushort8 inval;
    ushort8 outval;
    width  = width/8;

#pragma clang loop unroll_count(8)
    for (u32 i = 0 ; i < width; i++)
    {

       inval = mvuConvert_ushort8(*inV);
       outval = (ushort8)inval.s76543210;
       *(outV) = mvuConvert_uchar8(outval);
       inV++;
       outV--;
    }
}
#endif
