#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/sLaplacian3x3Fp16ToFp16/sLaplacian3x3Fp16ToFp16.h>

/// Laplacian filter  - applies a Laplacian filter with custom size (see http://en.wikipedia.org/wiki/Discrete_Laplace_operator )
/// @param[in] in     - array of pointers to input lines
/// @param[out] out   - pointer to output line
/// @param[in] width  - width of input line

void sLaplacian3x3Fp16ToFp16(half** in, half** out, UInt32 inWidth)
{
    Int32 x,y;
    UInt32 i;
    half* lines[3];
    half sum;
    int lapl[9]={ 0, -1, 0, -1, 4, -1, 0, -1, 0};

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];

    //Go on the whole line
    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;
        for (x = 0; x < 3; x++)
        {
            for (y = -1; y < 2; y++){
                sum += (half)(lines[x][y] * lapl[x * 3 + y + 1]);
            }
            lines[x]++;
        }

        /*if (sum >= 255)
            sum = 255.0;
        if (sum <= 0)
            sum = 0.0;*/
        out[0][i] = (half)(sum);
    }
}

void svusLaplacian3x3Fp16ToFp16(SippFilter *fptr)
{
    half *output;
    half *source[3];

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        source[0] =(half *)getInPtr(fptr,0,iterNum,0,0);  //(half *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][0], svuNo);
        source[1] =(half *)getInPtr(fptr,0,iterNum,1,0);  //(half *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][1], svuNo);
        source[2] =(half *)getInPtr(fptr,0,iterNum,2,0);  //(half *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][2], svuNo);

        //the output line
        output = (half *)getOutPtr(fptr, iterNum, 0); //(half *)WRESOLVE((void*)fptr->dbLineOut[runNo&1], svuNo);

    //#ifdef SIPP_USE_MVCV
        //mvcvsLaplacian3x3Fp16ToFp16_asm(source, &output, fptr->sliceWidth);
    //#else
        sLaplacian3x3Fp16ToFp16(source, &output, fptr->sliceWidth);
    //#endif
    }
}