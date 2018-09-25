#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/censusTransform5x5/censusTransform5x5.h>

/// censusTransform5x5 kernel
/// @param[in] in         - array of pointers to input lines
/// @param[in] out        - pointer to output line
/// @param[in] inWidth    - width of input line
#ifdef SIPP_USE_MVCV
void mvcvCensusTransform5x5_asm(UInt8 **in, UInt32 *out, UInt32 width);
#else
void mvcvCensusTransform5x5    (UInt8 **in, UInt32 *out, UInt32 width)
{
    for (int i = 0; i < width; i++)
    {
        out[i] = 0;
        for (int line = 0; line < 5; line++)
            for (int col = -2; col <= 2; col++)
            {
                out[i] *= 2;
                if (*(in[line] + i + col) < *(in[2] + i))
                {
                    out[i] += 1;
                } // set 1
            } // finish Census Tranform for one pixel

    } // finish Census Transform for all pixels
}
#endif

void svuCensusTransform5x5(SippFilter *fptr)
{
    UInt8 *input[5];
    UInt32 *output;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 5 input lines
        input[0] = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        input[1] = (UInt8 *)getInPtr(fptr, 0, iterNum, 1, 0);
        input[2] = (UInt8 *)getInPtr(fptr, 0, iterNum, 2, 0);
        input[3] = (UInt8 *)getInPtr(fptr, 0, iterNum, 3, 0);
        input[4] = (UInt8 *)getInPtr(fptr, 0, iterNum, 4, 0);

        //the output line
        output = (UInt32 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvCensusTransform5x5_asm(input, output, fptr->sliceWidth);
    #else
        mvcvCensusTransform5x5(input, output, fptr->sliceWidth);
    #endif
    }
}
