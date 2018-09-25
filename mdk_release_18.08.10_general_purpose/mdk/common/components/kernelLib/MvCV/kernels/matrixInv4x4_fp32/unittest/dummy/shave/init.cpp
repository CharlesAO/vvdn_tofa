///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     MatrixInv4x4_fp32 kernel function call for unitary test
///

#include "matrixInv4x4_fp32.h"
#include <stdio.h> 
#include "svuCommonShave.h"
#include "mv_types.h"
#include <moviVectorTypes.h>

#define SIZE 4
fp32 input[SIZE * SIZE] __attribute__ ((aligned (16)));

/*output pre pad marker*/
uint32_t  __attribute__((section(".kept.data"))) output_u32prePad[4] __attribute__ ((aligned (16)));
/*output data marker*/
fp32 __attribute__((section(".kept.data"))) output[SIZE * SIZE] __attribute__ ((aligned (16)));
/*output post pad marker*/
uint32_t __attribute__((section(".kept.data"))) output_u32postPad[4] __attribute__ ((aligned (16)));

void local(fp32 *in, fp32 *out)
{
  float4 line0, line1, line2, line3;
  float4 invLine0, invLine1, invLine2, invLine3;

  for(int i = 0; i < 4; i++)
  {
    invLine0[i] = 0;
    invLine1[i] = 0;
    invLine2[i] = 0;
    invLine3[i] = 0;
  }
  
  invLine0[0] = 1.0f;
  invLine1[1] = 1.0f;
  invLine2[2] = 1.0f;
  invLine3[3] = 1.0f;
  
  
  line0[0] = in[0];     line1[0] = in[4];
  line0[1] = in[1];     line1[1] = in[5];
  line0[2] = in[2];     line1[2] = in[6];
  line0[3] = in[3];     line1[3] = in[7];
  
  line2[0] = in[8];     line3[0] = in[12];
  line2[1] = in[9];     line3[1] = in[13];
  line2[2] = in[10];    line3[2] = in[14];
  line2[3] = in[11];    line3[3] = in[15];
    
  if(line0[0] == 0.0f)
  {
    if(line1[0] != 0.0f)
    {
      line0 = __builtin_shave_vau_add_f32_rr(line0, line1);
      invLine0 = __builtin_shave_vau_add_f32_rr(invLine0, invLine1);
    }
    else
    {
      if (line2[0] != 0.0f)
      {
        line0 = __builtin_shave_vau_add_f32_rr(line0, line2);
        invLine0 = __builtin_shave_vau_add_f32_rr(invLine0, invLine2);
      }
      else
        if (line3[0] != 0.0f)
        {
          line0 = __builtin_shave_vau_add_f32_rr(line0, line3);
          invLine0 = __builtin_shave_vau_add_f32_rr(invLine0, invLine3);
        }
    }
  }  
 
  float4 inv0;
  inv0[0] = 1.0f / line0[0];
  inv0[1] = inv0[0];
  inv0[2] = inv0[0];
  inv0[3] = inv0[0];
  line0 = __builtin_shave_vau_mul_f32_rr(line0, inv0);
  invLine0 = __builtin_shave_vau_mul_f32_rr(invLine0, inv0);
  
  float4 tmp, tmpLine, tmpInvLine;
  tmp[0] = line1[0];
  tmp[1] = line1[0];
  tmp[2] = line1[0];
  tmp[3] = line1[0];
  tmpLine = __builtin_shave_vau_mul_f32_rr(tmp, line0);
  line1 = __builtin_shave_vau_sub_f32_rr(line1, tmpLine);
  tmpInvLine = __builtin_shave_vau_mul_f32_rr(tmp, invLine0);
  invLine1 = __builtin_shave_vau_sub_f32_rr(invLine1, tmpInvLine);
  
  tmp[0] = line2[0];
  tmp[1] = line2[0];
  tmp[2] = line2[0];
  tmp[3] = line2[0];
  tmpLine = __builtin_shave_vau_mul_f32_rr(tmp, line0);
  line2 = __builtin_shave_vau_sub_f32_rr(line2, tmpLine);
  tmpInvLine = __builtin_shave_vau_mul_f32_rr(tmp, invLine0);
  invLine2 = __builtin_shave_vau_sub_f32_rr(invLine2, tmpInvLine);
  
  tmp[0] = line3[0];
  tmp[1] = line3[0];
  tmp[2] = line3[0];
  tmp[3] = line3[0];
  tmpLine = __builtin_shave_vau_mul_f32_rr(tmp, line0);
  line3 = __builtin_shave_vau_sub_f32_rr(line3, tmpLine);
  tmpInvLine = __builtin_shave_vau_mul_f32_rr(tmp, invLine0);
  invLine3 = __builtin_shave_vau_sub_f32_rr(invLine3, tmpInvLine);
  
  if(line1[1] == 0.0f)
  {
    if(line2[1] != 0.0f)
    {
      line1 = __builtin_shave_vau_add_f32_rr(line1, line2);
      invLine1 = __builtin_shave_vau_add_f32_rr(invLine1, invLine2);
    }
    else
    {
      if (line3[1] != 0.0f)
      {
        line1 = __builtin_shave_vau_add_f32_rr(line1, line3);
        invLine1 = __builtin_shave_vau_add_f32_rr(invLine1, invLine3);
      }
    }
  }  
    
  float4 inv1;
  inv1[0] = 1.0f / line1[1];
  inv1[1] = inv1[0];
  inv1[2] = inv1[0];
  inv1[3] = inv1[0];
  line1 = __builtin_shave_vau_mul_f32_rr(line1, inv1);
  invLine1 = __builtin_shave_vau_mul_f32_rr(invLine1, inv1);
  
  tmp[0] = line2[1];
  tmp[1] = line2[1];
  tmp[2] = line2[1];
  tmp[3] = line2[1];
  tmpLine = __builtin_shave_vau_mul_f32_rr(tmp, line1);
  line2 = __builtin_shave_vau_sub_f32_rr(line2, tmpLine);
  tmpInvLine = __builtin_shave_vau_mul_f32_rr(tmp, invLine1);
  invLine2 = __builtin_shave_vau_sub_f32_rr(invLine2, tmpInvLine);
  
  tmp[0] = line3[1];
  tmp[1] = line3[1];
  tmp[2] = line3[1];
  tmp[3] = line3[1];
  tmpLine = __builtin_shave_vau_mul_f32_rr(tmp, line1);
  line3 = __builtin_shave_vau_sub_f32_rr(line3, tmpLine);
  tmpInvLine = __builtin_shave_vau_mul_f32_rr(tmp, invLine1);
  invLine3 = __builtin_shave_vau_sub_f32_rr(invLine3, tmpInvLine);
  
  tmp[0] = line0[1];
  tmp[1] = line0[1];
  tmp[2] = line0[1];
  tmp[3] = line0[1];
  tmpLine = __builtin_shave_vau_mul_f32_rr(tmp, line1);
  line0 = __builtin_shave_vau_sub_f32_rr(line0, tmpLine);
  tmpInvLine = __builtin_shave_vau_mul_f32_rr(tmp, invLine1);
  invLine0 = __builtin_shave_vau_sub_f32_rr(invLine0, tmpInvLine);
  
  if(line2[2] == 0.0f)
  {
    if(line3[2] != 0.0f)
    {
      line2 = __builtin_shave_vau_add_f32_rr(line2, line3);
      invLine2 = __builtin_shave_vau_add_f32_rr(invLine2, invLine3);
    }
  }
  
  float4 inv2;
  inv2[0] = 1.0f / line2[2];
  inv2[1] = inv2[0];
  inv2[2] = inv2[0];
  inv2[3] = inv2[0];
  line2 = __builtin_shave_vau_mul_f32_rr(line2, inv2);
  invLine2 = __builtin_shave_vau_mul_f32_rr(invLine2, inv2);
  
  tmp[0] = line3[2];
  tmp[1] = line3[2];
  tmp[2] = line3[2];
  tmp[3] = line3[2];
  tmpLine = __builtin_shave_vau_mul_f32_rr(tmp, line2);
  line3 = __builtin_shave_vau_sub_f32_rr(line3, tmpLine);
  tmpInvLine = __builtin_shave_vau_mul_f32_rr(tmp, invLine2);
  invLine3 = __builtin_shave_vau_sub_f32_rr(invLine3, tmpInvLine);
  
  tmp[0] = line0[2];
  tmp[1] = line0[2];
  tmp[2] = line0[2];
  tmp[3] = line0[2];
  tmpLine = __builtin_shave_vau_mul_f32_rr(tmp, line2);
  line0 = __builtin_shave_vau_sub_f32_rr(line0, tmpLine);
  tmpInvLine = __builtin_shave_vau_mul_f32_rr(tmp, invLine2);
  invLine0 = __builtin_shave_vau_sub_f32_rr(invLine0, tmpInvLine);
  
  tmp[0] = line1[2];
  tmp[1] = line1[2];
  tmp[2] = line1[2];
  tmp[3] = line1[2];
  tmpLine = __builtin_shave_vau_mul_f32_rr(tmp, line2);
  line1 = __builtin_shave_vau_sub_f32_rr(line1, tmpLine);
  tmpInvLine = __builtin_shave_vau_mul_f32_rr(tmp, invLine2);
  invLine1 = __builtin_shave_vau_sub_f32_rr(invLine1, tmpInvLine);
  
  float4 inv3;
  inv3[0] = 1.0f / line3[3];
  inv3[1] = inv3[0];
  inv3[2] = inv3[0];
  inv3[3] = inv3[0];
  line3 = __builtin_shave_vau_mul_f32_rr(line3, inv3);
  invLine3 = __builtin_shave_vau_mul_f32_rr(invLine3, inv3);
  
  tmp[0] = line0[3];
  tmp[1] = line0[3];
  tmp[2] = line0[3];
  tmp[3] = line0[3];
  tmpLine = __builtin_shave_vau_mul_f32_rr(tmp, line3);
  line0 = __builtin_shave_vau_sub_f32_rr(line0, tmpLine);
  tmpInvLine = __builtin_shave_vau_mul_f32_rr(tmp, invLine3);
  invLine0 = __builtin_shave_vau_sub_f32_rr(invLine0, tmpInvLine);
  
  tmp[0] = line1[3];
  tmp[1] = line1[3];
  tmp[2] = line1[3];
  tmp[3] = line1[3];
  tmpLine = __builtin_shave_vau_mul_f32_rr(tmp, line3);
  line1 = __builtin_shave_vau_sub_f32_rr(line1, tmpLine);
  tmpInvLine = __builtin_shave_vau_mul_f32_rr(tmp, invLine3);
  invLine1 = __builtin_shave_vau_sub_f32_rr(invLine1, tmpInvLine);
  
  tmp[0] = line2[3];
  tmp[1] = line2[3];
  tmp[2] = line2[3];
  tmp[3] = line2[3];
  tmpLine = __builtin_shave_vau_mul_f32_rr(tmp, line3);
  line2 = __builtin_shave_vau_sub_f32_rr(line2, tmpLine);
  tmpInvLine = __builtin_shave_vau_mul_f32_rr(tmp, invLine3);
  invLine2 = __builtin_shave_vau_sub_f32_rr(invLine2, tmpInvLine);
   
  out[0] = invLine0[0];     out[4] = invLine1[0];
  out[1] = invLine0[1];     out[5] = invLine1[1];
  out[2] = invLine0[2];     out[6] = invLine1[2];
  out[3] = invLine0[3];     out[7] = invLine1[3];
  
  out[8]  = invLine2[0];    out[12] = invLine3[0];
  out[9]  = invLine2[1];    out[13] = invLine3[1];
  out[10] = invLine2[2];    out[14] = invLine3[2];
  out[11] = invLine2[3];    out[15] = invLine3[3];
  
  return; 
}



int main( void )
{ 
  
	#ifdef UNIT_TEST_USE_C_VERSION
	mvcvMatrixInv4x4_fp32(input, output);
	#else  
  mvcvMatrixInv4x4_fp32_asm(&input[0], &output[0]);  
  //local(&input[0], &output[0]);  
	#endif
 
	SHAVE_HALT; 
  return 0;
}
