#ifndef _HISTOGRAM_STAT_ASM_TEST_H_
#define _HISTOGRAM_STAT_ASM_TEST_H_

extern unsigned int histogramStatCycleCount;
void histogramStat_asm_test(unsigned char *intputR, unsigned char *inputG, unsigned char *inputB, unsigned int *histR, unsigned int *histG, unsigned int *histB, unsigned int width, unsigned int step);

#endif //_HISTOGRAM_STAT_ASM_TEST_H_
