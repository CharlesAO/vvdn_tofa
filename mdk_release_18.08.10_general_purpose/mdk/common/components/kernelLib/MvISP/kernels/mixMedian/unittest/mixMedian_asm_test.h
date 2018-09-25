#ifndef _mixMedian_ASM_TEST_H_
#define _mixMedian_ASM_TEST_H_

extern unsigned int cycleCount;

void mixMedian_asm_test(u8* out[3], u8 *in0[3], u8 *in1[3], 
    u8 *ref, float offset, float slope, unsigned int width);

#endif //_mixMedian_ASM_TEST_H_
