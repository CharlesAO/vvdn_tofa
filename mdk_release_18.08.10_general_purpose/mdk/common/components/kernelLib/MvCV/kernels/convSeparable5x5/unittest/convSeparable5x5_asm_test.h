#ifndef _CONVSEPARABELE5x5_ASM_TEST_H_
#define _CONVSEPARABELE5x5_ASM_TEST_H_

#include "half.h"

extern unsigned int convSeparable5x5CycleCount;
void convSeparable5x5_asm_test(unsigned char** out, unsigned char** in, float conv[3], unsigned int width);

#endif //_CONVSEPARABELE5x5_ASM_TEST_H_
