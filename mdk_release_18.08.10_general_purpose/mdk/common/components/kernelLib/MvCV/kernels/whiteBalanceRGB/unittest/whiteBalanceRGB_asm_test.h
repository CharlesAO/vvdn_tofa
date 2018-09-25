#ifndef _whiteBalanceRGB_ASM_TEST_H_
#define _whiteBalanceRGB_TEST_H_

extern unsigned int whiteBalanceRGBCycleCount;
//	 void whiteBalanceRGB(u16** inR, u16** inG, u16** inB, u16 awbCoef[3], 16** oR, u16** oG, u16** oB, u16 clamp, u32 width) 
void whiteBalanceRGB_asm_test(unsigned short **inputR, unsigned short **inputG, unsigned short **inputB, unsigned short awbCoef[3], unsigned short **Rout, unsigned short **Gout, unsigned short **Bout, unsigned short clamp[1], unsigned int width);

#endif //_whiteBalanceRGB_ASM_TEST_H_
