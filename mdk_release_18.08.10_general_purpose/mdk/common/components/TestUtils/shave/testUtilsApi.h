///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Software test library
///

#ifndef __TEST_UTILS_API_H__
#define __TEST_UTILS_API_H__

/// 1: Includes
/// ----------------------------------------------------------------------------
#include <testUtilsApiDefines.h>
/// 2:  Source Specific #defines and types  (typedef, enum, struct)
/// ----------------------------------------------------------------------------
///
/// 3: Static Local Data
///
/// ----------------------------------------------------------------------------
///
/// 4:  Exported Functions (non-inline)
///
/// ----------------------------------------------------------------------------

/// @brief Initializes the performance counters and its performance structure
/// @param[in] perfStruct   - performance structure where counter values are stored
/// @param[in] stallsTypes  - the types of stalls the user wants to count
/// @return void
///
void shaveProfileInit(performanceStruct *perfStruct, u32 stallsTypes);

/// @brief Resets values registered by the 4 counters
/// @return void
///
void shaveProfileReset( void );

/// @brief Enables history registers and counters
/// @param[in] perfStruct  - performance structure where counter values are stored
/// @return void
///
void shaveProfileStart( performanceStruct *perfStruct );

/// @brief Gets value from given counterType
/// @param[in] perfStruct  - performance structure where counter values are stored
/// @param[in] counterType - either instruction, stall, clock cycle or branch
/// @return fieldValue     - value of one of the 4 counterTypes retreved from perfStruct
///
u32 shaveGetFieldValue( performanceStruct *perfStruct, \
                                    performanceCounterDef conterType );

/// @brief Prints all the default measured cycles
/// @param[in] perfStruct  - performance structure where counter values are stored
/// @return void
///
void shaveProfilePrint( performanceStruct *perfStruct );

/// @brief Gets all the counter values and prints them
/// @param[in] perfStruct  - performance structure where counter values are stored
/// @return void
///
void shaveShowCounterValues( performanceStruct *perfStruct );

#endif //__TEST_UTILS_API_H__
