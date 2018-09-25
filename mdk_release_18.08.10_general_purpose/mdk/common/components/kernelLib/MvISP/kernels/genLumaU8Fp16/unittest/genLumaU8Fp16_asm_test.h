#ifndef _genLumaU8Fp16_asm_test_H_
#define _genLumaU8Fp16_asm_test_H_

#include "half.h"

extern unsigned int cycleCount;
void genLumaU8Fp16_asm_test(unsigned char* inR, unsigned char* inG, unsigned char *inB, half *outY, half coefs[3], unsigned int width);

#endif //_genLumaU8Fp16_asm_test_H_
