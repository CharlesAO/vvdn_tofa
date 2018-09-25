#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/convertF16ToU8/convertF16ToU8.h>

/// Convert a 16-bit floating point line to 8-bit line, with saturation
/// @param[in] in              - Input line
/// @param[out] out            - Output line
/// @param[in] width           - Width of the input line
/// @return    Nothing

#ifdef SIPP_USE_MVCV
void mvcvConvert_fp16_u8_asm(half* in, UInt8* out, UInt32 width);
#else
void convertF16ToU8(half* in, UInt8* out, UInt32 width)
{
    int i;

    for (i = 0; i < width; i++)
    {
        if (in[i] < 0)
            out[i] = 0;
        else
            if (in[i] > 255)
                out[i] = 255;
            else
                out[i] = (UInt8)in[i];
    }
}
#endif

void svuConvertF16ToU8(SippFilter *fptr)
{
    half *input;
    UInt8 *output;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input line
        input = (half *)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvConvert_fp16_u8_asm(input, output, fptr->sliceWidth);
    #else
        convertF16ToU8(input, output, fptr->sliceWidth);
    #endif
    }
}