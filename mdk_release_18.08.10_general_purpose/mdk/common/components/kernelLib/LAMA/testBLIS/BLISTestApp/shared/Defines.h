///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     
///
#ifndef __DEFINES_H__
#define __DEFINES_H__

/* Shave allocation */
#if (defined MYRIAD1)
 #define MAX_SHAVES      8
#elif (defined MYRIAD2)
 #define MAX_SHAVES     12
#endif

/* PLEASE DON'T CHANGE THESE VALUES WITHOUT CHANGING CORRESPONDING DEPENDENTS */
/* MAX_M_ITER = (BLIS_DEFAULT_MC_S / 4) */
/* MAX_K      = BLIS_DEFAULT_KC_S       */
#if (defined MYRIAD1)
 #define MAX_M_ITER      16
 #define MAX_K          256
#elif (defined MYRIAD2)
 #define MAX_M_ITER       8
 #define MAX_K          512
#endif

#define DDR_TO_DDRCACHE(_address) ((u8*)(((u32)_address)&0xF7FFFFFF))
#define ALIGNED(_value) __attribute__((aligned(_value)))
#if (defined MYRIAD1)
 #define DDR_DATA      __attribute__((section(".ddr_direct.data"))) ALIGNED(16)
 #define DDR_BSS       __attribute__((section(".ddr_direct.bss"))) ALIGNED(16)
 #define DDR_LEON_HEAP __attribute__((section(".ddr_direct.data.leon_heap"))) ALIGNED(16)
#elif (defined MYRIAD2)
 #define DDR_DATA      __attribute__((section(".ddr.data"))) ALIGNED(16)
 #define DDR_BSS       __attribute__((section(".ddr.bss"))) ALIGNED(16)
 #define DDR_LEON_HEAP __attribute__((section(".ddr.bss"))) ALIGNED(16)
#endif


typedef struct
{
   void *a;
   void *b;
   void *c;
   void *alpha;
   void *beta;
   int  k;
   int  n_iter;
   int  m_iter;
   int  n_left;
   int  m_left;
   int  rs_c;
   int  cs_c;
   int  rstep_a;
   int  cstep_b;
} kernelInfo_t;

#endif /*__DEFINES_H__*/
