#ifndef _CONVSEPARABELE9x9_ASM_TEST_H_
#define _CONVSEPARABELE9x9_ASM_TEST_H_

#include "half.h"

extern unsigned int convSeparable9x9CycleCount;
void convSeparable9x9_asm_test(unsigned char** out, unsigned char** in, float conv[5], unsigned int width);

#endif //_CONVSEPARABELE9x9_ASM_TEST_H_
