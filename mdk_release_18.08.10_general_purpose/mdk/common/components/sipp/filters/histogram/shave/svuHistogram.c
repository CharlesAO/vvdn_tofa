#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/histogram/histogram.h>

/// histogram kernel - makes a histogram on a given plane
/// @param[in] in         - array of pointers to input lines
/// @param[out]hist       - array oh values from histogram
/// @param[in] width      - width of input line

#ifdef SIPP_USE_MVCV
void mvcvHistogram_asm(UInt8** in, UInt32 *hist, UInt32 width);
#else
void histogram        (UInt8** in, UInt32 *hist, UInt32 width)
{
    UInt8 *in_line;
    UInt32 i;
    in_line  = *in;

    for (i=0;i<width; i++)
    {
        hist[in_line[i]]++;
    }
}
#endif

void svuHistogram(SippFilter *fptr)
{
    UInt8 *iline[1];
    HistogramParam *param = (HistogramParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input line
        iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);

    #ifdef SIPP_USE_MVCV
        mvcvHistogram_asm(iline, param->hist, fptr->sliceWidth);
    #else
        histogram(iline, param->hist, fptr->sliceWidth);
    #endif
    }
}

