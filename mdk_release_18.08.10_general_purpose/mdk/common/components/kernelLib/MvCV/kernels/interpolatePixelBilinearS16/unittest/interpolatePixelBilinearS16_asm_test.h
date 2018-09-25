#ifndef _INTERPOLATEPIXELBILINEARS16_ASM_TEST_H_
#define _INTERPOLATEPIXELBILINEARS16_ASM_TEST_H_

extern unsigned int cycleCount;

void interpolatePixelBilinearS16_asm_test(short* line1, short* line2, float x, float y, float* result, unsigned int width);

#endif //_INTERPOLATEPIXELBILINEARS16_ASM_TEST_H_
