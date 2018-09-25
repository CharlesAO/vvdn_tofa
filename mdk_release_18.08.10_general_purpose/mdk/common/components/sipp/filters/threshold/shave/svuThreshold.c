#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/threshold/threshold.h>

/// threshold kernel  computes the output image based on a threshold value and a threshold type
/// @param[in] in             - array of pointers to input lines
/// @param[out] out           - array of pointers to output lines
/// @param[in] width          - width of the input line
/// @param[in] height         - height of the input line
/// @param[in] thresh         - threshold value
/// @param[in] thresh_type    - one of the 5 available thresholding types:
///                           - Thresh_To_Zero: values below threshold are zeroed
///                           - Thresh_To_Zero_Inv: opposite of Thresh_To_Zero
///                           - Thresh_To_Binary: values below threshold are zeroed and all others are saturated to pixel max value
///                           - Thresh_To_Binary_Inv: opposite of Thresh_To_Binary
///                           - Thresh_Trunc: values above threshold are given threshold value
///                           - default mode: Thresh_Trunc
/// @return    Nothing

#ifdef SIPP_USE_MVCV
void mvcvThresholdKernel_asm(UInt8** in, UInt8** out, UInt32 width, UInt32 height, UInt8 thresh, UInt32 thresh_type);
#else
void thresholdKernel        (UInt8** in, UInt8** out, UInt32 width, UInt32 height, UInt8 thresh, UInt32 thresh_type)
{
    UInt32 i, j;
    UInt8* in_1;
    in_1 = *in;

    UInt8 max_value = 0xff;

    for(i = 0; i < height ; i++)
    {
        for(j = 0; j < width; j++)
        {
            switch(thresh_type)
            {
                case Thresh_To_Zero:
                    if ((in_1[j+i*width]) > thresh)
                    {
                        out[i][j] = (in_1[j+i*width]);
                    }
                    else
                    {
                        out[i][j] = 0;
                    }
                    break;
                case Thresh_To_Zero_Inv:
                    if ((in_1[j+i*width]) > thresh)
                    {
                        out[i][j] = 0;
                    }
                    else
                    {
                        out[i][j] = (in_1[j+i*width]);
                    }
                    break;
                case Thresh_To_Binary:
                    if ((in_1[j+i*width]) > thresh)
                    {
                        out[i][j] = max_value;
                    }
                    else
                    {
                        out[i][j] = 0;
                    }
                    break;
                case Thresh_To_Binary_Inv:
                    if ((in_1[j+i*width]) > thresh)
                    {
                        out[i][j] = 0;
                    }
                    else
                    {
                        out[i][j] = max_value;
                    }
                    break;
                case Thresh_Trunc:
                    if((in_1[j+i*width]) > thresh)
                    {
                        out[i][j] = thresh;
                    }
                    else
                    {
                        out[i][j] = (in_1[j+i*width]);
                    }
                    break;
                default:
                    if ((in_1[j+i*width]) > thresh)
                    {
                        out[i][j] = thresh;
                    }
                    else
                    {
                        out[i][j] = (in_1[j+i*width]);
                    }
                    break;
            }
        }
    }
}
#endif

void svuThreshold(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[1];
    ThresholdParam *param = (ThresholdParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input line
        iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvThresholdKernel_asm(iline, &output, fptr->sliceWidth, 1, param->thresholdValue, param->threshType);
    #else
        thresholdKernel(iline, &output, fptr->sliceWidth, 1, param->thresholdValue, param->threshType);
    #endif
    }
}