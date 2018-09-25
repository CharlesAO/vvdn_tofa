#ifndef _disp2depth_ASM_TEST_H_
#define _disp2depth_ASM_TEST_H_

#include "half.h"

extern unsigned int disp2depthCycleCount;

void disp2depth_asm_test(unsigned char* in, half* out, half* lut_table, unsigned int flip_disp2depth, unsigned int width);

#endif //_disp2depth_ASM_TEST_H_
