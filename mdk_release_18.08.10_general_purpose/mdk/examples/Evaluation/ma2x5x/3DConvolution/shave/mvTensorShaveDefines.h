///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Simple effect header
///

#ifndef __MV_TENSOR_SHV_DEF_H__
#define __MV_TENSOR_SHV_DEF_H__

// Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"

// Include MvCV kerneles for Convolutions and Accumulate
#include <convolution3x3Fp16ToFp16.h>
#include <convolution5x5Fp16ToFp16.h>
#include <convolution7x7Fp16ToFp16.h>
#include <convolution9x9Fp16ToFp16.h>
#include <convolution3x3s2hhhh.h>
#include <convolution3x3s3hhhh.h>
#include <convolution3x3s4hhhh.h>
#include <convolution3x3s8hhhh.h>
#include <convolution5x5s2hhhh.h>
#include <convolution5x5s3hhhh.h>
#include <convolution5x5s4hhhh.h>
#include <convolution5x5s8hhhh.h>
#include <convolution7x7s2hhhh.h>
#include <convolution7x7s4hhhh.h>
#include <convolution7x7s8hhhh.h>
#include <convolution9x9s2hhhh.h>
#include <convolution9x9s3hhhh.h>
#include <convolution9x9s4hhhh.h>
#include <convolution9x9s8hhhh.h>
#include <convolution11x11s1hhhh.h>
#include <convolution11x11s2hhhh.h>
#include <convolution11x11s4hhhh.h>
#include <convolution11x11s8hhhh.h>

#include <accumulateFp16.h>

#include "../shared/mvTensorApi.h"

// Defines
// ----------------------------------------------------------------------------
#define MAX_KERNEL_SIZE 11
typedef half input_t;
typedef half output_t;
typedef half taps_t;

typedef void (*FUNCPTR_T)(void **a,
                          void **b,
                          void  *c,
                          u32 width);

static FUNCPTR_T convMatrix[SIZE_NO][STRIDE_NO][TYPE_NO] =
{
    {
        //size 3x3
        {NULL, (FUNCPTR_T)mvcvConvolution3x3Fp16ToFp16_asm, NULL} ,
        {NULL, (FUNCPTR_T)mvcvConvolution3x3s2hhhh_asm,     NULL},
        {NULL, (FUNCPTR_T)mvcvConvolution3x3s3hhhh_asm,     NULL},
        {NULL, (FUNCPTR_T)mvcvConvolution3x3s4hhhh_asm,     NULL},
        {NULL, (FUNCPTR_T)mvcvConvolution3x3s8hhhh_asm,     NULL}
    },

    {   //size 5x5
        {NULL, (FUNCPTR_T)mvcvConvolution5x5Fp16ToFp16_asm, NULL},
        {NULL, (FUNCPTR_T)mvcvConvolution5x5s2hhhh_asm,     NULL},
        {NULL, (FUNCPTR_T)mvcvConvolution5x5s3hhhh_asm,     NULL},
        {NULL, (FUNCPTR_T)mvcvConvolution5x5s4hhhh_asm,     NULL},
        {NULL, (FUNCPTR_T)mvcvConvolution5x5s8hhhh_asm,     NULL}
    },

    {   //size 7x7
        {NULL, (FUNCPTR_T)mvcvConvolution7x7Fp16ToFp16_asm, NULL},
        {NULL, (FUNCPTR_T)mvcvConvolution7x7s2hhhh_asm,     NULL},
        {NULL, NULL,                             NULL},
        {NULL, (FUNCPTR_T)mvcvConvolution7x7s4hhhh_asm,     NULL},
        {NULL, (FUNCPTR_T)mvcvConvolution7x7s8hhhh_asm,     NULL}
    },

    {   //size 9x9
        {NULL, (FUNCPTR_T)mvcvConvolution9x9Fp16ToFp16_asm, NULL},
        {NULL, (FUNCPTR_T)mvcvConvolution9x9s2hhhh_asm,     NULL},
        {NULL, (FUNCPTR_T)mvcvConvolution9x9s3hhhh_asm,     NULL},
        {NULL, (FUNCPTR_T)mvcvConvolution9x9s4hhhh_asm,     NULL},
        {NULL, (FUNCPTR_T)mvcvConvolution9x9s8hhhh_asm,     NULL}
    },

    {   // size 11x11
        {NULL, (FUNCPTR_T)mvcvConvolution11x11s1hhhh_asm, NULL},
        {NULL, (FUNCPTR_T)mvcvConvolution11x11s2hhhh_asm, NULL},
        {NULL, (FUNCPTR_T)mvcvConvolution11x11s4hhhh_asm, NULL},
        {NULL, (FUNCPTR_T)mvcvConvolution11x11s8hhhh_asm, NULL}
    }
};

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

#endif//__MV_TENSOR_SHV_DEF_H__
