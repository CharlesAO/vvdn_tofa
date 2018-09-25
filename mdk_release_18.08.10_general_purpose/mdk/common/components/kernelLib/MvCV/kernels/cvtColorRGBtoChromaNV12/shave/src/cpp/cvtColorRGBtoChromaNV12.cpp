#include "cvtColorRGBtoChromaNV12.h"
#include <stdio.h>
#include <math.h> 

void mvcvCvtColorRGBtoChromaNV12(u8* in1RGB[3], u8* in2RGB[3], u8* uvOut, float mat[3*3], float offset[1*3], u32 width) 
{
	u32 i;
	float u, v;
    u32 uv_idx = 0;
    	
    for (i = 0; i < width; i+=2)
    {
	//R = (((R00 + R01+1)>>1) + ((R10 + R11+1)>>1) + 1)>>1; 
		u = (((((in1RGB[0][i] + in1RGB[0][i + 1])>>1) + ((in2RGB[0][i] + in2RGB[0][i + 1])>>1) + 1) >> 1)/255.0f * mat[3]  + 
		    ((((in1RGB[1][i] + in1RGB[1][i + 1])>>1) + ((in2RGB[1][i] + in2RGB[1][i + 1])>>1) + 1) >> 1)/255.0f * mat[4]  + 
			((((in1RGB[2][i] + in1RGB[2][i + 1])>>1) + ((in2RGB[2][i] + in2RGB[2][i + 1])>>1) + 1) >> 1)/255.0f * mat[5]) + offset[1];
        u = u * 255;                                                                                      
		v = (((((in1RGB[0][i] + in1RGB[0][i + 1])>>1) + ((in2RGB[0][i] + in2RGB[0][i + 1])>>1) + 1) >> 1)/255.0f * mat[6]  + 
			((((in1RGB[1][i] + in1RGB[1][i + 1])>>1) + ((in2RGB[1][i] + in2RGB[1][i + 1])>>1) + 1) >> 1)/255.0f * mat[7]  + 
			((((in1RGB[2][i] + in1RGB[2][i + 1])>>1) + ((in2RGB[2][i] + in2RGB[2][i + 1])>>1) + 1) >> 1)/255.0f * mat[8]) + offset[2];
		v = v * 255;
	
	    // u = (round(((float)(in1RGB[0][i] + in1RGB[0][i + 1] + in2RGB[0][i] + in2RGB[0][i + 1]) * 0.25f))/255.0f * mat[3]  + 
			 // round(((float)(in1RGB[1][i] + in1RGB[1][i + 1] + in2RGB[1][i] + in2RGB[1][i + 1]) * 0.25f))/255.0f * mat[4]  + 
			 // round(((float)(in1RGB[2][i] + in1RGB[2][i + 1] + in2RGB[2][i] + in2RGB[2][i + 1]) * 0.25f))/255.0f * mat[5]) + offset[1];
        // u = u * 255;                                                                            
		// v = (round(((float)(in1RGB[0][i] + in1RGB[0][i + 1] + in2RGB[0][i] + in2RGB[0][i + 1]) * 0.25f))/255.0f * mat[6]  + 
			 // round(((float)(in1RGB[1][i] + in1RGB[1][i + 1] + in2RGB[1][i] + in2RGB[1][i + 1]) * 0.25f))/255.0f * mat[7]  + 
			 // round(((float)(in1RGB[2][i] + in1RGB[2][i + 1] + in2RGB[2][i] + in2RGB[2][i + 1]) * 0.25f))/255.0f * mat[8]) + offset[2];
		// v = v * 255;
	
        uvOut[uv_idx    ] = (u8) (u > 255 ? 255 : u < 0 ? 0 : u);
        uvOut[uv_idx + 1] = (u8) (v > 255 ? 255 : v < 0 ? 0 : v);
        uv_idx = uv_idx + 2;
	}
	return;
}
