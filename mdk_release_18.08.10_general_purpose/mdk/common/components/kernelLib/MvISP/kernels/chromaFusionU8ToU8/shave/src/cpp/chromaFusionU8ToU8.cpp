#include "chromaFusionU8ToU8.h"

void mvispChromaFusionU8ToU8(u8* inCRshort, u8* inCGshort, u8* inCBshort, u8* inCRlong, u8* inCGlong, u8* inCBlong, half* inAlpha, u8* outR, u8* outG, u8* outB, u32 width)
{
    u32 i;

    for(i = 0; i < width; i++)
    {

        half OutHalfR;
        half OutHalfG;
        half OutHalfB;
        half inHalfCRlong;
        half inHalfCGlong;
        half inHalfCBlong;
        half inHalfCRshort;
        half inHalfCGshort;
        half inHalfCBshort;

        inHalfCRlong  = (half)(inCRlong[i]);
        inHalfCGlong  = (half)(inCGlong[i]);
        inHalfCBlong  = (half)(inCBlong[i]);
        inHalfCRshort = (half)(inCRshort[i]);
        inHalfCGshort = (half)(inCGshort[i]);
        inHalfCBshort = (half)(inCBshort[i]);

        OutHalfR = inHalfCRlong * inAlpha[i] + inHalfCRshort * (1 - inAlpha[i]);
        OutHalfG = inHalfCGlong * inAlpha[i] + inHalfCGshort * (1 - inAlpha[i]);
        OutHalfB = inHalfCBlong * inAlpha[i] + inHalfCBshort * (1 - inAlpha[i]);

        outR[i] = (u8)(OutHalfR);
        outG[i] = (u8)(OutHalfG);
        outB[i] = (u8)(OutHalfB);
    }
}
