#ifndef _sLaplacian3x3_ASM_TEST_H_
#define _sLaplacian3x3_ASM_TEST_H_

#include "half.h"

extern unsigned int sLaplacian3x3CycleCount;
void sLaplacian3x3_asm_test(unsigned char** in, unsigned char** out, unsigned int width);

#endif //_sLaplacian3x3_ASM_TEST_H_
