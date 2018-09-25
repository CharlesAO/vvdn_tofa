#include "sLaplacian3x3Fp16ToFp16.h"
#include <stdio.h>

void mvcvSLaplacian3x3Fp16ToFp16(half** in, half** out, u32 inWidth)
{


 int i,x,y;
    half* lines[3];
	half sum;
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
                sum += (half)(lines[x][y] * lapl[x * 3 + y + 1]);
            }
            lines[x]++;
        }

      /*  if (sum >= 255.0f)
            sum = 255.0f;
        if (sum <= 0.0f)
            sum = 0.0f; */
        out[0][i] =(half)(sum);
    }
	
    return;
}
