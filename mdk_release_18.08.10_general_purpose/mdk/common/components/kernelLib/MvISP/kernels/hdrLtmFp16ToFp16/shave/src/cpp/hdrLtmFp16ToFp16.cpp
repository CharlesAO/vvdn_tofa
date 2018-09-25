///
/// @file      
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// 
///
#include <mv_types.h>
#include "hdrLtmFp16ToFp16.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define chmin(a, b) (((a) < (b)) ? (a) : (b))
#define chmax(a, b) (((a) > (b)) ? (a) : (b))
#define bli_fabs(a)    ((a) <= 0.0 ? -(a) : (a))
#define oneoverln2 1.44269504

void mvispHdrLtmFp16ToFp16(half* inLumaFusion, half* inGaussUp, half* output, u32 width, HdrLtmParam* params)
{
    u32 i;
    half interAux;
    half thr = (half)params->thr;
    half thr2 = (half)params->thr2;
    half thr3 = (half)params->thr3;
    half eps = (half)params->eps;

    for (i = 0; i < width; i++) {
        if (inGaussUp[i] - inLumaFusion[i] >= 0.0f)
            inGaussUp[i] = inLumaFusion[i] + chmin((half)bli_fabs(inGaussUp[i] - inLumaFusion[i]), thr);
        else
            inGaussUp[i] = inLumaFusion[i] - chmin((half)bli_fabs((float)(inGaussUp[i] - inLumaFusion[i])), thr);

        interAux = inGaussUp[i] * thr2 + inLumaFusion[i] - inGaussUp[i];
        output[i] = (__builtin_shave_sau_exp2_f16_l_r(interAux * oneoverln2) - eps) * thr3;

        if (output[i] > 1.0f)
            output[i] = 1.0f;
        else if (output[i] < 0.0f)
            output[i] = 0.0f;
    }
}
