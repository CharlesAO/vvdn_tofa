#ifndef _CONVSEPARABELE5x5FP16TOFP16_ASM_TEST_H_
#define _CONVSEPARABELE5x5FP16TOFP16_ASM_TEST_H_

#include "half.h"

extern unsigned int convSeparable5x5Fp16ToFp16CycleCount;
void convSeparable5x5Fp16ToFp16_asm_test(half** out, half** in, half conv[3], unsigned int width);

#endif //_CONVSEPARABELE5x5FP16TOFP16_ASM_TEST_H_
