#include "convolution5x5s2hhhh.h"

void mvcvConvolution5x5s2hhhh(half** in, half** out, half conv[25], u32 inWidth)
{
    int x, y;
    unsigned int i;
    half* lines[5];
    half sum;


    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];

    //Go on the whole line
    for (i = 0; i < inWidth / 2; i++){
        sum = 0.0;
        for (x = 0; x < 5; x++)
        {
            for (y = 0; y < 5; y++)
            {
                sum += lines[x][y - 2] * conv[x * 5 + y];
            }
            lines[x] += 2;
        }

        out[0][i] = sum;
    }
}
