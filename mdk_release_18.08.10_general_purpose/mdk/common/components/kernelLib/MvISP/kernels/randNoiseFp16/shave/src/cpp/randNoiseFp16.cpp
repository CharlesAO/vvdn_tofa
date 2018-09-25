///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief  generate a random noise on a fp16, [0,1] interval image, in interval +-0.5*strength parameter
///

#include "randNoiseFp16.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __PC__
#include "svuCommonShave.h"
#else
// on PC is simulated running just on shave 0
static int scGetShaveNumber(void)
{
    return 0;
}
#endif

const unsigned int mskBit = ((1<<15) + (1<<13) + (1<<4) + (1<<0)); //0xA011; // other polynom that in examples
half normValue = (half)0.003921509; //(1/255 = 0x1C04)
//
static half clamp01(half x)
{
    if     (x < 0.0f) x = 0.0f;
    else if(x > 1.0f) x = 1.0f;
    return x;
}
// this is one instruction in asm,
unsigned short sauOnesX16(unsigned short in)
{
    int i;
    unsigned short count = 0;
    for (i = 0; i < 16; i++)
    {
        if (((in>>i) & 0x01)==0x1)
        {
            count++;
        }
    }
    return count;
}
//
unsigned short genRand(unsigned short seed)
{
        //unsigned bit  = ((seed >> 15) ^ (seed >> 13) ^ (seed >> 4) ^ (seed >> 0) ) & 1;
        unsigned short bit = sauOnesX16((seed & mskBit)) & 0x1;
        return  ((seed >> 1) | (bit << 15));
}

/// Random noise apply on a fp16 frame (custom size)
/// @param[in] out              - pointer to output line
/// @param[in] in               - array of pointers to input lines
/// @param[in] noiseStrength    - noise strength [0,1]
/// @param[in] width            - width of input line
void mvispRandNoiseFp16(half *output, half *input, float noiseStrength, unsigned int width)
{
    unsigned int i;
    unsigned int j;
    // identify the seed list involve in current shave, in asm read trf with this information
    unsigned short seedlist[12*8] = {
     48983, 45013, 19197, 10539, 13476,  3369, 31472, 22447,
     30400, 22649, 46471, 44385, 60118, 49067, 18879, 42139,
     23493, 18725, 21065, 19968, 12366, 25348, 52182, 63519,
     25325, 21617,   337, 20501, 18752, 29257,   457, 12295,
     27449,  9644, 50556, 31831, 40725, 53234, 49595, 64496,
     12042,  8516, 29204, 24711, 51586, 31526, 18924, 58697,
     23228, 25686, 44433, 44726, 58042, 22242, 50523, 41777,
      6150, 50968,  5916,   369,  2049,  4224, 55504, 58211,
     46402, 45323, 62350, 41020, 54912, 17622, 55249, 30196,
      9648, 50460,  3153, 17172, 44300,  3994, 52286, 58207,
      4872, 13075,  2892, 45101, 32394,  8084, 32543, 54467,
      8175, 25086, 16030, 36775,  7742, 29155, 41415, 32929
    };
    unsigned int shaveId = scGetShaveNumber();
    unsigned short *seed = (unsigned short *)((shaveId * 8/*val*/ * 2/*size of short*/) + ((unsigned char*)seedlist));
    //Noise
    for(i=0; i<width; i+=8)
    {
        for (j = 0; j < 8; j++)
        {
            seed[j] = genRand(seed[j]);
            signed char value = ((signed short) seed[j])>>8;
            half val = ((half)((float)value)) * normValue;
            output[i+j] = clamp01((input[i+j] + (val * (half)input[i+j] * (half)noiseStrength)));
        }
        //printf("\n InLoop%4d: %6x %6x %6x %6x %6x %6x %6x %6x\n",i , seed[0], seed[1], seed[2], seed[3], seed[4], seed[5], seed[6], seed[7]);
    }
    // 3 extra seed calculation calculation,
    // for similitude with asm implementation where this
    // is generated automatically for parallelism reason
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 8; j++)
        {
            seed[j] = genRand(seed[j]);
        }
        //printf("\n OuLoop%4d: %6x %6x %6x %6x %6x %6x %6x %6x\n",i , seed[0], seed[1], seed[2], seed[3], seed[4], seed[5], seed[6], seed[7]);
    }
}
