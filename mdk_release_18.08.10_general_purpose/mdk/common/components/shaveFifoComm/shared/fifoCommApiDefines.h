///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon header
///

#ifndef FIFOCOMMAPIDEFINES_H_
#define FIFOCOMMAPIDEFINES_H_
#include "mv_types.h"
// 1: Includes
// ----------------------------------------------------------------------------
// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
typedef void (*fifoCommTaskCallback_t)(void*);
typedef void (*fifoCommMasterCallback_t)(void*, void*);

typedef struct fifoCommTask_t
{
    u32 taskFifoNr;
    u32 responseFifoNr;
    fifoCommTaskCallback_t initHandler;
    fifoCommTaskCallback_t taskHandler;
    struct fifoCommTask_t* next;
}fifoCommTask_t;


typedef struct fifoCommMasterHandler_t
{
    fifoCommTask_t* taskTypeList;
    fifoCommMasterCallback_t masterHandler;
}fifoCommMasterHandler_t;

typedef struct fifoCommSlaveHandler_t
{
    fifoCommTask_t* taskTypeList;
    volatile u32 stopRequest;
}fifoCommSlaveHandler_t;
// 3: Static Local Data
// ----------------------------------------------------------------------------

// 4:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------
#endif
