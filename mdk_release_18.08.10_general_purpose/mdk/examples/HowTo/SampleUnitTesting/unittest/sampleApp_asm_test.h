#ifndef _SAMPLEAPP_ASM_TEST_H_
#define _SAMPLEAPP_ASM_TEST_H_

extern unsigned int SampleAppCycleCount;

void SampleApp_asm_test(unsigned char *in, unsigned int width, unsigned char value, unsigned char operation, unsigned int pxToChange, unsigned char *out);

#endif //_SAMPLEAPP_ASM_TEST_H_
