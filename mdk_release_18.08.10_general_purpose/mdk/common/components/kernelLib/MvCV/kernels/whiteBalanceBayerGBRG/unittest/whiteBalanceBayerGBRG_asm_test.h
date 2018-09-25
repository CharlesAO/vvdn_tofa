#ifndef _whiteBalanceBayerGBRG_ASM_TEST_H_
#define _whiteBalanceBayerGBRG_TEST_H_

extern unsigned int whiteBalanceBayerGBRGCycleCount;
//	 void whiteBalanceRGB(u16** in, u16 awbCoef[3], u16** out, u16 clamp, u32 width, u8 line) 
void whiteBalanceBayerGBRG_asm_test(unsigned short **input, unsigned short awbCoef[3], unsigned short **out, unsigned short clamp[1], unsigned int width, unsigned char line);

#endif //_whiteBalanceBayerGBRG_ASM_TEST_H_
