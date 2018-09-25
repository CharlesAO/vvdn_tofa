#include "convert_u8_fp16.h"
#include <stdlib.h>

void mvcvConvert_u8_fp16(u8* in, half* out, u32 width)

{
    unsigned int j;
    for (j = 0; j < width; j++)
        out[j] = (half) in[j];
    
    return;
}
