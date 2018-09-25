#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/convertFrom12BppTo8Bpp/convertFrom12BppTo8Bpp.h>

/// ConvertFrom12BppTo8Bpp kernel
///This kernel converts from 12 bpp to 8 bpp
///@param[in]   in          - Input line
///@param[out]  out         - Output line
///@param[in]   width       - Width of the input line

#ifdef SIPP_USE_MVCV
void mvcvConvert12BppTo8Bpp_asm(UInt8* out, UInt8* in, u32 width);
#else
void mvcvConvert12BppTo8Bpp    (UInt8* out, UInt8* in, u32 width)
{
    UInt8* inLine = in;
    UInt8* outLine = out;

    for (u32 i = 0; i < width; i++)
    {
        outLine[i] = (inLine[i << 1] >> 4) | (inLine[(i << 1) + 1] << 4);
    }
}
#endif

void svuConvertFrom12BppTo8Bpp(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *input;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        input = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvConvert12BppTo8Bpp_asm(output, input, fptr->sliceWidth);
    #else
        mvcvConvert12BppTo8Bpp(output, input, fptr->sliceWidth);
    #endif
    }
}
