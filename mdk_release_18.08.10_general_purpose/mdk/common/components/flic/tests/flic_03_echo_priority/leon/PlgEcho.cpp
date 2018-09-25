///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugins
///

#include "PlgEcho.h"
#include <UnitTestApi.h>

//##############################################################
void PlgEchoSend::Create() {Add(&out); }

void * PlgEchoSend::threadFunc(void *)
{
    EchoMsg msg;
    msg.data = this->what;

    while(Alive())
    {
      sleep(1);
      msg.data++;
      printf(">>> %d\n", (int)msg.data);
      out.Send(&msg);
    }
    return NULL;
}

//##############################################################
void PlgEchoRecv::Create()
{
    sync.Create();
    inS.Create(4, &sync);
    inV.Create(4, &sync);
    Add(&inS); Add(&inV);
}
//=============================================
void PlgEchoRecv::Stop(){
    sync.Destroy ();
    IPlugin::Stop();
}
//=============================================
void * PlgEchoRecv::threadFunc(void *)
{
    while(Alive())
    {
        EchoMsg msg;
        msg.data = 0; //clear

       //1)Wait for something to happen
        if(OK == sync.Wait())
        {
          //2)Analyse
                if(OK == inV.TryReceive(&msg))
                   printf("inV:%d\n", (int)msg.data);
           else if(OK == inS.TryReceive(&msg))
                   printf("inS:%d\n", (int)msg.data);
           else {//sem incremented, but not events detected => err
                   unitTestAssert(0);
           }
       }
    }
    return NULL;
}