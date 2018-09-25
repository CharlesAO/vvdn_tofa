#include "cvtColorRGBtoLumaNV12.h"

void mvcvCvtColorRGBtoLumaNV12(u8* inR, u8* inG, u8* inB, u8* yOut, float* mat, float* offset, u32 width) {
   u32 i;
	//coefsMat = {0.299f, 0.587f, 0.114f};	
    float y;
	float r,g,b;

    for (i = 0; i < width; i++)
    {
		r = (float)inR[i]/255;
		g = (float)inG[i]/255;
		b = (float)inB[i]/255;
	
        y = (mat[0] * r + mat[1] * g + mat[2] * b) + offset[0];
		y = y * 255;		
		
		yOut[i] = (u8) ((y > 255 ? 255 : y < 0 ? 0 : y));
    }
	return;
}
