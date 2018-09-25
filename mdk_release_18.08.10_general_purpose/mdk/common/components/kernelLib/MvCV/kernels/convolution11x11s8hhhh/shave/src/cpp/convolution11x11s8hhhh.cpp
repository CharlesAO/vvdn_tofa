#include "convolution11x11s8hhhh.h"

void mvcvConvolution11x11s8hhhh(half** in, half** out, half conv[121], u32 inWidth)
{
    int x, y;
    unsigned int i;
    half* lines[11];
    half sum;

    //Initialize lines pointers
    lines[0]  = in[0];
    lines[1]  = in[1];
    lines[2]  = in[2];
    lines[3]  = in[3];
    lines[4]  = in[4];
    lines[5]  = in[5];
    lines[6]  = in[6];
    lines[7]  = in[7];
    lines[8]  = in[8];
    lines[9]  = in[9];
    lines[10] = in[10];

    //Go on the whole line
    for (i = 0; i < inWidth/8; i++){
        sum = 0.0;
        for (x = 0; x < 11; x++)
        {
            for (y = 0; y < 11; y++)
            {
                sum += (float)(lines[x][y - 5] * conv[x * 11 + y]);
            }
            lines[x]=lines[x]+8;
        }
        // if (sum >= 255)
            // sum = 255.0;
        // if (sum <= 0)
            // sum = 0.0;
        out[0][i] = sum;

    }
    return;
}
