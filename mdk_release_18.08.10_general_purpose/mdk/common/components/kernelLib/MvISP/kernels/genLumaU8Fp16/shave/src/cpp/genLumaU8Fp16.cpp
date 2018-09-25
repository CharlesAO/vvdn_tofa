///
/// @file      genLumaU8Fp16.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Generate Luma from RGB short tint (U8) input data in Fp16 output data
///
#include <mv_types.h>
#include "genLumaU8Fp16.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void mvispGenLumaU8Fp16(u8* inR, u8 *inG, u8 *inB, half *outY, half* coefs, u32 width)
{
    int i;

    for(i=0; i<(int)width; i++) 
    {
      outY[i] = (inR[i]*coefs[0] + inG[i]*coefs[1] + inB[i]*coefs[2]) * (half)(1.0f/255.0f);
    }
}

