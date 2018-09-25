#include "cvtColorKernelYUV422ToRGB.h"

void mvcvCvtColorKernelYUV422ToRGB(u8** input, u8** rOut, u8** gOut, u8** bOut, u32 width)
{	
	unsigned int j;
	int r, g, b, y1, y2, u, v;
	int out_index = 0;
	u8*  in = input[0];
	u8* outR = rOut[0];
	u8* outG = gOut[0];
	u8* outB = bOut[0];
	
	for(j = 0; j < width * 2; j+=4)
	{
		y1 = in[j];
		u  = in[j + 1] - 128;
		y2 = in[j + 2];
		v  = in[j + 3] - 128;
		
		r = y1 + (int)(1.402f * v);
        g = y1 - (int)(0.344f * u + 0.714f * v);
        b = y1 + (int)(1.772f * u);
		
		outR[out_index] = (u8) (r>255 ? 255 : r<0 ? 0 : r);
		outG[out_index] = (u8) (g>255 ? 255 : g<0 ? 0 : g);
		outB[out_index] = (u8) (b>255 ? 255 : b<0 ? 0 : b);
		out_index++;
	
		r = y2 + (int)(1.402f * v);
        g = y2 - (int)(0.344f * u + 0.714f * v);
        b = y2 + (int)(1.772f * u);

		outR[out_index] = (u8) (r>255 ? 255 : r<0 ? 0 : r);
		outG[out_index] = (u8) (g>255 ? 255 : g<0 ? 0 : g);
		outB[out_index] = (u8) (b>255 ? 255 : b<0 ? 0 : b);
		out_index++;
	}
}
