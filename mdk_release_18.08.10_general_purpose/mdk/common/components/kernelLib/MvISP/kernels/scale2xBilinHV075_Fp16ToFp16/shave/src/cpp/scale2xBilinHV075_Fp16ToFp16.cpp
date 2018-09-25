///
/// @file      scale2xBilinHV075_U16ToU16.cpp
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.tit
///
/// @brief     Up scale by 2 in each direction with bilinear (sw) kernel.
///            Fp16 in and out
///

#include <scale2xBilinHV075_Fp16ToFp16.h>

/// scale2xBilinHV075_Fp16ToFp16 kernel
/// @param[in] in         	- array of pointers to input lines
/// @param[in] out        	- pointer to output line
/// @param[in] inWidth		  - width of the output line
void mvispScale2xBilinHV075_Fp16ToFp16(half** in, half** out, u32 inWidth)
{
	half *output = out[0];
	int j;	

  half weight[2][4] = {
			{3.0/16, 9.0/16, 1.0/16, 3.0/16},
			{9.0/16, 3.0/16, 3.0/16, 1.0/16},		
	};
	
	for (j = 0; j < (int)inWidth; j+=2) 
	{
		output[j] = in[0][j/2 - 1]*weight[0][0] + in[0][j/2]*weight[0][1]   + in[1][j/2-1]*weight[0][2] + in[1][j/2]*weight[0][3];
		output[j+1] = in[0][j/2]*weight[1][0]   + in[0][j/2+1]*weight[1][1] + in[1][j/2]*weight[1][2]   + in[1][j/2+1]*weight[1][3];
  }
	
    return;
}
