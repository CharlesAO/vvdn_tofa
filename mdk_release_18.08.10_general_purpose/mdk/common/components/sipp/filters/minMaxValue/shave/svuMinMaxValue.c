#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/minMaxValue/minMaxValue.h>

/// minMax kernel - computes the minimum and the maximum value of a given input image
/// @param[in] in         - array of pointers to input lines
/// @param[in] width      - line`s width(length)
/// @param[in] height     - height of image (defaulted to one line)
/// @param[in] minVal     - stores the minimum value on the line
/// @param[in] maxVal     - stores the maximum value on the line
/// @param[in] maskAddr   - mask filled with 1s and 0s which determines the image area to compute minimum and maximum
/// @return               - Nothing

#ifdef SIPP_USE_MVCV
void mvcvMinMaxKernel_asm(UInt8** in, UInt32 width, UInt32 height, UInt8* minVal, UInt8* maxVal, UInt8* maskAddr);
#else
void minMaxKernel        (UInt8** in, UInt32 width, UInt32 height, UInt8* minVal, UInt8* maxVal, UInt8* maskAddr)
{
    UInt8* in_1;
    in_1 = in[0];
    UInt8 minV = 0xFF;
    UInt8 maxV = 0x00;
    UInt32 i, j;

    for(j = 0; j < width; j++)
    {
        if((maskAddr[j]) != 0)
        {
            if (in_1[j] < minV)
            {
                minV = in_1[j];
            }

            if (in_1[j] > maxV)
            {
                maxV = in_1[j];
            }
        }
    }

    if (minVal != NULL)
    {
        *minVal = minV;
    }
    if (maxVal != NULL)
    {
        *maxVal = maxV;
    }
}
#endif

void svuMinMaxValue(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[1];
    UInt32 i;

    minMaxValParam *param = (minMaxValParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input lines
        iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

        for(i=0; i < fptr->sliceWidth; i++)
            output[i] = 0x00;

        param->maxVal = 0;
        param->minVal = 255;

    #ifdef SIPP_USE_MVCV
        mvcvMinMaxKernel_asm(iline, (fptr->sliceWidth), 1, &param->minVal, &param->maxVal, param->maskAddr);
    #else
        minMaxKernel(iline, (fptr->sliceWidth), 1, &param->minVal, &param->maxVal, param->maskAddr);
    #endif

        output[0] = param->minVal;
        output[1] = param->maxVal;
    }
}
