#ifndef __COMPUTE_COMBINED_COST3_ASM_WINDOW_TEST_H__
#define __COMPUTE_COMBINED_COST3_ASM_WINDOW_TEST_H__


extern unsigned int mvcvComputeCombinedCost3WindowCycleCount;
void mvcvComputeCombinedCost3Window_asm_test(unsigned char *disparityCost, unsigned char *adCost, unsigned int alpha, unsigned int beta, unsigned int width, unsigned int normFactor);

#endif //__COMPUTE_COMBINED_COST3_ASM_WINDOW_TEST_H__
