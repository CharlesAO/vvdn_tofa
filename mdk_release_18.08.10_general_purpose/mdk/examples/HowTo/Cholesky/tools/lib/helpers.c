#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "solver.h"

unsigned long long n_add, n_mul, n_div, n_sqrt;
// number of scan operations for gauss-jordan, attempting to find a suitable
// next row to operate on
unsigned long long n_scan;

void reset_stats()
{
  n_add = n_mul = n_div = n_sqrt = 0;
  n_scan = 0;
}


/*{ generate a random matrix/vector with values in [-1.0 .. 1.0) }************/

void genf( float *A, int n, float min, float max )
{
  int i, j;
  for( i = 0; i < n; ++i )
    *A++ = min +(( 1.0f /( 1.0f + (float)RAND_MAX )* rand() ))*( max - min );
}

/*[ display a square matrix ]*************************************************/

extern int alldata;
void printm( const char *hdr, const float *A, int n )
{
  int symmetric = issymm( A, n );
  int triangular = isltm( A, n );
  printf( "%s %s%s= {\n", hdr, symmetric?"#SYM ":"", triangular?"#LT ":"" );
  int i;
  for( i = 0; i < n; ++i ) {
    printf( "  {" );
    int j;
    for( j = 0; j < n; ++j ) {
      float t = A[i*n+j];
      if( triangular &&( j > i ))
        printf( "      -   " );
      else if( symmetric &&( j > i ))
        printf( "      s   " );
      else if( isnan(t) )
        printf( "     NaN  " );
      else if( isinf(t) &&( t > 0 ))
        printf( "     Inf  " );
      else if( isinf(t) )
        printf( "    -Inf  " );
      else
        printf( "% 10.3f", t );
      if( !alldata && j == 15 && n > 16 ) {
        printf( " ..." );
        break;
      }
    }
    printf( " }\n" );
    if( !alldata && i == 15 && n > 16 ) {
      printf( "  ...\n" );
      break;
    }
  }
  printf( "}\n" );
}

/*[ check if a square matrix is symmetrical ]*********************************/

int issymm( const float *A, int n )
{
  int i, j;
  for( i = 1; i < n; ++i )
    for( j = 0; j < i; ++j )
      if( A[i*n+j] != A[j*n+i] )
        return 0;
  return 1;
}

/*[ check if a square matrix is lower triangular ]****************************/

int isltm( const float *A, int n )
{
  int i, j;
  for( i = 0; i < n; ++i )
    for( j = i+1; j < n; ++j )
      if( A[i*n+j] )
        return 0;
  return 1;
}

/*[ display a vector ]********************************************************/

void printv( const char *hdr, const float *x, int n )
{
  int i;
  printf( "%s = \n", hdr );
  printf( "  {" );
  for( i = 0; i < n; ++i ) {
    float t = *x++;
    if( isnan(t) )
      printf( "     NaN  " );
    else if( isinf(t) )
      printf( "    %sInf  ", (t>0)?" ":"-" );
    else
      printf( "% 10.3f", t );
    if( !alldata && i == 15 && n > 16 ) {
      printf( " ..." );
      break;
    }
  }
  printf( " }\n" );
}

/*[ save a vector ]***********************************************************/

static char fname[256];
void savev( const float *a, int n, const char *name )
{
  snprintf( fname, sizeof(fname), "%s_%u.f32", name, n );
  FILE *f;
  if( !( f = fopen( fname, "wb" )))
    return;
  fwrite( a, 1, n*sizeof(float), f );
  fclose( f );
}

/*[ save a matrix ]***********************************************************/

void savem( const float *a, int n, const char *name )
{
  snprintf( fname, sizeof(fname), "%s_%ux%u.f32", name, n, n );
  FILE *f;
  if( !( f = fopen( fname, "wb" )))
    return;
  fwrite( a, 1, n*n*sizeof(float), f );
  fclose( f );
}

/*[ compute the absolute difference between two vectors ]*********************/

double norm( const float *a, const float *b, int n )
{
  double e = 0.0;
  int i;
  for( i = 0; i < n; ++i )
    e +=( a[i] - b[i] )*( a[i] - b[i] );
  return sqrt( e )/ n;
}
