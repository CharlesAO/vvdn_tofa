///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test defs
///

#ifndef __TEST_CONFIG_H__
#define __TEST_CONFIG_H__

#if 1
  #define SZ_ETC  "20s"
  #define N        3 //number of cross-leon chains
  #define DELAY  100 //stall param [ms]
  #define N_MSG   50
  #define PRINT    1
#else
  #define SZ_ETC  "1m:30s"
  #define N       16 //number of cross-leon chains
  #define DELAY    0 //stall param [ms]
  #define N_MSG  (100*1000)
  #define PRINT    0
#endif

struct MyMsg{
  uint32_t data;
};

inline bool operator==(const MyMsg& lhs, const MyMsg& rhs)
{
   if(lhs.data == rhs.data) return true;
   else                     return false;
}

#endif