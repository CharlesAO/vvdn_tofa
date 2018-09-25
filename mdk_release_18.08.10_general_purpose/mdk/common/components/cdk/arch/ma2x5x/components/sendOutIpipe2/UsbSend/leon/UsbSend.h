///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup
/// @{
/// @brief
///
///
///


// 1: Includes
// ----------------------------------------------------------------------------
#ifndef USBSEND_H
#define USBSEND_H

#include "mv_types.h"
#include "IcTypes.h"
#include "sendOutApi.h"

// if defined it prints info about frames transmission
#define DBG_ERROR_PRINTF(...)            printf(__VA_ARGS__)

#ifdef DBG_USBSEND
    #define DBG_PRINTF(...)            printf(__VA_ARGS__)
#else
    #define DBG_PRINTF(...)
#endif

typedef struct
{
    u8            busy;
    u8            filled;
    u8            sendInProgress;
    u8            notSent;
    SendOutElement_t *task;
} sUsbTest;

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

void UsbSendCreate(UsbCfg_t *cfg);
static inline void UsbSendInit(void) {/*printf("\n\n UsbSendInit \n\n\n");*/};
void UsbSendFrame(SendOutElement_t *task);
static inline void UsbSendFini(void) {};

#define TRANSPORT_create        UsbSendCreate
#define TRANSPORT_init          UsbSendInit
#define TRANSPORT_SendFrame     UsbSendFrame
#define TRANSPORT_Fini          UsbSendFini

#define TRANSPORT_PARAM         usbCfg

#endif // USBSEND_H
