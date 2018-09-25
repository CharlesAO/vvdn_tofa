#ifndef _censusTransformAverageRef7x7_ASM_TEST_H_
#define _censusTransformAverageRef7x7_ASM_TEST_H_

#include "half.h"

extern unsigned int censusTransformAverageRef7x7CycleCount;
void censusTransformAverageRef7x7_asm_test(unsigned char** in, unsigned long long* out, unsigned int width);

#endif //_censusTransformAverageRef7x7_ASM_TEST_H_
