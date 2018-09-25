#ifndef _convolution7x1Fp16ToFp16_ASM_TEST_H_
#define _convolution7x1Fp16ToFp16_ASM_TEST_H_

#include "half.h"

extern unsigned int convolution7x1Fp16ToFp16CycleCount;
void convolution7x1Fp16ToFp16_asm_test(half** in, half* out, half* conv, unsigned int width);

#endif //_convolution7x1Fp16ToFp16_ASM_TEST_H_
