#ifndef _MINTEST3x3_S16_ASM_TEST_H_
#define _MINTEST3x3_S16_ASM_TEST_H_

extern unsigned int cycleCount;

void minTest3x3_s16_asm_test(short* inBufferCandidates, short** inBuffer, unsigned int minLocationsIn[],
                              unsigned int minLocationsOut[], unsigned int minCountIn,  unsigned int* minCountOut);

#endif //_MINTEST3x3_S16_ASM_TEST_H_
