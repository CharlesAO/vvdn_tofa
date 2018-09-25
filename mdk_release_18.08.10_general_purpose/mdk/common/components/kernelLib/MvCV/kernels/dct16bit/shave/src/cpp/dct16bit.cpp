#include "dct16bit.h"
#include <stdlib.h>
#include <stdio.h>

/******************************************************************************
 **  dct
 **  --------------------------------------------------------------------------
 **  Fast DCT - Discrete Cosine Transform.
 **  This function converts 8x8 pixel block into frequencies.
 **  Lowest frequencies are at the upper-left corner.
 **  The input and output could point at the same array, in this case the data
 **  will be overwritten.
 **
 **
 **  ARGUMENTS:
 **      pixels  - 8x8 pixel array;
 **      data    - 8x8 freq block;
 **
 **  RETURN: -
 ******************************************************************************/
void mvcvDCT16bit(short pixels[8][8], short data[8][8])

{
    short rows[8][8];
    unsigned i;


    static const short // Ci = cos(i*PI/16)*sqrt(2)*(1 << 13);
    C1 = 11363,
    C2 = 10703,
    C3 = 9633,
    C5 = 6436,
    C6 = 4433,
    C7 = 2260;
    short kRound11Bit = 1 << 10;
    short kRound15Bit = 1 << 14;
    // simple but fast DCT - 20*16 multiplication 34*16 additions and 8*16 shifts.

    /* transform rows */
    for (i = 0; i < 8; i++)
    {
        int s07, s16, s25, s34, s0734, s1625;
        int d07, d16, d25, d34, d0734, d1625;

        s07 = pixels[i][0] + pixels[i][7];
        d07 = pixels[i][0] - pixels[i][7];
        s16 = pixels[i][1] + pixels[i][6];
        d16 = pixels[i][1] - pixels[i][6];
        s25 = pixels[i][2] + pixels[i][5];
        d25 = pixels[i][2] - pixels[i][5];
        s34 = pixels[i][3] + pixels[i][4];
        d34 = pixels[i][3] - pixels[i][4];

        rows[i][1] =
            (C1 * d07 + C3 * d16 + C5 * d25 + C7 * d34 + kRound11Bit) >> 11;
        rows[i][3] =
            (C3 * d07 - C7 * d16 - C1 * d25 - C5 * d34 + kRound11Bit) >> 11;
        rows[i][5] =
            (C5 * d07 - C1 * d16 + C7 * d25 + C3 * d34 + kRound11Bit) >> 11;
        rows[i][7] =
            (C7 * d07 - C5 * d16 + C3 * d25 - C1 * d34 + kRound11Bit) >> 11;

        s0734 = s07 + s34;
        d0734 = s07 - s34;
        s1625 = s16 + s25;
        d1625 = s16 - s25;

        rows[i][0] = (s0734 + s1625) << 2;
        rows[i][4] = (s0734 - s1625) << 2;

        rows[i][2] = (C2 * d0734 + C6 * d1625 + kRound11Bit) >> 11;
        rows[i][6] = (C6 * d0734 - C2 * d1625 + kRound11Bit) >> 11;
    }

    /* transform columns */
    for (i = 0; i < 8; i++)
    {
        int s07, s16, s25, s34, s0734, s1625;
        int d07, d16, d25, d34, d0734, d1625;


        s07 = rows[0][i] + rows[7][i];
        d07 = rows[0][i] - rows[7][i];
        s16 = rows[1][i] + rows[6][i];
        d16 = rows[1][i] - rows[6][i];
        s25 = rows[2][i] + rows[5][i];
        d25 = rows[2][i] - rows[5][i];
        s34 = rows[3][i] + rows[4][i];
        d34 = rows[3][i] - rows[4][i];

        data[1][i] =
            (C1 * d07 + C3 * d16 + C5 * d25 + C7 * d34 + kRound15Bit) >> 18;
        data[3][i] =
            (C3 * d07 - C7 * d16 - C1 * d25 - C5 * d34 + kRound15Bit) >> 18;
        data[5][i] =
            (C5 * d07 - C1 * d16 + C7 * d25 + C3 * d34 + kRound15Bit) >> 18;
        data[7][i] =
            (C7 * d07 - C5 * d16 + C3 * d25 - C1 * d34 + kRound15Bit) >> 18;

        s0734 = s07 + s34;
        d0734 = s07 - s34;
        s1625 = s16 + s25;
        d1625 = s16 - s25;

        data[0][i] = (s0734 + s1625) >> 5;
        data[4][i] = (s0734 - s1625) >> 5;

        data[2][i] = (C2 * d0734 + C6 * d1625 + kRound15Bit) >> 18;
        data[6][i] = (C6 * d0734 - C2 * d1625 + kRound15Bit) >> 18;
    }
    return;
}