#ifndef _CONVOLUTIONSEPARABLE3x3_ASM_TEST_H_
#define _CONVOLUTIONSEPARABLE3x3_ASM_TEST_H_

#include "half.h"

extern unsigned int cycleCount;
void convSeparable3x3_asm_test(unsigned char* out, unsigned char** in, float conv[2], unsigned int width);

#endif //_CONVOLUTIONSEPARABLE3x3_ASM_TEST_H_
