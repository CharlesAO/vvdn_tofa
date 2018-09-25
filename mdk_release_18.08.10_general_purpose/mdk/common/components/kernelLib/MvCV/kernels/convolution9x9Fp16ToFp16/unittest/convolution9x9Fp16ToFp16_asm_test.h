#ifndef _CONVOLUTION9x9Fp16ToFp16_ASM_TEST_H_
#define _CONVOLUTION9x9Fp16ToFp16_ASM_TEST_H_

#include "half.h"

extern unsigned int convolution9x9Fp16ToFp16CycleCount;
void Convolution9x9Fp16ToFp16_asm_test(half** in, half** out, half conv[81], unsigned int width);

#endif //_CONVOLUTION9x9Fp16ToFp16_ASM_TEST_H_
