#include "convolution3x3s4hhhh.h"

void mvcvConvolution3x3s4hhhh(half** in, half** out, half conv[9], u32 inWidth)
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
  for (i = 0; i < inWidth/4; i++){
        sum = 0.0f;
        for (x = 0; x < 3; x++){
            for (y = 0; y < 3; y++){
                sum += (half)(lines[x][y - 1] * conv[x * 3 + y]);
            }
            lines[x]+=4;
        }

      
        out[0][i] = (half)(sum);
    }
    return;

}
