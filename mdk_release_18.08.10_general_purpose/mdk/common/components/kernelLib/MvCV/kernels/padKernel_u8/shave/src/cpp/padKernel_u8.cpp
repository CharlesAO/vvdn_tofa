///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     API for some required Leon Math functions
///
#include <mv_types.h>
#include "padKernel_u8.h"
#include <stdio.h>

u8 mirror(u8 x)
{	
    u8 y = (x >> 4) | (x << 4);

    return y;
}

void mvcvPadKernel_u8(u8* iBuf, u32 iBufLen, u8* oBuf, u32 padSz, u32 padMode, u32 padType, u8 pixValue)
{
	u32 i;

	if(padMode == Left)
	{
		u8 m = mirror(iBuf[0]);
		for(i = 0; i < iBufLen; i++)
			oBuf[i + padSz] = iBuf[i];
		for(i = 0; i< padSz; i++)
			switch(padType)
			{
				case AllZero:
					oBuf[i] = 0;
					break;
				case AllOne:
					oBuf[i] = 1;
					break;
				case Mirror:
					oBuf[i] = m;
					break;
				case BlackPixel:
					oBuf[i] = 0x00;
					break;
				case WhitePixel:
					oBuf[i] = 0xff;
					break;
				case PixelValue:
					oBuf[i] = pixValue;
					break;
			}
	}
	else
		if(padMode == Right)
		{
			u8 m = mirror(iBuf[iBufLen - 1]);
			
			for(i = 0; i < iBufLen; i++)
				oBuf[i] = iBuf[i];
			for(i = 0; i < padSz; i++)
				switch(padType)
				{
				case AllZero:
					oBuf[iBufLen + i] = 0;
					break;
				case AllOne:
					oBuf[iBufLen + i] = 1;
					break;
				case Mirror:
					oBuf[iBufLen + i] = m;
					break;
				case BlackPixel:
					oBuf[iBufLen + i] = 0x00;
					break;
				case WhitePixel:
					oBuf[iBufLen + i] = 0xff;
					break;
				case PixelValue:
					oBuf[iBufLen + i] = pixValue;
					break;
				}
		}
	else
		if(padMode == LeftAndRight)
		{
			u8 mL = mirror(iBuf[0]);
			u8 mR = mirror(iBuf[iBufLen - 1]);
		
			for(i = 0; i < iBufLen; i++)
				oBuf[i + padSz] = iBuf[i];
			for(i = 0; i < padSz; i++)
				switch(padType)
				{
				case AllZero:
					oBuf[i] = 0;
					oBuf[i + padSz + iBufLen] = 0;
					break;
				case AllOne:
					oBuf[i] = 1;
					oBuf[i + padSz + iBufLen] = 1;
					break;
				case Mirror:
					oBuf[i] = mL;
					oBuf[i + padSz + iBufLen] = mR;
					break;
				case BlackPixel:
					oBuf[i] = 0x00;
					oBuf[i + padSz + iBufLen] = 0x00;
					break;
				case WhitePixel:
					oBuf[i] = 0xff;
					oBuf[i + padSz + iBufLen] = 0xff;
					break;
				case PixelValue:
					oBuf[i] = pixValue;
					oBuf[i + padSz + iBufLen] = pixValue;
					break;
				
				}	
		}
}