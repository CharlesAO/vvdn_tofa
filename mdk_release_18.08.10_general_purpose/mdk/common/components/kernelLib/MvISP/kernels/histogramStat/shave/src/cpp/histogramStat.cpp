///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Calculate histogram statistics for RGB image
///

#include "histogramStat.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

//							i18         i17         i16  		i15   		i14   		i13  		i12			i11              
void mvispHistogramStat(u8 *inputR, u8 *inputG, u8 *inputB, u32 *histR, u32 *histG, u32 *histB, u32 width, u32 step) 
{
    
	int incr = step * 8 + 8;
	
	for(int i = 0; i < (int)(width - 7); i+=incr)
	{
		for(int j = 0; j < 8; j++)
		{
			histR[inputR[i+j]>>2]++;
			histG[inputG[i+j]>>2]++;
			histB[inputB[i+j]>>2]++;
		}
	}
	return;
}
