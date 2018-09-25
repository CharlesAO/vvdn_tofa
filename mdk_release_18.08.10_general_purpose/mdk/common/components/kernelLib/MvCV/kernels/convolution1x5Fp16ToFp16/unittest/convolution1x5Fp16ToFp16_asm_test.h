#ifndef _CONVOLUTION1x5Fp16ToFp16_ASM_TEST_H_
#define _CONVOLUTION1x5Fp16ToFp16_ASM_TEST_H_

#include "half.h"

extern unsigned int convolution1x5Fp16ToFp16CycleCount;
void Convolution1x5Fp16ToFp16_asm_test(half** in, half** out, half conv[5], unsigned int width);

#endif //_CONVOLUTION1x5_ASM_TEST_H_
