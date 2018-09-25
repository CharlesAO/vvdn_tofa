#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/cornerMinEigenVal/cornerMinEigenVal.h>
#include <math.h>

#define MAX_WIDTH 1050
#define PADDING 16
#define INTERMEDIARY_BUFFER_LINE_NUMBER 24 // 6 lines of float = 24 lines of Uint8

//#include <stdio.h>
//#include <stdlib.h>

/// CornerMinEigenVal filter - is 5x5 kernel size
/// @param[in]   input_lines   - pointer to input pixel
/// @param[out]  output_line   - position on line
/// @param[in]   width         - width of line

#ifdef SIPP_USE_MVCV
void mvcvCornerMinEigenVal_asm(UInt8 **in_lines, UInt8 **out_line, UInt8 *buffer, UInt32 width);
#else
void CornerMinEigenVal(UInt8 **in_lines, UInt8 **out_line, UInt8 *buffer, UInt32 width)
{
    float *dx1, *dx2, *dx3;
    float *dy1, *dy2, *dy3;

    float a, b, c, xu, x, y, number;
    UInt8    *output = *out_line;
    UInt32 i, j, contor = 0;

    //sobel matrix
    float VertSobel[3][3]={
            {1, 0, -1},
            {2, 0, -2},
            {1, 0, -1}
    };

    float HorzSobel[3][3]={
            { 1,  2,  1},
            { 0,  0,  0},
            {-1, -2, -1}
    };

    dx1   = (float *)buffer;
    dx2   = (float *)dx1 + width +8 ;
    dx3   = (float *)dx2 + width +8 ;
    dy1   = (float *)dx3 + width +8 ;
    dy2   = (float *)dy1 + width +8 ;
    dy3   = (float *)dy2 + width +8 ;

    for(i=0;i<5;i++)
    {
        in_lines[i]=in_lines[i] - 1;
    }

    for (i=0; i<width+2; i++)
    {
        dx1[i]    =   VertSobel[0][0]*in_lines[0][i-1]+ VertSobel[0][1]*in_lines[0][i-0] +VertSobel[0][2]*in_lines[0][i+1]+
                    VertSobel[1][0]*in_lines[1][i-1]+ VertSobel[1][1]*in_lines[1][i-0] +VertSobel[1][2]*in_lines[1][i+1]+
                    VertSobel[2][0]*in_lines[2][i-1]+ VertSobel[2][1]*in_lines[2][i-0] +VertSobel[2][2]*in_lines[2][i+1];

        dy1[i]    =     HorzSobel[0][0]*in_lines[0][i-1]+ HorzSobel[0][1]*in_lines[0][i-0] +HorzSobel[0][2]*in_lines[0][i+1]+
                    HorzSobel[1][0]*in_lines[1][i-1]+ HorzSobel[1][1]*in_lines[1][i-0] +HorzSobel[1][2]*in_lines[1][i+1]+
                    HorzSobel[2][0]*in_lines[2][i-1]+ HorzSobel[2][1]*in_lines[2][i-0] +HorzSobel[2][2]*in_lines[2][i+1];

        dx2[i]    =    VertSobel[0][0]*in_lines[1][i-1]+ VertSobel[0][1]*in_lines[1][i-0] +VertSobel[0][2]*in_lines[1][i+1]+
                    VertSobel[1][0]*in_lines[2][i-1]+ VertSobel[1][1]*in_lines[2][i-0] +VertSobel[1][2]*in_lines[2][i+1]+
                    VertSobel[2][0]*in_lines[3][i-1]+ VertSobel[2][1]*in_lines[3][i-0] +VertSobel[2][2]*in_lines[3][i+1];

        dy2[i]    =     HorzSobel[0][0]*in_lines[1][i-1]+ HorzSobel[0][1]*in_lines[1][i-0] +HorzSobel[0][2]*in_lines[1][i+1]+
                    HorzSobel[1][0]*in_lines[2][i-1]+ HorzSobel[1][1]*in_lines[2][i-0] +HorzSobel[1][2]*in_lines[2][i+1]+
                    HorzSobel[2][0]*in_lines[3][i-1]+ HorzSobel[2][1]*in_lines[3][i-0] +HorzSobel[2][2]*in_lines[3][i+1];

        dx3[i]    =    VertSobel[0][0]*in_lines[2][i-1]+ VertSobel[0][1]*in_lines[2][i-0] +VertSobel[0][2]*in_lines[2][i+1]+
                    VertSobel[1][0]*in_lines[3][i-1]+ VertSobel[1][1]*in_lines[3][i-0] +VertSobel[1][2]*in_lines[3][i+1]+
                    VertSobel[2][0]*in_lines[4][i-1]+ VertSobel[2][1]*in_lines[4][i-0] +VertSobel[2][2]*in_lines[4][i+1];

        dy3[i]    =     HorzSobel[0][0]*in_lines[2][i-1]+ HorzSobel[0][1]*in_lines[2][i-0] +HorzSobel[0][2]*in_lines[2][i+1]+
                    HorzSobel[1][0]*in_lines[3][i-1]+ HorzSobel[1][1]*in_lines[3][i-0] +HorzSobel[1][2]*in_lines[3][i+1]+
                    HorzSobel[2][0]*in_lines[4][i-1]+ HorzSobel[2][1]*in_lines[4][i-0] +HorzSobel[2][2]*in_lines[4][i+1];
    }

dx1   = dx1   + 1;
dx2   = dx2   + 1;
dx3   = dx3   + 1;
dy1   = dy1   + 1;
dy2   = dy2   + 1;
dy3   = dy3   + 1;

    for (i=0; i<width;i++)
    {
        a    = ((dx1[i-1]*dx1[i-1] + dx1[i+0]*dx1[i+0] + dx1[i+1]*dx1[i+1] +
                 dx2[i-1]*dx2[i-1] + dx2[i+0]*dx2[i+0] + dx2[i+1]*dx2[i+1] +
                 dx3[i-1]*dx3[i-1] + dx3[i+0]*dx3[i+0] + dx3[i+1]*dx3[i+1])/9.0f) * 0.5f;
        b    =  (dx1[i-1]*dy1[i-1] + dx1[i+0]*dy1[i+0] + dx1[i+1]*dy1[i+1] +
                 dx2[i-1]*dy2[i-1] + dx2[i+0]*dy2[i+0] + dx2[i+1]*dy2[i+1] +
                 dx3[i-1]*dy3[i-1] + dx3[i+0]*dy3[i+0] + dx3[i+1]*dy3[i+1])/9.0f;
        c    = ((dy1[i-1]*dy1[i-1] + dy1[i+0]*dy1[i+0] + dy1[i+1]*dy1[i+1] +
                 dy2[i-1]*dy2[i-1] + dy2[i+0]*dy2[i+0] + dy2[i+1]*dy2[i+1] +
                 dy3[i-1]*dy3[i-1] + dy3[i+0]*dy3[i+0] + dy3[i+1]*dy3[i+1])/9.0f)* 0.5f;
        x = (a + c) - sqrt((a - c)*(a - c) + b*b);
        x = x > 255 ? 255 : x;
        output[i] = (UInt8)(x);
    }
}
#endif

UInt8 buffer[(MAX_WIDTH + PADDING)*INTERMEDIARY_BUFFER_LINE_NUMBER];
void svuCornerMinEigenVal(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[5];
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input line
        iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        iline[1]=(UInt8 *)getInPtr(fptr, 0, iterNum, 1, 0);
        iline[2]=(UInt8 *)getInPtr(fptr, 0, iterNum, 2, 0);
        iline[3]=(UInt8 *)getInPtr(fptr, 0, iterNum, 3, 0);
        iline[4]=(UInt8 *)getInPtr(fptr, 0, iterNum, 4, 0);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #if defined (SIPP_USE_MVCV)
        mvcvCornerMinEigenVal_asm(iline, &output, (UInt8 *)buffer, fptr->sliceWidth); // for myriad2, 4 parameters
    #else
        CornerMinEigenVal(iline, &output, (UInt8 *)buffer, fptr->sliceWidth); // C code, 4 parameters
    #endif
    }
}
