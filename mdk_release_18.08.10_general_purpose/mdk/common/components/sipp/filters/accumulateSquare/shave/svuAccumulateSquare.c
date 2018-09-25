#include <sipp.h>
#include <sippShaveMacros.h>
#include <filters/accumulateSquare/accumulateSquare.h>

///Adds the square of the source image to the accumulator.
///@param srcAddr  The input image, 1- or 3-channel, 8-bit or 32-bit floating point
///@param destAddr The accumulator image with the same number of channels as input image, 32-bit or 64-bit floating-point
///@param maskAddr Optional operation mask
///@param width    Width of input image
///@param height   Number of lines of input images (defaulted to one line)
#ifdef SIPP_USE_MVCV
void mvcvAccumulateSquare_asm(UInt8** srcAddr, UInt8** maskAddr, float** destAddr, UInt32 width, UInt32 height);
#else
void AccumulateSquare        (UInt8** srcAddr, UInt8** maskAddr, float** destAddr, UInt32 width, UInt32 height)
{
    UInt32 i,j;
    UInt8* src;
    UInt8* mask;
    float* dest;

    src = *srcAddr;
    mask = *maskAddr;
    dest = *destAddr;

    for (i = 0; i < height; i++){
        for (j = 0;j < width; j++){
            if(mask[j + i * width]){
                dest[j + i * width] =  dest[j + i * width] + src[j + i * width] * src[j + i * width];
            }
        }
    }
}
#endif

void svuAccumulateSquare(SippFilter *fptr)
{
    UInt8 *iline[2];
    float *output;
    float *inLFloat;
    UInt32 i;
    UInt32 numLinesThisIter = sippFilterGetLinesThisIter (fptr);
    UInt32 iterNum;

    for (iterNum = 0; iterNum < numLinesThisIter; iterNum++)
    {
        inLFloat = (float *)getInPtr(fptr, 2, iterNum, 0, 0);

        //the 2 input lines
        iline[0]=(UInt8 *)getInPtr(fptr, 0, iterNum, 0, 0);
        iline[1]=(UInt8 *)getInPtr(fptr, 1, iterNum, 0, 0);

        //the output line
        output = (float*)getOutPtr(fptr, iterNum, 0);

        for(i=0; i<fptr->sliceWidth; i++)
        {
            output[i] = inLFloat[i];
        }

    #ifdef SIPP_USE_MVCV
        mvcvAccumulateSquare_asm(&iline[0], &iline[1], &output, fptr->sliceWidth, 1);
    #else
        AccumulateSquare(&iline[0], &iline[1], &output, fptr->sliceWidth, 1);
    #endif
    }
}
