#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/censusMatching16/censusMatching16.h>

/// mvcvCensusMatching        - performs an XOR operation between pixel one pixel in *in1 and 16 pixels from *in2 and counts up how many values of 1 are in the result
/// @param[in] in1            - Input lines of the left image
/// @param[in] in2            - Input lines of the right image
/// @param[out] out           - array of disparity cost
/// @param[in] width          - Width of the input lines
/// @return    Nothing
#ifdef SIPP_USE_MVCV
void mvcvCensusMatching16_asm(UInt32 *in1, UInt32 *in2, UInt8 *out, UInt32 width);
#else
void mvcvCensusMatching16    (UInt32 *in1, UInt32 *in2, UInt8 *out, UInt32 width)
{
    UInt32 resultXOR, disparities = 16;
    UInt8 bitCount;

    for (UInt32 positionL = 0; positionL < width; positionL++)
    { // for each pixels

        for (UInt32 indexR = 0; indexR < disparities; indexR++)
        { // for all disparities

            resultXOR = *(in1 + positionL) ^ *(in2 + positionL - indexR);

            for(bitCount = 0; resultXOR; resultXOR >>= 1)
            {
                bitCount += resultXOR & 1;
            }

            out[positionL * disparities + indexR] = bitCount;
        } // Finish all disparities of one pixel

    } // Finish all pixels
}
#endif

void svuCensusMatching16(SippFilter *fptr)
{
    UInt32 *input1;
    UInt32 *input2;
    UInt8 *output;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;
    UInt32 parSliceWidth = sippFilterGetParentSliceWidth (fptr, 0);

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input lines
        input1 = (UInt32 *)getInPtr(fptr, 0, iterNum, 0, 0);
        input2 = (UInt32 *)getInPtr(fptr, 1, iterNum, 0, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvCensusMatching16_asm(input1, &input2[15], output, parSliceWidth);
    #else
        mvcvCensusMatching16(input1, &input2[15], output, parSliceWidth);
    #endif
    }
}
