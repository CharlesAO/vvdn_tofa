#include "integralImageSquareSumFloatM2.h"

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvIntegralimage_sqsum_f32_M2_opt(float * __restrict out, unsigned char * __restrict in, unsigned int runNr, float ** __restrict prevPointer, unsigned int width) {
  float4 * __restrict out4 = (float4 *) out;
  uchar4 * __restrict in4 = (uchar4 *) in;
  float4 * __restrict prevPointer4 = (float4 *) *prevPointer;
  float onLineSum = 0.0f;
  
  float4 input = mvuConvert_float4(in4[0]);
  
  float4 prev4;
  if (runNr != 0)
    prev4 = prevPointer4[0];
  
  #pragma unroll 4
  for(unsigned int i = 0; i < width/4; i+=2) {
    #pragma unroll 2
    for (unsigned int j = 0; j < 2; ++j) {
      float4 mul = input * input;
      
      float4 result = mul + onLineSum;
      
      result += __builtin_shufflevector(mul, (float4) 0.0f, 4, 0, 0, 0);
      result += __builtin_shufflevector(mul, (float4) 0.0f, 4, 4, 1, 1);
      result += __builtin_shufflevector(mul, (float4) 0.0f, 4, 4, 4, 2);
      
      onLineSum = result[3];
      
      if (runNr != 0) {
        result += prev4;
        prev4 = prevPointer4[i+j+1];
      }
      
      input = mvuConvert_float4(in4[i+j+1]);
      
      out4[i+j] = result;
    }
  }
  *prevPointer = out;
}
#endif

void mvcvIntegralimage_sqsum_f32_M2(float* out, u8* in, u32 runNr, float** prevPointer, u32 width)
{
	 if (0 == runNr) 
    {
        unsigned int x;
        float onLineSum = 0;;
        for(x = 0; x < width; x++)
        {
            onLineSum += in[x] * in[x];
            out[x] = onLineSum;
        }
        *prevPointer = out;
    }
    else
    {
        unsigned int x;
        float onLineSum = 0;;
        for(x = 0; x < width; x++)
        {
            onLineSum += in[x] * in[x];
            out[x] = (*prevPointer)[x] + onLineSum;
        }
        *prevPointer = out;
    }
	return;
}


