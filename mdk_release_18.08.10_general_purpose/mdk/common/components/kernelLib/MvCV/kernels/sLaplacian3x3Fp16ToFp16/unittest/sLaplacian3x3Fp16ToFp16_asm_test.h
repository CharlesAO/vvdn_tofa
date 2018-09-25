#ifndef _sLaplacian3x3Fp16ToFp16_ASM_TEST_H_
#define _sLaplacian3x3Fp16ToFp16_ASM_TEST_H_

#include "half.h"

extern unsigned int sLaplacian3x3Fp16ToFp16CycleCount;
void sLaplacian3x3Fp16ToFp16_asm_test(half** in, half** out, unsigned int width);

#endif //_sLaplacian3x3Fp16ToFp16_ASM_TEST_H_