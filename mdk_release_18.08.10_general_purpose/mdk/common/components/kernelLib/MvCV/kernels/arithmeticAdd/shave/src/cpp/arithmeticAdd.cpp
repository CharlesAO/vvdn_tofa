#include "arithmeticAdd.h"

void mvcvArithmeticAdd(u8** src1, u8** src2, u8** dst, u32 width)
{
	int i;
	float add;
	for(i = 0; i < (int)width; i++)
	{
		add = src1[0][i] + src2[0][i];
	
		if (add >= 255)
			add = 255.0f;
		if (add <= 0)
			add = 0.0f;
		dst[0][i] = (u8)(add);
	
	}
}
