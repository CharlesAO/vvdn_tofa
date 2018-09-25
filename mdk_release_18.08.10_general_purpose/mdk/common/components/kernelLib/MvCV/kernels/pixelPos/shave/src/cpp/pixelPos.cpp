#include "pixelPos.h"

void mvcvPixelPos(u8** srcAddr, u8* maskAddr, u32 width, u8 pixelValue, u32* pixelPosition, u8* status)
{
    int j;
    u8* src;
    src = srcAddr[0];
    u32 location = 0;
    *status = 0;

    for(j = width-1; j >= 0; j--)
    {
        if (maskAddr[j] != 0)
        {
            if(src[j] == pixelValue)
            {
                location = j;
                *status = 0x11;
            }
        }
    }
    *pixelPosition = location ;
    return;
}
