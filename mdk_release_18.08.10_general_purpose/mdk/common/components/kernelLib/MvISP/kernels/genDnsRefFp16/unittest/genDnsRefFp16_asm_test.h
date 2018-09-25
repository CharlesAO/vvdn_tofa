#ifndef _genDnsRefFp16_asm_test_H_
#define _genDnsRefFp16_asm_test_H_

#include "half.h"
#include "genDnsRefFp16.h"

extern unsigned int cycleCount;
void genDnsRefFp16_asm_test(half *inY, unsigned int yc, 
    int x0,  YDnsRefFp16Param* param, 
    unsigned char* outRef, unsigned int width);

#endif //_genDnsRefFp16_asm_test_H_
