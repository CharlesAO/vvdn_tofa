///
/// @file scale05Lanc6HV.cpp
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Downscale with factor 0.5 (half) in each direction with lanczos kernel
///            Kernel size is 6. 
///

#include <mv_types.h>
#include <scale05Lanc6HV.h>
#include <string.h>
#include "stdio.h"
#include <stdlib.h>

static int kern[6] = {
	-3, 7, 28, 28, 7, -3
};
static int normalizeFactor = 12;

int vStep(u8 *inRGB[6], int pos)
{
     return kern[0]*inRGB[0][pos] +
                kern[1]*inRGB[1][pos] +
                kern[2]*inRGB[2][pos] +
                kern[3]*inRGB[3][pos] +
                kern[4]*inRGB[4][pos] +
                kern[5]*inRGB[5][pos];
}


/// lanczos downscale 0.5  - Filter, apply a lanczos downscale, with factor 0.5, and 6 taps 
/// @param[in] in     - array of pointers to 6 input lines
/// @param[out]out    - pointers to output lines
/// @param[in] width  - width of output line
void mvispScale05Lanc6HV(u8 *in[6], u8 *out,unsigned int width) 
{
   int  i, col, refPos; 
   int     vstep[6]; //temp array for Vertical Step
   int 	 outputVal = 0;
    for(i = 0; i < (int)width; i++)
    {
        //Chromas: 1/2 downsampling, so:
        refPos = 2*i;//horizontal position
        //Vertical step:
        for(col = -2; col <= 3; col++)
		{
           vstep[col+2] = vStep(in, refPos+col);
		}
        //Horiz step:
		outputVal = (kern[0] * vstep[0] +
					kern[1] * vstep[1] +
                    kern[2] * vstep[2] +
                    kern[3] * vstep[3] +
                    kern[4] * vstep[4] +
                    kern[5] * vstep[5]);
		if (outputVal < 0)
		{
			outputVal = 0;
		}	
        if (outputVal > 1044480 ) //0xFF000
        {
            outputVal = 1044480;
        }		
		outputVal = outputVal >> normalizeFactor;
        out[i] = (u8)outputVal;
    }
}