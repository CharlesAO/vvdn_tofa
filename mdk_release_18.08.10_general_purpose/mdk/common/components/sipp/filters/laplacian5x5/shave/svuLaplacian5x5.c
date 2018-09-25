#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/laplacian5x5/laplacian5x5.h>

/// Laplacian filter  - applies a Laplacian filter with custom size (see http://en.wikipedia.org/wiki/Discrete_Laplace_operator )
/// @param[in] in     - array of pointers to input lines
/// @param[out] out   - pointer to output line
/// @param[in] width  - width of input line

#ifdef SIPP_USE_MVCV
void mvcvSLaplacian5x5_asm      (UInt8** in, UInt8** out, UInt32 inWidth);
#else
void sLaplacian5x5Implementation(UInt8** in, UInt8** out, UInt32 inWidth)
{
    int i,x,y;
    UInt8* lines[5];
    float sum;
    int lapl[25]={ 0, 0, -1, 0, 0, 0, -1, -2, -1, 0, -1, -2, 16, -2, -1, 0, -1, -2, -1, 0, 0, 0, -1, 0, 0};

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];

    //Go on the whole line
    for (i = 0; i < (int)inWidth; i++)
    {
        sum = 0.0f;
        for (x = 0; x < 5; x++){
            for (y = -2; y < 3; y++){
                sum += (float)(lines[x][y] * lapl[x * 5 + y + 2]);
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

void svuLaplacian5x5(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *source[5];

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        source[0] =(UInt8 *)getInPtr(fptr,0,iterNum,0,0);    //source[0] =(UInt8 *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][0], svuNo);
        source[1] =(UInt8 *)getInPtr(fptr,0,iterNum,1,0);    //source[1] =(UInt8 *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][1], svuNo);
        source[2] =(UInt8 *)getInPtr(fptr,0,iterNum,2,0);    //source[2] =(UInt8 *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][2], svuNo);
        source[3] =(UInt8 *)getInPtr(fptr,0,iterNum,3,0);    //source[3] =(UInt8 *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][3], svuNo);
        source[4] =(UInt8 *)getInPtr(fptr,0,iterNum,4,0);    //source[4] =(UInt8 *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][4], svuNo);
        //the output line
        output = (UInt8 *)getOutPtr(fptr,iterNum,0);   //output = (UInt8 *)WRESOLVE((void*)fptr->dbLineOut[runNo&1], svuNo);

    #ifdef SIPP_USE_MVCV
        mvcvSLaplacian5x5_asm(source, &output, fptr->sliceWidth);
    #else
        sLaplacian5x5Implementation(source, &output, fptr->sliceWidth);
    #endif
    }
}
