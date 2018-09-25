///
/// @file SglResMgr.h
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup SglResMgr SglResMgr Driver
/// @{
/// @brief     Resource Manager API
///


#ifndef _SGL_RESMGR_LL_H_
#define _SGL_RESMGR_LL_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdint.h>

#include "SglResMgrDefines.h"

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

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
/// @param[in] owner - the processor that calls the function.
///
/// @return same value than req_status field.
///
int32_t SglResMgrAllocate(SglResMgrRequest *request, uint32_t owner);

/// @brief Allows to release a previously allocated resource.
///
/// Release the allocated resource by the owner that acquired it.
///
/// @param[in] handler - the resource handler that has to be released.
/// @param[in] owner - the processor that calls the function.
///
/// @return
///     - SGL_SUCCESS - resource allocated successfully.
///     - SGL_ERR_PARAM - the input parameter is not specified correctly or
///       there is a parameter corruption.
///     - SGL_ERR_OWN - the owner that requested the release of the resource
///       is not the actual owner of it.
///
int32_t SglResMgrRelease(SglResMgrHandler *handler, uint32_t owner);

/// @brief Allows to write clock configuration.
///
/// Set the clocks configuration information for it to be available for
/// everybody.
///
/// @param[in] infoclk - structure containing clocks configuration.
///
/// @return
///     - SGL_SUCCESS - Clock entry set successfully.
///     - SGL_ERR_PARAM - infoclk parameter is not specified correctly.
///
int32_t SglResMgrSetClockEntry(SglResMgrInfoClockEntry *infoclk);

/// @brief Allows to set specific clock entry.
///
/// Set the clock configuration information indicated by the user.
///
/// @param[in] clock_entry - type of clock entry that is wanted to be set.
///     Can be one of:
///     - SGL_RESMGR_OSC1 - First oscillator
///     - SGL_RESMGR_OSC2 - Secondary oscillator
///     - SGL_RESMGR_PLL1 - PLL1 entry
///     - SGL_RESMGR_PLL2 - PLL2 entry
///     - SGL_RESMGR_SYSCLK - System clock entry
/// @param[in] value - value of the clock entry.
///
/// @return
///     - SGL_SUCCESS - Clock entry set successfully.
///     - SGL_ERR_PARAM - clock_entry parameter is not specified correctly.
///
int32_t SglResMgrSetClockEntryField(SglResMgrClk clock_entry, uint32_t value);

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
int32_t SglResMgrGetClockEntry(SglResMgrInfoClockEntry *infoclk);

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
int32_t SglResMgrGetClockEntryField(SglResMgrClk clock_entry, uint32_t *value);

/// @brief Allows to get the information for a given SHAVE L2 cache partition.
///
/// @param[out] part_entry - structure containing partition information.
/// @param[in] id - partition number.
///
/// @return
///     - SGL_SUCCESS - partition information returned successfully.
///     - SGL_ERR_PARAM - parameters are not specified correctly.
///
int32_t SglResMgrGetShaveL2cPart(SglResMgrShaveL2cPartEntry *part_entry,
                                 uint8_t id);

/// @brief Allows to get the information for all the SHAVE L2 cache partitions
///        at the same time.
///
/// @param[out] part_entry - Array of structures containing each partition's
///             information.
///             /!\ This array must have a size of SGL_SHAVE_L2C_PART_NB.
///
/// @return
///     - SGL_SUCCESS - partitions information returned successfully.
///     - SGL_ERR_PARAM - parameters are not specified correctly.
///
int32_t SglResMgrGetShaveL2cPartMap(SglResMgrShaveL2cPartEntry part_entry[]);

/// @brief Allows to set the information for a given SHAVE L2 cache partition.
///
/// @param[in] part_entry - structure containing partition information.
/// @param[in] id - partition number.
///
/// @return
///     - SGL_SUCCESS - partition information set successfully.
///     - SGL_ERR_PARAM - parameters are not specified correctly.
///
int32_t SglResMgrSetShaveL2cPart(SglResMgrShaveL2cPartEntry *part_entry,
                                 uint8_t id);

/// @brief Allows to set the information for all the SHAVE L2 cache partitions
///        at the same time.
///
/// @param[in] part_entry - Array of structures containing each partition's
///            information.
///            /!\ This array must have a size of SGL_SHAVE_L2C_PART_NB.
///
/// @return
///     - SGL_SUCCESS - partitions information set successfully.
///     - SGL_ERR_PARAM - parameters are not specified correctly.
///
int32_t SglResMgrSetShaveL2cPartMap(SglResMgrShaveL2cPartEntry part_entry[]);

/// @brief Initialize SglResMgr module.
///
/// This function should be called only once from the platform initialization
/// procedure. However, to avoid initialisation errors, this function is
/// protected against multiple calls.
///
void SglResMgrInit(void);

#ifdef __cplusplus
}
#endif

#endif /* _SGL_RESMGR_LL_H_ */

///@}
