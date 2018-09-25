#include "convert_fp16_u8.h"
#include <stdlib.h>

void mvcvConvert_fp16_u8(half* in, u8* out, u32 width)

{
    unsigned int j;
    for (j = 0; j < width; j++)
    {
        if (in[j] < (half)0)
            out[j] = 0;
        else if(in[j] > (half)255)
            out[j] = 255;
        else
            out[j] = (u8) in[j];
    }
    return;
}
