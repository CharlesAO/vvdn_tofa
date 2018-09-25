#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void help( const char* );
void printsqm( const float *a, int n );
void printv( const float *a, int n );

int main( int argc, char **argv )
{
  const char *whoami, *z;
  const char *fn1 = 0, *fn2 = 0;
  int lt = 0, ut = 0, vec = 0;
  int verbose = 0, anyname = 0;
  FILE *f1, *f2;
  int i;

  whoami = argv[0];
  if(( z = strrchr( whoami, '/' )))
    whoami = z = z+1;
  if(( z = strrchr( whoami, '\\' )))
    whoami = z = z+1;
  if(( z = strstr( whoami, ".exe" )))
    *(char*)z = 0;

  // parse the command line
  for( i = 1; i < argc; ++i ) {
    if( !anyname && !strcasecmp( argv[i], "-lt" ))
      lt = 1;
    else if( !anyname && !strcasecmp( argv[i], "-ut" ))
      ut = 1;
    else if( !anyname && !strcasecmp( argv[i], "-vec" ))
      vec = 1;
    else if( !anyname && !strcasecmp( argv[i], "-v" ))
      verbose = 1;
    else if( !anyname && !strcasecmp( argv[i], "-h" ) || !strcmp( argv[i], "-?" )) {
      help( whoami );
      return 0;
    }
    else if( !anyname && !strcmp( argv[i], "--" ))
      anyname = 1;
    else if( !anyname && argv[i][0] == '-' ) {
      printf( "Unknown argument `%s'\n\n", argv[i] );
      help( whoami );
      return -1;
    }
    else if( !fn1 )
      fn1 = argv[i];
    else if( !fn2 )
      fn2 = argv[i];
    else {
      printf( "Extra argument `%s'\n\n", argv[i] );
      help( whoami );
      return -2;
    }
  }
  if( !fn1 || !fn2 ) {
    help( whoami );
    return -3;
  }
  if( vec &&( lt || ut )) {
    printf( "Cannot specify triangular flags for vectors\n" );
    help( whoami );
    return -4;
  }

  // read the files
  if( !( f1 = fopen( fn1, "rb" )))
    return printf( "Cannot read `%s'\n", fn1 );
  fseek( f1, 0, SEEK_END );
  size_t fsz = ftell( f1 );
  fseek( f1, 0, SEEK_SET );
  if( !( f2 = fopen( fn2, "rb" )))
    return printf( "Cannot read `%s'\n", fn2 );
  fseek( f2, 0, SEEK_END );
  size_t fsz2 = ftell( f2 );
  fseek( f2, 0, SEEK_SET );
  if( fsz != fsz2 )
    return printf( "Files have a different size (%lu vs %lu)\n", fsz, fsz2 );
  if( fsz & 3 )
    return printf( "Bad file size %lu, not an f32 container\n", fsz );
  int n = fsz / 4;
  int m = n;
  if( !vec ) {
    m = (int)sqrt( n );
    if( m*m != n ) {
      printf( "Matrices aren't square.\n" );
      return -5;
    }
  }
  float *a = (float*)malloc( fsz );
  float *b = (float*)malloc( fsz );
  float *e = (float*)malloc( fsz );
  if( !a || !b )
    return printf( "Not enough memory.\n" );
  if( fread( a, 1, fsz, f1 )< 1 )
    return printf( "Cannot read from <%s>\n", fn1 );
  if( fread( b, 1, fsz, f2 )< 1 )
    return printf( "Cannot read from <%s>\n", fn2 );
  fclose(f1);
  fclose(f2);

  // apply the traingular patch(-es)
  if( lt )
    for( i = 0; i < m-1; ++i ) {
      memset( a+i*m+1+i, 0, (m-1-i)*sizeof(float) );
      memset( b+i*m+1+i, 0, (m-1-i)*sizeof(float) );
    }
  if( ut )
    for( i = 1; i < m; ++i ) {
      memset( a+i*m, 0, i*sizeof(float) );
      memset( b+i*m, 0, i*sizeof(float) );
    }

  // diff the two matrices
  double err = 0.0;
  double norm = 0.0;
  for( i = 0; i < n; ++i ) {
     e[i] = a[i] - b[i];
     norm += (double)e[i] * e[i];
     err += fabs( a[i] - b[i] );
  }
  norm = sqrt( norm ) / n;
  if( verbose &&( isnan(err) || isinf(err) || err )) {
    if( !vec )
      printsqm( e, m );
    else
      printv( e, m );
  }
  if( isnan( err ))
    return printf( "%s different, error is NaN\n", vec?"Vectors":"Matrices" ), -1;
  if( isinf( err ))
    return printf( "%s different, error is Inf\n", vec?"Vectors":"Matrices" ), -2;
  if( err > 1e-4 )
    return printf( "%s different, errors: normalized=%.7g, absolute=%.7g\n", vec?"Vectors":"Matrices", norm, err ), -3;
  if( err || norm )
    printf( "%s equal, errors: normalized=%.7g, absolute=%.7g\n", vec?"Vectors":"Matrices", norm, err );
  else
    printf( "%s are binarily identical\n", vec?"Vectors":"Matrices" );
  return 0;
}

void help( const char *whoami )
{
  printf( "Use: %s [-lt] [-ut] [-v] [--] <mat1.f32> <mat2.f32> or\n"
          "     %s -vec [-v] [--] <vec1.f32> <vec2.f32> or\n"
          "Where `-lt' means lower triangular, `-ut' means upper triangular,\n"
          "      `-lt -ut' means diagonal, `-vec' means vector, and\n"
          "      `-v' enables verbose mode\n", whoami, whoami );
}

void printsqm( const float *a, int n )
{
  struct colparam_s {
    int n, p; // number of chars / number of fractional digits
    int np, nP; // chars before dot / char after, and including, dot
  } *cols;
  cols = (struct colparam_s*)malloc( n * sizeof(cols[0]) );
  // find the format for all the columns
  int j;
  for( j = 0; j < n; ++j ) {
    // figure out how the numbers should be printed
    float maxval = 1.0;
    int have_normal = 0, have_large = 0;
    int have_neg = 0, have_large_neg = 0;
    int i;
    for( i = 0; i < n; ++i ) {
      float t = a[i*n+j], tt = fabsf( t );
      if( isfinite(t) ) {
        // regular sized things
        if(( t < 0 )&&( tt >= 1.0e10 )) {
          have_large_neg = 1;
          have_large = 1;
        }
        if( !have_large ) {
          if( tt >= 0.5e-6f )
            have_normal = 1;
          if(( t > 0 )&&( tt > maxval ))
            maxval = tt;
          else if(( t < 0 )&&( 10.0*tt > maxval ))
            maxval = 10.0*tt;
        }
      }
    }
    maxval *= 0.1;
    int n_digits_pre = 1;
    while( maxval >= 1.0 )
      ++n_digits_pre, maxval *= 0.1;
    int n_digits_post = 6 - n_digits_pre;
    if( n_digits_post < 1 )
      n_digits_post = 1;
    int n_digits = 1+n_digits_pre+1+n_digits_post;
    // if we'd take more than 12 chars (abcdefghij.k) constrain
    // (will use exponent format)
    if( have_large &&( n_digits < 12+have_large_neg )) {
      n_digits_post += 12 + have_large_neg - n_digits;
      n_digits = 12 + have_large_neg;
    }
    int hdr_size = n_digits - n_digits_post + 1;
    int ftr_size = n_digits - hdr_size;
    if( !have_normal && !have_large) {
      hdr_size = 4 + have_neg;
      ftr_size = 0;
    }
    cols[j].n = n_digits;
    cols[j].p = n_digits_post;
    cols[j].np = hdr_size;
    cols[j].nP = ftr_size;
  }
  // print
  int i;
  printf( "diff[%d][%d] = { \n", n, n );
  for( i = 0; i < n; ++i ) {
    printf( "  {" );
    for( j = 0; j < n; ++j ) {
      float t = *a++, tt = fabsf(t);
      if( isnan(t) )
        printf( "%*s%*s", cols[j].np, "NaN", cols[j].nP, "" );
      else if( isinf(t) &&( t > 0 ))
        printf( "%*s%*s", cols[j].np, "Inf", cols[j].nP, "" );
      else if( isinf(t) )
        printf( "%*s%*s", cols[j].np, "-Inf", cols[j].nP, "" );
      else if( tt >= 1.0e10 )
        printf( "%*.5e", cols[j].n, t );
      else if( tt >= 0.5e-6f )
        printf( "%*.*f", cols[j].n, cols[j].p, t );
      else if( tt )
        printf( "%*s%-*s", cols[j].np-1, "0.", cols[j].nP+1, "0" );
      else
        printf( "%*s%*s", cols[j].np, "0", cols[j].nP, "" );
    }
    printf( " }\n" );
  }
  printf( " }\n" );
}

void printv( const float *a, int n )
{
  printf( "diff[%d] =\n", n );
  printf( "  {" );
  int i;
  for( i = 0; i < n; ++i ) {
    float t = a[i], tt = fabsf( t );
    if( isnan(t) )
      printf( "  NaN" );
    else if( isinf(t) )
      printf(( t > 0 )? "  Inf":"  -Inf" );
    else if( tt >= 1.0e6 )
      printf( "  %.5e", t );
    else if( tt >= 0.5e-6f ) {
      int n_digits_pre = 1;
      float z = tt;
      while( z >= 1.0 )
        ++n_digits_pre, z *= 0.1;
      printf( "  %.*f", 6 - n_digits_pre, t );
    }
    else if( tt > 0 )
      printf(( t > 0 )? "  0.0" : "  -0.0" );
    else
      printf( "  0" );
    if( i == 15 && n > 16 ) {
      printf( " ..." );
      break;
    }
  }
  printf( "  }\n" );
}
