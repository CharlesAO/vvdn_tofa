#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "lib/solver.h"

#ifndef N
# define N 504
#endif

float *A;  // input : NxN system matrix
float *b;  // input : Nx dangling coefficients
float *x;  // output: Nx solution

float *M;  // SPD matrix for A, triangular (M=A'A), symmetric storage; NxN
float *c;  // adjusted vector for b (c=A'b); N
float *L;  // Cholesky matrix from M, symmetric storage; NxN
float *y;  // solution for the triangular solve lower step; N

float *B;  // A^-1 as produced by Gauss-Jordan; NxN
float *z;  // solution produced using Gauss-Jordan; N

// checking the results
float *p; // Ax (cholesky); N
float *q; // Az (gauss-jordan); N

int n = N;
int seed = N/4;
float min = -1.0;
float max = 1.0;
int verbose = 0, quiet = 0;
int alldata = 0;
int gendata = 0;
int genintdata = 0;
int use_gj = 1;

void parse_args( int, char** );
void mem_alloc();
void reset_stats();

int main( int argc, char **argv )
{
  // parse command line
  parse_args( argc, argv );
start:
  printf( "Solving an n=%d system, RNG seed=%d, vals=[%.2f:%.2f)\n", n, seed, min, max );
  fflush( stdout );
  srand( seed );
  
  // allocate the matrices
  mem_alloc();
  
  // generate A
  genf( A, n*n, min, max );
  // check that the system is non-singular
  syrk_tn_l( M, A, n );
  potrf_ln( L, M, n );
  if( isnan( L[(n-1)*n+(n-1)] )|| 
      isinf( L[(n-1)*n+(n-1)] )||
      ( fabsf( L[(n-1)*n+(n-1)] )< 1e-6 ))
  {
    printf( "WARNING: RNG seed %d produced an indeterminate system.\n"
            "         Trying %d...\n", seed, seed+1 );
    ++seed;
    goto start;
  }
  gemv_t( c, A, b, n );
  if( verbose ||( !quiet &&( n <= 16 )))
    printm( "A", A, n );
  if( gendata )
    savem( A, n, "A" );
  // generate b
  genf( b, n, min, max );
  if( verbose ||( !quiet &&( n <= 16 )))
    printv( "b", b, n );
  if( gendata )
    savev( b, n, "b" );

  /*[ Cholesky based solution ]*/
  printf( "\n* CHOLESKY solution *\n" );
  reset_stats();

  // c = A'b
  gemv_t( c, A, b, n );
  if( verbose ||( !quiet &&( n <= 16 )))
    printv( "c = A'b", c, n );
  if( genintdata )
    savev( c, n, "c" );
  // M = A'A
  syrk_tn_l( M, A, n );
  if( verbose ||( !quiet &&( n <= 16 )))
    printm( "M = A'A", M, n );
  if( genintdata )
    savem( M, n, "M" );
  // LL' = M
  potrf_ln( L, M, n );
  if( verbose ||( !quiet &&( n <= 16 )))
    printm( "L | LL'=M", L, n );
  if( genintdata )
    savem( L, n, "L" );
  // solve Ly = c
  trsv_ln( y, L, c, n );
  if( verbose ||( !quiet &&( n <= 16 )))
    printv( "y = c\\L", y, n );
  if( genintdata )
    savev( y, n, "y" );
  // solve L'x = y
  trsv_lt( x, L, y, n );
  if( verbose ||( !quiet &&( n <= 16 )))
    printv( "x = y\\L'", x, n );
  if( gendata )
    savev( x, n, "x" );

  printf( "Total operations for Chokesky flow:\n"
          "  adds=%llu muls=%llu divs=%llu sqrts=%llu\n"
          "  scans=%llu\n",
          n_add, n_mul, n_div, n_sqrt, 
          n_scan );
  // p = Ax
  gemv_n( p, A, x, n );
  // e = ||p[i]-b[i]||
  double e = norm( p, b, n );
  printf( "Normalized error of the Cholesky model (||b-Ax||): %.7g\n", e );

  if( use_gj ) {
    /*[ Gauss-Jordan based solution ]*/
    printf( "\n* GAUSS-JORDAN solution *\n" );
    reset_stats();

    // B = A^-1
    gauss( B, A, n );
    // printm( "B = A^-1", B, n ); // no printing for Gauss-Jordan
    // z = Bb
    gemv_n( z, B, b, n );
    // printv( "z = Bb", z, n );  // no printing for Gauss-Jordan

    printf( "Total operations for Gauss-Jordan flow:\n"
            "  adds=%llu muls=%llu divs=%llu sqrts=%llu\n"
            "  scans=%llu\n",
            n_add, n_mul, n_div, n_sqrt, 
            n_scan );

    // q = Az
    gemv_n( q, A, z, n );
    e = norm( q, b, n );
    printf( "Normalized error of the Gauss-Jordan model (||b-Ax||): %.7g\n", e );
  }
  return 0;
}

void help( const char* );

void parse_args( int argc, char **argv )
{
  int i;
  for( i = 1; i < argc; ++i ) {
    if( !strcmp( argv[i], "-h" ) || !strcmp( argv[i], "--help" )) {
      help( argv[0] );
      exit( 0 );
    }
    if( !strcmp( argv[i], "-noGJ" ) || !strcmp( argv[i], "--disableGaussJordan" ) )
      use_gj = 0;
    else if( !strcmp( argv[i], "-g" ) || !strcmp( argv[i], "--gendata" ) )
      gendata = 1;
    else if( !strcmp( argv[i], "-gi" ) || !strcmp( argv[i], "--genintdata" ) )
      genintdata = 1;
    else if( !strcmp( argv[i], "-vv" ) || !strcmp( argv[i], "--alldata" ) ) {
      verbose = 1; quiet = 0; alldata = 1;
    }
    else if( !strcmp( argv[i], "-v" ) || !strcmp( argv[i], "--verbose" ) ) {
      verbose = 1; quiet = 0;
    }
    else if( !strcmp( argv[i], "-q" ) || !strcmp( argv[i], "--quiet" ) ) {
      verbose = 0; quiet = 1;
    }
    else if( !strcmp( argv[i], "-n" ) &&( i != argc-1 )) {
      n = atoi( argv[++i] );
      if(( n < 4 )||( n & 3 )) {
        printf( "error: matrix size has to be a positive, not null, multiple of 4 (got `%s')\n", argv[i] );
        exit( 1 );
      }
    }
    else if(( !strcmp( argv[i], "-s" )|| !strcmp( argv[i], "--seed" ))&&( i != argc-1 ))
      seed = atoi( argv[++i] );
    else if(( !strcmp( argv[i], "-m" )|| !strcmp( argv[i], "--minval" ))&&( i != argc-1 ))
      min = atof( argv[++i] );
    else if(( !strcmp( argv[i], "-M" )|| !strcmp( argv[i], "--maxval" ))&&( i != argc-1 ))
      max = atof( argv[++i] );
    else {
      printf( "Invalid syntax near argument `%s'\n", argv[i] );
      help( argv[0] );
      exit( 3 );
    }
  }
  if( min >= max ) {
    printf( "error: minimum value (%.2f) has to be less than maximum value (%.2f)\n", min, max );
    exit( 2 );
  }  
}

void help( const char *name )
{
  printf( "Use: %s [options]\n", name );
  printf( "Possible options:\n" );
  printf( "  -n <n>   : matrix size; has to be 4*k, with k>0 (default: %u)\n", N );
  printf( "  -s <seed>: random number generator seed (default: %u)\n", N/4 );
  printf( "  -m <min> : minimum value to be generated (default: -1.0)\n" );
  printf( "  -M <max> : maximum value to be generated; has to be greater than <min>;\n"
          "             (default: 1.0)\n" );
  printf( "  -v       : verbose mode; print data (default: verbose for n <= 16)\n" );
  printf( "  -vv      : really verbose mode; prints all the data (no limit on 16 rows/cols)\n" );
  printf( "  -q       : quiet mode; do not print data (default: quiet for n > 16)\n" );
  printf( "  -g       : generate input (A,b) / output (x) data files\n" );
  printf( "  -gi      : generate intermediate (M,c,L,y) data files\n" );
  printf( "  -noGJ    : disable Gauss/Jordan model\n" );
  printf( "\n" );
}

void mem_alloc() {
  A = (float*)malloc( n*n*sizeof(float) );
  b = (float*)malloc(   n*sizeof(float) );
  x = (float*)malloc(   n*sizeof(float) );

  M = (float*)malloc( n*n*sizeof(float) );
  c = (float*)malloc(   n*sizeof(float) );
  L = (float*)malloc( n*n*sizeof(float) );
  y = (float*)malloc(   n*sizeof(float) );

  B = (float*)malloc( n*n*sizeof(float) );
  z = (float*)malloc(   n*sizeof(float) );

  p = (float*)malloc(   n*sizeof(float) );
  q = (float*)malloc(   n*sizeof(float) );
  
  if( !A || !b || !x || !M || !c || !L || !y || !B || !z || !p || !q ) {
    printf( "error: not enough memory\n" );
    exit( 4 );
  }
}
