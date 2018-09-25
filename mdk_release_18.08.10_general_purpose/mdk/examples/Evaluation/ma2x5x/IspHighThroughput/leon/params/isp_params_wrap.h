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
#include "isp_params_still.h"

//=============================================================
//Octave dumps don't include fp16 LUT table, so updated PC test to dump
extern uint16_t gammaLutFp16[512*4];

#endif
