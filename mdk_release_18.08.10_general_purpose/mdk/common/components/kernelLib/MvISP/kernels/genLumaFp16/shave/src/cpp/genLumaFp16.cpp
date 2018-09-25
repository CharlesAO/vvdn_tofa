///
/// @file      genLumaFp16.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Extract Luma channel from RGB input data
///
#include <mv_types.h>
#include "genLumaFp16.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void mvispGenLumaFp16(half *outY, unsigned short* inR, unsigned short *inG, unsigned short *inB, float normVal, int width)
{
    int i;

    for(i=0; i<width; i++) 
    {
        //outY[i] = (inR[i]*0.25f + inG[i]*0.50f + inB[i]*0.25f);
        outY[i] = (half)( (float)(inR[i] + inG[i] + inG[i] + inB[i])*(normVal*0.25f));
    }
}

