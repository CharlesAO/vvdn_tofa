#ifndef _calcBxBy_ASM_TEST_H_
#define _calcBxBy_ASM_TEST_H_

#include <mv_types.h>
#include <mvcv_types.h>

extern unsigned int calcBxByCycleCount;
float* calcBxBy_asm_test(unsigned char* patchI, unsigned int* isz, float* Ix, float* Iy, 
                unsigned int* minI, unsigned char* patchJ, unsigned int* jsz, unsigned int* minJ, float* errorPtr);


#endif //_calcBxBy_ASM_TEST_H_
