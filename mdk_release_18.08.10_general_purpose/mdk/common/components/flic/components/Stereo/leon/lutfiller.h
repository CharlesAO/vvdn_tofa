/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     This file contains the prototypes for LUT filling function.
///

#ifndef __LUT_FILLER_H__
#define __LUT_FILLER_H__

#include <mv_types.h> //For half
#include <sippBaseTypes.h> //For UInt8

//Remember: Always specify a positive number of numFractionalBit, even if _16bitmode is off.
#ifdef __cplusplus
extern "C" {
#endif
void fillDisp2DepthLut(half * LUT, float focal, float baseline, UInt8 numDisp, UInt8 _16bitmode, UInt8 numFractionalBit, UInt8 companding);
#ifdef __cplusplus
}
#endif

#endif //__LUT_FILLER_H__