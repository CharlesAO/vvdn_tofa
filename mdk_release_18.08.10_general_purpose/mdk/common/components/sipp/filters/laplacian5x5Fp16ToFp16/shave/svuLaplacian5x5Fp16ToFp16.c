#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/laplacian5x5Fp16ToFp16/laplacian5x5Fp16ToFp16.h>

/// Laplacian filter  - applies a Laplacian filter with custom size (see http://en.wikipedia.org/wiki/Discrete_Laplace_operator )
/// @param[in] in     - array of pointers to input lines
/// @param[out] out   - pointer to output line
/// @param[in] width  - width of input line

#ifdef SIPP_USE_MVCV
void mvcvSLaplacian5x5Fp16ToFp16_asm      (half** in, half** out, UInt32 inWidth);
#else
void sLaplacian5x5Fp16ToFp16Implementation(half** in, half** out, UInt32 inWidth)
{
    int i,x,y;
    half* lines[5];
    half sum;
    half lapl[25]={ 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0, -2.0, -1.0, 0.0, -1.0, -2.0, 17.0, -2.0, -1.0, 0.0, -1.0, -2.0, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0};

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];

    //Go on the whole line
    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0;
        for (x = 0; x < 5; x++){
            for (y = -2; y < 3; y++){
                sum += lines[x][y] * lapl[x * 5 + y + 2];
            }
            lines[x]++;
        }

       /* if (sum >= 255)
            sum = 255.0f;
        if (sum <= 0)
            sum = 0.0f;*/
        out[0][i] = sum;
    }
}
#endif

void svuLaplacian5x5Fp16ToFp16(SippFilter *fptr)
{
    half *output;
    half *source[5];

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the 3 input lines
        source[0] =(half *)getInPtr(fptr,0,iterNum,0,0);    //source[0] =(UInt8 *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][0], svuNo);
        source[1] =(half *)getInPtr(fptr,0,iterNum,1,0);    //source[1] =(UInt8 *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][1], svuNo);
        source[2] =(half *)getInPtr(fptr,0,iterNum,2,0);    //source[2] =(UInt8 *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][2], svuNo);
        source[3] =(half *)getInPtr(fptr,0,iterNum,3,0);    //source[3] =(UInt8 *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][3], svuNo);
        source[4] =(half *)getInPtr(fptr,0,iterNum,4,0);    //source[4] =(UInt8 *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][4], svuNo);

        //the output line
        output = (half *)getOutPtr(fptr, iterNum, 0);   //output = (UInt8 *)WRESOLVE((void*)fptr->dbLineOut[runNo&1], svuNo);

    #ifdef SIPP_USE_MVCV
        mvcvSLaplacian5x5Fp16ToFp16_asm(source, &output, fptr->sliceWidth);
    #else
        sLaplacian5x5Fp16ToFp16Implementation(source, &output, fptr->sliceWidth);
    #endif
    }
}
