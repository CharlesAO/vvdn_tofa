#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "solver.h"

/* perform T = A'A ***********************************************************/

void syrk_tn_l( float *T_, const float *A_, int n )
{
# define T(i,j) T_[(i)*n+(j)]
# define A(i,j) A_[(i)*n+(j)]
  int i;
  for( i = 0; i < n; ++i ) {
    int j;
    for( j = 0; j <=i; ++j ) {
      float s = 0;
      int k;
      for( k = 0; k < n; ++k ) {
        s += A(k,i) * A(k,j);
        ++n_add; ++n_mul;
      }
      T(i,j) = s;
    }
  }
# undef A
# undef T
}

/* perform c = Ab ************************************************************/

void gemv_n( float *c, const float *A_, const float *b, int n )
{
# define A(i,j) A_[(i)*n+(j)]
  int i;
  for( i = 0; i < n; ++i ) {
    float s = 0;
    int j;
    for( j = 0; j < n; ++j ) {
      s += A(i,j) * b[j];
      ++n_add; ++n_mul;
    }
    c[i] = s;
  }
# undef A
}

/* perform c = A'b ***********************************************************/

void gemv_t( float *c, const float *A_, const float *b, int n )
{
# define A(i,j) A_[(i)*n+(j)]
  int i;
  for( i = 0; i < n; ++i ) {
    float s = 0;
    int j;
    for( j = 0; j < n; ++j ) {
      s += A(j,i) * b[j];
      ++n_add; ++n_mul;
    }
    c[i] = s;
  }
# undef A
}

/* perform L = cholesky(A) ***************************************************/

void potrf_ln( float *L_, const float *A_, int n )
{
# define A(i,j) A_[(i)*n+(j)]
# define L(i,j) L_[(i)*n+(j)]
  int i;

  for( i = 0; i < n; ++i ) {
    int j;
    for( j = 0; j < i; ++j ) {
      float s = A(i,j); 
      int k;
      for( k = 0; k < j; ++k ) {
        s -= L(i,k) * L(j,k);
        ++n_add; ++n_mul;
      }
      L(i,j) = s / L(j,j);
      ++n_div;
    }
    float s = A(i,i);
    for( j = 0; j < i; ++j ) {
      s -= L(i,j) * L(i,j);
      ++n_add; ++n_mul;
    }
    L(i,i) = sqrtf( s );
    ++n_sqrt;
  }
# undef A
# undef L
}

/* solve x from Lx = b *******************************************************/

// x[i] = 1/L[i][i] *( b[i] - sum:j=0..i-1:(L[i][j]*x[j] ) );
void trsv_ln( float *x, const float *L_, const float *b, int n )
{
# define L(i,j) L_[(i)*n+(j)]
  int i;
  for( i = 0; i < n; ++i ) {
    float s = b[i];
    int j;
    for( j = 0; j < i; ++j ) {
      s -= L(i,j) * x[j];
      ++n_add; ++n_mul;
    }
    x[i] = s / L(i,i);
    ++n_div;
  }
# undef L
}

/* solve x from Ux = b *******************************************************/

// x[i] = 1/U[i][i] *( b[i] - sum:j=i+1..n-1:(U[i][j]*x[j] ) );
void trsv_lt( float *x, const float *L_, const float *b, int n )
{
# define U(i,j) L_[(j)*n+(i)]
  int i;
  for( i = n-1; i >= 0; --i ) {
    float s = b[i];
    int j;
    for( j = n-1; j > i; --j ) {
      s -= U(i,j) * x[j];
      ++n_add; ++n_mul;
    }
    x[i] = s / U(i,i);
    ++n_div;
  }
# undef U
}

/* perform B = A^-1 **********************************************************/

static void row_add( float *d, float *s, int n );
static void row_mul_add( float *d, float *s, float k, int n );
static void row_scale( float *d, float k, int n );

void gauss( float *B_, const float *A_, int n )
{
# define B(i,j) B_[(i)*n+(j)]
  // duplicate A, as we're writing stuff to it
  float *myA = (float*)malloc( n*n*sizeof(float) );
  if( !myA )
    return;
  memcpy( myA, A_, n*n*sizeof(float) );
# define A(i,j) myA[(i)*n+(j)]
  // start with the unity matrix as a solution
  memset( B_, 0, n*n*sizeof(float) );
  int i;
  for( i = 0; i < n; ++i )
    B(i,i) = 1.0f;
  // for every row
  for( i = 0; i < n; ++i ) {
    // fix pivot element being 0, if need be
    if( fabsf( A(i,i) )<= 1e-6 ) {
      // find a suitable row
      int j;
      for( j = 0; j < n; ++j ) {
        if( i == j )
          continue;
        ++n_scan;
        if( fabsf( A(j,i) )> 1e-6 ) {
          row_add( &A(i,0), &A(j,0), n );
          row_add( &B(i,0), &B(j,0), n );
          break;
        }
      }
    }
    // scale the current column
    float k = 1.0f / A(i,i);
    ++n_div;
    row_scale( &A(i,0), k, n );
    row_scale( &B(i,0), k, n );
    // make zeros on the same column
    int j;
    for( j = 0; j < n; ++j ) {
      if( i == j )
        continue;
      float k = -A(j,i);
      ++n_add;
      row_mul_add( &A(j,0), &A(i,0), k, n );
      row_mul_add( &B(j,0), &B(i,0), k, n );
    }
  }
# undef A
# undef B
}

static void row_add( float *d, float *s, int n )
{
  int i;
  for( i = 0; i < n; ++i ) {
    *d++ += *s++;
    ++n_add;
  }
}

static void row_mul_add( float *d, float *s, float k, int n )
{
  int i;
  for( i = 0; i < n; ++i ) {
    *d++ += k * *s++;
    ++n_add; ++n_mul;
  }
}

static void row_scale( float *sd, float k, int n )
{
  int i;
  for( i = 0; i < n; ++i ) {
    *sd++ *= k;
    ++n_mul;
  }
}
