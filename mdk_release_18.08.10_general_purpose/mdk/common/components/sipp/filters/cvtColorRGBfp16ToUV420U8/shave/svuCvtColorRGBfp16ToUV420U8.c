#include <sipp.h>
//#include <sippMacros.h>
#include <filters/cvtColorRGBfp16ToUV420U8/cvtColorRGBfp16ToUV420U8.h>

/// Performs color space conversion RGBfp16 to UV420U8 for one line in an image
/// @param[in]  inRGB   - pointer to the first lines from input RGB planes
/// @param[Out] uOut    - pointer to the U plane
/// @param[Out] vOut    - pointer to the V plane
/// @param[in]  width   - line width in pixels

#ifdef SIPP_USE_MVCV
void mvcvCvtColorRGBfp16ToUV420U8_asm(half* inRGB[3], UInt8* uOut, UInt8* vOut, UInt32 width);
#else
void cvtColorRGBfp16ToUV420U8        (half* inRGB[3], UInt8* uOut, UInt8* vOut, UInt32 width)//, UInt32 line)
{
    UInt32 i,j;

    half *r   = inRGB[0];
    half *g   = inRGB[1];
    half *b   = inRGB[2];

    half ua, ub, va, vb;
    half  um, vm;
    UInt32 u_idx = 0;
    UInt32 v_idx = 0;

    for (i = 0; i < width; i++)
    {
        ua = ((r[i*2] * (-0.14713f)) + (g[i*2] * (-0.28886f)) + (b[i*2] * 0.436f     ) + (half)((float)(128.0f/255.0f)));
        va = ((r[i*2] * 0.615f     ) + (g[i*2] * (-0.51499f)) + (b[i*2] * (-0.10001f)) + (half)((float)(128.0f/255.0f)));

        //-----------------------------------------------------------------------------------------------------------------

        ub = ((r[i*2 + 1] * (-0.14713f)) + (g[i*2 + 1] * (-0.28886f)) + (b[i*2 + 1] * 0.436f     ) + (half)((float)(128.0f/255.0f)));
        vb = ((r[i*2 + 1] * 0.615f     ) + (g[i*2 + 1] * (-0.51499f)) + (b[i*2 + 1] * (-0.10001f)) + (half)((float)(128.0f/255.0f)));

        um = (ua + ub) / 2;
        vm = (va + vb) / 2;

        if (um <= (half)0.0f)
            um = (half)0.0f;
        if (um >= (half)1.0f)
            um = (half)1.0f;

        if (vm <= (half)0.0f)
            vm = (half)0.0f;
        if (vm >= (half)1.0f)
            vm = (half)1.0f;

        uOut[u_idx] = (UInt8)((float)um * 255.0f);
        vOut[u_idx] = (UInt8)((float)vm * 255.0f);
        u_idx++;
        v_idx++;
    }
}
#endif

//#######################################################################################
void svuCvtColorRGBfp16ToUV420U8(SippFilter *fptr)
{
    half *inRGB[3];
    UInt8 *outputU;
    UInt8 *outputV;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        inRGB[0] = (half *)getInPtr(fptr, 0, iterNum, 0, 0);
        inRGB[1] = (half *)getInPtr(fptr, 0, iterNum, 0, 1);
        inRGB[2] = (half *)getInPtr(fptr, 0, iterNum, 0, 2);

        //the 2 output line
        outputU = (UInt8 *)getOutPtr(fptr, iterNum, 0);
        outputV = (UInt8 *)getOutPtr(fptr, iterNum, 1);

    #ifdef SIPP_USE_MVCV
        mvcvCvtColorRGBfp16ToUV420U8_asm(inRGB, outputU, outputV, fptr->sliceWidth);
    #else
        cvtColorRGBfp16ToUV420U8(inRGB, outputU, outputV, fptr->sliceWidth);
    #endif
    }
}
