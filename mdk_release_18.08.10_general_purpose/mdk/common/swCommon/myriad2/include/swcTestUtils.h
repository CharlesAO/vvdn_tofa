///  
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved 
///            For License Warranty see: common/license.txt   
///
/// @defgroup TestUtilsApi Test Utilities API
/// @ingroup  TestUtilsApi
/// @{
/// @brief  Test Utils functions API   
/// 
/// Series of utility functions to facilitate automated test
/// 

#ifndef SOFTWARE_TEST_UTILS_H
#define SOFTWARE_TEST_UTILS_H 

// 1: Includes      
// ----------------------------------------------------------------------------
#include "swcTestUtilsDefines.h"
#include "mv_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------
// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

/// @brief This function recognizes the processor type the simulations are running on
/// @return    Processor type
///
tyProcessorType swcGetProcessorType(void);


/// @brief Function that initializes the benchmark structure`s elements
///
/// Initializes with either 0, or -1(-1 is used to avoid cases when execution cycles or stalls are 0)
/// @param[in]  perfStruct - pointer to the structure that should be initialized
/// @return  void
/// @code
///swcShaveProfInit(0, &perfStr);
///
///while( swcShaveProfGatheringDone(&perfStr) == -1)
///{
///
///    swcShaveProfStartGathering(0, &perfStr);
///    swcStartShave(0,(u32)&SVE0_main);
///    swcWaitShave(0);
///    swcShaveProfStopGathering(0, &perfStr);
///
///}
///swcShaveProfPrint(0, &perfStr);
///@endcode
///
void swcShaveProfInit(performanceStruct *perfStruct);


/// @brief Function that starts the counters for structure`s members
/// @param[in]  shaveNumber - shave number to start
/// @param[in]  perfStruct  - pointer to the structure that should be initialized
/// @return  void
///
void swcShaveProfStartGathering(u32 shaveNumber, performanceStruct *perfStruct);


/// @brief Function that verifies if all the structure`s parameters are updated with the values from the counters
// @param[in]  shaveNumber  shave number to start
/// @param[in]  perfStruct - pointer to the structure that should be updated with the values read from counters
/// @return  returns -1 if not all structure`s filed are updated and 1 if they are
///
int swcShaveProfGatheringDone(performanceStruct *perfStruct);


/// @brief Function that reads the values from the counters
/// @param[in]  shaveNumber - shave number to start
/// @param[in]  perfStruct  - pointer to the structure that should be updated with the counter values
/// @return  void
///
void swcShaveProfStopGathering(u32 shaveNumber, performanceStruct *perfStruct);


/// @brief Function that prints the values that were read from the counters
/// @param[in]  shaveNumber - shave number to start
/// @param[in]  perfStruct  - pointer to the structure whose params are all updated
/// @return  void
///
void swcShaveProfPrint(u32 shaveNumber, performanceStruct *perfStruct);

/// @brief Function that starts one counter at the time, finding information about one possible field only
/// @param[in]  shaveNumber - shave number to start
/// @param[in]  perfDefines - one of the fields from the enum perfCounterDef
/// @return  void
///
void swcShaveProfStartGatheringFields(u32 shaveNumber, performanceCounterDef perfDefines);

/// @brief Function that prints and reads values from counters
/// @param[in]  shaveNumber - shave number to start
/// @param[in]  perfDefines - one of the fields from the enum perfCounterDef(for stalls, instructions, branches, timer and clk cycles)
/// @return     void
void swcShaveProfStopFieldsGathering(u32 shaveNumber, performanceCounterDef perfDefines);

/// @brief Function that prints and reads values from counters
/// @param[in]  shaveNumber - shave number to start
/// @param[in]  perfDefines - one of the fields from the enum perfCounterDef(for stalls, instructions, branches, timer and clk cycles)
/// @return     void
/// @deprecated This function is deprecated. Use swcShaveProfStopFieldsGathering instead.
void swcShaveProfStopFieldsGatehering(u32 shaveNumber, performanceCounterDef perfDefines)
                                     __Deprecated__("Use swcShaveProfStopFieldsGathering instead");

/// @brief Function that start gathering information about cycles, stalls and instructions
/// @param[in]  shaveNumber - shave number to start
/// @return     void
///
void swcShaveProfileCyclesStart(u32 shaveNumber);

/// @brief Function that prints and reads values from counters
/// @param[in]  shaveNumber - shave number to start
/// @return     void
///
void swcShaveProfileCyclesStop(u32 shaveNumber);
///@}
#ifdef __cplusplus
}
#endif

#endif
