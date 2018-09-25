#include "Stereo.h"
#include <Fp16Convert.h>
#include <sipp.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <filters/disp2depth/disp2depth.h>
#include <filters/subpixelFilter/subpixelFilter.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include <OsDrvCmxDma.h>
#include "lutfiller.h"
#include "mvLog.h"
#include "registersMyriadMa2x8x.h"
//#define PRINT_T
//#define FR_DEBUG
//#define SU_DEBUG

//#define DEBUG_USE_STATIC_BUFFERS

#ifdef FR_DEBUG
#define frprintk printk
#else
#define frprintk(...)
#endif


#ifdef SU_DEBUG
#define suprintk printk
#else
#define suprintk(...)
#endif


#ifdef PRINT_TIME
#include "utils.h"
#endif

extern "C" void sippPlatformInitAsync();
extern u8   mbinImgSterSipp[];
uint16_t aggLut[] ALIGNED(16) __attribute__((section(".cmx.data"))) =   //precomputed P1=250, P2=500)
{
    0xfffa,0xfffa,0xfa7d,0xa653,0x7d3e,0x6432,0x5329,0x4723,0x3e1f,0x371b,0x3219,0x2d16,0x2914,0x2613,0x2311,0x2110,
    0x1f0f,0x1d0e,0x1b0d,0x1a0d,0x190c,0x170b,0x160b,0x150a,0x140a,0x140a,0x1309,0x1209,0x1108,0x1108,0x1008,0x1008,
    0x0f07,0x0f07,0x0e07,0x0e07,0x0d06,0x0d06,0x0d06,0x0c06,0x0c06,0x0c06,0x0b05,0x0b05,0x0b05,0x0b05,0x0a05,0x0a05,
    0x0a05,0x0a05,0x0a05,0x0904,0x0904,0x0904,0x0904,0x0904,0x0804,0x0804,0x0804,0x0804,0x0804,0x0804,0x0804,0x0703,
    0x0703,0x0703,0x0703,0x0703,0x0703,0x0703,0x0703,0x0703,0x0603,0x0603,0x0603,0x0603,0x0603,0x0603,0x0603,0x0603,
    0x0603,0x0603,0x0603,0x0603,0x0502,0x0502,0x0502,0x0502,0x0502,0x0502,0x0502,0x0502,0x0502,0x0502,0x0502,0x0502,
    0x0502,0x0502,0x0502,0x0502,0x0502,0x0402,0x0402,0x0402,0x0402,0x0402,0x0402,0x0402,0x0402,0x0402,0x0402,0x0402,
    0x0402,0x0402,0x0402,0x0402,0x0402,0x0402,0x0402,0x0402,0x0402,0x0402,0x0402,0x0402,0x0402,0x0402,0x0301,0x0301,
    0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,
    0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,
    0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0301,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,
    0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,
    0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,
    0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,
    0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,
    0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0201,0x0100,0x0100,0x0100,0x0100,0x0100,
};

uint8_t subpixelLUT5bits[] ALIGNED(16) __attribute__((section(".cmx.data"))) =
{
 0,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
16, 0, 8,11,12,13,13,14,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
16, 8, 0, 5, 8,10,11,11,12,12,13,13,13,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
16,11, 5, 0, 4, 6, 8, 9,10,11,11,12,12,12,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
16,12, 8, 4, 0, 3, 5, 7, 8, 9,10,10,11,11,11,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
16,13,10, 6, 3, 0, 3, 5, 6, 7, 8, 9, 9,10,10,11,11,11,12,12,12,12,12,13,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,15,15,
16,13,11, 8, 5, 3, 0, 2, 4, 5, 6, 7, 8, 9, 9,10,10,10,11,11,11,11,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
16,14,11, 9, 7, 5, 2, 0, 2, 4, 5, 6, 7, 7, 8, 9, 9, 9,10,10,10,11,11,11,11,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
16,14,12,10, 8, 6, 4, 2, 0, 2, 3, 4, 5, 6, 7, 7, 8, 8, 9, 9,10,10,10,10,11,11,11,11,11,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,
16,14,12,11, 9, 7, 5, 4, 2, 0, 2, 3, 4, 5, 6, 6, 7, 8, 8, 8, 9, 9, 9,10,10,10,10,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,14,14,14,14,14,14,
16,14,13,11,10, 8, 6, 5, 3, 2, 0, 1, 3, 4, 5, 5, 6, 7, 7, 8, 8, 8, 9, 9, 9,10,10,10,10,10,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
16,15,13,12,10, 9, 7, 6, 4, 3, 1, 0, 1, 2, 3, 4, 5, 6, 6, 7, 7, 8, 8, 8, 9, 9, 9, 9,10,10,10,10,10,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,13,13,13,13,
16,15,13,12,11, 9, 8, 7, 5, 4, 3, 1, 0, 1, 2, 3, 4, 5, 5, 6, 6, 7, 7, 8, 8, 8, 9, 9, 9, 9,10,10,10,10,10,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,13,
16,15,14,12,11,10, 9, 7, 6, 5, 4, 2, 1, 0, 1, 2, 3, 4, 4, 5, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 9,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,
16,15,14,13,11,10, 9, 8, 7, 6, 5, 3, 2, 1, 0, 1, 2, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 9, 9,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
16,15,14,13,12,11,10, 9, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 3, 4, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,
16,15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 9, 9,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,11,12,12,12,12,12,12,12,
16,15,14,13,12,11,10, 9, 8, 8, 7, 6, 5, 4, 3, 2, 1, 0, 1, 2, 2, 3, 4, 4, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9,10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,11,11,12,12,12,
16,15,14,13,12,12,11,10, 9, 8, 7, 6, 5, 4, 4, 3, 2, 1, 0, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9,10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,11,11,
16,15,14,13,13,12,11,10, 9, 8, 8, 7, 6, 5, 4, 3, 3, 2, 1, 0, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 6, 6, 7, 7, 7, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,
16,15,14,14,13,12,11,10,10, 9, 8, 7, 6, 6, 5, 4, 3, 2, 2, 1, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9,10,10,10,10,10,10,10,10,10,11,11,11,11,11,
16,15,14,14,13,12,11,11,10, 9, 8, 8, 7, 6, 5, 5, 4, 3, 2, 2, 1, 0, 1, 1, 2, 3, 3, 4, 4, 4, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9,10,10,10,10,10,10,10,10,10,10,11,11,
16,15,15,14,13,12,12,11,10, 9, 9, 8, 7, 7, 6, 5, 4, 4, 3, 2, 1, 1, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9,10,10,10,10,10,10,10,10,10,
16,15,15,14,13,13,12,11,10,10, 9, 8, 8, 7, 6, 6, 5, 4, 3, 3, 2, 1, 1, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9,10,10,10,10,10,10,10,
16,15,15,14,13,13,12,11,11,10, 9, 9, 8, 7, 7, 6, 5, 5, 4, 3, 3, 2, 1, 1, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9,10,10,10,10,
16,15,15,14,13,13,12,12,11,10,10, 9, 8, 8, 7, 6, 6, 5, 4, 4, 3, 3, 2, 1, 1, 0, 1, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9,10,10,
16,15,15,14,14,13,12,12,11,10,10, 9, 9, 8, 7, 7, 6, 6, 5, 4, 4, 3, 2, 2, 1, 1, 0, 1, 1, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 9,
16,15,15,14,14,13,12,12,11,11,10, 9, 9, 8, 8, 7, 7, 6, 5, 5, 4, 4, 3, 2, 2, 1, 1, 0, 1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9,
16,15,15,14,14,13,13,12,11,11,10,10, 9, 9, 8, 7, 7, 6, 6, 5, 5, 4, 3, 3, 2, 2, 1, 1, 0, 1, 1, 2, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9,
16,15,15,14,14,13,13,12,12,11,10,10, 9, 9, 8, 8, 7, 7, 6, 6, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8, 9, 9,
16,15,15,14,14,13,13,12,12,11,11,10,10, 9, 9, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 1, 1, 1, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, 8, 8,
16,15,15,14,14,13,13,12,12,11,11,10,10, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0, 1, 1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8,
16,16,15,14,14,14,13,12,12,12,11,10,10,10, 9, 8, 8, 8, 7, 6, 6, 6, 5, 4, 4, 4, 3, 2, 2, 2, 1, 0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 8, 8, 8,
16,16,15,15,14,14,13,13,12,12,11,11,10,10, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 7, 8,
16,16,15,15,14,14,13,13,12,12,11,11,10,10, 9, 9, 8, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7,
16,16,15,15,14,14,13,13,12,12,11,11,11,10,10, 9, 9, 8, 8, 7, 7, 6, 6, 5, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7,
16,16,15,15,14,14,13,13,12,12,12,11,11,10,10, 9, 9, 8, 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 7, 7, 7,
16,16,15,15,14,14,13,13,13,12,12,11,11,10,10,10, 9, 9, 8, 8, 7, 7, 6, 6, 6, 5, 5, 4, 4, 3, 3, 3, 2, 2, 1, 1, 0, 0, 0, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 7,
16,16,15,15,14,14,13,13,13,12,12,11,11,11,10,10, 9, 9, 8, 8, 8, 7, 7, 6, 6, 5, 5, 5, 4, 4, 3, 3, 3, 2, 2, 1, 1, 0, 0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6,
16,16,15,15,14,14,14,13,13,12,12,11,11,11,10,10, 9, 9, 9, 8, 8, 7, 7, 7, 6, 6, 5, 5, 5, 4, 4, 3, 3, 2, 2, 2, 1, 1, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6,
16,16,15,15,14,14,14,13,13,12,12,12,11,11,10,10,10, 9, 9, 8, 8, 8, 7, 7, 6, 6, 6, 5, 5, 4, 4, 4, 3, 3, 2, 2, 2, 1, 1, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6,
16,16,15,15,14,14,14,13,13,12,12,12,11,11,11,10,10, 9, 9, 9, 8, 8, 7, 7, 7, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 2, 2, 2, 1, 1, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6,
16,16,15,15,14,14,14,13,13,13,12,12,11,11,11,10,10,10, 9, 9, 8, 8, 8, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5,
16,16,15,15,15,14,14,13,13,13,12,12,12,11,11,10,10,10, 9, 9, 9, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
16,16,15,15,15,14,14,13,13,13,12,12,12,11,11,11,10,10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 1, 1, 1, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5,
16,16,15,15,15,14,14,14,13,13,12,12,12,11,11,11,10,10,10, 9, 9, 9, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5,
16,16,15,15,15,14,14,14,13,13,13,12,12,11,11,11,10,10,10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4,
16,16,15,15,15,14,14,14,13,13,13,12,12,12,11,11,11,10,10,10, 9, 9, 9, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4,
16,16,15,15,15,14,14,14,13,13,13,12,12,12,11,11,11,10,10,10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4,
16,16,15,15,15,14,14,14,13,13,13,12,12,12,11,11,11,10,10,10, 9, 9, 9, 8, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4,
16,16,15,15,15,14,14,14,13,13,13,12,12,12,12,11,11,11,10,10,10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3,
16,16,15,15,15,14,14,14,13,13,13,13,12,12,12,11,11,11,10,10,10, 9, 9, 9, 8, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3,
16,16,15,15,15,14,14,14,14,13,13,13,12,12,12,11,11,11,10,10,10,10, 9, 9, 9, 8, 8, 8, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3,
16,16,15,15,15,14,14,14,14,13,13,13,12,12,12,11,11,11,11,10,10,10, 9, 9, 9, 8, 8, 8, 8, 7, 7, 7, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3,
16,16,15,15,15,15,14,14,14,13,13,13,12,12,12,12,11,11,11,10,10,10, 9, 9, 9, 9, 8, 8, 8, 7, 7, 7, 7, 6, 6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
16,16,15,15,15,15,14,14,14,13,13,13,13,12,12,12,11,11,11,10,10,10,10, 9, 9, 9, 8, 8, 8, 8, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2,
16,16,15,15,15,15,14,14,14,13,13,13,13,12,12,12,11,11,11,11,10,10,10, 9, 9, 9, 9, 8, 8, 8, 7, 7, 7, 7, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 2, 2,
16,16,15,15,15,15,14,14,14,13,13,13,13,12,12,12,12,11,11,11,10,10,10,10, 9, 9, 9, 8, 8, 8, 8, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 2,
16,16,15,15,15,15,14,14,14,14,13,13,13,12,12,12,12,11,11,11,10,10,10,10, 9, 9, 9, 9, 8, 8, 8, 7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1,
16,16,15,15,15,15,14,14,14,14,13,13,13,12,12,12,12,11,11,11,11,10,10,10, 9, 9, 9, 9, 8, 8, 8, 8, 7, 7, 7, 7, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1,
16,16,15,15,15,15,14,14,14,14,13,13,13,13,12,12,12,11,11,11,11,10,10,10,10, 9, 9, 9, 9, 8, 8, 8, 7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 1, 1,
16,16,15,15,15,15,14,14,14,14,13,13,13,13,12,12,12,12,11,11,11,10,10,10,10, 9, 9, 9, 9, 8, 8, 8, 8, 7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0, 1,
16,16,15,15,15,15,14,14,14,14,13,13,13,13,12,12,12,12,11,11,11,11,10,10,10,10, 9, 9, 9, 9, 8, 8, 8, 7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0, 0,
16,16,15,15,15,15,14,14,14,14,13,13,13,13,12,12,12,12,11,11,11,11,10,10,10,10, 9, 9, 9, 9, 8, 8, 8, 8, 7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 3, 3, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 0, 0
};

//half disp2depthLUT[256] __attribute__((section(".shv2.S.data")));
#ifdef BUILD_WARP_OUT
UInt8  DDR_DATA oBufWarp[SIPP_CV_OPIPE_NUM_INPUTS][1280*720*2] ALIGNED(16);
#endif

#ifdef DEBUG_USE_STATIC_BUFFERS
uint8_t StatLeft[1024*1024]__attribute__((section(".ddr.bss")));
uint8_t StatRight[1024*1024]__attribute__((section(".ddr.bss")));
uint8_t StatDisp[1024*1024]__attribute__((section(".ddr.bss")));
uint8_t StatDepth[1024*1024]__attribute__((section(".ddr.bss")));
#endif
std::vector <objpipemap> objpipe;
bool sipp_initialised = false;



void FlipHomogHorz(float *homog ,uint32_t width)
{
    float a = homog[0];
    float c = homog[2];
    float d = homog[3];
    float f = homog[5];
    float g = homog[6];
    float i = homog[8];

    homog[0] = -a;
    homog[2] = (width * a) + c;
    homog[3] = -d;
    homog[5] = (width * d) + f;
    homog[6] = -g;
    homog[8] = (width * g) + i;
}

void wsgbmCallback ( SippPipeline *             pPipeline,
                       eSIPP_PIPELINE_EVENT       eEvent,
                       SIPP_PIPELINE_EVENT_DATA * ptEventData
                     )
{
    rtems_status_code sc;

    UNUSED(ptEventData);
    frprintk("wsgbmCallback eEvent:%d \n",eEvent);

   auto iter = std::find_if(objpipe.begin(), objpipe.end(),  [&pPipeline](objpipemap& f)
    {
        return f.pl_wsgbm == pPipeline;
    } );

    Stereo *myself = (Stereo*)iter->obj;


    if (eEvent == eSIPP_PIPELINE_FRAME_DONE)
    {
        sc = rtems_semaphore_release(myself->wsgbm_cb_sem);
        frprintk("wsgbmCallback eSIPP_PIPELINE_FRAME_DONE\n");
        assert(sc == RTEMS_SUCCESSFUL);
    }

}

void Stereo::CreateSipp()
{
    suprintk("CreateSipp\n");

    objpipemap objpl;

    //u32    sippErrorLog[SIPP_ERROR_HISTORY_SIZE];
    if (sipp_initialised == false)
    {

    #ifndef SIPP_PC
        // Initialise CMX DMA driver
        OsDrvCmxDmaInitialize (NULL);
        sippPlatformInitAsync();
    #endif

        sipp_initialised = true;
    }
    if(0==wsgbm_cb_sem)
    {
        rtems_status_code sc = rtems_semaphore_create(rtems_build_name('W', 'S', 'G', 'B'), 0,
                                RTEMS_SIMPLE_BINARY_SEMAPHORE, 0, &wsgbm_cb_sem);
        assert(sc == RTEMS_SUCCESSFUL);
        suprintk("wsgbm_cb_sem created\n");

    }
    else
    {
        suprintk("wsgbm_cb_sem NOT created\n");
    }

    SetDefaultStereoParam();
    SetMaskSize(cfg_.stereocore.ctCfg.ctKernel);

    if(!cfg_.stereocore.algoCfg.subpixelEn)
    {
        buildStereoPipe (&stereoPipe);
    }
    else
    {
        buildStereoPipeSubpixel (&stereoPipe);
    }
    configStereoPipe (&stereoPipe);

    if (cfg_.wsgbmEn)
    {
        sippRegisterEventCallback (stereoPipe.pl_wsgbm,
                                   wsgbmCallback);
        suprintk("sippFinalizePipeline wsgbm\n");
        sippFinalizePipeline (stereoPipe.pl_wsgbm);
    }

    objpl.obj = this;
    objpl.pl_wsgbm = stereoPipe.pl_wsgbm;
    //objpl.pl_disp2depth = stereoPipe.pl_disp2depth;
    objpipe.push_back(objpl);
    suprintk("PUSHED BACK SEMS: %lu\n",objpl.obj->wsgbm_cb_sem);


}






void Stereo::RunSIPP(frameBuffer &fLeft, frameBuffer &fRight, frameBuffer &fDisparity, frameBuffer &fDepth)
{

    UNUSED(fLeft);
    UNUSED(fRight);
    UNUSED(fDisparity);
    UNUSED(fDepth);

    assert(sipp_initialised);
#ifdef DEBUG

    //saveMemoryToFile((uint32_t)fRight.p1, fRight.spec.width*fRight.spec.height, "RIGHT_AT SIPP_IN.raw");
    //saveMemoryToFile((uint32_t)fLeft.p1, fLeft.spec.width*fLeft.spec.height, "LEFT_AT SIPP_IN.raw");
#endif
    //2) Process
    rtems_status_code sc;
    frprintk("Waiting for sipp prot sem\n");
    sc = rtems_semaphore_obtain(res_.sipp_prot_sem,RTEMS_WAIT,0);
    assert(sc == RTEMS_SUCCESSFUL);

    if (cfg_.wsgbmEn)
    {
        WarpParam *   warpCfg    = (WarpParam *)stereoPipe.warp->params;
        warpCfg->pContext[left_id]->inBufBase = (uint32_t)fLeft.p1;
        warpCfg->pContext[right_id]->inBufBase = (uint32_t)fRight.p1;

        DmaParam *    dmaDispCfg = (DmaParam *)stereoPipe.dmaOutDisp->params;
        dmaDispCfg->ddrAddr = (UInt32)fDisparity.p1;

        StereoParam * stereoCfg = (StereoParam *)stereoPipe.stereo->params;
        stereoCfg->ratio_threshold = cfg_.confidenceThr;

#ifdef DEBUG_USE_STATIC_BUFFERS
#error 13123
warpCfg->pContext[0]->inBufBase = (uint32_t)StatLeft;
warpCfg->pContext[1]->inBufBase = (uint32_t)StatRight;
dmaDispCfg->ddrAddr = (UInt32)StatDisp;
#endif


    }

    if (cfg_.disp2depthEn)
    {
        DmaParam *    dmaDepthCfg = (DmaParam *)stereoPipe.dmaOutDepth->params;
        dmaDepthCfg->ddrAddr = (UInt32)fDepth.p1;

#ifdef DEBUG_USE_STATIC_BUFFERS
dmaDepthCfg->ddrAddr = (UInt32)StatDepth;
dmaDispInCfg->ddrAddr = (UInt32)StatDisp;
#endif
    }



#ifdef PRINT_TIME
    int64_t start, stop;
    start = getTimeStamps();
#endif

    if (cfg_.wsgbmEn)
    {
        frprintk("wsgbm sippProcessFrameNB\n");
        SET_REG_WORD(CV_STEREO_IBUF0_ICTX_ADR , 0x80000000);
        sippProcessFrameNB(stereoPipe.pl_wsgbm);
        frprintk("Waiting for wsgbm callback\n");

        sc = rtems_semaphore_obtain(wsgbm_cb_sem,RTEMS_WAIT,1000);
        assert(sc == RTEMS_SUCCESSFUL);
    }

#ifdef PRINT_TIME
    stop = getTimeStamps();
    printf("wsgbm: %f \n" , (float)(stop-start)/1000);
#endif

    frprintk("Callbacks recieved\n");

    sc = rtems_semaphore_release(res_.sipp_prot_sem);
    assert(sc == RTEMS_SUCCESSFUL);

#ifdef BUILD_WARP_OUT
    saveMemoryToFile((uint32_t)oBufWarp, 2*fLeft.spec.width*fLeft.spec.height, "WARP_OUT.raw");
#endif

}

void Stereo::buildStereoPipeSubpixel (StereoPipe * pPl)
{


    if (cfg_.wsgbmEn)
    {
        pPl->pl_wsgbm       = sippCreateBasicPipeline();
        eSIPP_STATUS status = sippAddPipeResource (pPl->pl_wsgbm,
                                         res_.wsgbm_cmx_start,
                                         res_.wsgbm_cmx_end - res_.wsgbm_cmx_start + 1,
                                         res_.wsgbm_cmx_start,
                                         1,
                                         SIPP_MBIN(mbinImgSterSipp));

        printf("Pipeline status %d\n", status);
        pPl->warp        =  sippCreateFilter(pPl->pl_wsgbm, 0x00, cfg_.calib.width, cfg_.calib.height, N_PL(SIPP_CV_OPIPE_NUM_INPUTS), in_bytesize, SIPP_AUTO, (FnSvuRun)CV_WARP_0_ID, 0);
        pPl->stereo      =  sippCreateFilter(pPl->pl_wsgbm, 0x00, cfg_.calib.width, cfg_.calib.height, N_PL(1), numDisp, SIPP_AUTO, (FnSvuRun)CV_STEREO_ID, 0);
        pPl->subpixel    =  sippCreateFilter(pPl->pl_wsgbm, 0x00, cfg_.calib.width, cfg_.calib.height, N_PL(1), SZ(UInt16), SZ(SubpixelFilterParam), SVU_SYM(svuSubpixelFilter), 0);
        pPl->dmaOutDisp  =  sippCreateFilter(pPl->pl_wsgbm, 0x00, cfg_.calib.width, cfg_.calib.height, N_PL(1), SZ(UInt16), SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID,  0);
        sippFilterAddOBuf (pPl->stereo, 1, sizeof(UInt8));
        
        // Link filters
        sippLinkFilter (pPl->stereo,  pPl->warp, ker_y, ker_x);
        sippLinkFilter (pPl->subpixel, pPl->stereo, 1, 1);
        sippLinkFilterSetOBuf (pPl->subpixel, pPl->stereo, 1);
        sippLinkFilter (pPl->subpixel, pPl->stereo, 1, 1);
        sippLinkFilter (pPl->dmaOutDisp, pPl->subpixel, 1, 1);

    }

    if (cfg_.disp2depthEn)
    {
        pPl->disp2depth  =  sippCreateFilter(pPl->pl_wsgbm, 0x00, cfg_.calib.width, cfg_.calib.height, N_PL(1), SZ(half), SZ(Disp2depthParam), SVU_SYM(svudisp2depth), 0);
        pPl->dmaOutDepth    =  sippCreateFilter(pPl->pl_wsgbm, 0x00, cfg_.calib.width, cfg_.calib.height, N_PL(1), SZ(half), SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID,  0);
        
        sippLinkFilter (pPl->disp2depth, pPl->subpixel, 1, 1);
        sippLinkFilter (pPl->dmaOutDepth, pPl->disp2depth, 1, 1);
    }
}


void Stereo::buildStereoPipe (StereoPipe * pPl)
{


    if (cfg_.wsgbmEn)
    {
        pPl->pl_wsgbm       = sippCreateBasicPipeline();
        eSIPP_STATUS status = sippAddPipeResource (pPl->pl_wsgbm,
                                         res_.wsgbm_cmx_start,
                                         res_.wsgbm_cmx_end - res_.wsgbm_cmx_start + 1,
                                         res_.wsgbm_cmx_start,
                                         1,
                                         SIPP_MBIN(mbinImgSterSipp));

        printf("Pipeline status %d\n", status);
        pPl->warp        =  sippCreateFilter(pPl->pl_wsgbm, 0x00, cfg_.calib.width, cfg_.calib.height, N_PL(SIPP_CV_OPIPE_NUM_INPUTS), in_bytesize, SIPP_AUTO, (FnSvuRun)CV_WARP_0_ID, 0);
        pPl->stereo      =  sippCreateFilter(pPl->pl_wsgbm, 0x00, cfg_.calib.width, cfg_.calib.height, N_PL(1), SZ(UInt8), SIPP_AUTO, (FnSvuRun)CV_STEREO_ID, 0);
        pPl->dmaOutDisp     =  sippCreateFilter(pPl->pl_wsgbm, 0x00, cfg_.calib.width, cfg_.calib.height, N_PL(1), SZ(UInt8), SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID,  0);

        sippFilterAddOBuf (pPl->stereo, 1, sizeof(UInt8));
        // Link filters
        sippLinkFilter (pPl->stereo,  pPl->warp,   ker_y, ker_x);
        sippLinkFilter (pPl->dmaOutDisp, pPl->stereo, 1, 1);
        sippLinkFilterSetOBuf (pPl->dmaOutDisp, pPl->stereo, 1);
    }

    if (cfg_.disp2depthEn)
    {
        pPl->disp2depth  =  sippCreateFilter(pPl->pl_wsgbm, 0x00, cfg_.calib.width, cfg_.calib.height, N_PL(1), SZ(half), SZ(Disp2depthParam), SVU_SYM(svudisp2depth), 0);
        pPl->dmaOutDepth    =  sippCreateFilter(pPl->pl_wsgbm, 0x00, cfg_.calib.width, cfg_.calib.height, N_PL(1), SZ(half), SIPP_AUTO, (FnSvuRun)SIPP_DMA_ID,  0);
        
        sippLinkFilter (pPl->disp2depth, pPl->stereo, 1, 1);
        sippLinkFilter (pPl->dmaOutDepth, pPl->disp2depth, 1, 1);
        sippLinkFilterSetOBuf (pPl->disp2depth, pPl->stereo, 1);
    }
    else
    {
        /// If there is no SW filter in the pipeline = run in opipe RT
        pPl->pl_wsgbm->flags |= PLF_CONSIDER_OPIPE_RT;
    }

    if(cfg_.calib.width > 1000)
    {
        sippPipeSetLinesPerIter (pPl->pl_wsgbm, 8);
    }
    else
    {
        sippPipeSetLinesPerIter (pPl->pl_wsgbm, 16);
    }
}
void Stereo::SetMaskSize(OsDrvStereoCTKernelSize ct_ker)
{
    switch (ct_ker) {
        case OS_DRV_STEREO_CT_5x5:
            ker_x = 5;
            ker_y = 5;
            break;
        case OS_DRV_STEREO_CT_7x7:
            ker_x = 7;
            ker_y = 7;
            break;
        case OS_DRV_STEREO_CT_7x9:
            ker_x = 7;
            ker_y = 9;
            break;
        default:
        assert(0);

    }
}

uint64_t Stereo::ConfigMask(mask_type maskType)
{
    uint64_t mask = 0;

    if (maskType == MASK_RASTER)
    {
        if (ker_x == 5 && ker_y == 5)
            return 0XA82415;
        if (ker_x == 7 && ker_y == 7)
            return 0XAA02A8154055;
        if (ker_x == 7 && ker_y == 9)
            return 0X2AA00AA805540155;
    } 
    else if (maskType == MASK_ROW)
    {
        if (ker_x == 5 && ker_y == 5)
            return 0XF83C1F;
        if (ker_x == 7 && ker_y == 7)
            return 0XFE03F81FC07F;
        if (ker_x == 7 && ker_y == 9)
            return 0X3FE00FF807FC01FF;
    }
    else if (maskType == MASK_COL)
    {
        if (ker_x == 5 && ker_y == 5)
            return 0XAD66B5;
        if (ker_x == 7 && ker_y == 7)
            return 0XAB56ADB56AD5;
        if (ker_x == 7 && ker_y == 9)
            return 0X2AB55AAD2D56AB55;
    }
    return mask;
}

static void setTransformCfg( warpCtx* ctx, float* homog, bool enable){

    if (enable){
        ctx->filterMode       = 0x1;  // Bicubic
        ctx->bypassTransform  = 0x0;

        for(int n=0;n<9;n++)
        {
            ctx->transformMatrix[n] = *reinterpret_cast<UInt32*>(homog++);
            suprintk("H[%d]=%f\n", n, *homog);
        }
    }
    else {
        ctx->filterMode       = 0x2;  // Bypass
        ctx->bypassTransform  = 0x1;
    }

}

void Stereo::setConfidence( const uint8_t& new_conf_val){
    cfg_.confidenceThr = new_conf_val;
    //printf("stereosipp conf %d %d\n", cfg_.confidenceThr, new_conf_val);
}

void Stereo::setHomography( float* right_homog, bool init)
{
    UNUSED(right_homog);
    UNUSED(init);

    float* l_warp_homg = &cfg_.calib.LeftWarp.Homography[0];
    float* r_warp_homg = right_homog;

    if(cfg_.flipByWarp){
        if(init){
            FlipHomogHorz(l_warp_homg, cfg_.calib.width);
            cfg_.calib.LeftWarp.enable=true;
        }
        FlipHomogHorz(r_warp_homg, cfg_.calib.width);
        cfg_.calib.RightWarp.enable=true;

    }

    WarpParam* warpCfg = (WarpParam *)stereoPipe.warp->params;
    if(init){
        setTransformCfg( warpCfg->pContext[left_id], l_warp_homg, cfg_.calib.LeftWarp.enable);
    }
    setTransformCfg( warpCfg->pContext[right_id], r_warp_homg, cfg_.calib.RightWarp.enable);
}


void Stereo::configStereoPipe (StereoPipe * pPl)
{
    assert(
    (cfg_.calib.width != 0)||
    (cfg_.calib.height != 0)||
    (cfg_.calib.fov > 0.0)||
    (cfg_.calib.baseline > 0.0)
    ); // width, height,fov & baseline must be set on init now.


    if (cfg_.disp2depthEn)
    {
        Disp2depthParam* disp2depthCfg = (Disp2depthParam*)pPl->disp2depth->params;
        DmaParam *    dmaDepthCfg = (DmaParam *)pPl->dmaOutDepth->params;
          float fDegreesToRadians = 0.01745329251;

        dmaDepthCfg->ddrAddr = (UInt32)0x0; // NULL pointer (Is set in RunSIPP)

        float focal = (cfg_.calib.width) / (2 * tan((cfg_.calib.fov / 2) * fDegreesToRadians));
        fillDisp2DepthLut(disp2depthCfg->disp2depthLUT, focal, cfg_.calib.baseline, numDisp, cfg_.stereocore.algoCfg.subpixelEn, 5, cfg_.stereocore.algoCfg.compandEn);
        disp2depthCfg->flip_disp2depth = cfg_.flipByDisp2depth;
        disp2depthCfg->_16bitmode = cfg_.stereocore.algoCfg.subpixelEn;
    }


    if (cfg_.wsgbmEn)
    {

#ifdef BUILD_WARP_OUT
       DmaParam *    dmaWarpCfg = (DmaParam *)pPl->dmaOutWarp->params;
       dmaWarpCfg->ddrAddr = (UInt32)oBufWarp;
#endif

        DmaParam *    dmaDispCfg = (DmaParam *)pPl->dmaOutDisp->params;
        StereoParam * stereoCfg  = (StereoParam *)pPl->stereo->params;
        WarpParam *   warpCfg    = (WarpParam *)pPl->warp->params;



        dmaDispCfg->ddrAddr = (UInt32)0x0; // NULL pointer (Is set in RunSIPP)
        ////////////////////////////////////////////////////////
        // Warp cfgs


        warpCfg->pContext[0]->pfbcReqMode      = 0x2; // due to SIPP updates; related bugs 29394, 27811
        warpCfg->pContext[0]->tileXPrefLog2    = 0x0; // due to SIPP updates; related bugs 29394, 27811
        warpCfg->pContext[0]->meshType         = 0x0;  // This is a sparse mesh
        warpCfg->pContext[0]->meshRelative     = 0x0;  // Irrelevant is using sparse mesh

        warpCfg->pContext[0]->meshBypass       = 0x1;

        warpCfg->pContext[0]->inBufBase        = 0x0; // NULL pointer (Is set in PlgStereoProc::threadFunc)
        warpCfg->pContext[0]->inBufStride      = cfg_.calib.width * in_bytesize;

        warpCfg->pContext[0]->outBufBase       = 0;
        warpCfg->pContext[0]->outBufStride     = 0;


        warpCfg->pContext[0]->inputX           = cfg_.calib.width;
        warpCfg->pContext[0]->inputY           = cfg_.calib.height;
        warpCfg->pContext[0]->startX           = 0x0;
        warpCfg->pContext[0]->startY           = 0x0;
        warpCfg->pContext[0]->endX             = cfg_.calib.width - 0x1;
        warpCfg->pContext[0]->endY             = cfg_.calib.height - 0x1;

        warpCfg->pContext[0]->meshWidth        = 0;
        warpCfg->pContext[0]->meshHeight       = 0;

        warpCfg->pContext[0]->edgeMode         = 0x0;
        warpCfg->pContext[0]->edgeColour       = 0x0;

        warpCfg->pContext[0]->inPixWidth       = in_bitsize;
        warpCfg->pContext[0]->outPixWidth      = in_bitsize; // Does this need to be set, considering outBufStride is 0? i.e. warp is same output as input TBD: Check assumption

        warpCfg->pContext[0]->maxMeshYPositive = 8;
        warpCfg->pContext[0]->maxMeshYNegative = 8;
        warpCfg->pContext[0]->hFlip            = (uint32_t)cfg_.flipByWarp;

        // Move on to second plane / context (this test uses the same input for both planes!)
        warpCfg->pContext[1]->pfbcReqMode      = 0x2; // due to SIPP updates; related bugs 29394, 27811
        warpCfg->pContext[1]->tileXPrefLog2    = 0x0; // due to SIPP updates; related bugs 29394, 27811
        warpCfg->pContext[1]->meshType         = 0x0;  // This is a sparse mesh
        warpCfg->pContext[1]->meshRelative     = 0x0;  // Irrelevant is using sparse mesh

        warpCfg->pContext[1]->meshBypass       = 0x1;

        warpCfg->pContext[1]->inBufBase        = 0x0; // NULL pointer (Is set in PlgStereoProc::threadFunc)
        warpCfg->pContext[1]->inBufStride      = cfg_.calib.width * in_bytesize;

        warpCfg->pContext[1]->outBufBase       = 0;
        warpCfg->pContext[1]->outBufStride     = 0;


        warpCfg->pContext[1]->inputX           = cfg_.calib.width;
        warpCfg->pContext[1]->inputY           = cfg_.calib.height;
        warpCfg->pContext[1]->startX           = 0x0;
        warpCfg->pContext[1]->startY           = 0x0;
        warpCfg->pContext[1]->endX             = cfg_.calib.width - 0x1;
        warpCfg->pContext[1]->endY             = cfg_.calib.height - 0x1;

        warpCfg->pContext[1]->meshWidth        = 0;
        warpCfg->pContext[1]->meshHeight       = 0;

        warpCfg->pContext[1]->edgeMode         = 0x0;
        warpCfg->pContext[1]->edgeColour       = 0x0;

        warpCfg->pContext[1]->inPixWidth       = in_bitsize;
        warpCfg->pContext[1]->outPixWidth      = in_bitsize; // Does this need to be set, considering outBufStride is 0? i.e. warp is same output as input TBD: Check assumption

        warpCfg->pContext[1]->maxMeshYPositive = 8;
        warpCfg->pContext[1]->maxMeshYNegative = 8;
        warpCfg->pContext[1]->hFlip            = (uint32_t)cfg_.flipByWarp;

        if (cfg_.flipByWarp)
        {
            left_id=0;
            right_id=1;
        }
        else
        {
            left_id=1;
            right_id=0;
        }

        setHomography(cfg_.calib.RightWarp.Homography, true);
        ////////////////////////////////////////////////////////////
        // Here go through all the filter instances and assign the
        // value for all parameter fields as defined in the filter
        // property window

        stereoCfg->mode            = OS_DRV_STEREO_CT_CM_AGG;
        stereoCfg->aggLutH         = aggLut; // access to P1&P2 LUT (hor. path)
        stereoCfg->aggLutV         = aggLut; // access to P1&P2 LUT (vert. path)

        stereoCfg->in_m            = stereo_bitcfg;
        stereoCfg->out_m           = cfg_.stereocore.algoCfg.outMode;
        stereoCfg->ct_ker          = cfg_.stereocore.ctCfg.ctKernel;
        stereoCfg->ct_enThr        = 0;
        stereoCfg->ct_enMean       = 0;
        stereoCfg->ct_enMask       = cfg_.stereocore.ctCfg.maskEn;
        stereoCfg->dsp_wd          = cfg_.stereocore.algoCfg.dispRange;
        stereoCfg->ct_format       = cfg_.stereocore.ctCfg.ctFormat;
        stereoCfg->div_factor      = 3;

        stereoCfg->cme             = cfg_. stereocore.algoCfg.compandEn;
        stereoCfg->dd              = cfg_.stereocore.algoCfg.costDump;      // dump DBG_map output
        stereoCfg->invalid_disp    = 0;
        stereoCfg->cm_alfa         = 8;
        stereoCfg->cm_beta         = 12;
        stereoCfg->cm_threshold    = 0x7f;
        stereoCfg->ct_threshold    = 0;

        stereoCfg->ct_mask         = ConfigMask(cfg_.stereocore.ctCfg.maskT);

        if(cfg_.stereocore.algoCfg.subpixelEn)
        {
            stereoCfg->dd              = 2;      // dump DBG_map output
            SubpixelFilterParam * subpixelCfg = (SubpixelFilterParam *)pPl->subpixel->params;
            subpixelCfg->numDisp = numDisp;
            subpixelCfg->numFractionalBits = 5;
            subpixelCfg->LUT = subpixelLUT5bits;
        }
    }

}

void Stereo::UnRegPipeFromMap ()
{
    suprintk("CLEANUP:   UnRegPipeFromMap\n");

    auto iter = std::find_if(objpipe.begin(), objpipe.end(),  [this](objpipemap& f)
    {
        return f.obj == this;
    } );
    objpipe.erase(iter);
    suprintk("REMOVED SEMS: %lu\n",iter->obj->wsgbm_cb_sem);

}
