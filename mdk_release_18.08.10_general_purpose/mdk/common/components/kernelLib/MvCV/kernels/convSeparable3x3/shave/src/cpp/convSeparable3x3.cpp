#include "convSeparable3x3.h"

void mvcvConvSeparable3x3(u8* out, u8** in, float conv[2], u32 inWidth)
{
    int i;
    u8* lines[3];
    float sum = 0.0f;

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];

    //Go on the whole line
    for (i = 0; i < (int)inWidth; i++){
		sum = (((float)(lines[0][i-1] +  lines[2][i-1]) * conv[0] + (float)lines[1][i-1] * conv[1]) +
			   ((float)(lines[0][i+1] +  lines[2][i+1]) * conv[0] + (float)lines[1][i+1] * conv[1])) * conv[0] +
			   ((float)(lines[0][i  ] +  lines[2][i  ]) * conv[0] +	(float)lines[1][i]    * conv[1]) * conv[1];
        if (sum >= 255)
            sum = 255.0f;
        if (sum <= 0)
            sum = 0.0f;
        out[i] = (u8)(sum);
    }
    return;
}
