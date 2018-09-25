#ifndef _MINMAXKERNEL_ASM_TEST_H_
#define _MINMAXKERNEL_ASM_TEST_H_

extern unsigned int cycleCount;

void localMaxMin3x3_s16_asm_test(short** inBuffer, unsigned int  candidateLocationIn[], unsigned int countIn, unsigned int maxLocationList[],
        unsigned int minLocationList[], unsigned int* minCount,  unsigned int* maxCount);

#endif //_MINMAXKERNEL_ASM_TEST_H_
