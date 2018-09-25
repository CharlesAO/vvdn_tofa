#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/cvtColorYUV422ToRGB/cvtColorYUV422ToRGB.h>

#ifdef SIPP_USE_MVCV
void mvcvCvtColorKernelYUV422ToRGB_asm(UInt8** input, UInt8** rOut, UInt8** gOut, UInt8** bOut, UInt32 width);
#else
void cvtColorKernelYUV422ToRGB        (UInt8** input, UInt8** rOut, UInt8** gOut, UInt8** bOut, UInt32 width)
{
    int j;
    int r, g, b, y1, y2, u, v;
    int out_index = 0;
    UInt8*  in = input[0];
    UInt8* outR = rOut[0];
    UInt8* outG = gOut[0];
    UInt8* outB = bOut[0];

    for(j = 0; j < width * 2; j+=4)
    {
        y1 = in[j];
        u  = in[j + 1] - 128;
        y2 = in[j + 2];
        v  = in[j + 3] - 128;

        r = y1 + (int)(1.402f * v);
        g = y1 - (int)(0.344f * u + 0.714f * v);
        b = y1 + (int)(1.772f * u);

        outR[out_index] = (UInt8) (r>255 ? 255 : r<0 ? 0 : r);
        outG[out_index] = (UInt8) (g>255 ? 255 : g<0 ? 0 : g);
        outB[out_index] = (UInt8) (b>255 ? 255 : b<0 ? 0 : b);
        out_index++;

        r = y2 + (int)(1.402f * v);
        g = y2 - (int)(0.344f * u + 0.714f * v);
        b = y2 + (int)(1.772f * u);

        outR[out_index] = (UInt8) (r>255 ? 255 : r<0 ? 0 : r);
        outG[out_index] = (UInt8) (g>255 ? 255 : g<0 ? 0 : g);
        outB[out_index] = (UInt8) (b>255 ? 255 : b<0 ? 0 : b);
        out_index++;
    }
}
#endif

void svuCvtColorYUV422ToRGB(SippFilter *fptr)
{
    UInt8 *outputR;
    UInt8 *outputG;
    UInt8 *outputB;
    UInt8 *input;

    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the input line
        input = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);

        //the 3 output lines
        outputR = (UInt8 *)getOutPtr(fptr, iterNum, 0);
        outputG = (UInt8 *)getOutPtr(fptr, iterNum, 1);
        outputB = (UInt8 *)getOutPtr(fptr, iterNum, 2);

    #ifdef SIPP_USE_MVCV
        mvcvCvtColorKernelYUV422ToRGB_asm(&input, &outputR, &outputG, &outputB, fptr->sliceWidth);
    #else
        cvtColorKernelYUV422ToRGB(&input, &outputR, &outputG, &outputB, fptr->sliceWidth);
    #endif
    }
}
