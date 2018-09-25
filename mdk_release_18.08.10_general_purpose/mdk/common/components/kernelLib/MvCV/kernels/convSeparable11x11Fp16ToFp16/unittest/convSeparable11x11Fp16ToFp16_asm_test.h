#ifndef _CONVSEPARABELE11x11FP16TOFP16_ASM_TEST_H_
#define _CONVSEPARABELE11x11FP16TOFP16_ASM_TEST_H_

#include "half.h"

extern unsigned int convSeparable11x11Fp16ToFp16CycleCount;
void convSeparable11x11Fp16ToFp16_asm_test(half** out, half** in, half conv[6], unsigned int width);

#endif //_CONVSEPARABELE11x11FP16TOFP16_ASM_TEST_H_
