#ifndef _awbStatsSaturatedU32_asm_test_H_ 
#define _awbStatsSaturatedU32_asm_test_H_ 

#include "half.h"

extern unsigned int cycleCount;
void awbStatsSaturatedU32_asm_test(unsigned int *out, unsigned short *in, unsigned int nPaxelX, unsigned int gapPaxelX, unsigned int widthPaxelX, unsigned int satPaxelX, unsigned int width);

#endif //_awbStatsSaturatedU32_asm_test_H_
