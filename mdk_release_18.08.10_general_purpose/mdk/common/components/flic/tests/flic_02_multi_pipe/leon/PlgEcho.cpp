///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugins
///

#include "PlgEcho.h"
#include <UnitTestApi.h>

#define SEED 100

//#############################################
void PlgEchoSend::Create()
{
    Add(&out);
}
//=============================================
void * PlgEchoSend::threadFunc(void *)
{
    EchoMsg msg;
    msg.data = SEED;

    while(Alive()){
      rtems_task_wake_after(500);
      out.Send(&msg);
      msg.data++;
    }
    return NULL;
}

//#############################################
void PlgEchoRecv::Create()
{
    in.Create(8);
    Add(&in);
}
//=============================================
void * PlgEchoRecv::threadFunc(void *)
{
    EchoMsg msg;
    uint32_t i=0;

    while(Alive()){
        if(OK == in.Receive(&msg)){
          printf("%d\n", (int)msg.data);
          unitTestAssert(msg.data == SEED + i);
          i++;
        }
    }
    return NULL;
}