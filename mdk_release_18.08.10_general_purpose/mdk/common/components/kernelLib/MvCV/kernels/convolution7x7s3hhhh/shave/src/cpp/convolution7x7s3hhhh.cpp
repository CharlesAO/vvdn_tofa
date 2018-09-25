#include "convolution7x7s3hhhh.h"

void mvcvConvolution7x7s3hhhh(half** in, half** out, half conv[49], u32 inWidth)
{
    int x, y;
    unsigned int i;
    half* lines[7];
    half sum;

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];
    lines[5] = in[5];
    lines[6] = in[6];

    //Go on the whole line
    for (i = 0; i < inWidth / 3; i++)
    {
        sum = 0.0;
        for (x = 0; x < 7; x++)
        {
            for (y = 0; y < 7; y++)
            {
                sum += (half)(lines[x][y - 3] * conv[x * 7 + y]);
            }

            lines[x] += 3;
        }

        out[0][i] = sum;
    }
    return;
}
