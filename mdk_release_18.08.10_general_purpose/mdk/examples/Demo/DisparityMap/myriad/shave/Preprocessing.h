///
/// @file DisparityMap.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Module used to compute dense disparity map; Disparity Map Interface
///


#ifndef PREPROCESSING_SHAVE_H
#define DISP_MAP_H
#include <swcCdma.h>
#include <mv_types.h>

extern "C" void Preprocessing(fp32* H, fp32* inverseH);

extern "C" void Postprocessing(u8* input, u16* output, u32 width, u32 height);

#endif // PREPROCESSING_SHAVE_H
