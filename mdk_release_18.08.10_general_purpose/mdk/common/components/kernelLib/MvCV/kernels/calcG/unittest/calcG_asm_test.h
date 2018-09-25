#ifndef _calG_ASM_TEST_H_
#define _calG_ASM_TEST_H_

#include <mv_types.h>

extern unsigned int calcGCycleCount;
void calcG_asm_test(float* ix, unsigned int* isz, unsigned int* minI, float* iy, unsigned int* jsz, unsigned int* minJ, unsigned int* g);

#endif //_calG_ASM_TEST_H_
