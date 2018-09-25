#include "censusTransform5x5.h"
#include <stdio.h>

void mvcvCensusTransform5x5(u8 **in, u32 *out, u32 width)
{
    for (unsigned int i = 0; i < width; i++)
    {
		out[i] = 0;
		for (int line = 0; line < 5; line++)
            for (int col = -2; col <= 2; col++)
            {
                out[i] *= 2;
                if (*(in[line] + i + col) < *(in[2] + i))
                {
                    out[i] += 1;
                } // set 1
            } // finish Census Tranform for one pixel

    } // finish Census Transform for all pixels
}
