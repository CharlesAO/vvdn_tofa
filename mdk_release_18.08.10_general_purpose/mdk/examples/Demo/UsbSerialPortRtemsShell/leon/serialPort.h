
///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Video demo application context.
///

#ifndef _SERIALPORT_H_
#define _SERIALPORT_H_
// 1: Includes
// ----------------------------------------------------------------------------
#include "usbpump_usbseri.h"
#include "rtems.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define MAX_RX_SIZE 512
#define MAX_TX_SIZE 512
#define SERIALPORT_INSTANCE_NO 1
#define SERIALPORT_EVENT RTEMS_EVENT_5

typedef struct {
    rtems_id taskId;
} RtemsEvent_t;

// VideoDemo Application Context
typedef struct
{
    UPLATFORM *pPlatform;
    void* portHandle;
    uint32_t portInstanceNo;
} SerialPortContext;

SerialPortContext *SerialPortInit(UPLATFORM *pPlatform);

#endif
