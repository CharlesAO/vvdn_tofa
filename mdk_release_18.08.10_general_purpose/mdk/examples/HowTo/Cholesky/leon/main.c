#include <stdio.h>
#include <string.h>
#include <DrvLeonL2C.h>

#include <DrvLeonL2C.h>
#include <DrvSvu.h>
#include <DrvTimer.h>

#include "app_config.h"


#define N 100

// stack debug stuff
extern u8 __noinit_end[];
extern u8 _LEON_STACK_POINTER[];

// float __attribute__((noinit)) ...;
float __attribute__((section(".noinit"),aligned(16))) A[N][N];
float __attribute__((section(".noinit"),aligned(16))) b[N];
float __attribute__((section(".noinit"),aligned(16))) x[N];

float __attribute__((section(".noinit"),aligned(16))) tmp4cols[N][4]; // temp for in-place syrk

// backup areas for M[], c[] and y[] - which would be modified in-place
float __attribute__((section(".ddr.bss"),aligned(16))) bak_M[N][N];
float __attribute__((section(".ddr.bss"),aligned(16))) bak_c[N];
float __attribute__((section(".ddr.bss"),aligned(16))) bak_y[N];

// don't ask... :)
int __attribute__((section(".ddr.data"))) _force_ddr_init = 1;

// profiling thing
int sysclk_khz;
tyTimeStamp t[5];
extern const char *fns[5];

#define invd_cache() \
  { DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_INVALIDATE,0); asm volatile( "flush" ); }

void do_gemvl_t();
void do_syrk_tn_l();
void do_potrf_ln();
void do_trsvl_ln();
void do_trsvl_lt();

int main()
{
  initClocksAndMemory();
  // force initializing of the ddr, by actually having an used variable in .ddr.data
  // (just .ddr.bss sections aren't enough)
  _force_ddr_init = 2;

  // print the configuration
  sysclk_khz = DrvCprGetClockFreqKhz( SYS_CLK, 0 );
  printf( "System running at %u.%02uMHz\n"
          "Leon running from CMX slice %u; stack: size=0x%X, top=%p\n\n"
          "Buffers: A[%3u][%3u] @%p..%p ; A, then A'A, then L\n"
          "         b[%3u]      @%p..%p ; b\n"
          "         x[%3u]      @%p..%p ; A'b, then L\\(A'b),\n"
          "                                                      then L'\\y\n"
          "         tmp[%3u]    @%p..%p ; in-place syrk buffer\n\n",
    sysclk_khz/1000, (sysclk_khz%1000+5)/10,
    ((unsigned)&main - 0x70000000)>>17, (unsigned)_LEON_STACK_POINTER - (unsigned)__noinit_end, _LEON_STACK_POINTER,
    N, N,     A, (void*)( (u32)A      + N*N*sizeof(float) - 1 ),
    N,        b, (void*)( (u32)b        + N*sizeof(float) - 1 ),
    N,        x, (void*)( (u32)x        + N*sizeof(float) - 1 ),
    N, tmp4cols, (void*)( (u32)tmp4cols + N*sizeof(float) - 1 )
  );

  // cleanup the c / y / x buffer - fill it with NaN's
  memset( x, -1, sizeof(x) );
  
  swcResetShave( 0 );
  swcSetAbsoluteDefaultStack( 0 );

  printf( "Running a full lineq sequence...\n" );

  // perform gemvl_t() and syrk_tn_l()
  do_gemvl_t();
  invd_cache();
  do_syrk_tn_l();
  invd_cache();

  memcpy( bak_M, A, sizeof(A) );
  memcpy( bak_c, x, sizeof(x) );

  // perform potrf_ln()
  do_potrf_ln();
  invd_cache();

  // perform trsv_ln()
  do_trsvl_ln();
  invd_cache();

  memcpy( bak_y, x, sizeof(x) );

  // perform trsv_lt()
  do_trsvl_lt();
  invd_cache();

  printf( "All done.\n" );

  printf( "\nComputation took:\n" );
  unsigned i;
  tyTimeStamp t_all = 0;
  for( i = 0; i < sizeof(fns)/sizeof(fns[0]); ++i ) {
    t_all += t[i];
    printf( "  - %-14s: ", fns[i] );
    unsigned t_100us = (u32)( t[i] /( sysclk_khz / 10 ));
    printf( "%5u.%ums ", t_100us/10, t_100us%10 );
    printf( "(%11llucycles)\n", t[i] );
  }
  printf( "Total: " );
  unsigned t_100us = (unsigned)( t_all /( sysclk_khz / 10 ));
  printf( "%u.%ums ", t_100us/10, t_100us%10 );
  printf( "(%llucycles)\n", t_all );

  printf( "All done.\n" );
  return 0;
}



 void __attribute__((noreturn)) _exit();
extern u8 Cholesky0__exit[];

// gemvl_t( float *c, const float *A, const float *b, int n, int sA );
extern u8 Cholesky0_gemvl_t[];

void do_gemvl_t()
{
  const void *p = Cholesky0_gemvl_t;
  
  printf( ". gemvl_t( x @%p, A @%p, b @%p, %d, 0x%X )\n", 
          x, A, b, N, N*sizeof(float) );
  swcResetShave( 0 );
  swcSetAbsoluteDefaultStack( 0 );
  // set SHAVE's return address to _exit
  DrvSvutIrfWrite( 0, 30, (u32)Cholesky0__exit );
  DrvTimerStartTicksCount( &t[0] );  
  // gemvl_t( x, A, b, N, N*sizeof(float) );
  swcStartShaveCC( 0, (u32)p, "iiiii", x, A, b, N, N*sizeof(float) );
  swcWaitShave( 0 );
  DrvTimerGetElapsedTicks( &t[0], &t[0] );
}

// syrk_tn_l( float *A_, float *cA, int n, int sA );
extern u8 Cholesky0_syrk_tn_l[];

void do_syrk_tn_l()
{
  const void *p = Cholesky0_syrk_tn_l;


  printf( ". syrk_tn_l( A @%p, tmp @%p, %d, 0x%X )\n", 
          A, tmp4cols, N, N*sizeof(float) );
  // set SHAVE's return address to _exit
  DrvSvutIrfWrite( 0, 30, (u32)Cholesky0__exit );
  DrvTimerStartTicksCount( &t[1] );
  // syrk_tn_l( M, tmp4cols, N, N*sizeof(float) );
  swcStartShaveCC( 0, (u32)p, "iiii", A, tmp4cols, N, N*sizeof(float));
  swcWaitShave( 0 );
  DrvTimerGetElapsedTicks( &t[1], &t[1] );
}

// potrf_ln( float *L, int n, int sL );
extern u8 Cholesky0_potrf_ln[];

void do_potrf_ln() {

  const void *p = Cholesky0_potrf_ln;


  printf( ". potrf_ln( A @%p, %d, 0x%X );\n", 
          A, N, N*sizeof(float) );
  // set SHAVE's return address to _exit
  DrvSvutIrfWrite( 0, 30, (u32)Cholesky0__exit );
  DrvTimerStartTicksCount( &t[2] );
  // potrf_l( A, N, N*sizeof(float) );
  swcStartShaveCC( 0, (u32)p, "iii", A, N, N*sizeof(float) );
  swcWaitShave( 0 );
  DrvTimerGetElapsedTicks( &t[2], &t[2] );
}

// trsvl_ln( float *y, const float *L, float *b, int n, int sL );
extern u8 Cholesky0_trsvl_ln[];

void do_trsvl_ln() {
  const void *p = Cholesky0_trsvl_ln;

  printf( ". trsvl_ln( x @%p, A @%p, x @%p, %d, 0x%X )\n", 
          x, A, x, N, N*sizeof(float) );
  // set SHAVE's return address to _exit
  DrvSvutIrfWrite( 0, 30, (u32)Cholesky0__exit );
  DrvTimerStartTicksCount( &t[3] );
  // trsvl_ln( x, A, x, N, N*sizeof(float) );
  swcStartShaveCC( 0, (u32)p, "iiiii", x, A, x, N, N*sizeof(float) );
  swcWaitShave( 0 );
  DrvTimerGetElapsedTicks( &t[3], &t[3] );
}

// trsvl_lt( float *x, const float *L, float *y, int n, int sL );
extern u8 Cholesky0_trsvl_lt[];

void do_trsvl_lt() {

  const void *p = Cholesky0_trsvl_lt;

  printf( ". trsvl_lt( x @%p, A @%p, x @%p, %d, 0x%X )\n", 
          x, A, x, N, N*sizeof(float) );
  // set SHAVE's return address to _exit
  DrvSvutIrfWrite( 0, 30, (u32)Cholesky0__exit );
  DrvTimerStartTicksCount( &t[4] );
  // trsvl_lt( x, A, x, N, N*sizeof(float) );
  swcStartShaveCC( 0, (u32)p, "iiiii", x, A, x, N, N*sizeof(float) );
  swcWaitShave( 0 );
  DrvTimerGetElapsedTicks( &t[4], &t[4] );
}

const char *fns[] = {"gemvl_t@opt","syrk_tn_l@opt","potrf_ln@opt","trsvl_ln@opt","trsvl_lt@opt"};

