#include <sipp.h>
//#include <sippMacros.h>
#include <filters/cvtColorRGBfp16ToLumaU8/cvtColorRGBfp16ToLumaU8.h>

/// Performs color space conversion RGBfp16 to LumaU8 for one line in an image
/// @param[in]  inRGB   - pointer to the first lines from input RGB planes
/// @param[Out] yOut    - pointer to the luma plane
/// @param[in]  width   - line width in pixels

#ifdef SIPP_USE_MVCV
void mvcvCvtColorRGBfp16ToLumaU8_asm(half* inRGB[3], UInt8* yOut, UInt32 width);
#else
void cvtColorRGBfp16ToLumaU8       (half* inRGB[3], UInt8* yOut, UInt32 width)
{
    UInt32 i;

    half *r   = inRGB[0];
    half *g   = inRGB[1];
    half *b   = inRGB[2];
    half y;

    for (i = 0; i < width; i++)
    {
        y = 0.299f * r[i] + 0.587f * g[i] + 0.114f * b[i];

        if (y >= (half)1.0f)
            y = (half)1.0f;
        if (y <= (half)0.0f)
            y = (half)0.0f;
        yOut[i] = (UInt8)((float)y * 255.0f);
    }
}
#endif

//#######################################################################################
void svuCvtColorRGBfp16ToLumaU8(SippFilter *fptr)
{
    half *inRGB[3];
    UInt8 *outputY;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        inRGB[0] = (half *)getInPtr(fptr, 0, iterNum, 0, 0);
        inRGB[1] = (half *)getInPtr(fptr, 0, iterNum, 0, 1);
        inRGB[2] = (half *)getInPtr(fptr, 0, iterNum, 0, 2);

        //the output line
        outputY = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvCvtColorRGBfp16ToLumaU8_asm(inRGB, outputY, fptr->sliceWidth);
    #else
        cvtColorRGBfp16ToLumaU8        (inRGB, outputY, fptr->sliceWidth);
    #endif
    }
}
