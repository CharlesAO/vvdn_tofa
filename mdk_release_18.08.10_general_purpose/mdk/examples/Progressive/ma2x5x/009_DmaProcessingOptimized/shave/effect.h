///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Simple effect header
///

#ifndef __EFFECT_H__
#define __EFFECT_H__

// 1: Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

void Darkness(u8* Yplane, u8* outYplane, int width, int height);
void CopyPlane(u8* in_plane, u8* out_plane, int width, int height);

#endif//__EFFECT_H__
