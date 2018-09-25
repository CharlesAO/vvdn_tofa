#include "convSeparable3x3Fp16ToFp16.h"
#include <stdio.h>

void mvcvConvSeparable3x3Fp16ToFp16(half* out, half** in, half conv[2], u32 inWidth)
{
    int i;
    half* lines[3];
    float sum = 0.0f;

    //Initialize lines pointers
    lines[0] = in[0];
    lines[1] = in[1];
    lines[2] = in[2];

    //Go on the whole line
    for (i = 0; i < (int)inWidth; i++){
		sum = (((float)(lines[0][i-1] +  lines[2][i-1]) * (float)conv[0] + (float)lines[1][i-1] * (float)conv[1]) +
			   ((float)(lines[0][i+1] +  lines[2][i+1]) * (float)conv[0] + (float)lines[1][i+1] * (float)conv[1])) * (float)conv[0] +
			   ((float)(lines[0][i  ] +  lines[2][i  ]) * (float)conv[0] +	(float)lines[1][i]    * (float)conv[1]) * (float)conv[1];

        out[i] = (half)(sum);
    }
    return;
}

#ifdef MOVICOMPILE_OPTIMIZED
void mvcvConvSeparable3x3Fp16ToFp16_opt(short float * __restrict out, short float ** __restrict in, short float conv[2], unsigned int inWidth) {
  half8 * __restrict lines[3] = { (half8 *) in[0], (half8 *) in[1], (half8 *) in[2] };
  half8 * __restrict out8 = (half8 *) out;
  half8 convLocal[2] = { conv[0], conv[1] };

  half8 in0_minus = lines[0][-1];
  half8 in1_minus = lines[1][-1];
  half8 in2_minus = lines[2][-1];

  half8 in0 = lines[0][0];
  half8 in1 = lines[1][0];
  half8 in2 = lines[2][0];

  half8 in0_plus = lines[0][1];
  half8 in1_plus = lines[1][1];
  half8 in2_plus = lines[2][1];

  half8 calcMinus = (in0_minus + in2_minus) * convLocal[0] + in1_minus * convLocal[1];
  half8 calc = (in0 + in2) * convLocal[0] + in1 * convLocal[1];
  half8 calcPlus = (in0_plus + in2_plus) * convLocal[0] + in1_plus * convLocal[1];

  half8 shuffleMinus = __builtin_shufflevector(calcMinus, calc, 7, 8, 9, 10, 11, 12, 13 ,14);
  half8 shufflePlus = __builtin_shufflevector(calc, calcPlus, 1, 2, 3, 4, 5, 6, 7, 8);

  #pragma unroll 8
  for (unsigned int i = 0; i < inWidth/8; ++i) {
    half8 sum = (shuffleMinus + shufflePlus) * convLocal[0] + calc * convLocal[1];

    shuffleMinus = __builtin_shufflevector(calc, calcPlus, 7, 8, 9, 10, 11, 12, 13 ,14);

    calc = calcPlus;
    calcPlus = (lines[0][i+2] + lines[2][i+2]) * convLocal[0] + lines[1][i+2] * convLocal[1];

    shufflePlus = __builtin_shufflevector(calc, calcPlus, 1, 2, 3, 4, 5, 6, 7, 8);

    out8[i] = sum;
  }
}
#endif
