///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup LeonMath Leon Math Utilities
/// @{
/// @brief     API for some required Leon Math functions
///
/// Used to implement math functions
///

#ifndef __SWCLEONMATH_H__
#define __SWCLEONMATH_H__

#include <mv_types.h>

#ifdef __cplusplus
extern "C" {
#endif

/// @brief Utility trigonometric function to calculate the sine of an angle
/// @param[in] angle - angle on which to calculate sine
/// @return    sin(angle)
float swcMathSinf(float angle);

/// @briefUtility trigonometric function to calculate the cosine of an angle
/// 
/// @param[in] angle - angle on which to calculate cosine
/// @return    cosin(angle)
float swcMathCosf(float angle);

/// @brief Utility Integer function to raise base^exp
/// @param[in] base - 32 bit value on which to operate
/// @param[in] exp  - 32 bit exponent
/// @return    base^exp
u32 swcIPow(u32 base,u32 exp);


/// @brief Utility function to cast a 64 bit int to a double
/// 
/// This function is used to avoid the need to link in glibc for memory frugality reasons
/// @param[in] longVal - 64 bit integer to be cast
/// @return    floating point double equivalent of longVal
double swcLongLongToDouble(unsigned long long longVal);
/// @}
#ifdef __cplusplus
}
#endif

#endif //__SWCLEONMATH_H__
