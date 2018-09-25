#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/cvtColorYUVToRGB/cvtColorYUVToRGB.h>

#ifdef SIPP_USE_MVCV
void mvcvCvtColorKernelYUVToRGB_asm(UInt8* yIn, UInt8* uIn, UInt8* vIn, UInt8* out, UInt32 width);
#else

void cvtColorKernelYUVToRGB(UInt8* yIn, UInt8* uIn, UInt8* vIn, UInt8* out, UInt32 width)
{
    UInt32 i, j;
    UInt32 uv_idx = 0;
    UInt32 out_idx_1 = 0;

    int y[4], u, v, r, g, b;

    i = 0;
    while(i < width)
    {
        y[0] = yIn[i];
        y[1] = yIn[i + 1];

        u = uIn[uv_idx] - 128;
        v = vIn[uv_idx] - 128;
        uv_idx++;

        for(j = 0; j < 2; j++)
        {
            r = y[j] + (int)(1.402f*v);
            g = y[j] - (int)(0.344f*u +0.714f*v);
            b = y[j] + (int)(1.772f*u);

            r = r>255? 255 : r<0 ? 0 : r;
            g = g>255? 255 : g<0 ? 0 : g;
            b = b>255? 255 : b<0 ? 0 : b;

                out[out_idx_1++] = (UInt8)r;
                out[out_idx_1++] = (UInt8)g;
                out[out_idx_1++] = (UInt8)b;
        }
        i += 2;
    }
}
#endif
void svuCvtColorYUVToRGB(SippFilter *fptr)
{
    UInt8 *inputY;
    UInt8 *inputU;
    UInt8 *inputV;
    UInt8 *output;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {

        //the 3 input lines
        inputY = (UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        inputU = (UInt8 *)getInPtr(fptr, 1, iterNum, 0, 0);
        inputV = (UInt8 *)getInPtr(fptr, 1, iterNum, 0, 1);

        //the output lines
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        // The asm for this filter is not Sipp compatible
        // It generates 2 output lines, Sipp filters have to have only 1 output line
        mvcvCvtColorKernelYUVToRGB_asm(inputY, inputU, inputV, output, (fptr->sliceWidth/3));

    #else
        /// The filter is created with the outputWidth == 3*inputWidth
        /// and fptr->sliceWidth = outputWidth so we transmit it with /3 to do the good processing
        cvtColorKernelYUVToRGB(inputY, inputU, inputV, output, (fptr->sliceWidth/3));
    #endif
    }
}
