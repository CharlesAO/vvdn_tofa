#include "computeAD32.h"
#include "stdio.h"
#include "stdlib.h"



void mvcvComputeAD32(u8* inputLeft, u8* inputRight, u8* output, u32 width, u32 flag)
{
    int disparities = 32;
    int posL, posR;

   // compute absolute difference between each pixel from left image and 32 left pixels from right image
    for (posL = 0; posL < (int)width; posL++)
    {
        for (posR = 0; posR < disparities; posR++)
        {
            if(flag==0)
            {
                output[posL * disparities + posR] = abs(inputLeft[posL] - inputRight[posL - posR]);
            }
            else
                output[posL * disparities + posR] = abs(inputLeft[posL] - inputRight[posL + posR]);
        }
    }
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvComputeAD32_opt(unsigned char * __restrict inputLeft, unsigned char * __restrict inputRight, unsigned char * __restrict output, unsigned int width, unsigned int flag)
{
  if (flag == 0) {
    uchar16 * __restrict out16 = (uchar16 *) output;
    
    uchar16 left = inputLeft[0];
    uchar16 right0 = *((uchar16 *) &inputRight[-15]);
    uchar16 right1 = *((uchar16 *) &inputRight[-31]);
      
    // Invert the elements in right0
    uchar8 temp0 = right0.sfedcba98;
    uchar8 temp1 = right0.s76543210;
    right0.s01234567 = temp0;
    right0.s89abcdef = temp1;
    
    // Invert the elements in right1
    temp0 = right1.sfedcba98;
    temp1 = right1.s76543210;
    right1.s01234567 = temp0;
    right1.s89abcdef = temp1;
      
    #pragma unroll 2
    for (int i = 0; i < (int)(width&0xfffffff8); ++i) {
      out16[i*2] = __builtin_shave_vau_adiff_u8_rr(left, right0);
      out16[(i*2)+1] = __builtin_shave_vau_adiff_u8_rr(left, right1);
      
      left = inputLeft[i+1];
      right1 = right1.sf0123456789abcde;
      right1[0] = right0[15];
      right0 = right0.sf0123456789abcde;
      right0[0] = inputRight[i+1];
    }
  }
  else {
    uchar16 * __restrict out16 = (uchar16 *) output;
    
    uchar16 left = inputLeft[0];
    uchar16 right0 = *((uchar16 *) &inputRight[0]);
    uchar16 right1 = *((uchar16 *) &inputRight[16]);
      
    #pragma unroll 2
    for (int i = 0; i < (int)(width&0xfffffff8); ++i) {
      out16[i*2] = __builtin_shave_vau_adiff_u8_rr(left, right0);
      out16[(i*2)+1] = __builtin_shave_vau_adiff_u8_rr(left, right1);
      
      left = inputLeft[i+1];
      right0 = right0.s123456789abcdef0;
      right0[15] = right1[0];
      right1 = right1.s123456789abcdef0;
      right1[15] = inputRight[i+32];
    }
  }
}

#endif
