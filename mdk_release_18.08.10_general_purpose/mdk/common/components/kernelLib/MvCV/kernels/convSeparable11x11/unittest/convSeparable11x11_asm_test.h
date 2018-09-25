#ifndef _CONVSEPARABELE11x11_ASM_TEST_H_
#define _CONVSEPARABELE11x11_ASM_TEST_H_

#include "half.h"

extern unsigned int convSeparable11x11CycleCount;
void convSeparable11x11_asm_test(unsigned char** out, unsigned char** in, float conv[6], unsigned int width);

#endif //_CONVSEPARABELE11x11_ASM_TEST_H_
