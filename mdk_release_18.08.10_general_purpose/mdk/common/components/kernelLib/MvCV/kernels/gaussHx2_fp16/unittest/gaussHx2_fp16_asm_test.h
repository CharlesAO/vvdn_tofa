#ifndef _GAUSSHX2_FP16_ASM_TEST_H_
#define _GAUSSHX2_FP16_ASM_TEST_H_

#include "half.h"

extern unsigned int GaussHx2_fp16CycleCount;
void GaussHx2_fp16_asm_test(half** in, half** out, unsigned int width);

#endif //_GAUSSHX2_FP16_ASM_TEST_H_
