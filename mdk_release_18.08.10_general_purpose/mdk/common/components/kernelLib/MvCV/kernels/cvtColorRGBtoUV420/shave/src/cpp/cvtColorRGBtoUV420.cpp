#include "cvtColorRGBtoUV420.h"

void mvcvCvtColorRGBtoUV420(u8** inR, u8** inG, u8** inB, u8** uOut, u8** vOut, u32 width) 
{
	u32 i;
	
	u8* r = inR[0];
	u8* g = inG[0];
	u8* b = inB[0];
	u8* Uo = uOut[0];
	u8* Vo = vOut[0];
	
	float ua, ub, va, vb, um, vm;
	u32 u_idx = 0;
	u32 v_idx = 0;
	
	for (i = 0; i < width; i+=2)
    {
        ua = (((float)r[i] * (-0.14713f)) + ((float)g[i] * (-0.28886f)) + ((float)b[i] * 0.436f     ) + 128);
        va = (((float)r[i] * 0.615f     ) + ((float)g[i] * (-0.51499f)) + ((float)b[i] * (-0.10001f)) + 128);
        
	   //-----------------------------------------------------------------------------------------------------------------
		ub = (((float)r[i + 1] * (-0.14713f)) + ((float)g[i + 1] * (-0.28886f)) + ((float)b[i + 1] * 0.436f     ) + 128);
		vb = (((float)r[i + 1] * 0.615f     ) + ((float)g[i + 1] * (-0.51499f)) + ((float)b[i + 1] * (-0.10001f)) + 128);
        

		um = (ua + ub)/2;
        vm = (va + vb)/2;
        Uo[u_idx] = (u8) (um > 255 ? 255 : um < 0 ? 0 : um);
		Vo[v_idx] = (u8) (vm > 255 ? 255 : vm < 0 ? 0 : vm);
		u_idx++;
		v_idx++;
		}
}
