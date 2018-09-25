///
/// @file shave.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Module used to compute 7x7convoltion
///

#ifndef SHAVE_H
#define SHAVE_H

// 1: Includes
#include <mv_types.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

void shaveRun(u8* inputImage, u8 *outputImage, u32 width, u32 height);

#endif //SHAVE_H
