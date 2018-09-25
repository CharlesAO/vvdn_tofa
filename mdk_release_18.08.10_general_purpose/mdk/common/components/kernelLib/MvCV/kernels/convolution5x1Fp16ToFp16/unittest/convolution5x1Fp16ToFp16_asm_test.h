#ifndef _CONVOLUTION5x1Fp16ToFp16_ASM_TEST_H_
#define _CONVOLUTION5x1Fp16ToFp16_ASM_TEST_H_

#include "half.h"

extern unsigned int convolution5x1Fp16ToFp16CycleCount;
void Convolution5x1Fp16ToFp16_asm_test(half** in, half** out, half conv[5], unsigned int width);

#endif //_CONVOLUTION5x1_ASM_TEST_H_
