#include "matrixInv4x4_fp32.h"
#include <stdio.h>

float invf(int i, int j, const float *m);

void mvcvMatrixInv4x4_fp32(fp32 *in, fp32 *out)
{
  float inv[16];
  int i, j, k;
  double D = 0;  
  
  for(i = 0; i < 4;i++)
	  for (j = 0; j < 4; j++)
	    inv[j * 4 + i] = invf(i, j, in);
      
  for (k = 0; k < 4; k++) 
    D += in[k] * inv[k*4];
  //if (D == 0) 
  //  return false;
  
  D = 1.0 / D;
  for (i = 0; i < 16; i++) 
    out[i] = inv[i] * D;
    
  return; 
}


float invf(int i, int j, const float *m) {
  int   o;
  float inv;
  
  o = 2 + (j - i);
  i += 4 + o;
  j += 4 - o;
	
  #define e(a,b) m[ ((j+b)%4)*4 + ((i+a)%4) ]
	
  inv = + e(+1,-1)*e(+0,+0)*e(-1,+1)
	      + e(+1,+1)*e(+0,-1)*e(-1,+0)
	      + e(-1,-1)*e(+1,+0)*e(+0,+1)
	      - e(-1,-1)*e(+0,+0)*e(+1,+1)
	      - e(-1,+1)*e(+0,-1)*e(+1,+0)
	      - e(+1,-1)*e(-1,+0)*e(+0,+1);
	
  return (o%2)?inv : -inv;
  #undef e
} // invf()
