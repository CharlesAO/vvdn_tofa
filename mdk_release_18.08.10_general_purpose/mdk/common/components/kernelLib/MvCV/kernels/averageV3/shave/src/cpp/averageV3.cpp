#include "averageV3.h"

void mvcvAverageV3(half** in, half** out, u32 width)
{
    unsigned int i;

    //Go on the whole line
    for (i = 0; i < width; i++)
    {
		out[0][i]  = in[0][i] + in[1][i];
		out[0][i] += in[2][i];
		
		out[0][i] *= (half)0.33333333333;
    }
    return;
}
