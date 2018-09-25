#include "convolution3x3Fp16ToFp16.h"

void mvcvConvolution3x3Fp16ToFp16(half** in, half** out, half conv[9], u32 inWidth)
{
    int x,y;
	unsigned int i;
    half* lines[3];
    half sum;

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];

    //Go on the whole line
    for (i = 0; i < inWidth; i++){
        sum = 0.0f;
        for (x = 0; x < 3; x++){
            for (y = 0; y < 3; y++){
				half val =   (half)(lines[x][y-1]);
                sum += val * conv[x * 3 + y];
            }
            lines[x]++;
        }

        out[0][i] = sum;
    }
    return;
}
