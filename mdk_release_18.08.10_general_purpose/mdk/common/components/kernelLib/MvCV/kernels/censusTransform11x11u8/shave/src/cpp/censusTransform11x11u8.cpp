#include "censusTransform11x11u8.h"
#include <stdio.h>

void mvcvCensusTransform11x11u8(u8 **in, u8 *out, u32 width)
{
    int bit, k;

    for(unsigned int i = 0; i < width*16; i++)
        out[i] = 0;

    for (unsigned int i = 0; i < width; i++)
    {      
        k = 0;
        bit = 0;
        for (int line = 0; line < 11; line++)
            for (int col = -5; col <= 5; col++)
                {
                    if(bit == 8)
                    {
                        bit = 0;
                        k++;     
                    }
                    out[16*i+k] *= 2;
                    if (*(in[line] + i + col) < *(in[5] + i))
                    {
                        out[16*i+k] += 1; 
                    } 

                    bit ++;
                } 
    }
}