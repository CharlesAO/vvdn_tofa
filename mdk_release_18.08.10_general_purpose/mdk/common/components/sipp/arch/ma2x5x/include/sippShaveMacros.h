///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#ifndef __SIPP_SHAVE_MACROS_H__
#define __SIPP_SHAVE_MACROS_H__

#include <sipp.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static inline u8 clampU8(float in)
{
    in += 0.5; //round to neareset int
    if(in< 0)  return 0;
    if(in>255) return 255;
               return (u8)in;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static inline half clamp01(half x)
{
    if     (x < 0.0f) x = 0.0f;
    else if(x > 1.0f) x = 1.0f;
    return x;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static inline half clamp01ForP(half x)
{
   if  (x < 0.0f) {
      x = 0.0f;
   } else {
      if(x >= 1.0f) {
         x = 0.9995117f; // first value below 1 in fp16
      }
   }
   return x;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static inline u8 clampZ255(half x)
{
    if(x <   0.0f) x =   0.0f;
    if(x > 255.0f) x = 255.0f;

    return(u8)x;
}

#endif // !__SIPP_SHAVE_MACROS_H__
