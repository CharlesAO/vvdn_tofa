#ifndef _BOXFILTER_ASM_TEST_H_
#define _BOXFILTER_ASM_TEST_H_

extern unsigned int boxFilterCycleCount;
void boxfilter_asm_test(unsigned char** in, unsigned char** out, unsigned int kernelSize, unsigned int normalize, unsigned int width);

#endif //_BOXFILTER_ASM_TEST_H_
