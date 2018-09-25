#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/xyGen/xyGen.h>

/// xyGenerator for Bicubic
/// @param[in]  warp_matrix     - pointer to the warp_matrix
/// @param[out] output          - pointer to the xy line
/// @param[in]  width           - output width (number of xy)
/// @param[in]  lineNo          - number of output line
/// @param[in]  firstShave      - first shave number
/// @param[in]  lastShave       - last shave number
/// @return     Nothing

static inline float clamp_xy( float a )
{
    if( a < 0 )
        return a + 512;
    if( a >= 512 )
        return a - 512;
    return a;
}

void genXYlist(float *warp_matrix, UInt64 *output, UInt32 width, UInt32 lineNo, UInt32 firstShave, UInt32 lastShave)
{
    unsigned shaveId = scGetShaveNumber();

    float y_ofs_x = ((float)lineNo+0.5f)*warp_matrix[1];
    float y_ofs_y = ((float)lineNo+0.5f)*warp_matrix[4];

    float *list = (float*) output;
    UInt32 x0 =( shaveId - firstShave )* width;
    uint32_t x;
    for (x = x0; x < x0 + width; x++ )
    {
        // x coordinate
        *list++ = clamp_xy( ((float)x+0.5f)*warp_matrix[0] + y_ofs_x + warp_matrix[2] );
        // y coordinate
        *list++ = clamp_xy( ((float)x+0.5f)*warp_matrix[3] + y_ofs_y + warp_matrix[5] );
    }
}

//#######################################################################################
void svuXYgen(SippFilter *fptr)
{
    UInt64 *output;
    XYGenParam *param = (XYGenParam*)fptr->params;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 lpi       = sippFilterGetLinesPerIter (fptr);
    UInt32 runNo     = fptr->exeNo * lpi;
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++, runNo++)
    {
        output = (UInt64*)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
    #else
        genXYlist((float*)param->coefMat, output, fptr->sliceWidth, runNo, param->firstShave, param->lastShave);
    #endif
    }
}
