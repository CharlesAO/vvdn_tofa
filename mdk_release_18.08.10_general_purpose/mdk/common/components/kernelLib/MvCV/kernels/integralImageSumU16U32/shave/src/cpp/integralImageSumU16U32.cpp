#include "integralImageSumU16U32.h"

void mvcvIntegralImageSumU16U32(unsigned int* out, unsigned short* in, unsigned int runNr, unsigned int **previsionOutLnPointer, unsigned int width)
{
    if (0 == runNr) 
    {
        unsigned int x;
        unsigned int onLineSum = 0;;
        for(x = 0; x < width; x++)
        {
            onLineSum += in[x];
            out[x] = onLineSum;
        }
        *previsionOutLnPointer = out;
    }
    else
    {
        unsigned int x;
        unsigned int onLineSum = 0;;
        for(x = 0; x < width; x++)
        {
            onLineSum += in[x];
            out[x] = (*previsionOutLnPointer)[x] + onLineSum;
        }
        *previsionOutLnPointer = out;
    }
    return;
}
