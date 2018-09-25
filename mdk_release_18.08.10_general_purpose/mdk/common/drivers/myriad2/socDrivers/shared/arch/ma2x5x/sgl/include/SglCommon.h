///
/// @file SglCommon.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup SglCommon SglCommon Driver
/// @{
/// @brief     Common utilities for all Sgl layer drivers.
///


#ifndef _SGL_COMMON_H_
#define _SGL_COMMON_H_

#include "HglCommon.h"

// 1: Defines
// ----------------------------------------------------------------------------

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

/// Common return values for all SGL layer drivers
////////////////////////////////////////////////////////////////////////////////
//                                   / \                                      //
//                                  / ! \                                     //
//                                 /_____\                                    //
//                                                                            //
// Any new error code must be added after the existing ones and before        //
// SGL_RET_NB. Moreover the test in testApps/drivers/common/test_ret_code     //
// must be updated to include any new code so that the automatic verification //
// of return values' uniqueness works correctly.                              //
////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    SGL_SUCCESS = HGL_SUCCESS,
    SGL_ERR_RES_BUSY = HGL_RET_NB,
    SGL_ERR_PARAM,
    SGL_ERR_OWNER,
    SGL_ERR_UNSATISFIED,
    SGL_ERR_OTHER,
    SGL_RET_NB
} SglRet;

#endif // _SGL_COMMON_H_

///@}
