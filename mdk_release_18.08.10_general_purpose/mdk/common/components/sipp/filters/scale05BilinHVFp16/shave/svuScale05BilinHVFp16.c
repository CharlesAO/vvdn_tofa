#include <sipp.h>
#include <sippShaveMacros.h>

/// Scale05BilinHVFp16 kernel
/// @param[in] in           - array of pointers to input lines
/// @param[in] out          - array of pointers to output lines
/// @param[in] width        - width of input line

#ifdef SIPP_USE_MVCV
void mvispScale05BilinHVFp16_asm(half **in, half **out, UInt32 width);
#else
void scale05BilinHVFp16         (half **in, half **out, UInt32 width)
{
    int i;

    for (i = 0; i < (int)width; i++)
    {
        out[0][i] = (in[0][i * 2] + in[0][i * 2 + 1] +
            in[1][i * 2] + in[1][i * 2 + 1]) * .25;
    }
}
#endif

void svuScale05BilinHVFp16(SippFilter *fptr)
{
    half *output;
    half *input[2];

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the  input lines
        input[0] =(half *)getInPtr(fptr, 0, iterNum, 0, 0);
        input[1] =(half *)getInPtr(fptr, 0, iterNum, 1, 0);

        //the output line
        output = (half *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvispScale05BilinHVFp16_asm(input, &output, fptr->sliceWidth);
    #else
        scale05BilinHVFp16(input, &output, fptr->sliceWidth);
    #endif
    }
}
