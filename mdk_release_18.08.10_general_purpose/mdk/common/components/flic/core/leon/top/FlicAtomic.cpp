///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Atomic Section
///

#include "FlicAtomic.h"

void Atomic::Enter()
{
   rtems_interrupt_disable(level);
   rtems_task_mode(RTEMS_NO_PREEMPT, RTEMS_PREEMPT_MASK, &mode);
}

void Atomic::Leave()
{
   rtems_task_mode(mode, RTEMS_PREEMPT_MASK, &mode);
   rtems_interrupt_enable(level);
}