#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/laplacian3x3/laplacian3x3.h>

/// Laplacian filter  - applies a Laplacian filter with custom size (see http://en.wikipedia.org/wiki/Discrete_Laplace_operator )
/// @param[in] in     - array of pointers to input lines
/// @param[out] out   - pointer to output line
/// @param[in] width  - width of input line

#ifdef SIPP_USE_MVCV
void mvcvSLaplacian3x3_asm     (UInt8** in, UInt8** out, UInt32 inWidth);
#else
void Laplacian3x3Implementation(UInt8** in, UInt8** out, UInt32 inWidth)
{
    int i,x,y;
    UInt8* lines[3];
    float sum;
    int lapl[9]={ 0, -1, 0, -1, 4, -1, 0, -1, 0};

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];

    //Go on the whole line
    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0f;
        for (x = 0; x < 3; x++){
            for (y = -1; y < 2; y++){
                sum += (float)(lines[x][y] * lapl[x * 3 + y + 1]);
            }
            lines[x]++;
        }

        if (sum >= 255)
            sum = 255.0f;
        if (sum <= 0)
            sum = 0.0f;
        out[0][i] = (UInt8)(sum);
    }
}
#endif

void svuLaplacian3x3(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *source[3];

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        source[0] =(UInt8 *)getInPtr(fptr,0,iterNum,0,0);  //(UInt8 *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][0], svuNo);
        source[1] =(UInt8 *)getInPtr(fptr,0,iterNum,1,0);  //(UInt8 *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][1], svuNo);
        source[2] =(UInt8 *)getInPtr(fptr,0,iterNum,2,0);  //(UInt8 *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][2], svuNo);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0); //(UInt8 *)WRESOLVE((void*)fptr->dbLineOut[runNo&1], svuNo);

    #ifdef SIPP_USE_MVCV
        mvcvSLaplacian3x3_asm(source, &output, fptr->sliceWidth);
    #else
        Laplacian3x3Implementation(source, &output, fptr->sliceWidth);
    #endif
    }
}
