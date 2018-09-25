#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/arithmeticSubFp16ToFp16/arithmeticSubFp16ToFp16.h>

/// ArithmeticSub for two fp16 arrays
///@param[in]   src1Addr    - array of pointers to input lines from the first image
///@param[in]   src2Addr    - array of pointers to input lines from the second image
///@param[out]  destAddr    - array of pointers for output line
///@param[in]   width       - width of input line
#ifdef SIPP_USE_MVCV
void mvcvArithmeticSubFp16ToFp16_asm(half** src1Addr, half** src2Addr, half** destAddr, UInt32 width);
#else
void arithmeticSubFp16ToFp16        (half** src1Addr, half** src2Addr, half** destAddr, UInt32 width)
{
    UInt32 i;
    half* src1;
    half* src2;
    half* dest;

    src1 = *src1Addr;
    src2 = *src2Addr;
    dest = *destAddr;

    for(i = 0; i < width; i++)
    {
        dest[i] = src1[i] - src2[i];
    }
}
#endif

void svuArithmeticSubFp16ToFp16(SippFilter *fptr)
{
    half *input1[1];
    half *input2[1];
    half *output;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 2 input lines
        input1[0] = (half *)getInPtr(fptr, 0, iterNum, 0, 0);
        input2[0] = (half *)getInPtr(fptr, 1, iterNum, 0, 0);

        //the output line
        output = (half *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvArithmeticSubFp16ToFp16_asm(input1, input2, &output, fptr->sliceWidth);
    #else
        arithmeticSubFp16ToFp16(input1, input2, &output, fptr->sliceWidth);
    #endif
    }
}
