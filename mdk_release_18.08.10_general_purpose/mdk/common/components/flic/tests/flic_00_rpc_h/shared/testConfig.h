///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test defs
///

#ifndef __TEST_CONFIG_H__
#define __TEST_CONFIG_H__

#include <Pool.h>
#include <ImgFrame.h>

#define N_POOL_FRMS  4
#define FRM_SZ      32 //[bytes]

#if 1
  #define SZ_ETC  "2s"
  #define N        2 //number of cross-leon chains
  #define DELAY   300 //stall param [ms]
  #define N_MSG   30
#elif 0
  #define SZ_ETC  "1m:50s"
  #define N        8 //number of cross-leon chains
  #define DELAY    0 //stall param [ms]
  #define N_MSG   (500*1000)
#endif

#endif