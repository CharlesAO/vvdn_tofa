///
/// @file      sippTestCommon_ma2x5x.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#ifndef _SIPP_TEST_COMMON_MA2x5x_H_
#define _SIPP_TEST_COMMON_MA2x5x_H_

#ifdef __cplusplus
extern "C" {
#endif

extern UInt8   mbinImgSipp[];
extern void sippPlatformInit(void);
extern void sippPlatformInitAsync(void);

#ifdef __cplusplus
}
#endif

#endif // _SIPP_TEST_COMMON_MA2x5x_H_
