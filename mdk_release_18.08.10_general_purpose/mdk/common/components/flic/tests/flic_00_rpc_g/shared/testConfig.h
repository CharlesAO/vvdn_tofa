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

#define N_POOL_FRMS  3
#define FRM_SZ      32 //[bytes]

#if 1
  #define SZ_ETC  "1s"
  #define N        1 //number of cross-leon chains
  #define N_MSG   30
#elif 0
  #define SZ_ETC  "5m:45s"
  #define N        6 //number of cross-leon chains
  #define N_MSG   (500*1000)
#endif

#endif