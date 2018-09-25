#include "accumulateFp16MaxPool3s2withReLU.h"
#include <stdlib.h>

void mvcvAccumulateFp16MaxPool3s2withReLU(half** dst, half** input, u32 width)

{
    u32 i;
	half sum1, sum2, sum3, sumx;
    for (i = 0; i < width/2; i++)
    {
		sum1 = dst[0][i*2+0] + input[0][i*2+0]; 
		sum2 = dst[0][i*2+1] + input[0][i*2+1]; 
		sum3 = dst[0][i*2+2] + input[0][i*2+2]; 
		
		sumx = sum1;
        if(sum2 > sumx) sumx = sum2;
        if(sum3 > sumx) sumx = sum3;
		if(sumx<(half)0) sumx = (half)0;
        dst[0][i] = sumx;
    }
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvAccumulateFp16MaxPool3s2withReLU_opt(short float ** __restrict dst, short float ** __restrict input, unsigned int width) {
  short float * __restrict out = *dst;
  short float * __restrict in = *input;
  
  half8 out0 = *((half8 *) &out[0]);
  half2 out8 = *((half2 *) &out[8]);
  
  half8 in0 = *((half8 *) &in[0]);
  half2 in8 = *((half2 *) &in[8]);
  
  #pragma unroll 2
  for (unsigned int i = 0; i < width/2; i+=4) {
    half8 sum1 = out0 + in0;
    half8 sum2 = mvuAs_half8(__builtin_shave_cmu_shriv_x16_rr(mvuAs_short8(sum1), mvuAs_int(out8 + in8)));
    half8 sum3 = mvuAs_half8(__builtin_shave_cmu_shriv_x32_rr(mvuAs_int4(sum1), mvuAs_int(out8 + in8)));

    half8 sumx = sum1;
    sumx = __builtin_shave_cmu_max_f16_rr_half8(sumx, sum2);
    sumx = __builtin_shave_cmu_max_f16_rr_half8(sumx, sum3);
    sumx = __builtin_shave_cmu_clamp0_f16_rr_half8(sumx, (half8) 65504.0h);
    
    out0 = *((half8 *) &out[(i+4)*2]);
    out8 = *((half2 *) &out[(i+4)*2+8]);
    
    in0 = *((half8 *) &in[(i+4)*2]);
    in8 = *((half2 *) &in[(i+4)*2+8]);
    
    *((half4 *) &out[i]) = (__builtin_shufflevector(sumx, sumx, 0, 2, 4, 6, 8, 10, 12, 14)).s0123;
  }
}
#endif
