#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/nonMax3x3Fp32/nonMax3x3Fp32.h>

/// This kernel verifies whether each element from the central line is the maximum within a 3x3 range
/// @param[in] candPos      - X coordinate of the candidates
/// @param[in] candCount      - Number of candidates
/// @param[in] corners1     - Input line (N-1 lines)
/// @param[in] corners2     - Input line (N lines)
/// @param[in] corners3     - Input line (N+1 lines)
/// @param[out] candOutPos    - X coordinate of selected elements
/// @param[out] candOutCount  - Output elements

#ifdef SIPP_USE_MVCV
void mvcvNonMax3x3_fp32_asm(UInt16* candPos, UInt32 candCount, float* corners1, float* corners2,
            float* corners3, UInt16* candOutPos, UInt32 *candOutCount);
#else
void mvcvNonMax3x3_fp32(UInt16* candPos, UInt32 candCount, float* corners1, float* corners2,
            float* corners3, UInt16* candOutPos, UInt32 *candOutCount)
{
    UInt32 i;
    UInt16 candidateX;
    float candidate;
    UInt32 out = 0;

    for(i = 0; i < candCount; i++)
    {
        candidateX = candPos[i];
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
void svuNonMax3x3Fp32(SippFilter *fptr)
{
    float *corners1;
    float *corners2;
    float *corners3;
    UInt16 *output;
    nonMax3x3Fp32Param *param = (nonMax3x3Fp32Param*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        corners1 = (float *)getInPtr(fptr, 0, iterNum, 0, 0);
        corners2 = (float *)getInPtr(fptr, 0, iterNum, 1, 0);
        corners3 = (float *)getInPtr(fptr, 0, iterNum, 2, 0);

        //the output line
        output = (UInt16 *)getOutPtr(fptr, iterNum, 0);

        UInt32 i;
        for(i = 0; i < fptr->sliceWidth; i++)
            output[i] = 0;

    #ifdef SIPP_USE_MVCV
        mvcvNonMax3x3_fp32_asm(param->candPos, fptr->sliceWidth, corners1, corners2, corners3, &output[2], (UInt32*)&output[0]);
    #else
        mvcvNonMax3x3_fp32(param->candPos, fptr->sliceWidth, corners1, corners2, corners3, &output[2], (UInt32*)&output[0]);
    #endif
    }
}
