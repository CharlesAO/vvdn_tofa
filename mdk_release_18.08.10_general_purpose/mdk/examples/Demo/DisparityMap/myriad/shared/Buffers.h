///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///
// This header is left unguarded on purpose because it should be included only
// once in the entire project

#include <mv_types.h>
#include <Sections.h>
#include <Defines.h>

#define USED_BUF	5

u8 DDR_BSS      frameBufferLeft[MAX_WIDTH * MAX_HEIGHT];
u8 DDR_BSS      frameBufferRight[MAX_WIDTH * MAX_HEIGHT];
