#include "convolution7x7s4hhhh.h"

void mvcvConvolution7x7s4hhhh(half** in, half** out, half *conv, unsigned int inWidth)
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
    for (i = 0; i < inWidth/4; i++){
        sum = 0.0;
        for (x = 0; x < 7; x++)
        {
            for (y = -3; y <= 3; y++)
            {
                // convert value from u8 to u8f and after to fp16
                half val =   (half)((float)lines[x][y]);

                sum += val * conv[x * 7 + y + 3];
            }
            lines[x]+=4;
        }
        out[0][i] = (half)(sum);

    }
    return;
}
