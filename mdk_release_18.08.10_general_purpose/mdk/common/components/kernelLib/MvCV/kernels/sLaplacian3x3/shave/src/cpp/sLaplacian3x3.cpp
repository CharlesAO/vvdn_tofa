#include "sLaplacian3x3.h"

void mvcvSLaplacian3x3(u8** in, u8** out, u32 inWidth)
{
    int i,x,y;
    u8* lines[3];
	float sum;
	int lapl[9]={ 0, -1, 0, -1, 4, -1, 0, -1, 0};
		
    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
	
	//Go on the whole line
    for (i = 0; i < (int)inWidth; i++){
	
		sum = 0.0f;
        for (x = 0; x < 3; x++){
            for (y = -1; y < 2; y++){
                sum += (float)(lines[x][y] * lapl[x * 3 + y + 1]);
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
