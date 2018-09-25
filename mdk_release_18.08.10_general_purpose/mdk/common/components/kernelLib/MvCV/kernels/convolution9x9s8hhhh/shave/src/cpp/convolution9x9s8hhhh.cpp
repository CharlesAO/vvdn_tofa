#include "convolution9x9s8hhhh.h"

void mvcvConvolution9x9s8hhhh(half** in, half** out, half conv[81], u32 inWidth)
{
    int x, y;
    unsigned int i;
    half* lines[9];
    half sum;
	
    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];
    lines[5] = in[5];
    lines[6] = in[6];
    lines[7] = in[7];
    lines[8] = in[8];

    //Go on the whole line
    for (i = 0; i < inWidth/8; i++){
        sum = 0.0;
        for (x = 0; x < 9; x++)
        {
            for (y = 0; y < 9; y++)
            {
                sum += (half)(lines[x][y - 4] * conv[x * 9 + y]);
            }
            lines[x]+=8;
        }
       /* if (sum >= 255)
            sum = 255.0;
        if (sum <= 0)
            sum = 0.0; */
        out[0][i] = sum;

    }
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvConvolution9x9s8hhhh_opt(short float ** __restrict in, short float ** __restrict out, short float conv[81], unsigned int inWidth)
{
  short float * __restrict output = out[0];
  half8 conv_local8[9];
  short float conv_local1[9];
  
  for (unsigned int i = 0; i < 9; ++i) {
    conv_local8[i] = *((half8 *) &conv[i*9]);
    conv_local1[i] = conv[(i*9)+8];
  }

  for (unsigned int i = 0; i < inWidth; i+=8) {
    short float sum = 0.0h;
    
    for (int x = 0; x < 9; x++) {
      half8 in8 = *((half8 *) &in[x][i-4]);
      half8 conv8 = conv_local8[x];
      half8 sum8 = in8 * conv8;
      
      sum += __builtin_shave_sau_sumx_f16_r(sum8);
      
      sum += in[x][i+4] * conv_local1[x];
    }
    
    output[i/8] = sum;
  }
}
#endif
