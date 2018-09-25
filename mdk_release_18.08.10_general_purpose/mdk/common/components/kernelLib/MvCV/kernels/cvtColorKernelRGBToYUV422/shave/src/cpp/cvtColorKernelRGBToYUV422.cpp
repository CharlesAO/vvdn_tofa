#include "cvtColorKernelRGBToYUV422.h"

void mvcvCvtColorKernelRGBToYUV422(u8** rIn, u8** gIn, u8** bIn, u8** output, u32 width)
{
	int j;
	float y, u1, v1, u2, v2, u_avr, v_avr;
	int out_index = 0;
	u8* inR = rIn[0];
	u8* inG = gIn[0];
	u8* inB = bIn[0];
	u8* out_yuyv = output[0];
	
	for(j = 0; j < (int)width; j+=2)
	{
		y = 0.299f * inR[j] + 0.587f * inG[j] + 0.114f * inB[j];
		u1 = ((float)inB[j] - y) * 0.564f + 128;
        v1 = ((float)inR[j] - y) * 0.713f + 128;
		
		y = y>255 ? 255 : y<0 ? 0 : y;
		out_yuyv[out_index++] =(u8) y;

		y = 0.299f * inR[j + 1] + 0.587f * inG[j + 1] + 0.114f * inB[j + 1];
		y = y>255 ? 255 : y<0 ? 0 : y;
		u2 = ((float)inB[j + 1] - y) * 0.564f + 128;
        v2 = ((float)inR[j + 1] - y) * 0.713f + 128;

		u_avr = (u1 + u2) / 2;
		v_avr = (v1 + v2) / 2;
		
		out_yuyv[out_index++] =(u8) (u_avr>255 ? 255 : u_avr<0 ? 0 : u_avr);
		out_yuyv[out_index++] =(u8) y;	
		out_yuyv[out_index++] =(u8) (v_avr>255 ? 255 : v_avr<0 ? 0 : v_avr);		
	}
}
