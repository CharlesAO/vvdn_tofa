///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Header file to be included for applications to be usable with
///         USB loader app.
///
#ifndef USB_FLASHER_H_
#define USB_FLASHER_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include <rtems.h>

// 2: Defines
// ----------------------------------------------------------------------------

// 3:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 4:  Functions (non-inline)
// ----------------------------------------------------------------------------
rtems_status_code set_flash_update_flag();

#endif // USB_FLASHER_H_
