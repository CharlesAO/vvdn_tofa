///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///

#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/sobel/sobel.h>
#include <math.h>

/// sobel filter - Filter, calculates magnitude
/// @param[in] in     - array of pointers to input lines
/// @param[out]out    - array of pointers for output lines
/// @param[in] width  - width of input line

#ifdef SIPP_USE_MVCV
void mvcvSobel_asm(UInt8** in, UInt8** out, UInt32 width);
#else
void sobel        (UInt8** in, UInt8** out, UInt32 width)
{
    int i,x,y;
    UInt8* lines[5], *outLine;
    float sx=0, sy=0,s=0;
    float sum;
    unsigned char* aux;
    aux=(*out);

    //sobel matrix
    int VertSobel[3][3]={
            {1, 0, -1},
            {2, 0, -2},
            {1, 0, -1}
    };

    int HorzSobel[3][3]={
            { 1,  2,  1},
            { 0,  0,  0},
            {-1, -2, -1}
    };

    //Initialize lines pointers
    lines[0]=*in;
    lines[1]=*(in+1);
    lines[2]=*(in+2);

    //Go on the whole line
    for (i=0;i<width;i++)
    {
        sx =     VertSobel[0][0]*lines[0][i-1]+ VertSobel[0][1]*lines[0][i] +VertSobel[0][2]*lines[0][i+1]+
                 VertSobel[1][0]*lines[1][i-1]+ VertSobel[1][1]*lines[1][i] +VertSobel[1][2]*lines[1][i+1]+
                 VertSobel[2][0]*lines[2][i-1]+ VertSobel[2][1]*lines[2][i] +VertSobel[2][2]*lines[2][i+1];

        sy =     HorzSobel[0][0]*lines[0][i-1]+ HorzSobel[0][1]*lines[0][i] +HorzSobel[0][2]*lines[0][i+1]+
                 HorzSobel[1][0]*lines[1][i-1]+ HorzSobel[1][1]*lines[1][i] +HorzSobel[1][2]*lines[1][i+1]+
                 HorzSobel[2][0]*lines[2][i-1]+ HorzSobel[2][1]*lines[2][i] +HorzSobel[2][2]*lines[2][i+1];
        s= sqrtf(sx*sx+ sy*sy);
        if (s>255)
            aux[i]=255;
        else
            aux[i] = (UInt8)s;
    }
    return;
}
#endif

//##########################################################################################
void svuSobel(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[3];

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        iline[0] = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        iline[1] = (UInt8 *)getInPtr(fptr, 0, iterNum, 1, 0);
        iline[2] = (UInt8 *)getInPtr(fptr, 0, iterNum, 2, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvSobel_asm(iline, &output, fptr->sliceWidth);
    #else
        sobel(iline, &output, fptr->sliceWidth);
    #endif
    }
}
