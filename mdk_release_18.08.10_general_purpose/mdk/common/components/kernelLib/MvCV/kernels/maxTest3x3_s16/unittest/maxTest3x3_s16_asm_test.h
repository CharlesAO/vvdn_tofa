#ifndef _MAXTEST3x3_S16_ASM_TEST_H_
#define _MAXTEST3x3_S16_ASM_TEST_H_

extern unsigned int cycleCount;

void maxTest3x3_s16_asm_test(short* inBufferCandidates, short** inBuffer, unsigned int maxLocationsIn[],
                              unsigned int maxLocationsOut[], unsigned int maxCountIn,  unsigned int* maxCountOut);

#endif //_MAXTEST3x3_S16_ASM_TEST_H_
