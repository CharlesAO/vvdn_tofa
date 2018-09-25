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
#undef  ISPC_BAYER_W_IMX214
#undef  ISPC_BAYER_H_IMX214

#undef  ISPC_BAYER_W_IMX378
#undef  ISPC_BAYER_H_IMX378

#define ISPC_BAYER_W_IMX214   2080
#define ISPC_BAYER_H_IMX214   1564

#define ISPC_BAYER_W_IMX378   4032
#define ISPC_BAYER_H_IMX378   3048


//=============================================================
#include "isp_params_still.h"

//Patch image resolution

#undef  ISPC_BAYER_W_STILL_IMX214
#undef  ISPC_BAYER_H_STILL_IMX214

#undef  ISPC_BAYER_W_STILL_IMX378
#undef  ISPC_BAYER_H_STILL_IMX378

#define ISPC_BAYER_W_STILL_IMX214  ISPC_BAYER_W_IMX214
#define ISPC_BAYER_H_STILL_IMX214  ISPC_BAYER_H_IMX214

#define ISPC_BAYER_W_STILL_IMX378  ISPC_BAYER_W_IMX378
#define ISPC_BAYER_H_STILL_IMX378  ISPC_BAYER_H_IMX378

//=============================================================
//Octave dumps don't include fp16 LUT table, so updated PC test to dump
#define GammaLutTable 512*4
extern uint16_t gammaLutFp16[GammaLutTable];

#endif
