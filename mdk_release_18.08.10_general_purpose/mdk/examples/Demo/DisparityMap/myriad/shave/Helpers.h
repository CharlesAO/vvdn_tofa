///
/// @file Helpers.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief Helpers Interface
///
#ifndef HELPERS_FUNCTIONS_H
#define HELPERS_FUNCTIONS_H

#include <mv_types.h>
#include <mvcv_types.h>

void addPadding(u8 *pointer, u32 width, u32 paddingSize);
void alignLine(u8 *pointer, u32 width, u32 paddingSize);
void alignLineU32(u32 *pointer, u32 width, u32 paddingSize);
void rotateBuffers(u8* ptrs[], u32 num);
void rotateBuffersOnePosition(u8* ptrs[], u32 num);

#endif // HELPERS_FUNCTIONS_H