///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Suspend LRT component: API definitions
///

#ifndef _OS_DRV_PWRMANAGER_H_
#define _OS_DRV_PWRMANAGER_H_

// 1: Includes
// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data
// ----------------------------------------------------------------------------

// 3:  Exported Functions
// ----------------------------------------------------------------------------

extern rtems_status_code SuspendLrtInit(void);

#ifdef __cplusplus
}
#endif

#endif //_OS_DRV_PWRMANAGER_H_
