#ifndef _BOXFILTER_ASM_TEST_H_
#define _BOXFILTER_ASM_TEST_H_

extern unsigned int boxFilter11x11CycleCount;
void boxfilter11x11_asm_test(unsigned char** in, unsigned char** out, unsigned char normalize, unsigned int width);

#endif //_BOXFILTER_ASM_TEST_H_
