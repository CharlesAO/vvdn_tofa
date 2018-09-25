#include "matrixInv3x3_fp32.h"
#include <stdio.h>

void mvcvMatrixInv3x3_fp32(fp32 **in, fp32 **out)
{
  float determinant =   in[0][0] * ( in[1][1] * in[2][2] - in[2][1] * in[1][2] )
                      - in[0][1] * ( in[1][0] * in[2][2] - in[1][2] * in[2][0] )
                      + in[0][2] * ( in[1][0] * in[2][1] - in[1][1] * in[2][0] );
						
  float invdet = 1/determinant;
   
  out[0][0] =  (in[1][1] * in[2][2] - in[2][1] * in[1][2]) * invdet;
  out[0][1] = -(in[1][0] * in[2][2] - in[1][2] * in[2][0]) * invdet;
  out[0][2] =  (in[1][0] * in[2][1] - in[2][0] * in[1][1]) * invdet;
  
  out[1][0] = -(in[0][1] * in[2][2] - in[0][2] * in[2][1]) * invdet;  
  out[1][1] =  (in[0][0] * in[2][2] - in[0][2] * in[2][0]) * invdet;  
  out[1][2] = -(in[0][0] * in[2][1] - in[2][0] * in[0][1]) * invdet;    
    
  out[2][0] =  (in[0][1] * in[1][2] - in[0][2] * in[1][1]) * invdet;
  out[2][1] = -(in[0][0] * in[1][2] - in[1][0] * in[0][2]) * invdet;
  out[2][2] =  (in[0][0] * in[1][1] - in[1][0] * in[0][1]) * invdet;
  
  return;
}
