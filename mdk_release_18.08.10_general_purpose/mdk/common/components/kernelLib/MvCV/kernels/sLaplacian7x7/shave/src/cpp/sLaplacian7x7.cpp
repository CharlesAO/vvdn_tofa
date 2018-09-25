#include "sLaplacian7x7.h"

void mvcvSLaplacian7x7(u8** in, u8** out, u32 inWidth)
{
    int i,x,y;
    u8* lines[7];
	float sum;
	int lapl[49]={ -10, -5, -2, -1, -2, -5, -10, -5, 0, 3, 4, 3, 0, -5,
				    -2, 3, 6, 7, 6, 3, -2, -1, 4, 7, 8, 7, 4, -1,
				    -2, 3, 6, 7, 6, 3, -2, -5, 0, 3, 4, 3, 0, -5,
					-10, -5, -2, -1, -2, -5, -10 };
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
	
		sum = 0.0f;
        for (x = 0; x < 7; x++){
            for (y = -3; y < 4; y++){
                sum += (float)(lines[x][y] * lapl[x * 7 + y + 3]);
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
