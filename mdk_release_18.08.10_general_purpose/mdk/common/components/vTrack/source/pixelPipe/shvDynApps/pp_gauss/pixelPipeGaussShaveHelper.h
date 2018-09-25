#ifndef PIXEL_PIPE_GAUSS_SHAVE_HELPER_H
#define PIXEL_PIPE_GAUSS_SHAVE_HELPER_H
#include "vTrackPrivateTypes.h"
#include "pixelPipeHelper.h"

void padLine(u8* line, u32 width);
void* swcSolveShaveRelAddr(void* inAddr);
extern "C" 
void PyrLevel(void* params);


#endif
