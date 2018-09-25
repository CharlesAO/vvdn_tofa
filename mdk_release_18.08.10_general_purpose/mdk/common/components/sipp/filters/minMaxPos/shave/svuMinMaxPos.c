#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/minMaxPos/minMaxPos.h>

/// minMaxPos kernel     - computes the minimum and the maximum value of a given input line and their position
/// @param[in] in        - input line
/// @param[in] width     - line`s width(length)
/// @param[in] height    - height of image (defaulted to one line)
/// @param[in] minVal    - stores the minimum value on the line
/// @param[in] maxVal    - stores the maximum value on the line
/// @param[out] minPos   - stores the position occupied by the MIN value within line
/// @param[out] maxPos   - stores the position occupied by the MAX value within line
/// @param[in] maskAddr  - mask filled with 1s and 0s which determines the image area to compute minimum and maximum
/// @return              - Nothing

#ifdef SIPP_USE_MVCV
void mvcvMinMaxPos_asm(UInt8** in, UInt32 width, UInt8* minVal, UInt8* maxVal, UInt32* minPos, UInt32* maxPos, UInt8* maskAddr);
#else
void minMaxPos        (UInt8** in, UInt32 width, UInt8* minVal, UInt8* maxVal, UInt32* minPos, UInt32* maxPos, UInt8* maskAddr)
{
    UInt8* in_1;
    in_1 = in[0];
    UInt8 minV = 0xFF;
    UInt8 maxV = 0x00;
    UInt32 minLoc;
    UInt32 maxLoc;
    UInt32 i, j;

    for(j = 0; j < width; j++)
    {
        if((maskAddr[j]) != 0)
        {
            if (in_1[j] < minV)
            {
                minV = in_1[j];
                minLoc = j;
            }

            if (in_1[j] > maxV)
            {
                maxV = in_1[j];
                maxLoc = j;
            }
        }
    }
        *minVal = minV;
        *minPos = minLoc;
        *maxVal = maxV;
        *maxPos = maxLoc;
}
#endif

void svuMinMaxPos(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *input[1];
    MinMaxPosParam *param = (MinMaxPosParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input lines
        input[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvMinMaxPos_asm(input, fptr->sliceWidth, &output[0], &output[1], (UInt32 *)&output[2], (UInt32 *)&output[6], param->Mask);
    #else
        minMaxPos(input, fptr->sliceWidth, &output[0], &output[1], (UInt32 *)&output[2], (UInt32 *)&output[6], param->Mask);
    #endif
    }
}
