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
  #define SZ_ETC  "12s"
  #define N        2 //number of cross-leon chains
  #define N_MSG    30
  #define PRINT    1
#elif 0
  #define SZ_ETC  "3m"
  #define N         8 //number of cross-leon chains
  #define N_MSG   (100*1000)
  #define PRINT    0
#endif

typedef PoPtr<ImgFrame> MyFrmPtr;

typedef MyFrmPtr MyMsg;

#endif