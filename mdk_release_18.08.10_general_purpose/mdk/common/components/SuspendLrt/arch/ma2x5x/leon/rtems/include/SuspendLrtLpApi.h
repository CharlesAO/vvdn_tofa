///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Suspend LRT component: API definitions
///

#ifndef LOW_POWER_API_H_
#define LOW_POWER_API_H_

// 1: Includes
// ----------------------------------------------------------------------------

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
#define LP_INTERRUPT_PRIORITY 0xE
#define LP_INTERRUPT_NUM IRQ_DYNAMIC_0

extern void SuspendLrt(void);
extern void switchStackAndEntersLp_asm(void);
extern void restoreStackAndExitLp_asm(void);

#endif // LOW_POWER_API_H_
