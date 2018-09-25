///
/// @file DrvGpioDefines.h
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvGpio
/// @{
/// @brief     Definitions and types needed by GPIO Driver
///

#ifndef DRV_GPIO_DEF_H
#define DRV_GPIO_DEF_H

#include "mv_types.h"
#include "registersMyriad.h"


#if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
 #include "DrvGpioDefinesMa2x5x.h"
#endif

#include "HglGpioTimerDefines.h"

//common define used by HGL and driver
#define GPIO_MAX_IRQ_SRCS (HGL_GPIO_MAX_IRQ_SRCS)

//common type used by HGL and driver
typedef HglGpioTriggerType DrvGpioTriggerType;

#define DRV_GPIO_TRIGGER_NONE           HGL_GPIO_TRIGGER_NONE
#define DRV_GPIO_TRIGGER_EDGE_POSITIVE  HGL_GPIO_TRIGGER_EDGE_POSITIVE
#define DRV_GPIO_TRIGGER_EDGE_NEGATIVE  HGL_GPIO_TRIGGER_EDGE_NEGATIVE
#define DRV_GPIO_TRIGGER_EDGE_BOTH      HGL_GPIO_TRIGGER_EDGE_BOTH

#endif //DRV_GPIO_DEF_H

///@}
