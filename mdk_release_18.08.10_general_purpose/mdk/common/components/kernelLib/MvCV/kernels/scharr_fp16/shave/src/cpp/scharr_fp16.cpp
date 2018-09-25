#include <math.h>
#include "scharr_fp16.h"
#include <stdio.h>

void mvcvScharr_fp16(half** in, half** out, u32 width)
{
    int i;
    half* lines[3];
    fp32 sx=0, sy=0,s=0;
    half* aux;
    aux=(*out);

    //scharr matrix
    float VertScharr[3][3]={
        {-3, 0, 3},
        {-10, 0, 10},
        {-3, 0, 3}
    };

    float HorzScharr[3][3]={
        {-3, -10, -3},
        { 0,  0,  0},
        { 3,  10,  3}
    };


    //Initialize lines pointers
    lines[0]=*in;
    lines[1]=*(in+1);
    lines[2]=*(in+2);


    //Go on the whole line
    for (i=0;i<(int)width;i++){
        sx = VertScharr[0][0]*lines[0][i-1]+ VertScharr[0][1]*lines[0][i] +VertScharr[0][2]*lines[0][i+1]+
                        VertScharr[1][0]*lines[1][i-1]+ VertScharr[1][1]*lines[1][i] +VertScharr[1][2]*lines[1][i+1]+
                        VertScharr[2][0]*lines[2][i-1]+ VertScharr[2][1]*lines[2][i] +VertScharr[2][2]*lines[2][i+1];

        sy = HorzScharr[0][0]*lines[0][i-1]+ HorzScharr[0][1]*lines[0][i] +HorzScharr[0][2]*lines[0][i+1]+
                        HorzScharr[1][0]*lines[1][i-1]+ HorzScharr[1][1]*lines[1][i] +HorzScharr[1][2]*lines[1][i+1]+
                        HorzScharr[2][0]*lines[2][i-1]+ HorzScharr[2][1]*lines[2][i] +HorzScharr[2][2]*lines[2][i+1];

        s= sqrtf(sx*sx+ sy*sy);
        aux[i] = (half)s;
    }
    return;
}
#ifdef MOVICOMPILE_OPTIMIZED
  void mvcvScharr_fp16_opt(short float ** in, short float ** out, unsigned int width) {
  short float * __restrict__ output = out[0];

  short float * __restrict__ in0 = in[0];
  short float * __restrict__ in1 = in[1];
  short float * __restrict__ in2 = in[2];

  half8 in0c = *((half8 *) in0);
  half8 in0cMinus1 = mvuAs_half8(__builtin_shave_cmu_shliv_x16_rr(mvuAs_short8(in0c), mvuAs_short(in0[-1])));
  half8 in0cPlus1 = mvuAs_half8(__builtin_shave_cmu_shriv_x16_rr(mvuAs_short8(in0c), mvuAs_short(in0[8])));

  half8 in1c = *((half8 *) in1);
  half8 in1cMinus1 = mvuAs_half8(__builtin_shave_cmu_shliv_x16_rr(mvuAs_short8(in1c), mvuAs_short(in1[-1])));
  half8 in1cPlus1 = mvuAs_half8(__builtin_shave_cmu_shriv_x16_rr(mvuAs_short8(in1c), mvuAs_short(in1[8])));

  half8 in2c = *((half8 *) in2);
  half8 in2cMinus1 = mvuAs_half8(__builtin_shave_cmu_shliv_x16_rr(mvuAs_short8(in2c), mvuAs_short(in2[-1])));
  half8 in2cPlus1 = mvuAs_half8(__builtin_shave_cmu_shriv_x16_rr(mvuAs_short8(in2c), mvuAs_short(in2[8])));

  #pragma unroll 4
  for (int i = 0; i < (int)width; i+=8) {
    half8 sx = in0cMinus1 * -3.0h;
    sx +=      in0cPlus1  * 3.0h;
    sx +=      in1cMinus1 * -10.0h;
    sx +=      in1cPlus1  * 10.0h;
    sx +=      in2cMinus1 * -3.0h;
    sx +=      in2cPlus1  * 3.0h;

    half8 sy = in0cMinus1 * -3.0h;
    sy +=      in0c       * -10.0h;
    sy +=      in0cPlus1  * -3.0h;
    sy +=      in2cMinus1 * 3.0h;
    sy +=      in2c       * 10.0h;
    sy +=      in2cPlus1  * 3.0h;

    in0c = *((half8 *) &in0[i+8]);
    half8 prev = in0cPlus1;
    in0cPlus1 = mvuAs_half8(__builtin_shave_cmu_shriv_x16_rr(mvuAs_short8(in0c), mvuAs_short(in0[i+16])));
    in0cMinus1 = __builtin_shufflevector(in0cPlus1, prev, 14, 15, 0, 1, 2, 3, 4, 5);

    in1c = *((half8 *) &in1[i+8]);
    prev = in1cPlus1;
    in1cPlus1 = mvuAs_half8(__builtin_shave_cmu_shriv_x16_rr(mvuAs_short8(in1c), mvuAs_short(in1[i+16])));
    in1cMinus1 = __builtin_shufflevector(in1cPlus1, prev, 14, 15, 0, 1, 2, 3, 4, 5);

    in2c = *((half8 *) &in2[i+8]);
    prev = in2cPlus1;
    in2cPlus1 = mvuAs_half8(__builtin_shave_cmu_shriv_x16_rr(mvuAs_short8(in2c), mvuAs_short(in2[i+16])));
    in2cMinus1 = __builtin_shufflevector(in2cPlus1, prev, 14, 15, 0, 1, 2, 3, 4, 5);

    half8 s8 = sx*sx + sy*sy;
    half8 result;

    result[0] = __builtin_shave_sau_sqt_f16_ri(s8, 0);
    result[1] = __builtin_shave_sau_sqt_f16_ri(s8, 1);
    result[2] = __builtin_shave_sau_sqt_f16_ri(s8, 2);
    result[3] = __builtin_shave_sau_sqt_f16_ri(s8, 3);
    result[4] = __builtin_shave_sau_sqt_f16_ri(s8, 4);
    result[5] = __builtin_shave_sau_sqt_f16_ri(s8, 5);
    result[6] = __builtin_shave_sau_sqt_f16_ri(s8, 6);
    result[7] = __builtin_shave_sau_sqt_f16_ri(s8, 7);

    *((half8 *) &output[i]) = result;
  }
}
#endif
