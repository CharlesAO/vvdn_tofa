#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/bitwiseXorMask/bitwiseXorMask.h>

/// per-element bit-wise logical conjunction(XOR) for two arrays if mask == 1
///@param[in]   src1Addr    - array of pointers to input lines from the first image
///@param[in]   src2Addr    - array of pointers to input lines from the second image
///@param[in]   maskAddr    - array of pointers to input line that contain the mask
///@param[out]  destAddr    - array of pointers for output line
///@param[in]   width       - width of input line
#ifdef SIPP_USE_MVCV
void mvcvBitwiseXorMask_asm(UInt8** src1Addr, UInt8** src2Addr, UInt8** destAddr, UInt8** maskAddr, UInt32 width);
#else
void bitwiseXorMask        (UInt8** src1Addr, UInt8** src2Addr, UInt8** destAddr, UInt8** maskAddr, UInt32 width)
{
    UInt32 i;
    UInt8* src1;
    UInt8* src2;
    UInt8* mask;
    UInt8* dest;

    src1 = *src1Addr;
    src2 = *src2Addr;
    mask = *maskAddr;
    dest = *destAddr;

    for (i = 0; i < width; i++)
    {
        if(mask[i])
        {
            dest[i] =  src1[i] ^ src2[i];
        }
    }
}
#endif

void svuBitwiseXorMask(SippFilter *fptr)
{
    UInt32 i;
    UInt8 *input1;
    UInt8 *input2;
    UInt8 *mask;
    UInt8 *output;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input lines & mask
        input1 = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        input2 = (UInt8 *)getInPtr(fptr, 1, iterNum, 0, 0);
        mask   = (UInt8 *)getInPtr(fptr, 2, iterNum, 0, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);
        for(i=0; i<fptr->sliceWidth; i++)
            output[i]=0x00;

    #ifdef SIPP_USE_MVCV
        mvcvBitwiseXorMask_asm(&input1, &input2, &output, &mask, fptr->sliceWidth);
    #else
        bitwiseXorMask(&input1, &input2, &output, &mask, fptr->sliceWidth);
    #endif
    }
}
