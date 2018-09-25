///
/// @file OsDrvUsbPhy.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup OsDrvUsbPhy OsUsbPhy Driver
/// @{
/// @brief     USB PHY API function declarations
///


#ifndef _OSDRVUSBPHY_H_    /* prevent multiple includes */
#define _OSDRVUSBPHY_H_


// 1: Includes
// ----------------------------------------------------------------------------
#include "OsDrvUsbPhyDefines.h"

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
# endif

/// OsDrvUsbPhyInit
///
/// @brief This function initialize USB PHY module
///
/// @param[in]  pointer to a usbParam_t structure, containing the following parameters:
///             - fEnableOtgBlock - set to nonzero if OTG block is enabled, zero otherwise
///             - fUseExternalClock - set to nonzero if an external clock is used, zero otherwise
///             - fsel - reference clock speed
///             - refclk_sel0 - select the oscillator clock source
/// @return RTEMS_SUCCESSFUL - the PHY was successfully initialized
///         RTEMS_NOT_CONFIGURED - failed to retrieve USB clock information
///
/// @return no return value
int OsDrvUsbPhyInit(osDrvUsbPhyParam_t *initParam);

#ifdef __cplusplus
};
# endif

#endif // _OSDRVUSBPHY_H_

///@}
