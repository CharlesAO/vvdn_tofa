#include "maximumV9.h"

void mvcvMaximumV9(half** in, half** out, u32 width)
{
    unsigned int i;
	half max1;

    //Go on the whole line
    for (i = 0; i < width; i++)
    {
		max1 = in[0][i];                  
		if(in[1][i]>max1) max1 = in[1][i];
		if(in[2][i]>max1) max1 = in[2][i];
		if(in[3][i]>max1) max1 = in[3][i];
		if(in[4][i]>max1) max1 = in[4][i];
		if(in[5][i]>max1) max1 = in[5][i];
		if(in[6][i]>max1) max1 = in[6][i];
		if(in[7][i]>max1) max1 = in[7][i];
		if(in[8][i]>max1) max1 = in[8][i];
		
		out[0][i] = max1;
    }
    return;
}
