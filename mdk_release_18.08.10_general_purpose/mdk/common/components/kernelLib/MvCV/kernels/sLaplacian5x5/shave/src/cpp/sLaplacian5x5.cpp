#include "sLaplacian5x5.h"

void mvcvSLaplacian5x5(u8** in, u8** out, u32 inWidth)
{
    int i,x,y;
    u8* lines[5];
	float sum;
	int lapl[25]={ 0, 0, -1, 0, 0, 0, -1, -2, -1, 0, -1, -2, 16, -2, -1, 0, -1, -2, -1, 0, 0, 0, -1, 0, 0};
		
    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];
	
	//Go on the whole line
    for (i = 0; i < (int)inWidth; i++){
	
		sum = 0.0f;
        for (x = 0; x < 5; x++){
            for (y = -2; y < 3; y++){
                sum += (float)(lines[x][y] * lapl[x * 5 + y + 2]);
            }
            lines[x]++;
        }

        if (sum >= 255)
            sum = 255.0f;
        if (sum <= 0)
            sum = 0.0f;
        out[0][i] = (u8)(sum);
    }
    return;
}
