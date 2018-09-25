///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Atomic Section
///

#include <rtems.h>

#ifndef __FLIC_ATOMIC_H__
#define __FLIC_ATOMIC_H__

//Atomic section (disable IRQ & pre-emption)
class Atomic{
  private:
    rtems_interrupt_level level;
    rtems_mode            mode;
  public:
    void Enter();
    void Leave();
};

#endif