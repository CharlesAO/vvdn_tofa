#ifndef __ISP_PARAMS_WRAP_H__
#define __ISP_PARAMS_WRAP_H__

//=============================================================
#if defined(__sparc)
#define ALIGNED(x) __attribute__((aligned(x)))
#else
#define ALIGNED(x)                //nothing
#endif

//=============================================================
//This should be somewhre in isp_params, but anyway...
#define BAYER_PATTERN  RGGB

//=============================================================
#include "isp_params_pp.h"

//Patch image resolution
#undef  ISPC_BAYER_W
#undef  ISPC_BAYER_H
#define ISPC_BAYER_W   4208
#define ISPC_BAYER_H   3120

#define IMG_W ISPC_BAYER_W
#define IMG_H ISPC_BAYER_H

//=============================================================
#include "isp_params_still.h"

//Patch image resolution
#undef  ISPC_BAYER_W_STILL
#undef  ISPC_BAYER_H_STILL
#define ISPC_BAYER_W_STILL  ISPC_BAYER_W
#define ISPC_BAYER_H_STILL  ISPC_BAYER_H

//=============================================================
//Octave dumps don't include fp16 LUT table, so updated PC test to dump
extern uint16_t gammaLutFp16[512*4];

#endif
