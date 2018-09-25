#include "sLaplacian7x7Fp16ToFp16.h"

void mvcvSLaplacian7x7Fp16ToFp16(half** in, half** out, u32 inWidth)
{
    int i,x,y;
    half* lines[7];
	half sum;
	half lapl[49]={-10,-5,-2,-1,-2,-5,-10,-5, 0, 3, 4, 3,  0, -5, -2, 3,  6,  7,  6,  3, -2,-1,   4,  7,  8,  7,  4, -1, -2,   3,  6,  7,  6,  3, -2,-5,   0,  3,  4,  3,  0, -5,-10, -5, -2, -1, -2, -5, -10 };
    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];
    lines[5] = in[5];
    lines[6] = in[6];
	
	//Go on the whole line
    for (i = 0; i < (int)inWidth; i++){
	
		sum = 0.0;
        for (x = 0; x < 7; x++){
            for (y = -3; y < 4; y++){
                sum += lines[x][y] * lapl[x * 7 + y + 3];
            }
            lines[x]++;
        }

        /*if (sum >= 255)
            sum = 255.0f;
        if (sum <= 0)
            sum = 0.0f;*/
        out[0][i] = sum;
    }
    return;
}
