///
/// @file OsDrvResMgr.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup OsDrvResMgr OsResMgr Driver
/// @{
/// @brief     ResMgr RTEMS API Functions.
///
///            The Resource Manager allocates the following resources:
///              - 1 RTEMS semaphore.
///              - 6 HW mutexes (reserved for its own use and thus not available
///                to user applications).
///


#ifndef _OS_DRV_RESMGR_H_
#define _OS_DRV_RESMGR_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdint.h>

#include <rtems.h>
#include "SglResMgrDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

#define OS_DRV_RESMGR_DEVNAME "/dev/resmgr"

// 2:  Exported Global Data
// ----------------------------------------------------------------------------

// SGL exported objects re-mapping.
#define OS_DRV_RESMGR_REQ_ID_ANY SGL_RESMGR_REQ_ID_ANY

typedef SglResMgrType OsDrvResMgrType;
#define OS_DRV_RESMGR_SHAVE SGL_RESMGR_SHAVE
#define OS_DRV_RESMGR_HW_FILTER SGL_RESMGR_HW_FILTER
#define OS_DRV_RESMGR_AHB_DMA_CHAN SGL_RESMGR_AHB_DMA_CHAN
#define OS_DRV_RESMGR_CMX_DMA_CHAN SGL_RESMGR_CMX_DMA_CHAN
#define OS_DRV_RESMGR_LCD  SGL_RESMGR_LCD
#define OS_DRV_RESMGR_MIPI_TX SGL_RESMGR_MIPI_TX
#define OS_DRV_RESMGR_I2C SGL_RESMGR_I2C
#define OS_DRV_RESMGR_I2S SGL_RESMGR_I2S
#define OS_DRV_RESMGR_SPI SGL_RESMGR_SPI
#define OS_DRV_RESMGR_SDIO SGL_RESMGR_SDIO
#define OS_DRV_RESMGR_UART SGL_RESMGR_UART
#define OS_DRV_RESMGR_GPIO SGL_RESMGR_GPIO
#define OS_DRV_RESMGR_GPIO_IRQ SGL_RESMGR_GPIO_IRQ
#define OS_DRV_RESMGR_SHAVE_L2_CACHE SGL_RESMGR_SHAVE_L2_CACHE
#define OS_DRV_RESMGR_MUTEX SGL_RESMGR_MUTEX

typedef SglResMgrClk OsDrvResMgrClk;
#define OS_DRV_RESMGR_OSC1 SGL_RESMGR_OSC1
#define OS_DRV_RESMGR_OSC2 SGL_RESMGR_OSC2
#define OS_DRV_RESMGR_PLL1 SGL_RESMGR_PLL1
#define OS_DRV_RESMGR_PLL2 SGL_RESMGR_PLL2
#define OS_DRV_RESMGR_SYSCLK SGL_RESMGR_SYSCLK

typedef SglResMgrInfoClockEntry OsDrvResMgrInfoClockEntry;

typedef SglResMgrHandler OsDrvResMgrHandler;

typedef SglResMgrRequest OsDrvResMgrRequest;

// IOCTL commands.
typedef enum
{
    // Get the whole clock entry structure.
    // Param: OsDrvResMgrInfoClockEntry *
    OS_DRV_RESMGR_GET_CLOCK_ENTRY,
    // Get a clock entry field value.
    // Param: OsDrvResMgrClockEntryFieldParam *
    OS_DRV_RESMGR_GET_CLOCK_ENTRY_FIELD,
    // Allocate a resource.
    // Param: OsDrvResMgrRequest *
    OS_DRV_RESMGR_ALLOCATE,
    // Allocate a group of resource.
    // Param: OsDrvResMgrAllocateGroupParam *
    OS_DRV_RESMGR_ALLOCATE_GROUP,
    // Release a resource.
    // Param: OsDrvResMgrHandler *
    OS_DRV_RESMGR_RELEASE,
    // Allocate a group of resource.
    // Param: OsDrvResMgrAllocateGroupParam *
    OS_DRV_RESMGR_RELEASE_GROUP,
    // Execute a function under mutex protection.
    // Param: OsDrvResMgrExecuteProtectedParam *
    OS_DRV_RESMGR_EXECUTE_PROTECTED,
    // Number of commands.
    OS_DRV_RESMGR_IOCTL_CMD_NB
} OsDrvResMgrControl;

// Parameters for OS_DRV_RESMGR_GET_CLOCK_ENTRY_FIELD and
// OS_DRV_RESMGR_SET_CLOCK_ENTRY_FIELD (see OsDrvResMgrInternal.h) commands.
typedef struct
{
    OsDrvResMgrClk clock_entry;
    uint32_t value;
} OsDrvResMgrClockEntryFieldParam;

// Parameters for OS_DRV_RESMGR_ALLOCATE_GROUP command.
typedef struct
{
    // Request structures must be allocated as one array.
    OsDrvResMgrRequest *request;
    uint8_t req_nb;
    uint8_t atomic;
} OsDrvResMgrAllocateGroupParam;

// Parameters for OS_DRV_RESMGR_RELEASE_GROUP command.
typedef struct
{
    // Handler structures must be allocated as one array.
    OsDrvResMgrHandler *handler;
    uint8_t res_nb;
    uint8_t release_nb;
} OsDrvResMgrReleaseGroupParam;

// Parameters for OS_DRV_RESMGR_EXECUTE_PROTECTED command.
typedef struct
{
    int32_t (*protected_func)(void *);
    void *arg;
    int32_t res;
    int32_t level;
} OsDrvResMgrExecuteProtectedParam;

// 3:  Exported Functions
// ----------------------------------------------------------------------------
extern rtems_driver_address_table OsDrvResMgrTbl;

#ifdef __cplusplus
}
#endif

#endif //_OS_DRV_RESMGR_H_

///@}
