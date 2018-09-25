#include "minMaxPos.h"

void mvcvMinMaxPos(u8** in, u32 width, u8* minVal, u8* maxVal, u32* minPos, u32* maxPos, u8* maskAddr)
{
    u8* in_1;
    in_1 = in[0];
    u8 minV = 0xFF;
    u8 maxV = 0x00;
    u32 minLoc = 0;
    u32 maxLoc = 0;
    u32 j;

    for(j = 0; j < width; j++)
    {
        if((maskAddr[j]) != 0)
        {
            if (in_1[j] < minV)
            {
                minV = in_1[j];
                minLoc = j;
            }

            if (in_1[j] > maxV)
            {
                maxV = in_1[j];
                maxLoc = j;
            }
        }
    }
        *minVal = minV;
        *minPos = minLoc;
        *maxVal = maxV;
        *maxPos = maxLoc;
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvMinMaxPos_opt(u8** in, u32 width, u8* minVal, u8* maxVal, u32* minPos, u32* maxPos, u8* maskAddr)
{
    u8 minV = 0xFF;
    u8 maxV = 0x00;
    u32 minLoc = 0;
    u32 maxLoc = 0;
    u32 i, j;


    uchar16* maskPtr = (uchar16*)&maskAddr[0];
    uchar16* inPtr = *(uchar16**)in;
    uchar16 maskVec = *maskPtr++;
    uchar16 inVec = *inPtr++;
    u32 runNo = width & (~8); 
    
    for(i = 0; i < runNo; i+=16)
    {
        uchar16 maskVecNext = *maskPtr++;
        uchar16 inVecNext = *inPtr++;
        for(j = 0; j < 16; j++)
        {

            if((maskVec[j]) != 0)
            {
                if (inVec[j] < minV)
                {
                    minV = inVec[j];
                    minLoc = j + i;
                }
    
                if (inVec[j] > maxV)
                {
                    maxV = inVec[j];
                    maxLoc = j + i;
                }
            }
        }
        maskVec = maskVecNext;
        inVec = inVecNext;
    }

    if(width & 8)
    {
        for(j = 0; j < 8; j++)
        {

            if((maskVec[j]) != 0)
            {
                if (inVec[j] < minV)
                {
                    minV = inVec[j];
                    minLoc = j + (width-8);
                }
    
                if (inVec[j] > maxV)
                {
                    maxV = inVec[j];
                    maxLoc = j + (width-8);
                }
            }
        }
    }   


    *minVal = minV;
    *minPos = minLoc;
    *maxVal = maxV;
    *maxPos = maxLoc;
    return;
}
#endif //MOVICOMPILE_OPTIMIZED