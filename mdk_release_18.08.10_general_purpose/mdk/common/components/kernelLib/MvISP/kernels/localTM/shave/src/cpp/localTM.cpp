///
/// @file localTM.cpp
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief	 Applies a tone mapping function to the Luma channel.  
///          The mapping function is based on both the original Luma value, 
///          plus a second local brightness input image 
///          Local Tone Mapping increases contrast locally, applying a contrast 
///          curve that is adapted to the local image intensity
///

#include "localTM.h"

void mvispLocalTM(half** luma_in, u8** bg8, half** output, half *curves, u32 width, u32 run_no)
{
	half		*luma, *out;
	half		bg, csel, psel;
	half		alphaCol, alphaRow;
	
	unsigned int i, wi;
	int bgidx;
	int		csel1, csel2, psel1, psel2;
	static half	wt[4][4] = {
				{ 3.0/4096, 9.0/4096, 1.0/4096, 3.0/4096 },
				{ 9.0/4096, 3.0/4096, 3.0/4096, 1.0/4096 },
				{ 1.0/4096, 3.0/4096, 3.0/4096, 9.0/4096 },
				{ 3.0/4096, 1.0/4096, 9.0/4096, 3.0/4096 },
	};
	
	luma = *luma_in;
	out  = *output;
	
	bgidx = -1;
	for (i = 0; i < width; i++) {
		/*
		 * Bilinear upsample of "background" input, and conversion
		 * from U8f to FP16.
		 */
		wi = (run_no & 1) << 1 | (i & 1);
		bg = (half)(float)bg8[0][bgidx]   * wt[wi][0] +
		     (half)(float)bg8[0][bgidx+1] * wt[wi][1] +
		     (half)(float)bg8[1][bgidx]   * wt[wi][2] +
		     (half)(float)bg8[1][bgidx+1] * wt[wi][3];

		/* Pick two curves - two adjacent columns */
		csel = bg*8;
		csel1 = csel;
		alphaCol = csel - csel1;
		csel2 = csel1+1;


		/*
		 * Pick points in curves to interpolate between (row indices).
		 *
		 * Note: It's important that "luma" is >= 0.  Otherwise we
		 * could exceed the bounds of the array.  Might want to
		 * explicitly clamp "luma" to 0, but it isn't needed if the
		 * input is guaranteed to be >= 0.
		 */
		psel = luma[i]*14;
		psel1 = psel;
		alphaRow = psel - psel1;
		psel2 = psel1+1;


		out[i] = curves[psel1 * 10 + csel1] * (1-alphaRow) * (1-alphaCol) +
		         curves[psel1 * 10 + csel2] * (1-alphaRow) * alphaCol +
		         curves[psel2 * 10 + csel1] * alphaRow * (1-alphaCol) +
		         curves[psel2 * 10 + csel2] * alphaRow * alphaCol;

		if ((i & 1) == 0) {
			bgidx++;
		}
	}
}
