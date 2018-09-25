#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/censusMatchingPyr/censusMatchingPyr.h>

/// mvcvCensusMatching        - performs an XOR operation between pixel one pixel in *in1 and 7 pixels from *in2, based on predicted disparities, and counts up how many values of 1 are in the result
/// @param[in] in1            - pointer to input lines of the left image
/// @param[in] in2            - pointer to input lines of the right image
/// @param[in] predicted      - pointer to predicted disparities
/// @param[out] out           - array of disparity cost
/// @param[in] width          - width of the input lines
/// @return    Nothing
#ifdef SIPP_USE_MVCV
void mvcvCensusMatchingPyr_asm(UInt32 *in1, UInt32 *in2, UInt8 *predicted, UInt8 *out, UInt32 width);
#else
void mvcvCensusMatchingPyr    (UInt32 *in1, UInt32 *in2, UInt8 *predicted, UInt8 *out, UInt32 width)
{
    UInt32 resultXOR;
    UInt8 bitCount;
    Int32 offsetRight;

    for (UInt32 positionL = 0; positionL < width; positionL++)
    { // for each pixels

        for (UInt32 indexR = 0; indexR < 7; indexR++)
        { // for all disparities

            // right img index = (left img index - predected offset * 2) +/- 3
            // positionL/2 - because predicted line is downsampled with a factor of 2
            // predicted offset * 2 - because predicted offest is computed at half current resolution
            offsetRight = predicted[(UInt32)(positionL/2)] * 2 + indexR - 3;

            if (offsetRight < 0)
            {
                out[positionL * 7 + indexR] = 25; // any value bigger then 25
            }
            else
            {
                resultXOR = *(in1 + positionL) ^ *(in2 + positionL - offsetRight);

                for(bitCount = 0; resultXOR; resultXOR >>= 1)
                {
                    bitCount += resultXOR & 1;
                }

                out[positionL * 7 + indexR] = bitCount;
            }
        } // Finish all disparities of one pixel

    } // Finish all pixels
}
#endif

void svuCensusMatchingPyr(SippFilter *fptr)
{
    UInt32 *input1;
    UInt32 *input2;
    UInt8 *output;
    CensusMatchingPyrParam *param = (CensusMatchingPyrParam*)fptr->params;

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
        mvcvCensusMatchingPyr_asm(input1, &input2[6], param->predicted, output, parSliceWidth);
    #else
        mvcvCensusMatchingPyr(input1, &input2[6], param->predicted, output, parSliceWidth);
    #endif
    }
}
