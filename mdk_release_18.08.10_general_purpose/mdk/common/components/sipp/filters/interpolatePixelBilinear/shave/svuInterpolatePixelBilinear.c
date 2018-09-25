#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/interpolatePixelBilinear/interpolatePixelBilinear.h>

/// Bilinear interpolation of four pixels
/// @param[in]  Line1   - First input line
/// @param[in]  Line2   - Second input line
/// @param[in]  x       - The x coordinate of the pixel.
/// @param[in]  y       - The y coordinate of the pixel. Because we work with two lines only, only the fractional part of the number matters.
/// @return     The value of the interpolated pixel.

#ifdef SIPP_USE_MVCV
half mvcvInterpolatePixelBilinear_asm(half* line1, half* line2, float x, float y);
#else
half mvcvInterpolatePixelBilinear(half* line1, half* line2, float x, float y)
{
    int xp, yp;
    int xp_plus_1, yp_plus_1;
    half w0, w1, w2, w3;
    half* p0;
    half* p1;
    half result;

    xp = (int)x;
    yp = (int)y;
    xp_plus_1 = xp+1;
    yp_plus_1 = yp+1;

    p0 = line1;
    p1 = line2;

    w0 = (xp_plus_1 - x)  * (yp_plus_1 - y);
    w1 = (x         - xp) * (yp_plus_1 - y);
    w2 = (xp_plus_1 - x)  * (y         - yp);
    w3 = (x         - xp) * (y         - yp);

    result = w0*p0[xp] + w1*p0[xp_plus_1] + w2*p1[xp] + w3*p1[xp_plus_1];

    return result;
}
#endif

void svuInterpolatePixelBilinear(SippFilter *fptr)
{
    half *line1;
    half *line2;
    half *output;
    InterpolatePixelBilinearParam *param = (InterpolatePixelBilinearParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input lines
        line1 = (half *)getInPtr(fptr, 0, iterNum, 0, 0);
        line2 = (half *)getInPtr(fptr, 0, iterNum, 1, 0);

        //the output line
        output = (half *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        output[0] = mvcvInterpolatePixelBilinear_asm(line1, line2, param->x, param->y);
    #else
        output[0] = mvcvInterpolatePixelBilinear(line1, line2, param->x, param->y);
    #endif
    }
}


