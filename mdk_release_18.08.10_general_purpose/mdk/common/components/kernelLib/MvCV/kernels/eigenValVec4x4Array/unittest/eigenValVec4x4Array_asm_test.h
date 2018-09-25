#ifndef _eigenValVec4x4Array_TEST_H_
#define _eigenValVec4x4Array_TEST_H_

extern unsigned int eigenValVec4x4ArrayCycleCount;
void eigenValVec4x4Array_asm_test(float** matrix, float* eigvals, float** eigvec, u32 num_matrix, float threshold);

#endif //_eigenValVec4x4Array_TEST_H_
