///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon header
///

#ifndef _FIFO_COMM_API_H_
#define _FIFO_COMM_API_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "fifoCommApiDefines.h"
// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

// 3: Static Local Data
// ----------------------------------------------------------------------------

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C"
#endif
/// @brief Add new task to be executed by the slaves
/// @param[in] taskType       - task to be executed
/// @param[in] taskParameters - parameters of the task
/// @return void
///
void fifoCommMasterAddTask(fifoCommTask_t* taskType, void* taskParameters);
#ifdef __cplusplus
extern "C"
#endif
/// @brief Wait for task to be executed
/// @param[in] taskType     - task we are waiting for
/// @return void
///
void* fifoCommMasterWaitTask(fifoCommTask_t* taskType);
#ifdef __cplusplus
extern "C"
#endif
/// @brief Slave reads task from FIFO
/// @param[in] taskType   - task to be read
/// @param[in] result     - stores the value read from the FIFO
/// @return if the read was successful or not
///
u32 fifoCommSlaveReadTask(fifoCommTask_t* taskType, u32* result);
#ifdef __cplusplus
extern "C"
#endif
/// @brief Write a 32-bit value to a Shave FIFO
/// @param[in] taskType   - response FIFO which will be used
/// @param[in] taskParameters
/// @return void
///
void fifoCommSlaveNotifyTaskCompletion(fifoCommTask_t* taskType, void* taskParameters);
#ifdef __cplusplus
extern "C"
#endif
/// @brief Runs the master shave
/// @param[in] handler   - master handler of the shave
/// @param[in] params    - master parameters
/// @return void
///
void fifoCommMasterRun(fifoCommMasterHandler_t* handler, void* params);
#ifdef __cplusplus
extern "C"
#endif
/// @brief Runs the slave shave
/// @param[in] handler   - handler of the slave
/// @return void
///
void fifoCommSlaveRun(fifoCommSlaveHandler_t* handler);

#endif
