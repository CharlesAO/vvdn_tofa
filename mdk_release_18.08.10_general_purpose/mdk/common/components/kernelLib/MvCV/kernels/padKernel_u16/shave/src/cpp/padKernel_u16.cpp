///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     API for some required Leon Math functions
///
#include <mv_types.h>
#include "padKernel_u16.h"
#include <stdio.h>


void mvcvPadKernel_u16(u16* iBuf, u32 iBufLen, u16* oBuf, u32 padSz, u32 padMode, u32 padType, u16 pixValue)
{
	u32 i;

	if(padMode == Left)
	{
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
					oBuf[i] = iBuf[padSz - i - 1];
					break;
				case BlackPixel:
					oBuf[i] = 0x00;
					break;
				case WhitePixel:
					oBuf[i] = 0xffff;
					break;
				case PixelValue:
					oBuf[i] = pixValue;
					break;
			}
	}
	else
		if(padMode == Right)
		{
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
					oBuf[iBufLen + i] = iBuf[iBufLen - i - 1];
					break;
				case BlackPixel:
					oBuf[iBufLen + i] = 0x00;
					break;
				case WhitePixel:
					oBuf[iBufLen + i] = 0xffff;
					break;
				case PixelValue:
					oBuf[iBufLen + i] = pixValue;
					break;
				}
		}
	else
		if(padMode == LeftAndRight)
		{		
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
					oBuf[i] = iBuf[padSz - i - 1];
					oBuf[i + padSz + iBufLen] = iBuf[iBufLen - i - 1];
					break;
				case BlackPixel:
					oBuf[i] = 0x00;
					oBuf[i + padSz + iBufLen] = 0x00;
					break;
				case WhitePixel:
					oBuf[i] = 0xffff;
					oBuf[i + padSz + iBufLen] = 0xffff;
					break;
				case PixelValue:
					oBuf[i] = pixValue;
					oBuf[i + padSz + iBufLen] = pixValue;
					break;
				
				}	
		}
}