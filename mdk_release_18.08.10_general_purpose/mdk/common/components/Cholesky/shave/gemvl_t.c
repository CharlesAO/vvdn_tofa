/* Matrix-by-vector multiplication [gemv   ]
   left-side matrix                [    l  ]
   transposed matrix               [     _t] */

// perform c = A'b
//   in A: A[n][n]; b: b[n]
//  out c: c[n]

#include "cholesky.h"
#include "cholesky_solver.h"

void ENTRYPOINT
gemvl_t( float* __restrict__ c, const float* __restrict__ A_, 
         const float* __restrict__ b, int n, int sA ) 
{
  const int nA = sA / sizeof(float);
# define A(i,j) A_[(i)*nA+(j)]
  int i;
  for( i = 0; i < n; i += 4 )
    gemvltNx4( &c[i], &A(0,i), b, n, sA );
# undef A
}
