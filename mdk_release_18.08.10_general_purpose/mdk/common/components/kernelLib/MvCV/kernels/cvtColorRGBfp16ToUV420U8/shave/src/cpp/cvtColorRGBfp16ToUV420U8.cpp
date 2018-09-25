#include "cvtColorRGBfp16ToUV420U8.h"

void  mvcvCvtColorRGBfp16ToUV420U8(half* inRGB[3], u8* uOut, u8* vOut, u32 width)
{
	u32 i;
	
	half *r   = inRGB[0];
	half *g   = inRGB[1];
	half *b   = inRGB[2];
	
	
	half ua, ub, va, vb;
	half um, vm;
	u32 u_idx = 0;
	u32 v_idx = 0;
	
	for (i = 0; i < width; i+=2)
    {
	
        ua = ((r[i] * (-0.14713f)) + (g[i] * (-0.28886f)) + (b[i] * 0.436f     ) + (128.0f/255.0f));
        va = ((r[i] * 0.615f     ) + (g[i] * (-0.51499f)) + (b[i] * (-0.10001f)) + (128.0f/255.0f));
        
	   //-----------------------------------------------------------------------------------------------------------------
	
		ub = ((r[i + 1] * (-0.14713f)) + (g[i + 1] * (-0.28886f)) + (b[i + 1] * 0.436f     ) + (128.0f/255.0f));
		vb = ((r[i + 1] * 0.615f     ) + (g[i + 1] * (-0.51499f)) + (b[i + 1] * (-0.10001f)) + (128.0f/255.0f));
        
		um = (half)((ua + ub)/2);
        vm = (half)((va + vb)/2);
        
		if (um <= (half)0.0f)
			um = (half)0.0f;
		if (um >= (half)1.0f)
			um = (half)1.0f;
		
		if (vm <= (half)0.0f)
			vm = (half)0.0f;
		if (vm >= (half)1.0f)
			vm = (half)1.0f;		
		
		uOut[u_idx] = (u8)(((float)um) * 255.0f);
		vOut[u_idx] = (u8)(((float)vm) * 255.0f);
		u_idx++;
		v_idx++;
	}
}
