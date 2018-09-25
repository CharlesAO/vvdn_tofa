#ifndef _randNoiseFp16_asm_test_H_
#define _randNoiseFp16_asm_test_H_

#include "half.h"

extern unsigned int cycleCount;
void genChromaFp16_asm_test(unsigned char* outC[3], unsigned short* inRGB[3], half *inY, float eps, float normVal, unsigned int width);

#endif //_randNoiseFp16_asm_test_H_
