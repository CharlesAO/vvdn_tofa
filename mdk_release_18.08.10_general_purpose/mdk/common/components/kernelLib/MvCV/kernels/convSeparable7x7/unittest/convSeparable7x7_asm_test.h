#ifndef _CONVSEPARABELE7x7_ASM_TEST_H_
#define _CONVSEPARABELE7x7_ASM_TEST_H_

#include "half.h"

extern unsigned int convSeparable7x7CycleCount;
void convSeparable7x7_asm_test(unsigned char** out, unsigned char** in, float conv[4], unsigned int width);

#endif //_CONVSEPARABELE7x7_ASM_TEST_H_
