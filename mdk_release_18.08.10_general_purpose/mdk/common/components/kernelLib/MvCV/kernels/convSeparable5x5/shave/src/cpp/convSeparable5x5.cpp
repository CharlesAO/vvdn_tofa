#include "convSeparable5x5.h"

void mvcvConvSeparable5x5(u8** out, u8** in, float conv[3], u32 inWidth)
{
    int i;
    u8* lines[5];
    float sum = 0.0f;

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];

    //Go on the whole line
    for (i = 0; i < (int)inWidth; i++){
		sum = ((float)(lines[0][i-2] + lines[4][i-2]) * conv[0] + (float)(lines[1][i-2] + lines[3][i-2]) * conv[1] + (float)lines[2][i-2] * conv[2] +
			   (float)(lines[0][i+2] + lines[4][i+2]) * conv[0] + (float)(lines[1][i+2] + lines[3][i+2]) * conv[1] + (float)lines[2][i+2] * conv[2]) * conv[0] +
			  ((float)(lines[0][i-1] + lines[4][i-1]) * conv[0] + (float)(lines[1][i-1] + lines[3][i-1]) * conv[1] + (float)lines[2][i-1] * conv[2] +
			   (float)(lines[0][i+1] + lines[4][i+1]) * conv[0] + (float)(lines[1][i+1] + lines[3][i+1]) * conv[1] + (float)lines[2][i+1] * conv[2]) * conv[1] +
			  ((float)(lines[0][i  ] + lines[4][i  ]) * conv[0] + (float)(lines[1][i  ] + lines[3][i  ]) * conv[1] + (float)lines[2][i  ] * conv[2]) * conv[2];


        if (sum >= 255)
            sum = 255.0f;
        if (sum <= 0)
            sum = 0.0f;
        out[0][i] = (u8)(sum);
    }
    return;
}
