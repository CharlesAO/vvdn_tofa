///
/// @file      purpleFlare.cpp
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Performs a restricted sharpening of the Blue channel, to reduce 
///            purple flare seen around bright objects
///

#include "purpleFlare.h"
#include <mv_types.h>
#include <stdio.h>

#define MIN(a,b)         ((a) < (b) ? (a) : (b))
#define MAX(a,b)         ((a) > (b) ? (a) : (b))

void mvispPurpleFlare(u16 *inGreen, u16 *blue[3], u16 *outBlue, u8 strength, u32 width)
{
    int i, in, bFilt, hp, out;

    for (i = 0; i < (int)width; i++) {
	in = blue[1][i];	/* Centre pixel */

	/* Apply kernel [ 1 2 1; 2 4 2; 1 2 1] */
        bFilt = (blue[0][i-1] << 0) + (blue[0][i] << 1) + (blue[0][i+1] << 0) +
                (blue[1][i-1] << 1) + (in         << 2) + (blue[1][i+1] << 1) +
                (blue[2][i-1] << 0) + (blue[2][i] << 1) + (blue[2][i+1] << 0);

	/* High pass (blurred version subtracted from centre pixel) */
        hp = in - (bFilt >> 4);

	/* Only darken blue pixels, never brighten them */
	if (hp > 0) {
	    hp = 0;
	}

	out = in + hp * strength;

	/*
	 * Never darken the blue channel such that its value is less than
	 * that of the green channel.  To achieve this, clamp the new blue
	 * value to be between the old blue and green values.
	 */
	outBlue[i] = MIN(in, MAX(inGreen[i], out));
    }
}
