#ifndef _sLaplacian7x7Fp16ToFp16_ASM_TEST_H_
#define _sLaplacian7x7Fp16ToFp16_ASM_TEST_H_

#include "half.h"

extern unsigned int sLaplacian7x7Fp16ToFp16CycleCount;
void sLaplacian7x7Fp16ToFp16_asm_test(half** in, half** out, unsigned int width);

#endif //_sLaplacian7x7Fp16ToFp16_ASM_TEST_H_
