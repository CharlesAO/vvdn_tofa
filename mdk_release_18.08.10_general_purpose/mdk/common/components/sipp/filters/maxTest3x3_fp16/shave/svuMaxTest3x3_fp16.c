#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/maxTest3x3_fp16/maxTest3x3_fp16.h>

/// This kernel will compare the points from inBufferCandidates to the corresponding 3x3 zone of inBuffer.
/// The function will check maxCountIn locations.
/// @param[in]  inBufferCandidates - Fp16 buffer, where the candidates can be found
/// @param[in]  inBuffer           - Address of the fp16 image buffer. This buffer should have a size of 3 * width * sizeof(fp16) bytes (3 lines)
/// @param[in]  width              - Line width in pixels as u32
/// @param[in]  maxLocationsIn     - Gives the x coordinates of the candidates. Only these candidates are checked.
/// @param[out] maxLocationsOut    - The values from maxLocationsIn which passed the filter.
/// @param[in]  maxCountIn         - Number of values in maxLocationsIn buffer
/// @param[out] maxCountOut        - Number of values in maxLocationsOut buffer

#ifdef SIPP_USE_MVCV
void mvcvMaxTest3x3_fp16_asm(half* inBufferCandidates, half** inBuffer, UInt32 width,
                     UInt32 maxLocationsIn[], UInt32 maxLocationsOut[],
                     UInt32 maxCountIn, UInt32* maxCountOut);
#else
void mvcvMaxTest3x3_fp16(half* inBufferCandidates, half** inBuffer, UInt32 width,
                     UInt32 maxLocationsIn[], UInt32 maxLocationsOut[],
                     UInt32 maxCountIn, UInt32* maxCountOut)
{
    UInt32 index;
    half candidate;
    UInt32 candidateX;
    half* lineP[3];
    lineP[0] = inBuffer[0];
    lineP[1] = inBuffer[1];
    lineP[2] = inBuffer[2];
    (*maxCountOut) = 0;
    for (index = 0; index < maxCountIn; index++)
    {
        candidateX = maxLocationsIn[index];
        candidate = inBufferCandidates[candidateX];
        if ((candidate > lineP[0][candidateX -1]) &&
                        (candidate > lineP[0][candidateX]) &&
                        (candidate > lineP[0][candidateX +1]) &&
                        (candidate > lineP[1][candidateX -1]) &&
                        (candidate > lineP[1][candidateX]) &&
                        (candidate > lineP[1][candidateX +1]) &&
                        (candidate > lineP[2][candidateX -1]) &&
                        (candidate > lineP[2][candidateX]) &&
                        (candidate > lineP[2][candidateX +1]))
        {
            /// The candidate is a maximum, add the X coordinate to the list
            maxLocationsOut[(*maxCountOut)++] = candidateX;
        }
    }
}
#endif

//#######################################################################################
void svuMaxTest3x3_fp16(SippFilter *fptr)
{
    half *inBuffer[3];
    UInt32 *output;
    UInt32 i;
    MaxTest3x3fp16Param *param = (MaxTest3x3fp16Param*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        inBuffer[0] = (half *)getInPtr(fptr, 0, iterNum, 0, 0);
        inBuffer[1] = (half *)getInPtr(fptr, 0, iterNum, 1, 0);
        inBuffer[2] = (half *)getInPtr(fptr, 0, iterNum, 2, 0);

        //the output line
        output = (UInt32 *)getOutPtr(fptr, iterNum, 0);
        for(i = 0; i < fptr->sliceWidth; i++)
            output[i] = 0;

    #ifdef SIPP_USE_MVCV
        mvcvMaxTest3x3_fp16_asm(param->inBufferCandidates, inBuffer, fptr->sliceWidth, param->maxLocationsIn, &output[1], param->maxCountIn, &output[0]);
    #else
        mvcvMaxTest3x3_fp16    (param->inBufferCandidates, inBuffer, fptr->sliceWidth, param->maxLocationsIn, &output[1], param->maxCountIn, &output[0]);
    #endif
    }
}
