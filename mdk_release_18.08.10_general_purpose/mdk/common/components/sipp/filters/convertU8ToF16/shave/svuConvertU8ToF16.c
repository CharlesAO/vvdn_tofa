#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/convertU8ToF16/convertU8ToF16.h>

/// Convert a 8-bit line to 16-bit floating point line
/// @param[in] in              - Input line
/// @param[out] out            - Output line
/// @param[in] width           - Width of the input line
/// @return    Nothing

#ifdef SIPP_USE_MVCV
void mvcvConvert_u8_fp16_asm(UInt8* in, half* out, UInt32 width);
#else
void convertU8ToF16(UInt8* in, half* out, UInt32 width)
{
    int i;
    float inFloat;

    for(i = 0; i < width; i++)
    {
        inFloat = (float)in[i];
        out[i] = (half)inFloat;
    }
}
#endif

void svuConvertU8ToF16(SippFilter *fptr)
{
    UInt8 *input;
    half *output;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        //the input line
        input = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);

        //the output line
        output = (half *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvConvert_u8_fp16_asm(input, output, fptr->sliceWidth);
    #else
        convertU8ToF16(input, output, fptr->sliceWidth);
    #endif
    }
}
