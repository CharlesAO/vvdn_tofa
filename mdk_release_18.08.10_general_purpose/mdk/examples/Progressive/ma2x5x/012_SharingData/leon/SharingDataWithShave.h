///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon header
///

#ifndef _TEST_L2_CACHE_
#define _TEST_L2_CACHE_

// 1: Includes
// ----------------------------------------------------------------------------
// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define SMALL_DATA_LENGTH 30
// 3: Static Local Data
// ----------------------------------------------------------------------------

#ifdef __cplusplus
    extern "C" {
#endif

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

int LeonShareBigDataWithShave(void);
void LeonShareSmallDataWithShave(u32* expectedBufferShaveLeon, 
								u32* expectedBufferShaveToShave);
#ifdef __cplusplus
}
#endif

#endif // _TEST_L2_CACHE_
