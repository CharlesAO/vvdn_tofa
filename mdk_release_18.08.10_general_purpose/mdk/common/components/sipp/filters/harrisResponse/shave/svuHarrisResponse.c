#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/harrisResponse/harrisResponse.h>

/// Computes Harris response over a patch of the image with a radius of 3. The patch
/// size is actually 8x8 to account for borders
/// @param[in] data         - Input patch including borders
/// @param[in] x            - X coordinate inside the patch. Only a value of 3 supported
/// @param[in] y            - Y coordinate inside the patch. Only a value of 3 supported
/// @param[in] step_width   - Step of the patch. Only a value 8 supported (2xradius + 2xborder)
/// @param[in] k            - Constant that changes the response to the edges. Typically 0.02 is used
/// return                  - Corner response value

#ifdef SIPP_USE_MVCV
float mvcvHarrisResponse_asm(UInt8 *patchStart, UInt32 x, UInt32 y, UInt32 stepWidth, float k);
#else
float HarrisResponse        (UInt8 *patchStart, UInt32 x, UInt32 y, UInt32 stepWidth, float k)
{
#define HARRIS_SW_RADIUS  (3) // asm, for speed reason suport just radius 3

    int dx;
    int dy;

    float xx = 0;
    float xy = 0;
    float yy = 0;

    // Skip border and move the pointer to the first pixel
    patchStart += stepWidth + 1;

    for (UInt32 r = y - HARRIS_SW_RADIUS; r < y + HARRIS_SW_RADIUS; r++)
    {
        for (UInt32 c = x - HARRIS_SW_RADIUS; c < x + HARRIS_SW_RADIUS; c++)
        {
            int index = r * stepWidth + c;
            int firstPatchXIndex = (index > 1) ?index - 1 : 0;
            int firstPatchYIndex = (index > (int)stepWidth) ? index - stepWidth : 0;
            UNUSED(firstPatchXIndex);
            UNUSED(firstPatchYIndex);
            dx = patchStart[index - 1] - patchStart[index + 1];
            dy = patchStart[index - stepWidth] - patchStart[index + stepWidth];
            xx += dx * dx;
            xy += dx * dy;
            yy += dy * dy;
        }
    }

    float det = xx * yy - xy * xy;
    float trace = xx + yy;

    //k changes the response of edges.
    //seems sensitive to window size, line thickness, and image blur =o/
    return (det - k * trace * trace);
}
#endif

void svuHarrisResponse(SippFilter *fptr)
{
    float *outputResponses;
    UInt8 *inputPatch;
    HarrisSwParam *param = (HarrisSwParam*)fptr->params;
    UInt32 i;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the  input lines
        inputPatch = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        outputResponses = (float*)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        for (i = 0; i < fptr->sliceWidth; i++)
            outputResponses[i] = mvcvHarrisResponse_asm(&inputPatch[i], 3, 3, (UInt32)fptr->lineStride[0], param->k);
    #else
        for (i = 0; i < fptr->sliceWidth; i++)
            outputResponses[i] = HarrisResponse(&inputPatch[i], 3, 3, (UInt32)fptr->lineStride[0], param->k);
    #endif
    }
}
