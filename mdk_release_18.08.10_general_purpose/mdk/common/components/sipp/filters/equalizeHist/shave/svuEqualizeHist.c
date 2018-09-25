#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/equalizeHist/equalizeHist.h>

/// equalizehistogram kernel - makes an equalization trough an image with a given histogram
/// @param[in] in            - array of pointers to input lines
/// @param[out]out           - array of pointers to output lines
/// @param[in] hist          - pointer to an input array that indicates the cumulative histogram of the image
/// @param[in] width         - width of input line

#ifdef SIPP_USE_MVCV
void mvcvEqualizeHist_asm(UInt8** in, UInt8** out, UInt32 *hist, UInt32 width);
#else
void equalizeHist        (UInt8** in, UInt8** out, UInt32 *hist, UInt32 width)
{
    UInt8 *in_line;
    UInt8 *out_line;
    UInt32 i;

    in_line   = *in;
    out_line  = *out;

    for(i=0;i<width;i++)
    {
        out_line[i]= (UInt8)((float)hist[in_line[i]] * ((float)255/(float)hist[255]));
    }
}
#endif

void svuEqualizeHist(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[1];

    EqualizeHistParam *param = (EqualizeHistParam*)fptr->params;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input line
        iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvEqualizeHist_asm(iline, &output, param->cum_hist, fptr->sliceWidth);
    #else
        equalizeHist(iline, &output, param->cum_hist, fptr->sliceWidth);
    #endif
    }
}
