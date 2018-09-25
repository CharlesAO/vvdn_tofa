#include "convolution7x7Fp16ToU8.h"


void mvcvConvolution7x7Fp16ToU8(half** in, u8** out, half *conv, unsigned int inWidth)
{
    int x, y;
    unsigned int i;
    half* lines[7];
    half sum;

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];
    lines[5] = in[5];
    lines[6] = in[6];

    //Go on the whole line
    for (i = 0; i < inWidth; i++){
        sum = 0.0;
        for (x = 0; x < 7; x++)
        {
            for (y = -3; y <= 3; y++)
            {
                // convert value from u8 to u8f and after to fp16
                half val =   lines[x][y];

                sum += val * conv[x * 7 + y + 3];
            }
            lines[x]++;
        }
        if (sum >= (half)1.0f)
            sum = (half)1.0f;
        if (sum <= (half)0.0f)
            sum = (half)0.0f;
        out[0][i] = (u8)((float)sum * 255.0f);

    }
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvConvolution7x7Fp16ToU8_opt(short float ** __restrict in, unsigned char ** __restrict out, short float * __restrict conv, unsigned int inWidth)
{
  half8 convLocal[7];
  
  for (unsigned int i = 0; i < 7; ++i)
    convLocal[i] = *((half8 *) &conv[i*7]);
  
  for (unsigned int i = 0; i < inWidth; i+=8) {
    half8 sum8 = {0.0h, 0.0h, 0.0h, 0.0h, 0.0h, 0.0h, 0.0h, 0.0h};
    
    #pragma unroll 7
    for (int j = 0; j < 7; ++j) {
      #pragma unroll 7
      for (int k = 0; k < 7; ++k) {
        half8 inVal = *((half8 *) &in[j][i+k-3]);
        half8 shuffledConv = {convLocal[j][k], convLocal[j][k], convLocal[j][k], convLocal[j][k], convLocal[j][k], convLocal[j][k], convLocal[j][k], convLocal[j][k]};
        sum8 += inVal * shuffledConv;
      }
    }
    
    __builtin_shave_lsu_st128_f16_u8f_rr(sum8, (uchar8 *) &out[0][i]);
  }
}
#endif

