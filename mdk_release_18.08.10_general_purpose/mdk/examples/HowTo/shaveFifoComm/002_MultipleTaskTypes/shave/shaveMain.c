///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Master-Slave source code
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include <svuCommonShave.h>
#include "fifoCommApi.h"
#include "common.h"
// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// 4: Static Local Data
// ----------------------------------------------------------------------------
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------
// 6: Functions Implementation
// ----------------------------------------------------------------------------

//Master shave function which will call the slaves
int masterEntry(fifoCommMasterHandler_t* masterHandler, MyDataType* dataS)
{
    int n = dataS->n;

    while (n)
    {
        for (int i = 0; i < TASKS; i++)
        {
            fifoCommMasterAddTask(masterHandler->taskTypeList,
                    (void*) dataS->data[i]);
            fifoCommMasterWaitTask(masterHandler->taskTypeList);
            fifoCommMasterAddTask(masterHandler->taskTypeList->next,
                               (void*) dataS->data[i]);
            fifoCommMasterWaitTask(masterHandler->taskTypeList->next);
        }
        n--;
    }

    return 0;
}

//initialize slave shaves
int slaveInitEntry(void* unnecessary, void* unnecessary2)
{
    (void) unnecessary;
    (void) unnecessary2;
    return 0;
}

//execution code for slave using first entry point
int slaveEntry(int (*data)[BUFFSIZE], void* unnecessary)
{
    (void) unnecessary;

    for (int j = 0; j < BUFFSIZE; j++)
        (*data)[j] = (*data)[j] + 1;

    return 0;
}

//execution code for slave using second entry point
int slaveEntry2(int (*data)[BUFFSIZE], void* unnecessary)
{
    (void) unnecessary;

    for (int j = 0; j < BUFFSIZE; j++)
        (*data)[j] = (*data)[j] + 100;

    return 0;
}
