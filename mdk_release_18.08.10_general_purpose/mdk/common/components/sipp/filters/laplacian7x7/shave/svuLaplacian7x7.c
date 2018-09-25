#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/laplacian7x7/laplacian7x7.h>

/// Laplacian filter  - applies a Laplacian filter with custom size (see http://en.wikipedia.org/wiki/Discrete_Laplace_operator )
/// @param[in] in     - array of pointers to input lines
/// @param[out] out   - pointer to output line
/// @param[in] width  - width of input line
#ifdef SIPP_USE_MVCV
void mvcvSLaplacian7x7_asm(UInt8** in, UInt8** out, UInt32 inWidth);
#else
void sLaplacian7x7Implementation(UInt8** in, UInt8** out, UInt32 inWidth)
{
    UInt32 i,x,y;
    UInt8* lines[7];
    float sum;
    int lapl[49]={ -10, -5, -2, -1, -2, -5, -10, -5, 0, 3, 4, 3, 0, -5,
                    -2, 3, 6, 7, 6, 3, -2, -1, 4, 7, 8, 7, 4, -1,
                    -2, 3, 6, 7, 6, 3, -2, -5, 0, 3, 4, 3, 0, -5,
                    -10, -5, -2, -1, -2, -5, -10 };
    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];
    lines[5] = in[5];
    lines[6] = in[6];

    //Go on the whole line
    for (i = 0; i < inWidth; i++)
    {
        sum = 0.0f;
        for (x = 0; x < 7; x++){
            for (y = -3; y < 4; y++){
                sum += (float)(lines[x][y] * lapl[x * 7 + y + 3]);
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

void svuLaplacian7x7(SippFilter *fptr)
{

    UInt8 *output;
    UInt8 *source[7];

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
        source[5] =(UInt8 *)getInPtr(fptr,0,iterNum,5,0);    //source[5] =(UInt8 *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][5], svuNo);
        source[6] =(UInt8 *)getInPtr(fptr,0,iterNum,6,0);    //source[6] =(UInt8 *)WRESOLVE((void*)fptr->dbLinesIn[0][runNo&1][6], svuNo);

        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);   //output = (UInt8 *)WRESOLVE((void*)fptr->dbLineOut[runNo&1], svuNo);

    #ifdef SIPP_USE_MVCV
        mvcvSLaplacian7x7_asm(source, &output, fptr->sliceWidth);
    #else
        sLaplacian7x7Implementation(source, &output, fptr->sliceWidth);
    #endif
    }
}
