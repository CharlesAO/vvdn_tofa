#ifndef _ResizeAlphaFp16ToFp16_ASM_TEST_H_
#define _ResizeAlphaFp16ToFp16_ASM_TEST_H_

#include "half.h"

extern unsigned int ResizeAlphaFp16ToFp16CycleCount;
void ResizeAlphaFp16ToFp16_asm_test(half** alpha, half* output, unsigned int width);

#endif //_ResizeAlphaFp16ToFp16_ASM_TEST_H_
