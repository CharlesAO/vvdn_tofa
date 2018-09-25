///
/// @file scResMgrInternal.h
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup scResMgr
/// @{
/// @brief     Resource Manager SHAVE driver internal API
///


#ifndef _RESMGR_INTERNAL_SC_H_
#define _RESMGR_INTERNAL_SC_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdint.h>

#include "SglResMgrDefines.h"
#include "scResMgr.h"

#ifdef  __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// SGL exported objects re-mapping.
typedef SglResMgrShaveL2cPartEntry ScResMgrShaveL2cPartEntry;

// CPU lock scope.
#define SC_RESMGR_CPU_LOCK_SHAVE 0
#define SC_RESMGR_CPU_LOCK_SYSTEM 1

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------


////////////////////////////////////////////////////////////////////////////////
//                                   / \                                      //
//                                  / ! \                                     //
//                                 /_____\                                    //
//                                                                            //
// All the functions declared in this file are supposed to be for drivers'    //
// internal use only! A user application should not need to call them and,    //
// thus, should not need to include this file.                                //
////////////////////////////////////////////////////////////////////////////////


/// @brief Allows to write clock configuration.
///
/// Set the clocks configuration information for it to be available for
/// everybody.
///
/// @param[in] info - structure containing clocks configuration.
///
/// @return
///     - SGL_SUCCESS - Clock entry set successfully.
///     - SGL_ERR_PARAM - infoclk parameter is not specified correctly.
///
int32_t ScResMgrSetClockEntry(ScResMgrInfoClockEntry *infoclk);

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
int32_t ScResMgrSetClockEntryField(ScResMgrClk clock_entry, uint32_t value);

/// @brief Allows to get the information for a given SHAVE L2 cache partition.
///
/// @param[out] part_entry - structure containing partition information.
/// @param[in] id - partition number.
///
/// @return
///     - SGL_SUCCESS - partition information returned successfully.
///     - SGL_ERR_PARAM - parameters are not specified correctly.
///
int32_t ScResMgrGetShaveL2cPart(ScResMgrShaveL2cPartEntry *part_entry,
                                uint8_t id);

/// @brief Allows to get the information for all the SHAVE L2 cache partitions
///        at the same time.
///
/// @param[out] part_entry - Array of structures containing each partition's
///             information.
///             /!\ This array must have a size of SGL_RESMGR_SHAVE_L2C_PART_NB.
///
/// @return
///     - SGL_SUCCESS - partitions information returned successfully.
///     - SGL_ERR_PARAM - parameters are not specified correctly.
///
int32_t ScResMgrGetShaveL2cPartMap(ScResMgrShaveL2cPartEntry part_entry[]);

/// @brief Allows to set the information for a given SHAVE L2 cache partition.
///
/// @param[in] part_entry - structure containing partition information.
/// @param[in] id - partition number.
///
/// @return
///     - SGL_SUCCESS - partition information set successfully.
///     - SGL_ERR_PARAM - parameters are not specified correctly.
///
int32_t ScResMgrSetShaveL2cPart(ScResMgrShaveL2cPartEntry *part_entry,
                                uint8_t id);

/// @brief Allows to set the information for all the SHAVE L2 cache partitions
///        at the same time.
///
/// @param[in] part_entry - Array of structures containing each partition's
///            information.
///            /!\ This array must have a size of SGL_RESMGR_SHAVE_L2C_PART_NB.
///
/// @return
///     - SGL_SUCCESS - partitions information set successfully.
///     - SGL_ERR_PARAM - parameters are not specified correctly.
///
int32_t ScResMgrSetShaveL2cPartMap(ScResMgrShaveL2cPartEntry part_entry[]);

/// @brief Obtain a given ResMgr CPU lock.
///
/// This can be used to protect a piece of code from interferences of other
/// CPUs.
///
/// @param[in] sys_wide - 1 for system-wide lock, 0 to lock other SHAVEs only.
/// @param[out] handler - handler on the obtained mutex, needed to release it.
///
/// @return
///     - SGL_SUCCESS - Lock obtained successfully.
///     - SGL_ERR_PARAM - parameters are not specified correctly.
///
int32_t ScResMgrCpuLock(uint8_t sys_wide, ScResMgrHandler *handler);

/// @brief Release the ResMgr CPU lock.
///
/// @param[in] handler - handler on the obtained mutex.
///
/// @return
///     - SGL_SUCCESS - Lock released successfully.
///     - SGL_ERR_PARAM - parameters are not specified correctly.
///
int32_t ScResMgrCpuUnlock(ScResMgrHandler *handler);

#ifdef __cplusplus
}
#endif

///@}

#endif /* _RESMGR_INTERNAL_SC_H_ */

///@}
