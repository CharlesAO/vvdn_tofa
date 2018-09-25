#include "jpegGetBlockY420.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/******************************************************************************
 **  JpegGetBlockY420
 **  --------------------------------------------------------------------------
 **  ARGUMENTS:
 **  mode             - has one of the following values: 0, 1, 2, 3 - selects one 8x8 block from the 16x16 macroblock
 **  imginfo_Y        - input buffer with  imginfo_width*(imginfo_height) dimension, where (imginfo_height)modulo 16 =0
 **  imginfo_width - width of input buffer
 **  imgspec_x     - macroblock x coordinate
 **  imgspec_y     - macroblock y coordinate
 **  imgspecY[8][8]- output 8x8 block
 **  RETURN: -
 ******************************************************************************/


 void mvcvjpegGetBlockY420(u32 mode, u8 *imginfo_Y, u32 imginfo_width, u32 imgspec_x,u32 imgspec_y, short imgspecY[8][8])

{

    u32 r1;
    u32 __attribute__((unused)) c1; //avoid 'dead assignment' error from scan build
    u32 x = imgspec_x;
    u32 y = imgspec_y;


    static u32 loops[4][4] = {{0, 8,  0,  8},
                              {0, 8,  8, 16},
                              {8, 16, 0,  8},
                              {8, 16, 8, 16}
                             };
    u8 *ypt = imginfo_Y;

    r1 = 0;
    c1 = 0;
    for (u32 r = loops[mode][0]; r < loops[mode][1]; r++)
    {
        c1 = 0;
        u32 offset = (y + r) * imginfo_width + x;
        for (u32 c = loops[mode][2]; c < loops[mode][3]; c++)
        {
            u32 i = offset + c;
            imgspecY[r1][c1] = ypt[i];
            imgspecY[r1][c1] -= 128;
            c1++;
        }
        r1++;
    }
    return;
}


#ifdef MOVICOMPILE_OPTIMIZED


void mvcvjpegGetBlockY420_opt(u32 mode, u8 * __restrict__ imginfo_Y, u32 imginfo_width, u32 imgspec_x,u32 imgspec_y, short imgspecY[8][8])
{
   short8 * outputRowPtr;

   // we need y to be offset by 0 for mode={0,1}  and by 8 for mode={2,3}
   u32 y = imgspec_y +  ((mode&2)*4);

   // we need x to be offset by 0 for mode={0,2}  and by 8 for mode={1,3}
   u32 c = ((mode & 0x1) * 8);

   // set the inputRowPtr initially to start of the first set of values to be extracted
   u32 offset = y * imginfo_width + imgspec_x + c;
   uchar8 *inputRowPtr = (uchar8 *)&imginfo_Y[offset];

   for (u32 count=0; count<8; count++)
   {
        uchar8 val = *inputRowPtr;
        outputRowPtr = (short8 *) &imgspecY[count][0];

        short8 outputVal = {val.s0, val.s1, val.s2, val.s3, val.s4, val.s5, val.s6, val.s7 };
        outputVal -= 128;

    *outputRowPtr = outputVal;

        // increment to same offset in next row
        inputRowPtr+=imginfo_width/8;
    }
    return;
}

#endif
