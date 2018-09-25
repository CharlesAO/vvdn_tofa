#ifndef _NONMAX_3x3_U8_ASM_TEST_H_
#define _NONMAX_3x3_U8_ASM_TEST_H_

extern unsigned int nonMax3x3_u8CycleCount;
void nonMax3x3_u8_asm_test(unsigned int width, unsigned char* corners1, unsigned char* corners2, unsigned char* corners3, unsigned short* candOutPos, unsigned int* candOutCount);
#endif //_NONMAX_3x3_U8_ASM_TEST_H_
