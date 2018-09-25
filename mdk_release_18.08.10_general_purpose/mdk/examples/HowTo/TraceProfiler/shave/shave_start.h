///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    Functions for shaves code
///

#ifndef _SINGLE_KERNEL_
#define _SINGLE_KERNEL_

// 1: Includes
#include <mv_types.h>
// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------

void ApplicationStart(u8* outFrBuff, u8* inFrBuff, u32 width, u32 height, u32 nrShaves, u32 sliceIdx, u32 kernel_size);

int clamp(int x, int y, int z)
{
    if(x < y)
	{
		return y;
    }
    else
	{
		if(x > z)
		{
			return z;
		}
		else
		{
			return x;
		}
	}
}
#endif
