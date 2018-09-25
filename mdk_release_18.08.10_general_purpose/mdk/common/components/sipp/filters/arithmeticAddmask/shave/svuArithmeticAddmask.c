#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/arithmeticAddmask/arithmeticAddmask.h>

/// ArithmeticAdd with mask for two arrays
///@param[in]   src1Addr    - array of pointers to input lines from the first image
///@param[in]   src2Addr    - array of pointers to input lines from the second image
///@param[out]  destAddr    - array of pointers for output line
///@param[in]   width       - width of input line
#ifdef SIPP_USE_MVCV
void mvcvArithmeticAddmask_asm      (UInt8** src1, UInt8** src2, UInt8** dst, UInt8** mask, UInt32 width);
#else
void arithmeticAddmaskImplementation(UInt8** src1, UInt8** src2, UInt8** dst, UInt8** mask, UInt32 width)
{
    int i;
    float add;

    for(i = 0; i < width; i++)
    {
        if (mask[0][i] > 0)
        {
            add = src1[0][i] + src2[0][i];

            if (add >= 255)
                add = 255.0f;
            if (add <= 0)
                add = 0.0f;
        dst[0][i] = (UInt8)(add);
        }
    }
}
#endif

void svuArithmeticAddmask(SippFilter *fptr)
{
    UInt32 i;
    UInt8 *input1[1];
    UInt8 *input2[1];
    UInt8 *input3[1];
    UInt8 *output;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        //the 2 input lines

        input1[0] = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        input2[0] = (UInt8 *)getInPtr(fptr, 1, iterNum, 0, 0);
        input3[0] = (UInt8 *)getInPtr(fptr, 2, iterNum, 0, 0);


        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);
        for(i=0; i<fptr->sliceWidth; i++)
            output[i]=0x00;

    #ifdef SIPP_USE_MVCV
        mvcvArithmeticAddmask_asm(input1, input2, &output, input3, fptr->sliceWidth);
    #else
        arithmeticAddmaskImplementation(input1, input2, &output, input3, fptr->sliceWidth);
    #endif
    }
}