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
#define PTR_IN_MSG   1 //0 or 1

#if 1
  #define SZ_ETC  "12s"
  #define N        2 //number of cross-leon chains
  #define DELAY   300 //stall param [ms]
  #define N_MSG   30
  #define PRINT    1
#elif 0
  #define SZ_ETC  "1m:30s"
  #define N        16 //number of cross-leon chains
  #define DELAY     0 //stall param [ms]
  #define N_MSG   (100*1000)
  #define PRINT    0
#elif 0
  #define SZ_ETC  "6m:20s"
  #define N        8 //number of crosses
  #define DELAY    0 //stall param [ms]
  #define N_MSG   (1000*1000)
  #define PRINT    0
#endif

typedef PoPtr<ImgFrame> MyFrmPtr;

//============================================================
#if (PTR_IN_MSG)
  struct MyMsg{
   uint32_t data;
   MyFrmPtr frm;
 };

 inline bool operator==(const MyMsg& lhs, const MyMsg& rhs)
 {
   if(lhs.data == rhs.data) return true;
   else                     return false;
 }
//============================================================
#else
 typedef MyFrmPtr MyMsg;
 inline bool operator==(const MyMsg& lhs, const MyMsg& rhs)
 {
   if((lhs.ptr       == rhs.ptr  ) &&
      (lhs.ref       == rhs.ref  ) &&
      (lhs.pool      == rhs.pool ) && // ...
      (lhs.ptr->seqNo == rhs.ptr->seqNo)) return true;
   else                                 return false;
 }
#endif

#endif