#ifndef _INTERPOLATEPIXELBILINEAR_ASM_TEST_H_
#define _INTERPOLATEPIXELBILINEAR_ASM_TEST_H_
#include "half.h"

extern unsigned int cycleCount;

void interpolatePixelBilinear_asm_test(half* line1, half* line2, float x, float y, half* result, unsigned int width);

#endif //_INTERPOLATEPIXELBILINEAR_ASM_TEST_H_
