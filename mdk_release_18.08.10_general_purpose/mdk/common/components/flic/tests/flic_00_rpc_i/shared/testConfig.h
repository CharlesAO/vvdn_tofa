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

#if 0
  #define SZ_ETC  "1s"
  #define N        3        //number of cross-leon chains
  #define DELAY   (500)    //us
  #define N_MSG   (3*1000)
#elif 1
  #define SZ_ETC  "30s"
  #define N         6       //number of cross-leon chains
  #define DELAY    500      //us
  #define N_MSG   (100*1000)
#elif 0
  #define SZ_ETC  "6m:15s"
  #define N         8       //number of cross-leon chains
  #define DELAY    500      //us
  #define N_MSG   (1000*1000)
#endif

#endif