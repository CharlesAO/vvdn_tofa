///
/// @file DrvLeon.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvLeon Leon Driver
/// @{
/// @brief     API for basic runtime control on the Leon co-processor
///


#ifndef DRV_LEON_H
#define DRV_LEON_H

// 1: Includes
// ----------------------------------------------------------------------------

#include <mv_types.h>

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// DrvLeonRtStartup
///
/// This function resets the LeonRT processor and starts it executing from the startup Address
/// @param[in] leonStartupAddress  - Address from which LeonRt should start execution
/// @return    0 on success, non-zero otherwise
/// @note      The leonStartupAddress parameter MUST be aligned on a 4KB memory boundary
void DrvLeonRTStartup(u32 leonStartupAddress);

/// DrvLeonRtStartupStart
///
/// This function resets the LeonRT processor and starts it executing from the start symbol
/// @return    0 on success, non-zero otherwise
void DrvLeonRTStartupStart(void);

/// DrvLeonSignalStop
///
/// This function signals that LeonRT has stopped. Should only be called from LeonRT.
/// @param[in] none
/// @return    none
/// @note      There is no reason in calling this from LeonOS
void DrvLeonRTSignalStop(void);

/// DrvLeonRTHasStopped
///
/// This function checks if Leon RT has stopped.
/// @param[in] none
/// @return    none
/// @note      This function, if called from LeonRT, will result in a link error. It can only be called from LeonOS
int DrvLeonRTHasStopped(void);

/// DrvLeonRTWaitExecution
///
/// This function Waits for LeonRT to have signalled it's done with relevant execution
/// @param[in] none
/// @return    none
/// @note      This function, if called from LeonRT, will result in a link error. It can only be called from LeonOS
void DrvLeonRTWaitExecution(void);

/// DrvLeonRTSignalBootCompleted
///
/// This function signals that LeonRT has booted successfully. Should only be called from LeonRT.
/// @param[in] none
/// @return    none
/// @note      There is no reason in calling this from LeonOS
void DrvLeonRTSignalBootCompleted(void);

/// DrvLeonRTHasBooted
///
/// This function checks if Leon RT has booted.
/// @param[in] none
/// @return    none
/// @note      This function, if called from LeonRT, will result in a link error. It can only be called from LeonOS
int DrvLeonRTHasBooted(void);

/// DrvLeonRTWaitForBoot
///
/// This function Waits for LeonRT to have signalled it's done with booting
/// @param[in] none
/// @return    none
/// @note      This function, if called from LeonRT, will result in a link error. It can only be called from LeonOS
void DrvLeonRTWaitForBoot(void);

#ifdef __cplusplus
}
#endif

#endif // DRV_CPR_H

///@}
