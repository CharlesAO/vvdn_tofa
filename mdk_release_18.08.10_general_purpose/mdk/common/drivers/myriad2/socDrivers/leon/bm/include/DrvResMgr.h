///
/// @file DrvResMgr.h
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvResMgr ResMgr Driver
/// @{
/// @brief     Resource Manager bare metal Leon driver API
///
///            The Resource Manager allocates the following resources:
///              - 6 HW mutexes (reserved for its own use and thus not available
///                to user applications).
///


#ifndef _RESMGR_BM_H_
#define _RESMGR_BM_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdint.h>

#include "SglResMgrDefines.h"

#ifdef  __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// SGL exported objects re-mapping.
#define DRV_RESMGR_REQ_ID_ANY SGL_RESMGR_REQ_ID_ANY

typedef SglResMgrType DrvResMgrType;
#define DRV_RESMGR_SHAVE SGL_RESMGR_SHAVE
#define DRV_RESMGR_HW_FILTER SGL_RESMGR_HW_FILTER
#define DRV_RESMGR_AHB_DMA_CHAN SGL_RESMGR_AHB_DMA_CHAN
#define DRV_RESMGR_CMX_DMA_CHAN SGL_RESMGR_CMX_DMA_CHAN
#define DRV_RESMGR_LCD  SGL_RESMGR_LCD
#define DRV_RESMGR_MIPI_TX SGL_RESMGR_MIPI_TX
#define DRV_RESMGR_I2C SGL_RESMGR_I2C
#define DRV_RESMGR_I2S SGL_RESMGR_I2S
#define DRV_RESMGR_SPI SGL_RESMGR_SPI
#define DRV_RESMGR_SDIO SGL_RESMGR_SDIO
#define DRV_RESMGR_UART SGL_RESMGR_UART
#define DRV_RESMGR_GPIO SGL_RESMGR_GPIO
#define DRV_RESMGR_GPIO_IRQ SGL_RESMGR_GPIO_IRQ
#define DRV_RESMGR_SHAVE_L2_CACHE SGL_RESMGR_SHAVE_L2_CACHE
#define DRV_RESMGR_MUTEX SGL_RESMGR_MUTEX

typedef SglResMgrClk DrvResMgrClk;
#define DRV_RESMGR_OSC1 SGL_RESMGR_OSC1
#define DRV_RESMGR_OSC2 SGL_RESMGR_OSC2
#define DRV_RESMGR_PLL1 SGL_RESMGR_PLL1
#define DRV_RESMGR_PLL2 SGL_RESMGR_PLL2
#define DRV_RESMGR_SYSCLK SGL_RESMGR_SYSCLK

typedef SglResMgrInfoClockEntry DrvResMgrInfoClockEntry;

typedef SglResMgrHandler DrvResMgrHandler;

typedef SglResMgrRequest DrvResMgrRequest;

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// @brief Allows to retrieve the clock configuration.
///
/// Get the clocks configuration information to the outside world.
///
/// @param[out] infoclk - structure containing clocks configuration.
///
/// @return
///     - SGL_SUCCESS - Clock entry returned successfully.
///     - SGL_ERR_PARAM - infoclk parameter is not specified correctly.
///
int32_t DrvResMgrGetClockEntry(DrvResMgrInfoClockEntry *infoclk);

/// @brief Allows to retrieve a specific clock entry.
///
/// Get the clock configuration information indicated by the user.
///
/// @param[in] clock_entry - type of clock entry that the user wants to get.
///     Can be one of:
///     - SGL_RESMGR_OSC1 - First oscillator
///     - SGL_RESMGR_OSC2 - Secondary oscillator
///     - SGL_RESMGR_PLL1 - PLL1 entry
///     - SGL_RESMGR_PLL2 - PLL2 entry
///     - SGL_RESMGR_SYSCLK - System clock entry
/// @param[out] value - value of the clock entry.
///
/// @return
///     - SGL_SUCCESS - Clock entry returned successfully.
///     - SGL_ERR_PARAM - clock_entry parameter is not specified correctly.
///
int32_t DrvResMgrGetClockEntryField(DrvResMgrClk clock_entry, uint32_t *value);

/// @brief Allocate a resource to a requester.
///
/// Request a resource. req_status field in the request structure will inform
/// the caller whether the allocation has been granted or not. The possible
/// values are:
///     - SGL_SUCCESS - resource allocated successfully.
///     - SGL_ERR_PARAM - request parameter is not specified correctly or
///       there is a parameter corruption.
///     - SGL_ERR_BUSY - the requested resource is not available.
///     - SGL_ERR_OWN - the requested resource is already owned by the user.
///
/// On success it will be possible to check which ID has been assigned inside
/// the handler.
///
/// @param[in/out] request - allocation request information.
///
/// @return same value than req_status field.
///
int32_t DrvResMgrAllocate(DrvResMgrRequest *request);

/// @brief Allocate a group of resource to the requester
///
/// Resources can be of different types and ID can be specified or not.
/// Allocation can be atomic or not:
///   - if atomic, either all resources are granted or none of them is. ResMgr
///     mutex is held during all the allocation process.
///   - if not atomic, each requested will be individually granted or not. This
///     can be checked using the req_status fields' values in request array.
///     ResMgr mutex is released and re-acquired between each resource
///     allocation.
///
/// @param[in/out] request - array allocation request information.
/// @param[in] req_nb - the number of resource to request allocation for.
/// @param[in] atomic - 1 to make the allocation process atomic, 0 otherwise.
///
/// @return
///     - MYR_DRV_SUCCESS - resource allocated successfully. If atomic is not
///       set, success or failure of allocations must be checked individually
///       using the req_status fields' values in request array.
///     - SGL_ERR_PARAM/MYR_DRV_INVALID_PARAM - the input parameter is not
///       specified correctly or there is a parameter corruption.
///     - SGL_ERR_BUSY - the requested resource is not available. Can only
///       be returned in atomic case.
///     - SGL_ERR_OWN - the requested resource is already owned by the user.
///       Can only be returned in atomic case.
///
int32_t DrvResMgrAllocateGroup(DrvResMgrRequest request[], uint8_t req_nb,
                               uint8_t atomic);

/// @brief Allows to release a previously allocated resource.
///
/// Release the allocated resource by the owner that acquired it.
///
/// @param[in] handler - the resource handler that has to be released.
///
/// @return
///     - SGL_SUCCESS - resource released successfully.
///     - SGL_ERR_PARAM - the input parameter is not specified correctly or
///       there is a parameter corruption.
///     - SGL_ERR_OWN - the owner that requested the release of the resource
///       is not the actual owner of it.
///
int32_t DrvResMgrRelease(DrvResMgrHandler *handler);

/// @brief Allows to release a group of previously allocated resource.
///
/// Release the allocated resources by the owner that acquired it. If one of the
/// resource fails to be released, this function will return immediately with
/// the return value set to the reason of failure. release_nb will then indicate
/// which is the resource that failed.
///
/// @param[in] handler - array of the resources handlers to be released.
/// @param[in] res_nb - the number of resources to release.
/// @param[out] release_nb - the number of resources actually released.
///
/// @return
///     - MYR_DRV_SUCCESS - resource released successfully.
///     - SGL_ERR_PARAM/MYR_DRV_INVALID_PARAM - the input parameter is not
///       specified correctly or there is a parameter corruption.
///     - SGL_ERR_OWN - the owner that requested the release of the resource
///       is not the actual owner of it.
///
int32_t DrvResMgrReleaseGroup(DrvResMgrHandler handler[], uint8_t res_nb,
                              uint8_t *release_nb);

/// @brief Protected execution of a given function.
///
/// Protect the function containing critical code section.
///
/// @param[in] level - level of protection.
/// @param[in] protected_func(void *) - the function that will get called in a
///            protected manner.
/// @param[in] arg - the argument the protected_func will be called with.
/// @param[out] res - return value of the protected function call.
///
/// @return
///     - MYR_DRV_SUCCESS - protected function was successfully executed.
///     - MYR_DRV_ERR_PARAM - the input parameter is not specified correctly or
///       there is a parameter corruption.
///
int32_t DrvResMgrExecuteProtected(uint8_t level, int32_t protected_func(void *),
                                  void *arg, int32_t *res);

/// @brief Initialize ResMgr driver.
///
void DrvResMgrInit(void);

#ifdef __cplusplus
}
#endif

///@}

#endif /* _RESMGR_BM_H_ */

///@}
