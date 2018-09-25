///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     shave Fifo communication init functions
///

#ifndef FIFOCOMMINITAPI_H_
#define FIFOCOMMINITAPI_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "fifoCommApiDefines.h"
#include "OsDrvSvu.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
// 3: Static Local Data
// ----------------------------------------------------------------------------

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif
/// @brief Open and start master
/// @param[in] masterHandler     - handler of the used master
/// @param[in] handler           - handler of the used shave
/// @param[in] masterParams      - parameter of the called function
/// @param[in] shaveUsed         - shaved used for the master
/// @return void
///
int fifoCommStartMaster(fifoCommMasterHandler_t* masterHandler,
        osDrvSvuHandler_t* handler, u32* fifoCommMasterRun, void* masterParams, u32 shaveNumber);

/// @brief Open and start slave
/// @param[in] slaveHandler     - handler of the used slave
/// @param[in] handler          - handler of the used shave
/// @param[in] shaveNumber      - slave entry point id
/// @return void
///
void fifoCommStartSlave(fifoCommSlaveHandler_t* slaveHandler,
        osDrvSvuHandler_t* handler, u32* slaveRun, int shaveNumber);

/// @brief The function will wait for the master to finish its task. After the task is finished, the shave will be closed.
/// @param[in] handler     - shave handler
/// @return void
///
void fifoCommWaitMaster(osDrvSvuHandler_t* handler);

/// @brief The function will wait for the slave to finish its task. After the task is finished, the shave will be closed.
/// @param[in] handler     - shave handler
/// @return void
///
void fifoCommWaitSlave(osDrvSvuHandler_t* handler);

/// @brief Master entry point. The function pointer which will be executed on the master shave
/// @param[in] handler     - master handler to store the required private information.
/// @param[in] taskHandler - master entry point
/// @return void
///
void fifoCommMasterInit(fifoCommMasterHandler_t* handler, fifoCommMasterCallback_t taskHandler);

/// @brief Initialize slave handler
/// @param[in] handler - slave handler to store the required private information
/// @return void
///
void fifoCommSlaveInit(fifoCommSlaveHandler_t* handler);

/// @brief Create a new task type and register it to the master.
/// @param[in] masterHandler  - the already initialized master handler
/// @param[in] taskType       - pointer to a taskType. This will be initialized by the function
/// @param[in] taskFifoNr     - the FIFO from which the task will be executes
/// @param[in] responseFifoNr - the response FIFO number
/// @return void
///
void fifoCommMasterRegisterTaskType(fifoCommMasterHandler_t* masterHandler, fifoCommTask_t* taskType,
                                    u32 taskFifoNr, u32 responseFifoNr);

/// @brief Set on slave handler the task which can be done by the slave
/// @param[in] slaveHandler - handler of the used slave
/// @param[in] taskType     - pointer to a taskType. This will be initialized by the function
/// @param[in] masterTask   - pointer to the already initialized masterTaskType. The slave task will be linked to this type of task on master
/// @param[in] initHandler  - slave init entry point. The function pointer which will be executed on the slave shave once, at the beginning
/// @param[in] taskHandler  - slave entry point. The function pointer which will be executed on the slave shave each time a new task of this type was added
/// @return void
///
void fifoCommSlaveRegisterTaskType(fifoCommSlaveHandler_t* slaveHandler, fifoCommTask_t* taskType,
                                   fifoCommTask_t* masterTask, fifoCommTaskCallback_t initHandler,
                                   fifoCommTaskCallback_t taskHandler);
#ifdef __cplusplus
}
#endif
#endif
