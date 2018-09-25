#include "integralImageSquareSumM2.h"

void mvcvIntegralimage_sqsum_u32_M2(u32* out, u8* in, u32 runNr, u32** prevPointer, u32 width)
{
    if (0 == runNr) 
    {
        unsigned int x;
        unsigned int onLineSum = 0;;
        for(x = 0; x < width; x++)
        {
            onLineSum += in[x] * in[x];
            out[x] = onLineSum;
        }
        *prevPointer = out;
    }
    else
    {
        unsigned int x;
        unsigned int onLineSum = 0;;
        for(x = 0; x < width; x++)
        {
            onLineSum += in[x] * in[x];
            out[x] = (*prevPointer)[x] + onLineSum;
        }
        *prevPointer = out;
    }
	return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvIntegralimage_sqsum_u32_M2_opt(u32* out, u8* in, u32 runNr, u32** prevPointer, u32 width)
{
    if (0 == runNr) 
    {
        unsigned int x;
        unsigned int onLineSum = 0;

        uchar4* inPtr = (uchar4*)in;
        in+=4;
        uchar4 input = *inPtr;
        uint4 inputU32 = mvuConvert_uint4(input);
        uint4 inputSq = inputU32 * inputU32;



        for(x = 0; x < width; x+=4)
        {
            inPtr = (uchar4*)in;
            in+=4;
            onLineSum += inputSq[0];
            out[x] = onLineSum;
            onLineSum += inputSq[1];
            out[x+1] = onLineSum;

            input = *inPtr;            

            onLineSum += inputSq[2];
            out[x+2] = onLineSum;
            onLineSum += inputSq[3];
            out[x+3] = onLineSum;          
            
            inputU32 = mvuConvert_uint4(input);
            inputSq = inputU32 * inputU32;
        }
        *prevPointer = out;
    }
    else
    {
        unsigned int x;  
        unsigned int onLineSum = 0;

        uchar4* inPtr = (uchar4*)in;
        in+=4;
        uchar4 input = *inPtr;
        uint4 inputU32 = mvuConvert_uint4(input);
        uint4 inputSq = inputU32 * inputU32;

        for(x = 0; x < width; x+=4)
        {
            inPtr = (uchar4*)in;
            in+=4;
            onLineSum += inputSq[0];
            out[x] = (*prevPointer)[x] + onLineSum;
            onLineSum += inputSq[1];
            out[x+1] = (*prevPointer)[x+1] + onLineSum;

            input = *inPtr;

            onLineSum += inputSq[2];
            out[x+2] = (*prevPointer)[x+2] + onLineSum;
            onLineSum += inputSq[3];
            out[x+3] = (*prevPointer)[x+3] + onLineSum;            
            
            inputU32 = mvuConvert_uint4(input);
            inputSq = inputU32 * inputU32;

        }
        *prevPointer = out;
    }
    return;
}
#endif //MOVICOMPILE_OPTIMIZED