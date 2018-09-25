#include <matrixVectorMultfp16x4.h>

//!@{
/// multiply 4xN matrix by Nx1 vector to give 4x1 result vector.
/// N is specified by num_elements and is always be a multiple of 16
/// @param[in]  inArrayA     - Input rows of the 4xN matrix
/// @param[in]  inVecB       - Nx1 vector
/// @param[in]  num_elements - N - number of elements
/// @param[out] outVec       - 4x1 result vector

void mvcvmatrixVectorMultfp16x4(half **inArrayA, half *inVecB, u32 num_elements, half *outVec)
{
    half *inA0, *inA1, *inA2, *inA3;
    u32 i;

    inA0 = inArrayA[0];
    inA1 = inArrayA[1];
    inA2 = inArrayA[2];
    inA3 = inArrayA[3];
    outVec[0] = 0;
    outVec[1] = 0;
    outVec[2] = 0;
    outVec[3] = 0;

    for(i = 0; i < num_elements; i++)
    {
        outVec[0] += inA0[i]*inVecB[i];
        outVec[1] += inA1[i]*inVecB[i];
        outVec[2] += inA2[i]*inVecB[i];
        outVec[3] += inA3[i]*inVecB[i];
    }
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvmatrixVectorMultfp16x4_opt(short float ** __restrict inArrayA, short float * __restrict inVecB, unsigned int num_elements, short float * __restrict outVec) {
  half8 * __restrict inA[4] = {(half8 *) inArrayA[0], (half8 *) inArrayA[1], (half8 *) inArrayA[2], (half8 *) inArrayA[3]};
  half8 * __restrict inB = (half8 *) inVecB;
  half4 result = {0.0h, 0.0h, 0.0h, 0.0h};

  #pragma unroll 4
  for (unsigned int i = 0; i < (num_elements&0xfffffff0)/8; ++i) {
    result[0] += __builtin_shave_sau_sumx_f16_r(inA[0][i] * inB[i]);
    result[1] += __builtin_shave_sau_sumx_f16_r(inA[1][i] * inB[i]);
    result[2] += __builtin_shave_sau_sumx_f16_r(inA[2][i] * inB[i]);
    result[3] += __builtin_shave_sau_sumx_f16_r(inA[3][i] * inB[i]);
  }

  half4 * output = (half4 *) outVec;
  *output = result;
}
#endif
