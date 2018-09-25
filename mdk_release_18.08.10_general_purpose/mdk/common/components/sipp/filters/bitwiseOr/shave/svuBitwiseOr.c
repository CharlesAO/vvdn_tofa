#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/bitwiseOr/bitwiseOr.h>

/// per-element bit-wise logical conjunction(OR) for two arrays
///@param[in]   src1Addr    - array of pointers to input lines from the first image
///@param[in]   src2Addr    - array of pointers to input lines from the second image
///@param[out]  destAddr    - array of pointers for output line
///@param[in]   width       - width of input line
#ifdef SIPP_USE_MVCV
void mvcvBitwiseOr_asm(UInt8** src1Addr, UInt8** src2Addr, UInt8** destAddr, UInt32 width);
#else
void bitwiseOr        (UInt8** src1Addr, UInt8** src2Addr, UInt8** destAddr, UInt32 width)
{
    UInt32 i;
    UInt8* src1;
    UInt8* src2;
    UInt8* dest;

    src1 = *src1Addr;
    src2 = *src2Addr;
    dest = *destAddr;

    for(i = 0; i < width; i++)
    {
        dest[i] = src1[i] | src2[i];
    }
}
#endif

void svuBitwiseOr(SippFilter *fptr)
{
    UInt8 *input1;
    UInt8 *input2;
    UInt8 *output;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input lines
        input1 = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        input2 = (UInt8 *)getInPtr(fptr, 1, iterNum, 0, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvBitwiseOr_asm(&input1, &input2, &output, fptr->sliceWidth);
    #else
        bitwiseOr(&input1, &input2, &output, fptr->sliceWidth);
    #endif
    }
}