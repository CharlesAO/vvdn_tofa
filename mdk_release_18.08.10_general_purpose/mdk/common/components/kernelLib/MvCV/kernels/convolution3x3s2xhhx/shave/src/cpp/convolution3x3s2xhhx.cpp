#include "convolution3x3s2xhhx.h"

void mvcvConvolution3x3s2xhhx(u8** in, u8** out, half conv[9], u32 inWidth)
{
    int x,y;
	unsigned int i;
    u8* lines[3];
    half sum;

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];

    //Go on the whole line
    for (i = 0; i < inWidth/2; i++){
        sum = 0.0f;
        for (x = 0; x < 3; x++){
            for (y = 0; y < 3; y++){
                sum += (half)(lines[x][y - 1] * conv[x * 3 + y]);
            }
            lines[x]+=2;
        }

        if (sum >= (half)255)
            sum = 255.0f;
        if (sum <= (half)0)
            sum = 0.0f;
        out[0][i] = (u8)(sum);
    }
    return;
}


