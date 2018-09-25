///
/// @file HglCommon.h
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup HglCommon HglCommon Driver
/// @{
/// @brief     Header of common HGL definitions and functions prototypes/inlines
///


#ifndef HGL_COMMON_H__
#define HGL_COMMON_H__

#include <mv_types.h>

/// Implement a delay function without dependencies on the TIMER block
///@param [in]
///@remarks this is not cycle accurate
///         TODO rewrite this function to be cycle accurate
static inline void HglDelay(u32 ticks)
{
    volatile u32 i;     // Using volatile rather than NOPS as it doesn't impact compiler optimisation

    for (i = 0; i < (ticks >> 2); i ++);
}

/// Common return values for all HGL layer drivers
////////////////////////////////////////////////////////////////////////////////
//                                   / \                                      //
//                                  / ! \                                     //
//                                 /_____\                                    //
//                                                                            //
// Any new error code must be added after the existing ones and before        //
// HGL_RET_NB. Moreover the test in testApps/drivers/common/test_ret_code     //
// must be updated to include any new code so that the automatic verification //
// of return values' uniqueness works correctly.                              //
////////////////////////////////////////////////////////////////////////////////

typedef enum
{
    HGL_SUCCESS = 0,
    HGL_ERROR,
    HGL_INVALID_PARAM,
    HGL_RET_NB,
} HGL_RETURN_TYPE;

#endif

///@}
