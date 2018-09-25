#include "arithmeticSubmask.h"

 void mvcvArithmeticSubmask(u8** src1, u8** src2, u8** dst, u8** mask, u32 width) 
 {
	int i;
	float sub;
	
	for(i = 0; i < (int)width; i++) 
	{
		if (mask[0][i] > 0) 
		{
			sub = src1[0][i] - src2[0][i];
		
			if (sub >= 255)
				sub = 255.0f;
			if (sub <= 0)
				sub = 0.0f;
			dst[0][i] = (u8)(sub);
		}
	}
}
