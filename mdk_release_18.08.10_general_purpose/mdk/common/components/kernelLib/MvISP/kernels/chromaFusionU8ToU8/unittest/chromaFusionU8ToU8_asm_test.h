#ifndef _ChromaFusionU8ToU8_ASM_TEST_H_
#define _ChromaFusionU8ToU8_ASM_TEST_H_

#include "half.h"

extern unsigned int ChromaFusionU8ToU8CycleCount;
void ChromaFusionU8ToU8_asm_test(unsigned char* inCRshort, unsigned char* inCGshort, 
unsigned char* inCBshort, unsigned char* inCRlong, unsigned char* inCGlong, 
unsigned char* inCBlong, half* inAlpha, unsigned char* outR, unsigned char* outG,
 unsigned char* outB, unsigned int width);

#endif //_ChromaFusionU8toU8_ASM_TEST_H_
