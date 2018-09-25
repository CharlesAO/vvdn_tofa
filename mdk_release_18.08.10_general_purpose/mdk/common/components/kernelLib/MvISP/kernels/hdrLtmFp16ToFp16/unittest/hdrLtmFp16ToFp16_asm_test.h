#ifndef _hdrLtmFp16ToFp16_asm_test_H_
#define _hdrLtmFp16ToFp16_asm_test_H_

#include "half.h"

extern unsigned int cycleCount;
void hdrLtmFp16ToFp16_asm_test(half* inLumaFusion, half* inGaussUp, half* output, unsigned int width, float* params);

#endif //_hdrLtmFp16ToFp16_asm_test_H_
