#ifndef _genLumaFp16_asm_test_H_
#define _genLumaFp16_asm_test_H_

#include "half.h"

extern unsigned int cycleCount;
void genLumaFp16_asm_test(half *outY, unsigned short* inR, unsigned short *inG, unsigned short *inB, float normVal, int width);

#endif //_genLumaFp16_asm_test_H_
