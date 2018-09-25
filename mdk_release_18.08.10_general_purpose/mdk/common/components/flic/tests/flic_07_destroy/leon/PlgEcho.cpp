///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugins
///

#include "PlgEcho.h"
#include <UnitTestApi.h>

//#############################################
void PlgEchoSend::Create() {
   Add(&out);
   unitTestAssert(nIO == 1);
}

void * PlgEchoSend::threadFunc(void *)
{
    EchoMsg msg;
    msg.data = 100;
    while(Alive()){
      rtems_task_wake_after(200);
      msg.data++;
      out.Send(&msg);
    }
    return NULL;
}

//#############################################
void PlgEchoRecv::Create()
{
    in.Create(4);
    Add(&in);
    unitTestAssert(nIO == 1);
}

//=============================================
void * PlgEchoRecv::threadFunc(void *)
{
    EchoMsg msg;
    while(Alive()){
        if(OK == in.Receive(&msg))
           printf("%d\n", (int)msg.data);
    }
    return NULL;
}