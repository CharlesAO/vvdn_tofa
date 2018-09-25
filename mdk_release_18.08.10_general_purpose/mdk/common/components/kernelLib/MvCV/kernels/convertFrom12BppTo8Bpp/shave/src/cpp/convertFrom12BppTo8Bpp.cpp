#include <convertFrom12BppTo8Bpp.h>


void mvcvConvert12BppTo8Bpp(u8* out, u8* in, u32 width)
{
    u8* inLine = in;
    u8* outLine = out;
    
    for (u32 i = 0; i < width; i++)
    {
        outLine[i] = (inLine[i << 1] >> 4) | (inLine[(i << 1) + 1] << 4);
    }
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvConvert12BppTo8Bpp_opt(u8* out, u8* in, u32 width)
{
    const uchar16* __restrict inputLine = (const uchar16*__restrict)in;
    uchar8* __restrict outputLine = (uchar8*__restrict)out;

    uchar16 next = inputLine[0];

    for (unsigned int i = 0; i < width / 8; ++i)
    {
        uchar16 highs = next >> 4;
        uchar16 lows  = next.s123456789ABCDEF0 << 4;
        ushort8 orred = mvuAs_ushort8(highs | lows);
        outputLine[i] = mvuConvert_uchar8(orred);
        next = inputLine[i + 1];
    }
}
#endif
