#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/accumulateWeighted/accumulateWeighted.h>

/// AccumulateWeighted kernel  - The function calculates the weighted sum of the input image (srcAddr) and the accumulator (destAddr) so that accumulator becomes a running average of frame sequence
/// @param[in] srcAddr         - array of pointers to input lines
/// @param[in] maskAddr        - array of pointers to input lines of mask
/// @param[out] destAddr       - array of pointers for output lines
/// @param[in] width           - width of input line
/// @param[in] alpha           - Weight of the input image must be a fp32 between 0 and 1
#ifdef SIPP_USE_MVCV
void mvcvAccumulateWeighted_asm(UInt8** srcAddr, UInt8** maskAddr, float** destAddr,UInt32 width, float alpha);
#else
void AccumulateWeighted        (UInt8** srcAddr, UInt8** maskAddr, float** destAddr,UInt32 width, float alpha)
{
    UInt32 i;
    UInt8* src;
    UInt8* mask;
    float* dest;

    src = *srcAddr;
    mask = *maskAddr;
    dest = *destAddr;

    for (i = 0;i < width; i++)
    {
        if(mask[i]){
                dest[i] =  (1 - alpha) * dest[i] + alpha * src[i];
        }
    }
}
#endif
void svuAccumulateWeighted(SippFilter *fptr)
{
    float *output;
    UInt8 *iline[2];
    float *inLFloat;
    UInt32 i;

    AccumulateWeightedParam *param = (AccumulateWeightedParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        inLFloat = (float *)getInPtr(fptr, 2, iterNum, 0, 0);

        //the 2 input lines
        iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        iline[1]=(UInt8 *)getInPtr(fptr, 1, iterNum, 0, 0);

        //the output line
        output = (float*)getOutPtr(fptr, iterNum, 0);

        for(i=0; i<fptr->sliceWidth; i++)
        {
            output[i] = inLFloat[i];
        }

    #ifdef SIPP_USE_MVCV
        mvcvAccumulateWeighted_asm(&iline[0], &iline[1], &output, fptr->sliceWidth, param->alpha);
    #else
        AccumulateWeighted(&iline[0], &iline[1], &output, fptr->sliceWidth, param->alpha);
    #endif
    }

}



