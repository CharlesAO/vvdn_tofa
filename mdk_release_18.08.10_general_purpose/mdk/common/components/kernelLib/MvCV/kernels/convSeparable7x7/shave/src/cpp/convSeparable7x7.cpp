#include "convSeparable7x7.h"

void mvcvConvSeparable7x7(u8** out, u8** in, float conv[4], u32 inWidth)
{
    int i;
    u8* lines[7];
    float sum = 0.0f;

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
		sum = ((float)(lines[0][i-3] + lines[6][i-3]) * conv[0] + (float)(lines[1][i-3] + lines[5][i-3]) * conv[1] + (float)(lines[2][i-3] + lines[4][i-3]) * conv[2] + (float)lines[3][i-3] * conv[3] +
			   (float)(lines[0][i+3] + lines[6][i+3]) * conv[0] + (float)(lines[1][i+3] + lines[5][i+3]) * conv[1] + (float)(lines[2][i+3] + lines[4][i+3]) * conv[2] + (float)lines[3][i+3] * conv[3]) * conv[0] +
			  ((float)(lines[0][i-2] + lines[6][i-2]) * conv[0] + (float)(lines[1][i-2] + lines[5][i-2]) * conv[1] + (float)(lines[2][i-2] + lines[4][i-2]) * conv[2] + (float)lines[3][i-2] * conv[3] +
			   (float)(lines[0][i+2] + lines[6][i+2]) * conv[0] + (float)(lines[1][i+2] + lines[5][i+2]) * conv[1] + (float)(lines[2][i+2] + lines[4][i+2]) * conv[2] + (float)lines[3][i+2] * conv[3]) * conv[1] +
			  ((float)(lines[0][i-1] + lines[6][i-1]) * conv[0] + (float)(lines[1][i-1] + lines[5][i-1]) * conv[1] + (float)(lines[2][i-1] + lines[4][i-1]) * conv[2] + (float)lines[3][i-1] * conv[3] +
			   (float)(lines[0][i+1] + lines[6][i+1]) * conv[0] + (float)(lines[1][i+1] + lines[5][i+1]) * conv[1] + (float)(lines[2][i+1] + lines[4][i+1]) * conv[2] + (float)lines[3][i+1] * conv[3]) * conv[2] +
			  ((float)(lines[0][i  ] + lines[6][i  ]) * conv[0] + (float)(lines[1][i  ] + lines[5][i  ]) * conv[1] + (float)(lines[2][i  ] + lines[4][i  ]) * conv[2] + (float)lines[3][i  ] * conv[3]) * conv[3];

        if (sum >= 255)
            sum = 255.0f;
        if (sum <= 0)
            sum = 0.0f;
        out[0][i] = (u8)(sum);
    }
    return;
}
