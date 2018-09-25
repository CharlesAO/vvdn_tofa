///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Extract Chroma differences CrCgCb from (debayerd) RGB image
///

#include "genChromaRGB16.h"

void mvispGenChromaRGB16(u16** inRGB, u8 *inY, u8** outC, int width, int eps, int nBits, float *chromaScale)
{

  u32 i, scale = 1<<(nBits-8);
  u32 ooLuma;
  int out1, out2, out3;
  float out11, out22, out33;


  for(i=0; i<(unsigned int)width; i++)
  {
     ooLuma   =  (inY[i] + eps) * scale;
	 if (ooLuma == 0)
	 {
		out11 = 255;
		out22 = 255;
		out33 = 255;
	 }
	 else
	 {
	 out11 = ((float)inRGB[0][i] * chromaScale[0] / (float)ooLuma);
	 out22 = ((float)inRGB[1][i] * chromaScale[1] / (float)ooLuma);
	 out33 = ((float)inRGB[2][i] * chromaScale[2] / (float)ooLuma);
	 }

	 out1 = (int)(out11 * 85);
	 out2 = (int)(out22 * 85);
	 out3 = (int)(out33 * 85);

	 if (out1 < 0)
		out1 = 0;
	 if (out1 > 255)
		out1 = 255;
		outC[0][i] = (u8)out1;

	if (out2 < 0)
		out2 = 0;
	 if (out2 > 255)
		out2 = 255;
		outC[1][i] = (u8)out2;

	if (out3 < 0)
		out3 = 0;
	 if (out3 > 255)
		out3 = 255;
		outC[2][i] = (u8)out3;

  }
}
