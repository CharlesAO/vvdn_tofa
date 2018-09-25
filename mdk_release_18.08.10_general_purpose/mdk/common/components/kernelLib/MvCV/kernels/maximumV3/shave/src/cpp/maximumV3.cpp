#include "maximumV3.h"

void mvcvMaximumV3(half** in, half** out, u32 width)
{
    unsigned int i;
	half max1;

    //Go on the whole line
    for (i = 0; i < width; i++)
    {
		max1 = in[0][i];                  
		if(in[1][i]>max1) max1 = in[1][i];
		if(in[2][i]>max1) max1 = in[2][i];
		
		out[0][i] = max1;
    }
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvMaximumV3_opt(short float ** in, short float ** out, unsigned int width) {
  for (unsigned int i = 0; i < width; ++i) {
    short float max1 = in[0][i];                  
    if(in[1][i] > max1) max1 = in[1][i];
    if(in[2][i] > max1) max1 = in[2][i];

    out[0][i] = max1;
  }
}
#endif
