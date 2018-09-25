#ifndef _PAD_KERNEL_U8_ASM_TEST_H_
#define _PAD_KERNEL_U8_ASM_TEST_H_

extern unsigned int padKernel_u8CycleCount;
void padKernel_u8_asm_test(unsigned char* iBuf, unsigned int iBufLen, unsigned char* oBuf, unsigned int padSz,
							unsigned int padMode, unsigned int padType, unsigned char pixValue);
#endif //_PAD_KERNEL_U8_ASM_TEST_H_
