#include "convolution5x5s4hhhh.h"


void mvcvConvolution5x5s4hhhh(half** in, half** out, half conv[25], u32 inWidth)
{
    int x, y;
    unsigned int i;
    half* lines[5];
    half sum;


    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];
    lines[3] = in[3];
    lines[4] = in[4];

    //Go on the whole line
    for (i = 0; i < inWidth/4; i++){
        sum = 0.0;
        for (x = 0; x < 5; x++)
        {
            for (y = 0; y < 5; y++)
            {
                sum += lines[x][y - 2] * conv[x * 5 + y];
            }
            lines[x]= lines[x]+4;
        }
        out[0][i] = sum;
    }
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvConvolution5x5s4hhhh_opt(short float ** __restrict in, short float ** __restrict out, short float conv[25], unsigned int inWidth) {
  half8 conv8[3];
  short float conv1;
  
  short float * __restrict output = out[0];
  
  // +--------------------+
  // + a0, a1, a2, a3, a4 +
  // + b0, b1, b2, b3, b4 +
  // + c0, c1, c2, c3, c4 +
  // + d0, d1, d2, d3, d4 +
  // + e0, e1, e2, e3, e4 +
  // +--------------------+
    
  // conv8[0] = {a0, a1, a2, a3, b0, b1, b2, b3}
  conv8[0].s0123 = *((half4 *) &conv[0]);
  conv8[0].s4567 = *((half4 *) &conv[5]);
  
  // conv8[1] = {c0, c1, c2, c3, d0, d1, d2, d3}
  conv8[1].s0123 = *((half4 *) &conv[10]);
  conv8[1].s4567 = *((half4 *) &conv[15]);
  
  // conv8[2] = {e0, e1, e2, e3, a4, b4, c4, d4}
  conv8[2].s0123 = *((half4 *) &conv[20]);
  conv8[2].s4 = conv[4];
  conv8[2].s5 = conv[9];
  conv8[2].s6 = conv[14];
  conv8[2].s7 = conv[19];
  
  // conv1 = e4
  conv1 = conv[24];
    
  // Preload the 5x5 input (in the same pattern as conv) for the first iteration
  half4 in4[5];
  in4[0] = *((half4 *) &in[0][-2]);
  in4[1] = *((half4 *) &in[1][-2]);
  in4[2] = *((half4 *) &in[2][-2]);
  in4[3] = *((half4 *) &in[3][-2]);
  in4[4] = *((half4 *) &in[4][-2]);
  
  short float in1[5];
  in1[0] = in[0][2];
  in1[1] = in[1][2];
  in1[2] = in[2][2];
  in1[3] = in[3][2];
  in1[4] = in[4][2];
  
  #pragma unroll 4
  for (unsigned int i = 0; i < (inWidth&0xfffffff8); i+=4) {
    half8 in8;

    // in8 = {a0, a1, a2, a3, b0, b1, b2, b3}
    in8.s0123 = in4[0];
    in8.s4567 = in4[1];
    
    half8 partialSum = in8 * conv8[0];
    
    // in8 = {c0, c1, c2, c3, d0, d1, d2, d3}
    in8.s0123 = in4[2];
    in8.s4567 = in4[3];
    
    partialSum += in8 * conv8[1];
    
    // in8 = {e0, e1, e2, e3, a4, b4, c4, d4}
    in8.s0123 = in4[4];
    in8.s4 = in1[0];
    in8.s5 = in1[1];
    in8.s6 = in1[2];
    in8.s7 = in1[3];
       
    partialSum += in8 * conv8[2];
    
    short float sum = in1[4] * conv1;
    sum += __builtin_shave_sau_sumx_f16_r(partialSum);
    
    // Preload the 5x5 input for the next iteration
    in4[0] = *((half4 *) &in[0][i+2]);
    in4[1] = *((half4 *) &in[1][i+2]);
    in4[2] = *((half4 *) &in[2][i+2]);
    in4[3] = *((half4 *) &in[3][i+2]);
    in4[4] = *((half4 *) &in[4][i+2]);
    
    in1[0] = in[0][i+6];
    in1[1] = in[1][i+6];
    in1[2] = in[2][i+6];
    in1[3] = in[3][i+6];
    in1[4] = in[4][i+6];
    
    output[i/4] = sum;
  }
}
#endif
