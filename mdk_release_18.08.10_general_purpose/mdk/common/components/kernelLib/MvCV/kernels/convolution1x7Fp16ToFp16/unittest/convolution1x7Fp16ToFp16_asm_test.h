#ifndef _CONVOLUTION1x7Fp16ToFp16_ASM_TEST_H_
#define _CONVOLUTION1x7Fp16ToFp16_ASM_TEST_H_

#include "half.h"

extern unsigned int convolution1x7Fp16ToFp16CycleCount;
void Convolution1x7Fp16ToFp16_asm_test(half* in, half* out, half conv[7], unsigned int width);

#endif //_CONVOLUTION1x7_ASM_TEST_H_
