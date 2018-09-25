#include "thresholdDisparityFp16.h"

void mvcvThresholdDisparityFp16(u8* input, half* output, u32* countThresPixels, u32 width, u32 threshold, half setValue)
{ 
    for(u32 col = 0; col < width; col++)
    {
        if (input[col/2] > threshold)
        {
            output[col] = (half)setValue;
            (*countThresPixels)++;
        }
    }
}

