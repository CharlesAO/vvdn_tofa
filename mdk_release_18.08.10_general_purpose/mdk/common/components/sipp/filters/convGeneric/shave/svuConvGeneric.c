#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/convGeneric/convGeneric.h>

#ifdef SIPP_USE_MVCV
void mvcvConvolution_asm(UInt8** in, UInt8** out, UInt32 kernelSize, half* conv, UInt32 inWidth);
#else
void Convolution        (UInt8** in, UInt8** out, UInt32 kernelSize, half* conv, UInt32 inWidth)
{
    unsigned int i, x, y;
    UInt8* lines[15];
    float sum = 0.0;

    //Initialize lines pointers
    for (x = 0; x < kernelSize; x++)
    {
        lines[x] = *(in + x);
    }

    //Go on the whole line
    for (i = 0; i < inWidth; i++){

        sum = 0.0;

        for (x = 0; x < kernelSize; x++)
        {
            for (y = 0; y < kernelSize; y++)
            {
                sum += (float)(lines[x][y] * conv[x * kernelSize + y]);
            }
            lines[x]++;
        }

        if (sum >= 255)
            sum = 255.0;
        if (sum <= 0)
            sum = 0.0;
        out[0][i] = (UInt8)(sum);
    }
}
#endif

void svuConvGeneric(SippFilter *fptr)
{
    UInt8 *output;
    UInt8 *iline[15];
    ConvGenericParam *param = (ConvGenericParam*)fptr->params;
    UInt32 i;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        //the FILTER_SIZE input lines
        for(i = 0; i < param->filterSize; i++)
        {
            iline[i]=(UInt8 *)getInPtr(fptr, 0, iterNum, i, 0);
        }
        //the output line
        output = (UInt8 *)getOutPtr(fptr, iterNum, 0);

    #ifdef SIPP_USE_MVCV
        mvcvConvolution_asm(iline, &output, param->filterSize, (half*)param->cMat, fptr->sliceWidth);
    #else
        Convolution(iline, &output, param->filterSize, (half*)param->cMat, fptr->sliceWidth);
    #endif
    }
}
