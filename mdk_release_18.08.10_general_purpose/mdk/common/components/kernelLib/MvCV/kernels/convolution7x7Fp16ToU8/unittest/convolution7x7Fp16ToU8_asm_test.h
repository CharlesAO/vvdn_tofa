#ifndef _CONVOLUTION7x7Fp16ToU8_ASM_TEST_H_
#define _CONVOLUTION7x7Fp16ToU8_ASM_TEST_H_

#include "half.h"

extern unsigned int convolution7x7Fp16ToU8CycleCount;
void Convolution7x7Fp16ToU8_asm_test(half** in, unsigned char** out, half conv[49], unsigned int width);

#endif //_CONVOLUTION7x7Fp16ToU8_ASM_TEST_H_
