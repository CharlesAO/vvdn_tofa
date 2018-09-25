#ifndef _CONVOLUTION7x7Fp16ToFp16_ASM_TEST_H_
#define _CONVOLUTION7x7Fp16ToFp16_ASM_TEST_H_

#include "half.h"

extern unsigned int convolution7x7Fp16ToFp16CycleCount;
void Convolution7x7Fp16ToFp16_asm_test(half** in, half** out, half conv[49], unsigned int width);

#endif //_CONVOLUTION7x7_ASM_TEST_H_
