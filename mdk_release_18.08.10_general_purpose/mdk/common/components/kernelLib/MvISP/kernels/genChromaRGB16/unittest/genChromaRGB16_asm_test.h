#ifndef _genChromaRGB16_ASM_TEST_H_
#define _genChromaRGB16_TEST_H_

extern unsigned int genChromaRGB16CycleCount;
//	  
void genChromaRGB16_asm_test(unsigned short **input, unsigned char *inputY, unsigned char **output, unsigned int width, unsigned int eps, unsigned int nBits, float *chromaScale);

#endif //_genChromaRGB16_ASM_TEST_H_
