#ifndef _pixelUnpackerMipi10b_ASM_TEST_H_
#define _pixelUnpackerMipi10b_ASM_TEST_H_

extern unsigned int pixelUnpackerMipi10bCycleCount;
void PixelUnpackerMipi10b_asm_test(unsigned int *in, unsigned short *output, unsigned int width, unsigned int lineNo, unsigned int coefs[4], int noMipiRxWorkaround);

#endif //_pixelUnpackerMipi10b_ASM_TEST_H_
