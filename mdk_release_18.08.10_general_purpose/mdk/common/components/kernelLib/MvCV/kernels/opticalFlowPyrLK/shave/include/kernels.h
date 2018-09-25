#ifndef __KERNELS_H__
#define __KERNELS_H__

#include <mvcv_types.h>
#include "calcG.h"
#include "calcBxBy.h"
#include "bilinearInterpolation_u16.h"
#include "bilinearInterpolationAligned_u16.h"

#include "conv3x3fp32Scharr.h"

#ifdef __PC__
    #define mvcvCalcG_asm                       mvcvCalcG
    #define mvcvCalcBxBy_asm                    mvcvCalcBxBy
    #define mvcvConv3x3fp32Scharr_asm           mvcvConv3x3fp32Scharr
    #define mvcvBilinearInterpolation16u_asm    mvcvBilinearInterpolation16u
    #define mvcvBilinearInterpolationAligned16u_asm    mvcvBilinearInterpolation16u

#endif

#ifdef __MOVICOMPILE__
// Asm runs implicitly on Myriad - enable the macros below to switch to C
// #define mvcvCalcG_asm                       mvcvCalcG
// #define mvcvCalcBxBy_asm                    mvcvCalcBxBy
// #define mvcvConv3x3fp32Scharr_asm           mvcvConv3x3fp32Scharr
// #define mvcvBilinearInterpolation16u_asm    mvcvBilinearInterpolation16u
#endif


static inline float fastSqrt(float x)
{
    union {
        float x;
        int  i;
    } u;

    float xhalf = 0.5f*x;

    u.x = x ;

    // Give initial guess
    u.i = 0x5f3759df - (u.i >> 1);

    // Newton step
    u.x = u.x * (1.5f  - xhalf*u.x*u.x);

    return (float)(x*u.x);
}
#endif
