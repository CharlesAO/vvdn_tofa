#ifndef _GAUSSVX2_FP16_ASM_TEST_H_
#define _GAUSSVX2_FP16_ASM_TEST_H_

#include "half.h"

extern unsigned int GaussVx2_fp16CycleCount;
void GaussVx2_fp16_asm_test(half** in, half** out, unsigned int width);

#endif //_GAUSSVX2_FP16_ASM_TEST_H_
