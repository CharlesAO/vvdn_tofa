#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/censusMatching64/censusMatching64.h>

/// mvcvCensusMatching        - performs an XOR operation between pixel one pixel in *in1 and 16 pixels from *in2 and counts up how many values of 1 are in the result
/// @param[in] in1            - Input lines of the left image
/// @param[in] in2            - Input lines of the right image
/// @param[in] flag           - enable right crossing (by default left crossing)
/// @param[out] out           - array of disparity cost
/// @param[in] width          - Width of the input lines
#ifdef SIPP_USE_MVCV
void mvcvCensusMatching64_asm(UInt32 *in1, UInt32 *in2, UInt8 *out, UInt32 width, UInt32 flag);
#else
void mvcvCensusMatching64(UInt32 *in1, UInt32 *in2, UInt8 *out, UInt32 width, UInt32 flag)
{
    UInt32 resultXOR, disparities = 64;
    UInt8 bitCount;

    for (UInt32 positionL = 0; positionL < width; positionL++)
    { // for each pixels

        for (UInt32 indexR = 0; indexR < disparities; indexR++)
        { // for all disparities
            if (flag == 0)
            {
                resultXOR = *(in1 + positionL) ^ *(in2 + positionL - indexR);
            }
            else
            {
                resultXOR = *(in1 + positionL) ^ *(in2 + positionL + indexR);
            }

            for(bitCount = 0; resultXOR; resultXOR >>= 1)
            {
                bitCount += resultXOR & 1;
            }

            out[positionL * disparities + indexR] = bitCount;
        } // Finish all disparities of one pixel

    } // Finish all pixels
}
#endif

void svuCensusMatching64(SippFilter *fptr)
{
    UInt32 *input1;
    UInt32 *input2;
    UInt8 *output;
    CensusMatching64Param *param = (CensusMatching64Param*)fptr->params;
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
        mvcvCensusMatching64_asm(input1, &input2[63], output, parSliceWidth, param->flag);
    #else
        mvcvCensusMatching64(input1, &input2[63], output, parSliceWidth, param->flag);
    #endif
    }
}
