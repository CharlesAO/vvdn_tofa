#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/localMaxMin3x3_fp16/localMaxMin3x3_fp16.h>

/// This kernel will find the points which are minimums or maximums in their 3x3 zone.
/// The points from the middle line are compared to their neighbors
/// @param[in]  inBuffer        - Address of the fp16 image buffer. This buffer should have a size of 3 * width * sizeof(fp16) bytes (3 lines)
/// @param[in]  width           - Line width in pixels as UInt32
/// @param[out] minLocationList - UInt32 buffer, where the function will save the X coordinates of the minimums found
/// @param[out] maxLocationList - UInt32 buffer, where the function will save the X coordinates of the maximums found
/// @param[out] minCount        - The function will save the number of minimums found to this address
/// @param[out] maxCount        - The function will save the number of maximums found to this address

#ifdef SIPP_USE_MVCV
void mvcvLocalMaxMin3x3_fp16_asm(half** inBuffer, UInt32 width,
                             UInt32 minLocationList[], UInt32 maxLocationList[],
                             UInt32* minCount, UInt32* maxCount);
#else
void mvcvLocalMaxMin3x3_fp16(half** inBuffer, UInt32 width,
                             UInt32 minLocationList[], UInt32 maxLocationList[],
                             UInt32* minCount, UInt32* maxCount)
{
    UInt32 candidateX;
    half candidate;
    half* lineP[3];
    lineP[0] = inBuffer[0];
    lineP[1] = inBuffer[1];
    lineP[2] = inBuffer[2];
    *minCount = 0;
    *maxCount = 0;
    for (candidateX = 1; candidateX < width; candidateX++)
    {

        /// The value of the pixel which is checked.
        candidate = lineP[1][candidateX];
        if ((candidate < lineP[0][candidateX -1]) &&
                        (candidate < lineP[0][candidateX]) &&
                        (candidate < lineP[0][candidateX +1]) &&
                        (candidate < lineP[1][candidateX -1]) &&
                        (candidate < lineP[1][candidateX +1]) &&
                        (candidate < lineP[2][candidateX -1]) &&
                        (candidate < lineP[2][candidateX]) &&
                        (candidate < lineP[2][candidateX +1]))
        {
            /// The candidate is a minimum, add the X coordinate to the list
            minLocationList[(*minCount)++] = candidateX;
        }
        else if ((candidate > lineP[0][candidateX -1]) &&
                        (candidate > lineP[0][candidateX]) &&
                        (candidate > lineP[0][candidateX +1]) &&
                        (candidate > lineP[1][candidateX -1]) &&
                        (candidate > lineP[1][candidateX +1]) &&
                        (candidate > lineP[2][candidateX -1]) &&
                        (candidate > lineP[2][candidateX]) &&
                        (candidate > lineP[2][candidateX +1]))
        {
            /// The candidate is a maximum, add the X coordinate to the list
            maxLocationList[(*maxCount)++] = candidateX;
        }
    }
}
#endif

//#######################################################################################
void svuLocalMaxMin3x3_fp16(SippFilter *fptr)
{
    half *input[3];
    UInt32 *output;
    UInt32 i;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        input[0] = (half *)getInPtr(fptr, 0, iterNum, 0, 0);
        input[1] = (half *)getInPtr(fptr, 0, iterNum, 1, 0);
        input[2] = (half *)getInPtr(fptr, 0, iterNum, 2, 0);

        //the output line
        output = (UInt32 *)getOutPtr(fptr, iterNum, 0);
        for(i = 0; i < fptr->sliceWidth; i++)
            output[i] = 0;

    #ifdef SIPP_USE_MVCV
        mvcvLocalMaxMin3x3_fp16_asm(input, fptr->sliceWidth, &output[2], &output[264], &output[0], &output[1]);
    #else
        mvcvLocalMaxMin3x3_fp16    (input, fptr->sliceWidth, &output[2], &output[264], &output[0], &output[1]);
    #endif
    }
}
