#ifndef _pixelUnpacker_ASM_TEST_H_
#define _pixelUnpacker_ASM_TEST_H_

extern unsigned int pixelUnpackerCycleCount;
void pixelUnpacker_asm_test(unsigned int *input32, unsigned char *input8, unsigned short *output, unsigned int width, unsigned char shift);

#endif //_pixelUnpacker_ASM_TEST_H_
