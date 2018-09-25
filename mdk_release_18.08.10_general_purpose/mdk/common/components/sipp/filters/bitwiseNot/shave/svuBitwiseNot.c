#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/bitwiseNot/bitwiseNot.h>

/// per-element bit-wise NOT
///@param[in]   srcAddr     - array of pointers to input line
///@param[out]  destAddr    - array of pointers for output line
///@param[in]   width       - width of input line
#ifdef SIPP_USE_MVCV
void mvcvBitwiseNot_asm(UInt8** srcAddr, UInt8** destAddr, UInt32 width);
#else
void bitwiseNot        (UInt8** srcAddr, UInt8** destAddr, UInt32 width)
{
    UInt32 i;
    UInt8* src;
    UInt8* dest;

    src = *srcAddr;
    dest = *destAddr;

    for(i = 0; i < width; i++)
    {
        dest[i] = ~src[i];
    }
}
#endif

void svuBitwiseNot(SippFilter *fptr)
{
    UInt8 *input;
    UInt8 *output;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input line
        input = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvBitwiseNot_asm(&input, &output, fptr->sliceWidth);
    #else
        bitwiseNot(&input, &output, fptr->sliceWidth);
    #endif
    }
}