#include "disp2depth.h"
#include <stdlib.h>

void mvcvDisp2depth(u8* in, half* out, half* lut_table, u32 flip_disp2depth, u32 width)

{
    int i;
    if (flip_disp2depth)
    {
            for (i=0; i<(int)width ; i+=4)
            {
                *out-- = lut_table[*in++];
                *out-- = lut_table[*in++];
                *out-- = lut_table[*in++];
                *out-- = lut_table[*in++];
            }
    }
 
    else
    {
            for (i=0; i<(int)width ; i+=4)
            {
                *out++ = lut_table[*in++];
                *out++ = lut_table[*in++];
                *out++ = lut_table[*in++];
                *out++ = lut_table[*in++];
            }
    }
}
