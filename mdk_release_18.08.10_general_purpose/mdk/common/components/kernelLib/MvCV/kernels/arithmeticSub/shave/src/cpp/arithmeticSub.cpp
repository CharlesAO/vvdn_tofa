#include "arithmeticSub.h"

void mvcvArithmeticSub(u8** src1, u8** src2, u8** dst, u32 width) 
{
	int i;
	float sub;
	for(i=0;i<(int)width;i++) 
	{
	sub = src1[0][i] - src2[0][i];
		
		if (sub >= 255)
			sub = 255.0f;
		if (sub <= 0)
			sub = 0.0f;
		
		dst[0][i] = (u8)(sub);
	}
}
