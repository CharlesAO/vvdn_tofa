#ifndef _randNoiseFp16_asm_test_H_
#define _randNoiseFp16_asm_test_H_

#include "half.h"

extern unsigned int cycleCount;
void randNoiseFp16_asm_test(half *output, half *input, float noiseStrength, unsigned int width);

#endif //_randNoiseFp16_asm_test_H_
