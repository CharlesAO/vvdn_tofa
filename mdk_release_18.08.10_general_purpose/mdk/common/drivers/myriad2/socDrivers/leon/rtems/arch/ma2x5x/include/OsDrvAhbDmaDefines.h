///
/// @file OsDrvAhbDmaDefines.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvAhbDma
/// @{
/// @brief     Definitions and types needed by the I2C Slave API RTEMS
///


#ifndef _AHB_DMA_DEFINES_H
#define _AHB_DMA_DEFINES_H

// System Includes
// ----------------------------------------------------------------------------
#include <rtems.h>
#include <OsCommon.h>
#include <DrvAhbDma.h>

// Application Includes
// ----------------------------------------------------------------------------

// Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define AHB_DMA_DEFAULTSEMAPHORETIMEOUT                  100

//events to wait for
#define AHB_DMA_EVENT(i)                     (0x1   << (i))
#define AHB_DMA_ERROR(i)                     (0x100 << (i))
#define AHB_DMA_CHANNEL(handlerPtr)          AHB_DMA_EVENT((handlerPtr)->channel)
#define AHB_DMA_ALL_CHANNEL_EVENTS           (0xFF)
#define AHB_DMA_ALL_CHANNEL_ERRORS           (0xFF00)

#define AHB_DMA_MAX_DISABLE_COUNT            (2)
#define AHB_DMA_TIMEOUT_US                   (100)

#define OS_DRV_AHB_WAIT_FOREVER              (0xFFFFFFFF)

#define DRV_AHB_DMA_START_WAIT    0
#define DRV_AHB_DMA_START_NO_WAIT 1

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------
typedef enum
{
    OS_MYR_DRV_AHB_DMA_TIMEOUT = MYR_DRV_CUSTOM_START_CODE
} OS_MYR_DRV_AHB_DMA_STATUS_CODE;

typedef enum {
    OS_DRV_AHB_DMA_FREE = 0,
    OS_DRV_AHB_DMA_TAKEN,
    OS_DRV_AHB_DMA_RUNNING,
    OS_DRV_AHB_DMA_ERROR
}osAhbDmaTransferStatus_t;

typedef ahbDmaHandler_t osDrvAhbDmaHandler_t;

#endif

///@}
