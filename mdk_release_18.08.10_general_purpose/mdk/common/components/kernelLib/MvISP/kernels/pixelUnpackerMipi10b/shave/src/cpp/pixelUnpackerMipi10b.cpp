///
/// @file      pixelUnpackerMipi10b.cpp
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Special kernel to interface to Myriad2 Mipi block
///            Converts 32bit value received as [12b | 10b | 10b]  to [6b | 10b | 6b | 10b]
///

#include <pixelUnpackerMipi10b.h>
#include <stdlib.h>

void  __attribute__((overloadable))  mvispPixelUnpackerMipi10b(u32 *in, u16 *out, u32 width, u32 lineNo, u32 coefs[4])
{  
	u32 mask1 = 0x000003FF;
	u32 mask2 = 0x000FFC00;
	int i;
	int j = 0;
	u32 coef1, coef2;
	double out1, out2;
	
	if(lineNo % 2 == 0)
	{
		coef1 = coefs[0];
		coef2 = coefs[1];
	}
	else
	{
		coef1 = coefs[2];
		coef2 = coefs[3];
	}
	
	for(i = 0; i < (int)width; i = i + 2)
	{
		out1 = (double)((in[j] & mask1)) - (double)coef2;
		out2 = (double)((in[j++] & mask2) >> 10) - (double)coef1; 
		
		if (out1 >= 65535)
			out1 = 65535;
		if (out1 <= 0)
			out1 = 0;
			
		if (out2 >= 65535)
			out2 = 65535;
		if (out2 <= 0)
			out2 = 0;
		
		out[i] = (u16)(out1);
		out[i + 1] = (u16)(out2);
	}
}

void  __attribute__((overloadable)) mvispPixelUnpackerMipi10b(u32 *in, u16 *out, u32 width, u32 lineNo, u32 coefs[4], int noMipiRxWorkaround)
{  
	u32 mask1;
	u32 mask2;
	int i, shift;
	int j = 0;
	u32 coef1, coef2;
	double out1, out2;

    if (noMipiRxWorkaround) {
        mask1 = 0x0000FFFF;
        mask2 = 0xFFFF0000;
        shift = 16;
    } else {
        mask1 = 0x000003FF;
        mask2 = 0x000FFC00;
        shift = 10;
    }
	
	if(lineNo % 2 == 0)
	{
		coef1 = coefs[0];
		coef2 = coefs[1];
	}
	else
	{
		coef1 = coefs[2];
		coef2 = coefs[3];
	}
	
	for(i = 0; i < (int)width; i = i + 2)
	{
		out1 = (double)((in[j] & mask1)) - (double)coef2;
		out2 = (double)((in[j++] & mask2) >> shift) - (double)coef1; 
		
		if (out1 >= 65535)
			out1 = 65535;
		if (out1 <= 0)
			out1 = 0;
			
		if (out2 >= 65535)
			out2 = 65535;
		if (out2 <= 0)
			out2 = 0;
		
		out[i] = (u16)(out1);
		out[i + 1] = (u16)(out2);
	}
}