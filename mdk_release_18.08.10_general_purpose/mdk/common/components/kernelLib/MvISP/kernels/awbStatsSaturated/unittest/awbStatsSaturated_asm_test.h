#ifndef _awbStatsSaturated_asm_test_H_ 
#define _awbStatsSaturated_asm_test_H_ 

#include "half.h"

extern unsigned int cycleCount;
void awbStatsSaturated_asm_test(unsigned char *out, unsigned short *in, unsigned int nPaxelX, unsigned int gapPaxelX, unsigned int widthPaxelX, unsigned int satPaxelX, unsigned int width);

#endif //_awbStatsSaturated_asm_test_H_
