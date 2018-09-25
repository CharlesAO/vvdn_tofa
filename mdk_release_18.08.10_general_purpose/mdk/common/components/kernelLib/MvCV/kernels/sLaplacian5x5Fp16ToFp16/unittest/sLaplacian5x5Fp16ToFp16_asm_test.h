#ifndef _sLaplacian5x5Fp16ToFp16_ASM_TEST_H_
#define _sLaplacian5x5Fp16ToFp16_ASM_TEST_H_

#include "half.h"

extern unsigned int sLaplacian5x5Fp16ToFp16CycleCount;
void sLaplacian5x5Fp16ToFp16_asm_test(half** in, half** out, unsigned int width);

#endif //_sLaplacian5x5Fp16ToFp16_ASM_TEST_H_
