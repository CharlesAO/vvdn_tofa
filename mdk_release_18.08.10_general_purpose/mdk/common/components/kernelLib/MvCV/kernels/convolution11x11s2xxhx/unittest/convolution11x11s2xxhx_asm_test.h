#ifndef _CONVOLUTION11x11S2XXHX_ASM_TEST_H_
#define _CONVOLUTION11x11S2XXHX_ASM_TEST_H_

#include "half.h"

extern unsigned int Convolution11x11s2xxhxCycleCount;
void Convolution11x11s2xxhx_asm_test(unsigned char** in, unsigned char** out, u8 conv[121], unsigned int width);

#endif //_CONVOLUTION11x11S2XXHX_ASM_TEST_H_
