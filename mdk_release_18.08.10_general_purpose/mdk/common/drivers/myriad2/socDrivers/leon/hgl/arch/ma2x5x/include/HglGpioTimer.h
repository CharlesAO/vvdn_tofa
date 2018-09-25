///
/// @file HglGpioTimer.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup HglGpioTimer HglGpioTimer Driver
/// @{
/// @brief     Header of HGL for GPIO Timer
///

#ifndef COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_HGL_INCLUDE_HGL_GPIO_TIMER_H_
#define COMMON_DRIVERS_MYRIAD2_SOCDRIVERS_LEON_HGL_INCLUDE_HGL_GPIO_TIMER_H_

#include "mv_types.h"
#include "HglGpioTimerDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Function to set the edge behaviour of a GPIO timestamp.
///
/// This function will set which edge to measure the timestamp on for a
/// configured GPIO IRQ source.
/// Timestamp N records the timestamp for state changes in GPIO IRQ source N.
/// @param[in] timestamp_num - Which timestamp to set the edge configuration of.
/// @param[in] trigger - Trigger type to record the timestamp on.
/// @return    < 0 if it fails, 0 on success

s32 HglGpioTimerSetConfig(const u32 timestamp_num,
                          const HglGpioTriggerType trigger);

/// Function to get a GPIO timestamp.
///
/// This function will return the timestamp of the last configured edge seen on
/// a configured GPIO IRQ source.
/// Timestamp N records the timestamp for state changes in GPIO IRQ source N.
/// @param[in] timestamp_num - Which timestamp to get.
/// @param[in] timestamp    - The timestamp of the most recent trigger event.
/// @return    < 0 if it fails, 0 on success

s32 HglGpioTimerGet(const u32 timestamp_num, u64 *const timestamp);

#ifdef __cplusplus
}
#endif

#endif

///@}
