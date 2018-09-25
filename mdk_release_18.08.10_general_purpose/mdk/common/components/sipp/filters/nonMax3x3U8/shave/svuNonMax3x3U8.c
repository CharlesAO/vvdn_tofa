#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/nonMax3x3U8/nonMax3x3U8.h>

/// This kernel verifies whether each element from the central line is the maximum within a 3x3 range
/// @param[in] width          - line width
/// @param[in] corners1       - input N-1 line
/// @param[in] corners2       - input N line
/// @param[in] corners3       - input N+1 line
/// @param[out] candOutPos    - X coordinate of output elements
/// @param[out] candOutCount  - number of output elements

#ifdef SIPP_USE_MVCV
void mvcvNonMax3x3_u8_asm(UInt32 width, UInt8* corners1, UInt8* corners2,
            UInt8* corners3, UInt16* candOutPos, UInt32 *candOutCount);
#else
void mvcvNonMax3x3_u8(UInt32 width, UInt8* corners1, UInt8* corners2,
            UInt8* corners3, UInt16* candOutPos, UInt32 *candOutCount)
{
    UInt32 i;
    UInt16 candidateX;
    UInt8 candidate;
    UInt32 out = 0;
    for(i = 0; i < width; i++)
    {
        candidateX = i;
        candidate = corners2[candidateX];
        if((corners1[candidateX - 1]  < candidate) &&
           (corners1[candidateX]      < candidate) &&
           (corners1[candidateX + 1]  < candidate) &&
           (corners2[candidateX - 1]  < candidate) &&
           (corners2[candidateX + 1]  < candidate) &&
           (corners3[candidateX - 1]  < candidate) &&
           (corners3[candidateX]      < candidate) &&
           (corners3[candidateX + 1]  < candidate))
        {
            candOutPos[out++] = candidateX;
        }
    }
    *candOutCount = out;
}
#endif

//#######################################################################################
void svuNonMax3x3U8(SippFilter *fptr)
{
    UInt8 *corners1;
    UInt8 *corners2;
    UInt8 *corners3;
    UInt16 *output;
    UInt32 i;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        corners1 = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        corners2 = (UInt8 *)getInPtr(fptr, 0, iterNum, 1, 0);
        corners3 = (UInt8 *)getInPtr(fptr, 0, iterNum, 2, 0);

        //the output line
        output = (UInt16 *)getOutPtr(fptr, iterNum, 0);
        for(i = 0; i < fptr->sliceWidth; i++)
            output[i] = 0;

    #ifdef SIPP_USE_MVCV
        mvcvNonMax3x3_u8_asm(fptr->sliceWidth, corners1, corners2, corners3, &output[2], (UInt32 *)&output[0]);
    #else
        mvcvNonMax3x3_u8(fptr->sliceWidth, corners1, corners2, corners3, &output[2], (UInt32 *)&output[0]);
    #endif
    }
}