#ifndef _gtmFp16ToFp16_asm_test_H_ 
#define _gtmFp16ToFp16_asm_test_H_ 

#include "half.h"

extern unsigned int cycleCount;
void gtmFp16ToFp16_asm_test(half *in, half *out, unsigned int width, half *scurve, int nPoints);

#endif //_gtmFp16ToFp16_asm_test_H_
