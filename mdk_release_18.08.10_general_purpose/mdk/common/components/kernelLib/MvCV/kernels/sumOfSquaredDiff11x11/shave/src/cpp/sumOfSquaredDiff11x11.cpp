#include "sumOfSquaredDiff11x11.h"

u16 mulSaturated(short a, short b);
u16 addSaturated(u16 a, u16 b);

void mvcvSumOfSquaredDiff11x11(u8** in1, u8** in2, u8** out, u32 width)
{
    int x, y;
    unsigned int i;
    u8 *lines1[11], *lines2[11];
    u16 sum, square;
    short diff;

	for(i = 0; i < 11; i++)
	{
		lines1[i] = in1[i];
		lines2[i] = in2[i];
	}

    for (i = 0; i < width; i++){
        sum = 0;
        for (x = 0; x < 11; x++)
        {
            for (y = 0; y < 11; y++)
            {
				      diff = lines1[x][y - 5] - lines2[x][y - 5];
				      square = mulSaturated(diff, diff);
              sum = addSaturated(sum, square);
            }
            lines1[x]++;
            lines2[x]++;
        }
        if (sum >= 255)
            sum = 255;
        out[0][i] = (u8)(sum);
    }
    return;
}

u16 mulSaturated(short a, short b)
{
  u32 s = a * b;
  if(s > 0xffff)
    return 0xffff;
  return s;
}

u16 addSaturated(u16 a, u16 b)
{
  u32 s = a + b;
  if(s > 0xffff)
    return 0xffff;
  return s;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvSumOfSquaredDiff11x11_opt(unsigned char ** __restrict in1, unsigned char ** __restrict in2, unsigned char ** __restrict out, unsigned int width)
{
  uint4 clampVal = {255, 255, 255, 255};
  uint4 clamp0 = {0, 0, 0, 0};
  
  unsigned char * __restrict in1_local[11];
  unsigned char * __restrict in2_local[11];
  unsigned char * __restrict output = out[0];
  
  for (unsigned int i = 0; i < 11; ++i) {
    in1_local[i] = in1[i];
    in2_local[i] = in2[i];
  }
  
  #pragma unroll 2
  for (unsigned int i = 0; i < (width&0xfffffff8); i+=4) {    
    ushort8 sum1 = {0, 0, 0, 0, 0, 0, 0, 0};
    ushort8 sum2 = {0, 0, 0, 0, 0, 0, 0, 0};
    
    #pragma unroll 11
    for (int j = 0; j < 11; ++j) {
      short8 diff1 = mvuConvert_short8(*((uchar8 *)&in1_local[j][i-5])) - mvuConvert_short8(*((uchar8 *)&in2_local[j][i-5]));
      short8 diff2 = mvuConvert_short8(*((uchar8 *)&in1_local[j][i+3])) - mvuConvert_short8(*((uchar8 *)&in2_local[j][i+3]));
      
      short8 square1 = __builtin_shave_vau_imuls_i16_rr(diff1, diff1);
      short8 square2 = __builtin_shave_vau_imuls_i16_rr(diff2, diff2);
      
      sum1 = __builtin_shave_vau_iadds_u16_rr(sum1, mvuAs_ushort8(square1));
      sum2 = __builtin_shave_vau_iadds_u16_rr(sum2, mvuAs_ushort8(square2));
    }
    
    unsigned int result1 = __builtin_shave_sau_sumx_u32_r(mvuConvert_uint4(sum1.s0123));
    result1 += __builtin_shave_sau_sumx_u32_r(mvuConvert_uint4(sum1.s4567));
    result1 += __builtin_shave_sau_sumx_u32_r(mvuConvert_uint4(sum2.s0123));
    result1 -= sum2[3];
    
    uint4 result = {result1, result1, result1, result1};
    
    result[1] += sum2[3] - sum1[0];
    result[2] = result[1] + sum2[4] - sum1[1];
    result[3] = result[2] + sum2[5] - sum1[2];
    
    result = __builtin_shave_cmu_clampab_u32_rrr_uint4(result, clamp0, clampVal);
    
    *((uchar4 *)&output[i]) = mvuConvert_uchar4(result);
  }
}
#endif //MOVICOMPILE_OPTIMIZED
