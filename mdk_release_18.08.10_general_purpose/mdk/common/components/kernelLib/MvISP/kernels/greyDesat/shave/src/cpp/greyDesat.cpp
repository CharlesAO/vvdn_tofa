///
/// @file	greyDesat.cpp
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Desaturates areas which are already very close to grey.  
///            It removes residual low-frequency color noise, which is most 
///            noticeable in grey areas.  This filter calculates a metric which 
///            indicates how close a pixel is to grey (the smaller the value 
///            the closer the pixel is to grey.  A blend factor is calculated which 
///            is used to interpolate between the pixel's chrominance values, and 
///            the chrominance values of a grey pixel
///
#include <greyDesat.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


static inline u8 clampU8(float in)
{
    in += 0.5; //round to nearest int
    if(in< 0)  { return 0; }
    if(in>255) { return 255; }
               return (u8)in;
}

void mvispGreyDesat(u8* in[3], u8* out[3], int offset, int slope, int grey[3], u32 width)
{
	int i;	
	short alpha;
	short off = (short)offset;
	short slp = (short)slope;
	short g[3];
	
	g[0] = (short)grey[0];
	g[1] = (short)grey[1];
	g[2] = (short)grey[2];
	
	for (i = 0; i < (int)width; i++) {
	alpha = abs((short)in[0][i] - g[0]) +
			abs((short)in[1][i] - g[1]) + 
			abs((short)in[2][i] - g[2]);
	
	alpha = clampU8(((alpha + off) * slp));
				
	out[0][i] = (u8)(((alpha * (in[0][i] - g[0])) >> 8) + g[0]);
	out[1][i] = (u8)(((alpha * (in[1][i] - g[1])) >> 8) + g[1]);
	out[2][i] = (u8)(((alpha * (in[2][i] - g[2])) >> 8) + g[2]);

	
	}
}
