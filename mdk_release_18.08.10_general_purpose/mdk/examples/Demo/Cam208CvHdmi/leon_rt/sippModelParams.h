/*
 * sippModelParams.h
 *
 *  Created on: Dec 23, 2014
 */

#ifndef LEON_RT_SIPPMODELPARAMS_H_
#define LEON_RT_SIPPMODELPARAMS_H_
#include <stdint.h>
#include "sippPlatform.h"

extern Int8   coefX[6];
extern Int8   coefY[6];

// Define the 3x3 erosion Matrix
extern UInt8 erodeMat[3*3];

extern UInt8 dilateMat[3*3];

// Define the gamma curve LUT
//The LUT contains u8 data, but LUT storage must always be u16
extern UInt16 lutTab[256];
extern uint32_t iBuf[2];
extern uint32_t oBuf[2];

// Input/output buffers' format
#define SIPP_FORMAT_8BIT    0x1
#define SIPP_FORMAT_16BIT   0x2
#define SIPP_FORMAT_32BIT   0x4
#define SIPP_FORMAT_PACK10  0x5
#define SIPP_FORMAT_PACK12  0x3

#define LUT_IN_FORMAT       SIPP_FORMAT_16BIT
#define LUT_LD_FORMAT       SIPP_FORMAT_16BIT
#define LUT_OUT_FORMAT      SIPP_FORMAT_8BIT


#endif /* LEON_RT_SIPPMODELPARAMS_H_ */
