/* Symmetric rank K update [syrk     ]
   transposed by normal    [    _tn  ]
   lower triangular output [       _l] */

// perform A = AA' (in-place syrk)
//    in A: A[n][n]
// temp cA: cA[n][4]
//   out A: A[n][n] #LT

#include "cholesky.h"
#include "cholesky_solver.h"

void ENTRYPOINT
syrk_tn_l( float* __restrict__ A_, float* __restrict__ cA_, int n, int sA )
{
  //const int nA = sA / sizeof(float);
# define A(i,j) A_[(i)*n+(j)]
# define cA(i,j) cA_[(i)*4+(j)]

  // compute the first column
  int i;
  for( i = 0; i < n; i += 4 )
    syrktnNx4( &cA(i,0), &A(0,0), &A(0,i), n, 4*sizeof(float), sA );
  // compute all subsequent columns
  for( i = 4; i < n; i += 4 ) {
    int j;
    for( j = i; j < n; j += 4 )
      syrktnNx4( &A(j,i-4), &A(0,i), &A(0,j), n, sA, sA );
  }

  // copy all the rows to the right,
  // inserting from cA into the first column of every row
  rcpy4xN( &A(0,0), &cA(0,0), 4, sA, 4*sizeof(float) );
  for( i = 4; i < n; i += 4 ) {
    rcpy4xN( &A(i,i), &A(i,i-4), i, sA, sA );
    rcpy4xN( &A(i,0), &cA(i,0), 4, sA, 4*sizeof(float) );
  }

# undef cA
# undef A
}
