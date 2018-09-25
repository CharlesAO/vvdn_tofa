#include <sampleApp.h>
#include <svuCommonShave.h>

// define your variables HERE
#define MAX_WIDTH 1920
unsigned char __attribute__ ((aligned (16))) in[MAX_WIDTH];
unsigned int  __attribute__ ((aligned (16))) width;
unsigned char __attribute__ ((aligned (16))) value;
unsigned char __attribute__ ((aligned (16))) op;
unsigned int  __attribute__ ((aligned (16))) pxToChange;
unsigned char __attribute__ ((aligned (16))) out[MAX_WIDTH];

int main( void)
{  
  
  //your code HERE
  sampleApp_asm(in, width, value, op, pxToChange, out);
  
  SHAVE_HALT;
  return 0;
}
