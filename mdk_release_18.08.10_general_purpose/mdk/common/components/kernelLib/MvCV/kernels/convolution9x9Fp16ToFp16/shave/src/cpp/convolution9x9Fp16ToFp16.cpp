#include "convolution9x9Fp16ToFp16.h"

void mvcvConvolution9x9Fp16ToFp16(half** in, half** out, half conv[81], u32 inWidth)
{
    int x, y;
    unsigned int i;
    half* lines[9];
    half sum;
	
    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];
    lines[5] = in[5];
    lines[6] = in[6];
    lines[7] = in[7];
    lines[8] = in[8];

    //Go on the whole line
    for (i = 0; i < inWidth; i++){
        sum = 0.0;
        for (x = 0; x < 9; x++)
        {
            for (y = 0; y < 9; y++)
            {
                sum += (half)(lines[x][y - 4] * conv[x * 9 + y]);
            }
            lines[x]++;
        }
       /* if (sum >= 255)
            sum = 255.0;
        if (sum <= 0)
            sum = 0.0; */
        out[0][i] = sum;

    }
    return;
}
