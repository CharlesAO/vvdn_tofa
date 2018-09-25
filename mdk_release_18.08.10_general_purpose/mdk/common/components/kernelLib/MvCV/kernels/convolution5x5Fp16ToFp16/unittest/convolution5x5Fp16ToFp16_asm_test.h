#ifndef _CONVOLUTION5x5Fp16ToFp16_ASM_TEST_H_
#define _CONVOLUTION5x5Fp16ToFp16_ASM_TEST_H_

#include "half.h"

extern unsigned int convolution5x5Fp16ToFp16CycleCount;
void Convolution5x5Fp16ToFp16_asm_test(half** in, half** out, half conv[25], unsigned int width);

#endif //_CONVOLUTION5x5Fp16ToFp16_ASM_TEST_H_
