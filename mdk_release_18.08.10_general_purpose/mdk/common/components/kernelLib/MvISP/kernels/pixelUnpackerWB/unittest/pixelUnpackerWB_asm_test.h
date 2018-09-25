#ifndef _pixelUnpackerWB_ASM_TEST_H_
#define _pixelUnpackerWB_ASM_TEST_H_

extern unsigned int pixelUnpackerWBCycleCount;
void pixelUnpackerWB_asm_test(unsigned int *input32, unsigned char *input8, unsigned short *output, unsigned int width, unsigned char shift, unsigned short *awbCoef, unsigned int line_no);

#endif //_pixelUnpackerWB_ASM_TEST_H_
