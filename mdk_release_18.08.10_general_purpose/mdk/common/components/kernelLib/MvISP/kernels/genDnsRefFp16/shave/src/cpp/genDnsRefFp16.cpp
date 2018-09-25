///
/// @file      genDnsRefFp16.c
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Generate Luma denoise reference plane as input for LumaDenoise kernel. 
///            The reference plane is used for calculating weights to be assigned to 
///            pixels in the neighbourhood
///
#include <mv_types.h>
#include "genDnsRefFp16.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char clampU8L(int in)
{
    if(in< 0)  { return 0; }
    if(in>255) { return 255; }
               return (unsigned char)in;
}

void mvispGenDnsRefFp16(half *inY, unsigned int yc, int x0,  YDnsRefFp16Param* param, unsigned char* outRef, unsigned int width)
{
    int x, xc;
    unsigned char idxLuma;
    for(x=0; x<(int)width; x++)
    {
        xc = (x0 + x);
        xc = xc * xc;
        // convert luma to u8 in order to be use as index in lut
        idxLuma = (unsigned char)(inY[x] * 255.0f);
        unsigned short value = (unsigned short)param->lutGamma[idxLuma] * 
            (unsigned short)param->lutDist[clampU8L(((xc+yc)>>param->shift))];
        value = value >> 8;
        outRef[x] = (unsigned char)value;
    }
}
