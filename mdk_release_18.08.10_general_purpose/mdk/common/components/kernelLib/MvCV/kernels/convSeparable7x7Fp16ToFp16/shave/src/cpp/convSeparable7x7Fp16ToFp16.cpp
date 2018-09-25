#include "convSeparable7x7Fp16ToFp16.h"

void mvcvConvSeparable7x7Fp16ToFp16(half** out, half** in, half conv[4], u32 inWidth)
{
    int i;
    half* lines[7];
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
		sum = ((float)(lines[0][i-3] + lines[6][i-3]) * (float)conv[0] + (float)(lines[1][i-3] + lines[5][i-3]) * (float)conv[1] + (float)(lines[2][i-3] + lines[4][i-3]) * (float)conv[2] + (float)lines[3][i-3] * (float)conv[3] +
			   (float)(lines[0][i+3] + lines[6][i+3]) * (float)conv[0] + (float)(lines[1][i+3] + lines[5][i+3]) * (float)conv[1] + (float)(lines[2][i+3] + lines[4][i+3]) * (float)conv[2] + (float)lines[3][i+3] * (float)conv[3]) * (float)conv[0] +
			  ((float)(lines[0][i-2] + lines[6][i-2]) * (float)conv[0] + (float)(lines[1][i-2] + lines[5][i-2]) * (float)conv[1] + (float)(lines[2][i-2] + lines[4][i-2]) * (float)conv[2] + (float)lines[3][i-2] * (float)conv[3] +
			   (float)(lines[0][i+2] + lines[6][i+2]) * (float)conv[0] + (float)(lines[1][i+2] + lines[5][i+2]) * (float)conv[1] + (float)(lines[2][i+2] + lines[4][i+2]) * (float)conv[2] + (float)lines[3][i+2] * (float)conv[3]) * (float)conv[1] +
			  ((float)(lines[0][i-1] + lines[6][i-1]) * (float)conv[0] + (float)(lines[1][i-1] + lines[5][i-1]) * (float)conv[1] + (float)(lines[2][i-1] + lines[4][i-1]) * (float)conv[2] + (float)lines[3][i-1] * (float)conv[3] +
			   (float)(lines[0][i+1] + lines[6][i+1]) * (float)conv[0] + (float)(lines[1][i+1] + lines[5][i+1]) * (float)conv[1] + (float)(lines[2][i+1] + lines[4][i+1]) * (float)conv[2] + (float)lines[3][i+1] * (float)conv[3]) * (float)conv[2] +
			  ((float)(lines[0][i  ] + lines[6][i  ]) * (float)conv[0] + (float)(lines[1][i  ] + lines[5][i  ]) * (float)conv[1] + (float)(lines[2][i  ] + lines[4][i  ]) * (float)conv[2] + (float)lines[3][i  ] * (float)conv[3]) * (float)conv[3];

        out[0][i] = (half)(sum);
    }
    return;
}
