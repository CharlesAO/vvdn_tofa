#ifndef _CONVSEPARABELE7x7FP16TOFP16_ASM_TEST_H_
#define _CONVSEPARABELE7x7FP16TOFP16_ASM_TEST_H_

#include "half.h"

extern unsigned int convSeparable7x7Fp16ToFp16CycleCount;
void convSeparable7x7Fp16ToFp16_asm_test(half** out, half** in, half conv[4], unsigned int width);

#endif //_CONVSEPARABELE7x7FP16TOFP16_ASM_TEST_H_
