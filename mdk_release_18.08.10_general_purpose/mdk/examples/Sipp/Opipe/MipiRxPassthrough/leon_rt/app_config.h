///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon header
///
#ifndef _PARAMS_H_
#define _PARAMS_H_
// 1: Includes
// ----------------------------------------------------------------------------
#include "Opipe.h"
// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define IMG_W 512
#define IMG_H 128
    
#define ISPC_BAYER_BITS	10
#define BAYER_PATTERN   RGGB

// 3: Static Local Data
// ----------------------------------------------------------------------------
// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
void appSetParams(Opipe *p);
#endif
