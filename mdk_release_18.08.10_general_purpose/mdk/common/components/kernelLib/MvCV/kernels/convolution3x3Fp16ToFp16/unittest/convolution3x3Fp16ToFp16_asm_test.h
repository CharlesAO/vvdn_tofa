#ifndef _CONVOLUTION3X3Fp16ToFp16_ASM_TEST_H_
#define _CONVOLUTION3X3Fp16ToFp16_ASM_TEST_H_

#include "half.h"

extern unsigned int convolution3x3Fp16ToFp16CycleCount;
void Convolution3x3Fp16ToFp16_asm_test(half** in, half** out, half conv[9], unsigned int width);

#endif //_CONVOLUTION3X3Fp16ToFp16_ASM_TEST_H_
