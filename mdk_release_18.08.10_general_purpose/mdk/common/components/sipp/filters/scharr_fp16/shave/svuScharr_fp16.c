///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///

#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/scharr_fp16/scharr_fp16.h>
#include <math.h>
#include <float.h>

/// This kernel performs scharr edge detection operator
/// @param[in] in     - Input lines
/// @param[out] out   - Output line
/// @param[in] width  - Width of input line

#ifdef SIPP_USE_MVCV
void mvcvScharr_fp16_asm(half** in, half** out, UInt32 width);
#else
void mvcvScharr_fp16    (half** in, half** out, UInt32 width)
{
    int i,x,y;
    half* lines[3];
    float sx=0, sy=0,s=0;
    half* aux;
    aux=(*out);

    //scharr matrix
    float VertScharr[3][3]={
        {-3.0, 0.0, 3.0},
        {-10.0, 0.0, 10.0},
        {-3.0, 0.0, 3.0}
    };

    float HorzScharr[3][3]={
        {-3.0, -10.0, -3.0},
        { 0.0,  0.0,  0.0},
        { 3.0,  10.0,  3.0}
    };

    //Initialize lines pointers
    lines[0]=*in;
    lines[1]=*(in+1);
    lines[2]=*(in+2);

    //Go on the whole line
    for (i=0;i<width;i++){
        sx = VertScharr[0][0]*lines[0][i-1]+ VertScharr[0][1]*lines[0][i] +VertScharr[0][2]*lines[0][i+1]+
                        VertScharr[1][0]*lines[1][i-1]+ VertScharr[1][1]*lines[1][i] +VertScharr[1][2]*lines[1][i+1]+
                        VertScharr[2][0]*lines[2][i-1]+ VertScharr[2][1]*lines[2][i] +VertScharr[2][2]*lines[2][i+1];

        sy = HorzScharr[0][0]*lines[0][i-1]+ HorzScharr[0][1]*lines[0][i] +HorzScharr[0][2]*lines[0][i+1]+
                        HorzScharr[1][0]*lines[1][i-1]+ HorzScharr[1][1]*lines[1][i] +HorzScharr[1][2]*lines[1][i+1]+
                        HorzScharr[2][0]*lines[2][i-1]+ HorzScharr[2][1]*lines[2][i] +HorzScharr[2][2]*lines[2][i+1];

        s = sqrtf(sx*sx+ sy*sy);
        aux[i] = (half)s;
    }
}
#endif

//##########################################################################################
void svuScharr_fp16(SippFilter *fptr)
{
    half *output;
    half *iline[3];

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        iline[0] = (half *)getInPtr(fptr, 0, iterNum, 0, 0);
        iline[1] = (half *)getInPtr(fptr, 0, iterNum, 1, 0);
        iline[2] = (half *)getInPtr(fptr, 0, iterNum, 2, 0);

        //the output line
        output = (half *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvScharr_fp16_asm(iline, &output, fptr->sliceWidth);
    #else
        mvcvScharr_fp16(iline, &output, fptr->sliceWidth);
    #endif
    }
}
