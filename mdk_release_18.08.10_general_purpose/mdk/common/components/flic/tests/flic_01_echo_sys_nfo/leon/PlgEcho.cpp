///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugins
///

#include "PlgEcho.h"

//#############################################
void PlgEchoSend::Create()
{
    Add(&out);
}
//=============================================
void * PlgEchoSend::threadFunc(void *)
{
    EchoMsg msg;
    msg.data = 100;

    while(1){
      sleep(1);
      msg.data++;
      out.Send(&msg);
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
    while(1){
        EchoMsg msg;
        if(OK == in.Receive(&msg))
         printf("%d\n", (int)msg.data);
    }
    return NULL;
}