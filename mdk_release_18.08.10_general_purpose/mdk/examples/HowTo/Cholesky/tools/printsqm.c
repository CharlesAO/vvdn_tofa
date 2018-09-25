#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main( int argc, char **argv )
{
  int lt = 0, ut = 0, anyname = 0, bad = 0;
  const char *fname = 0;
  FILE *f;
  int i;
  for( i = 1; i < argc; ++i ) {
    if( !anyname && argv[i][0] == '-' ) {
      if( !strcasecmp( 1+argv[i], "lt" )) {
        if( lt ) bad = 1;
        lt = 1;
      }
      else if( !strcasecmp( 1+argv[i], "ltx" )) {
        if( lt ) bad = 1;
        lt = 2;
      }
      else if( !strcasecmp( 1+argv[i], "ut" )) {
        if( ut ) bad = 1;
        ut = 1;
      }
      else if( !strcasecmp( 1+argv[i], "utx" )) {
        if( ut ) bad = 1;
        ut = 2;
      }
      else if( !strcmp( 1+argv[i], "-" ))
        anyname = 1;
      else
        bad = 1;
    }
    else {
      if( fname )
        bad = 1;
      fname = argv[i];
    }
  }
  if( bad || !fname )
    return printf( "Use: %s [-lt[x]] [-ut[x]] [--] <mat.bin>\n", argv[0] );

  if( !( f = fopen( fname, "rb" )))
    return printf( "Cannot read <%s>\n", argv[1] );
  fseek( f, 0, SEEK_END );
  size_t fsz = ftell( f );
  fseek( f, 0, SEEK_SET );
  if( fsz & 3 )
    return printf( "Bad file size %lu, not a matrix of floats\n", fsz );
  int n = (int)sqrt( (double)(fsz/4) );
  int m = n;
  if( n*m*4 != fsz ) {
    m = (int)sqrt( (double)(fsz/8) );
    n = 2*m;
    if( n*m*4 != fsz ) {
      m = (int)sqrt( (double)(fsz/12) );
      n = 3*m;
      if( n*m*4 != fsz ) {
        return printf( "Suspicious file size %lu, not 1/2/3 square matrices\n", fsz );
      }
    }
  }
  float *a = (float*)malloc( fsz );
  if( !a )
    return printf( "Not enough memory.\n" );
  if( fread( a, 1, fsz, f )< 1 )
    return printf( "Cannot read from <%s>\n", argv[1] );
  fclose(f);
  // cleanup above/below MD, if needed
  if( lt || ut ) {
    // foreach matrix
    for( i = 0; i < n; i += m ) {
      float *b = a + i*m*m;
      // foreach row
      int j;
      for( j = 0; j < m; ++j ) {
        if( lt == 2 && j < m-2 ) // lower triangular plus extra diagonal above
          memset( &b[j*m+j+2], 0, (m-(j+2))*sizeof(float) );
        else if( lt == 1 && j < m-1 ) // lower triangular
          memset( &b[j*m+j+1], 0, (m-(j+1))*sizeof(float) );
        if( ut == 2 && j > 1 ) // upper triangular plus extra diagonal above
          memset( &b[j*m], 0, (j-1)*sizeof(float) );
        else if( ut == 1 && j > 0 ) // lower triangular
          memset( &b[j*m], 0, j*sizeof(float) );
      }
    }
  }
  for( i = 0; i < n; ++i ) {
    if( !( i % m ))
      printf( "%c[%d][%d] =\n", 'A'+i/m, m, m );
    printf( "  {" );
    int j;
    for( j = 0; j < m; ++j ) {
      float t = a[i*m+j];
      if( isnan(t) )
        printf( "    NaN   " );
      else if( isinf(t) &&( t > 0 ))
        printf( "    Inf   " );
      else if( isinf(t) )
        printf( "   -Inf   " );
      else if( fabsf(t) >= 0.5e-6f )
        printf( "% 10.6f", t );
      else
        printf( "     0    " );
    }
    printf( " }\n" );
  }
  return 0;
}
