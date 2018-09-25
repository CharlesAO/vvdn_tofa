///
/// @file      scale2xBilinHV025_U16ToU16.cpp
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.tit
///
/// @brief     Up scale by 2 in each direction with bilinear (sw) kernel.
///            U16 in and out
///

#include <scale2xBilinHV025_U16ToU16.h>
#include <stdio.h>
//#include <half.h>
/// scale2xBilinHV025_U16ToU16 kernel
/// @param[in] in         	- array of pointers to input lines
/// @param[in] out        	- pointer to output line
/// @param[in] inWidth		  - width of the output line
void mvispScale2xBilinHV025_U16ToU16(u16** in, u16** out, u32 inWidth)
{
	u16 *output = out[0];
	int j;	
  u32 outputOdd;
  u32 outputEven;
	
	u32 weight[2][4] = {
			{1, 3, 3, 9},
			{3, 1, 9, 3},		
	};
	
	for (j = 0; j < (int)inWidth; j+=2) 
	{
    outputEven = in[0][j/2 - 1] * weight[0][0] + in[0][j/2]   * weight[0][1] + in[1][j/2-1] * weight[0][2] + in[1][j/2]    * weight[0][3];
    outputOdd =  in[0][j/2]     * weight[1][0] + in[0][j/2+1] * weight[1][1] + in[1][j/2]   * weight[1][2]  + in[1][j/2+1] * weight[1][3];
    
		output[j] = outputEven >> 4;
		output[j+1] = outputOdd >> 4;
  }
	
    return;
}
