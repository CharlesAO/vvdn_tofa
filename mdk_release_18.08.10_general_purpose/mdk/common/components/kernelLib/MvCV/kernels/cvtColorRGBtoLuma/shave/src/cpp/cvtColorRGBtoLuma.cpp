#include "cvtColorRGBtoLuma.h"

void mvcvCvtColorRGBtoLuma(u8** inR, u8** inG, u8** inB, u8** yOut, u32 width) {
	u32 i;
	
	u8* r = inR[0];
	u8* g = inG[0];
	u8* b = inB[0];
	u8* yo = yOut[0];
	
	
	int y;
	
	for (i = 0; i < width; i++){
        y = 0.299f * r[i] + 0.587f * g[i] + 0.114f * b[i];
        yo[i] = (u8)(y > 255 ? 255 : y < 0 ? 0 : y);
    }
}
