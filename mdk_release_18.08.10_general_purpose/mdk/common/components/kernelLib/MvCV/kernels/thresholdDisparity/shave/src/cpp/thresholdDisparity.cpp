#include "thresholdDisparity.h"

void mvcvThresholdDisparity(u8* input, u8* output, u32* countThresPixels, u32 width, u32 threshold, u32 setValue)
{ 
    for(u32 col = 0; col < width; col++)
    {
        if (input[col/2] > threshold)
        {
            output[col] = setValue;
            (*countThresPixels)++;
        }
    }
}