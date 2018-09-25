#include <censusTransform7x7.h>
#include <stdio.h>

void mvcvCensusTransform7x7(u8 **in, u64 *out, u32 width)
{
    for (unsigned int i = 0; i < width; i++)
    {
        out[i] = 0;
        for (int line = 0; line < 7; line++)
            for (int col = -3; col <= 3; col++)
            {
                out[i] *= 2;
                if (*(in[line] + i + col) < *(in[3] + i))
                {
                    out[i] += 1;
                } // set 1
            } // finish Census Tranform for one pixel
    } // finish Census Transform for all pixels
}
