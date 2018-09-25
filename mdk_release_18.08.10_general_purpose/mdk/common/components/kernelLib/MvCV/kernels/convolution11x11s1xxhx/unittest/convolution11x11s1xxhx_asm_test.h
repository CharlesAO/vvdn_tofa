#ifndef _CONVOLUTION11x11S1XXHX_ASM_TEST_H_
#define _CONVOLUTION11x11S1XXHX_ASM_TEST_H_

#include "half.h"

extern unsigned int Convolution11x11s1xxhxCycleCount;
void Convolution11x11s1xxhx_asm_test(unsigned char** in, unsigned char** out, u8 conv[121], unsigned int width);

#endif //_CONVOLUTION11x11S1XXHX_ASM_TEST_H_
