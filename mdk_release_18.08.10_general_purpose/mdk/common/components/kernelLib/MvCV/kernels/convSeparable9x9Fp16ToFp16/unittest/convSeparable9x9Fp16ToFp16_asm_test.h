#ifndef _CONVSEPARABELE9x9FP16TOFP16_ASM_TEST_H_
#define _CONVSEPARABELE9x9FP16TOFP16_ASM_TEST_H_

#include "half.h"

extern unsigned int convSeparable9x9Fp16ToFp16CycleCount;
void convSeparable9x9Fp16ToFp16_asm_test(half** out, half** in, half conv[5], unsigned int width);

#endif //_CONVSEPARABELE9x9FP16TOFP16_ASM_TEST_H_
