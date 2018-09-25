#include <stdlib.h>
#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/absdiff/absdiff.h>

/// AbsoluteDiff kernel  computes the absolute difference of two images given as parameters(used to estimate motion)
/// @param[in] in1             - array of pointers to input lines of the first image
/// @param[in] in2             - array of pointers to input lines of the second image
/// @param[out] out            - array of pointers to output line
/// @param[in] width           - width of the input lines
#ifdef SIPP_USE_MVCV
void mvcvAbsoluteDiff_asm (UInt8** in1, UInt8** in2, UInt8** out, UInt32 width);
#else
void AbsoluteDiff (UInt8** in1, UInt8** in2, UInt8** out, UInt32 width)
{
    UInt8* in_1;
    UInt8* in_2;
    in_1= *in1;
    in_2= *in2;

    //check if the two input images have the same size
    UInt32 j;
    if (sizeof(in1) != sizeof(in2))
    {
        exit(1);
    }
    else
    {
        for (j = 0; j < width; j++)
        {
            if (in_1[j] > in_2[j])
            {
                out[0][j] = in_1[j] - in_2[j];
            }
            else
            {
                out[0][j] = in_2[j] - in_1[j];
            }
        }
    }
}
#endif

void svuAbsdiff(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[2];

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input lines
        iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        iline[1]=(UInt8 *)getInPtr(fptr, 1, iterNum, 0, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvAbsoluteDiff_asm(&iline[0], &iline[1], &output, fptr->sliceWidth);
    #else
        AbsoluteDiff(&iline[0], &iline[1], &output, fptr->sliceWidth);
    #endif
    }
}
