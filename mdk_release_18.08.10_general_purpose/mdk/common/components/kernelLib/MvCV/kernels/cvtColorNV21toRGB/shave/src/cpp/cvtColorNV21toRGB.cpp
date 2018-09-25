#include "cvtColorNV21toRGB.h"

void mvcvCvtColorNV21toRGB(u8** yin, u8** uvin, u8** outR, u8** outG, u8** outB, u32 width) {
	u32 i;
	
	u8* y  = yin[0];
	u8* uv = uvin[0];
	u8* rr = outR[0];
	u8* gg = outG[0];
	u8* bb = outB[0];
	
	u32 uvidx = 0;
	int yy,u,v,r,g,b;
	for(i = 0; i < width; i+=2 )
	{
		yy = y[i];
		u = uv[uvidx] - 128;
		v = uv[uvidx+1] - 128;
		uvidx += 2;
			r =  yy + (int)(1.402f*v);
			rr[i] = (u8) (r > 255 ? 255 : r < 0 ? 0 : r);  
			g =  yy - (int)(0.344f*u + 0.714*v);
			gg[i] = (u8) (g > 255 ? 255 : g < 0 ? 0 : g);
			b =  yy + (int)(1.772f*u);
			bb[i] = (u8) (b > 255 ? 255 : b < 0 ? 0 : b);
	//----------------------------------------------
		yy = y[i + 1];
		r =  yy + (int)(1.402f*v);
		rr[i + 1] = (u8) (r > 255 ? 255 : r < 0 ? 0 : r);
		g =  yy - (int)(0.344f*u + 0.7148*v);
		gg[i + 1] = (u8) (g > 255 ? 255 : g < 0 ? 0 : g);
		b =  yy + (int)(1.772f*u);
		bb[i + 1] = (u8) (b > 255 ? 255 : b < 0 ? 0 : b);
	}
}
