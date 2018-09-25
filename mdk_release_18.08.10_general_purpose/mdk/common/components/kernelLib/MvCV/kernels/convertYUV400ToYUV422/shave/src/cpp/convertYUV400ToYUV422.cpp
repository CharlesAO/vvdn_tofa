#include "convertYUV400ToYUV422.h"

void mvcvConvertYUV400ToYUV422(u8* in, u16* out, u32 width)
{
    u8* inLine = in;
    u16* outLine = out;
    
    for (u32 i = 0; i < width; i++)
    {
        outLine[i] = inLine[i] | 0x8000;
    }
}
