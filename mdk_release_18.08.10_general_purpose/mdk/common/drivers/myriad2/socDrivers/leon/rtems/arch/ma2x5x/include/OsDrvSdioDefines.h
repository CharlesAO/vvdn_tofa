///
/// @file OsDrvSdioDefines.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup OsDrvSdio
/// @{
/// @brief     RTEMS SDIO Header File 
///

#ifndef OSDRVSDIODEFINES_H
#define OSDRVSDIODEFINES_H 

// Common for all OS drivers 
#include <rtems/diskdevs.h>
#include <OsCommon.h>
#include <DrvSdio.h>

// 1: Defines
// ----------------------------------------------------------------------------
// RTEMS has limited buffer space TBD if needed to increase it
#define OSDRVSDIO_MAX_SLOTS                             DRVSDIO_MAXDRIVERCARDS
#define OSDRVSDIO_DEFAULTBLOCKSIZE                      512

// 2: Typedefs (types, enums, structs)
// ----------------------------------------------------------------------------

// Struct that keeps the card slot that would be used and the device name
typedef struct {
    u8 slot;
    const char *devName;
    tyDrvSdioSpeedMode maxSpeed;
    DrvSdioVoltSwitchFunc volSwCallback;
} osDrvSdioEntry_t;
// Struct that keeps the number of card slots; Interrupt Priority that the would be set for the driver and a slot information for every slot
typedef struct
{
    u8 slots;
    u8 interruptPriority;
    osDrvSdioEntry_t slotInfo[OSDRVSDIO_MAX_SLOTS];
}osDrvSdioEntries_t;

typedef enum
{
    OS_DRV_SDIO_SUCCESS = OS_MYR_DRV_SUCCESS,
    OS_DRV_SDIO_ERROR = OS_MYR_DRV_ERROR,
    OS_DRV_SDIO_GET_CLOCKS_ERROR = OS_MYR_DRV_CLOCK_ERROR
}osSdioStatus_t;

typedef DrvSdio_Descriptors OsDrvSdio_Descriptors;

#endif // OSDRVSDIODEFINES_H

///@}
