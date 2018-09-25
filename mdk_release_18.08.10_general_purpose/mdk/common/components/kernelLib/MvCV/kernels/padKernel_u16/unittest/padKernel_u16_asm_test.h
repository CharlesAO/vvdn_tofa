#ifndef _PAD_KERNEL_U16_ASM_TEST_H_
#define _PAD_KERNEL_U16_ASM_TEST_H_

extern unsigned int padKernel_u16CycleCount;
void padKernel_u16_asm_test(unsigned short* iBuf, unsigned int iBufLen, unsigned short* oBuf, unsigned int padSz,
							unsigned int padMode, unsigned int padType, unsigned short pixValue);
#endif //_PAD_KERNEL_U16_ASM_TEST_H_
