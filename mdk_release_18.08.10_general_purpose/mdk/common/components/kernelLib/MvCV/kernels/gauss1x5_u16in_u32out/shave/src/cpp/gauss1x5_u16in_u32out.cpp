#include "gauss1x5_u16in_u32out.h"


#ifdef MOVICOMPILE_OPTIMIZED
void mvcvGauss1x5_u16in_u32out_opt(unsigned short ** __restrict in, unsigned int ** __restrict out, unsigned int inWidth) {
  unsigned short * __restrict input  = *in;
  unsigned int * __restrict output = *out;

  int8 value0 = mvuConvert_int8(*((ushort8 *) &input[-2]));
  int8 value1;
  value1.s0123 = value0.s4567;
  value1.s4567 = mvuConvert_int4(*((ushort4 *) &input[6]));
  
  #pragma unroll 16
  for (int i = 0; i < (int)inWidth; i+=8) {
    int4 sum0 = value0.s0123 + value0.s4567;
    sum0 += (value0.s1234 + value0.s3456) << 2;
    sum0 += (value0.s2345 << 2) + (value0.s2345 << 1);
    
    int4 sum1 = value1.s0123 + value1.s4567;
    sum1 += (value1.s1234 + value1.s3456) << 2;
    sum1 += (value1.s2345 << 2) + (value1.s2345 << 1);
    
    value0.s0123 = value1.s4567;
    value0.s4567 = mvuConvert_int4(*((ushort4 *) &input[i + 10]));    
    value1.s0123 = value0.s4567;
    value1.s4567 = mvuConvert_int4(*((ushort4 *) &input[i + 14]));
    
    *((int4 *) &output[i]) = sum0;
    *((int4 *) &output[i+4]) = sum1;
  }
}
#endif

void mvcvGauss1x5_u16in_u32out(u16 **in, u32 **out, u32 inWidth)
{
    s32 i;
    // Convolution matrix
    u32 sum = 0;
    u16    *input  = *in;
    u32    *output = *out;

    for (i = 0; i < (int)inWidth; i++)
    {
        sum = input[i -2 ] + input[i + 2];
        sum +=(input[i - 1] + input[i + 1]) << 2;
        sum += (input[i] << 2 ) + (input[i] << 1);
        output[i] = sum;
    }
}
