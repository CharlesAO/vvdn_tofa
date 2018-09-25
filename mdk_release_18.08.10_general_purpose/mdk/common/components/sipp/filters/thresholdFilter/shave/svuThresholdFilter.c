#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/thresholdFilter/thresholdFilter.h>

///@param[in] cornerScores  - pointer to the input line
///@param[in] threshold     - threshold value
///@param[in] width         - input line width
///@param[in] posOffset     - offset value
///@param[out] candPos      - pointer to the output line
///@param[out] candCount    - pointer to the width of the output line

#ifdef SIPP_USE_MVCV
void mvcvThresholdFilter_asm(float* cornerScores, float threshold, UInt32 width, UInt32 posOffset, UInt16* candPos, UInt32* candCount);
#else
void mvcvThresholdFilter    (float* cornerScores, float threshold, UInt32 width, UInt32 posOffset, UInt16* candPos, UInt32* candCount)
{
    UInt32 candidate;
    *candCount = 0;

    for (candidate = posOffset; candidate < posOffset + width; candidate++)
    {
        if (cornerScores[candidate] > threshold)
        {
            candPos[(*candCount)++] = candidate;
        }
    }
}
#endif

void svuThresholdFilter(SippFilter *fptr)
{
    float *cornerScores;
    UInt16 *output;
    ThresholdFilterParam *param = (ThresholdFilterParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input line
        cornerScores = (float*)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output = (UInt16*)getOutPtr(fptr, iterNum, 0);
        UInt32 i;
        for(i = 0; i < fptr->sliceWidth ;i++)
            output[i] = 0;

    #ifdef SIPP_USE_MVCV
        mvcvThresholdFilter_asm(cornerScores, param->threshold, fptr->sliceWidth, param->posOffset, &output[1], (UInt32*)&output[0]);
    #else
        mvcvThresholdFilter    (cornerScores, param->threshold, fptr->sliceWidth, param->posOffset, &output[1], (UInt32*)&output[0]);
    #endif
    }
}