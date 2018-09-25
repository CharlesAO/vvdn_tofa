#ifndef _ConvertFp16ToQ4_ASM_TEST_H_
#define _ConvertFp16ToQ4_ASM_TEST_H_

#include "half.h"

extern unsigned int ConvertFp16ToQ4CycleCount;

void ConvertFp16ToQ4_asm_test(half* in, unsigned short* out, unsigned int width);

#endif //_ConvertFp16ToQ4_ASM_TEST_H_
