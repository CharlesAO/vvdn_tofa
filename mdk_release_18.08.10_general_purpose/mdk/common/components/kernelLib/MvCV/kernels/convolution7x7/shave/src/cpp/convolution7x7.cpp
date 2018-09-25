#include "convolution7x7.h"

void mvcvConvolution7x7(u8** in, u8** out, half *conv, unsigned int inWidth)
{
    s32 x, y;
    u32 i;
    u8* lines[7];
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
    for (i = 0; i < inWidth; i++){
        sum = 0.0;
        for (x = 0; x < 7; x++)
        {
            for (y = -3; y <= 3; y++)
            {
                // convert value from u8 to u8f and after to fp16
                half val =   (half)((float)lines[x][y]/256.0f);
                sum += val * conv[x * 7 + y + 3];
            }
            lines[x]++;
        }
        if (sum >= (half)1.0f)
            sum = (half)0.9995117f;
        if (sum <= (half)0.0f)
            sum = (half)0.0f;
        out[0][i] = (u8)(sum * 256);

    }
    return;
}
