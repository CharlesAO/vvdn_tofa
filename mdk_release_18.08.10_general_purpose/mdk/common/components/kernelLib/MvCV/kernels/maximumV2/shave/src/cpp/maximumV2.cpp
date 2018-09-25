#include "maximumV2.h"

void mvcvMaximumV2(half** in, half** out, u32 width)
{
    unsigned int i;
	half max;


    //Go on the whole line
    for (i = 0; i < width; i++)
    {
        max = in[0][i];
        if(in[1][i]>max) max = in[1][i];

        out[0][i] = max;
    }
    return;
}
