///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugins
///

#include "PlgEcho.h"
#include <sched.h>
#include <UnitTestApi.h>

//#############################################
void PlgEchoSend::Create(int data, int howMany)
{
    what = data;
    num  = howMany;
    Add(&out);
}
//=============================================
void * PlgEchoSend::threadFunc(void *)
{
    EchoMsg msg;
    msg.data = what;

    assert(num>0);
  //printf("Sending %ld, %2ld times\n", what, num);

    for(uint32_t i=0; i<num; i++){
      out.Send(&msg);
    //sched_yield();
    }

    return NULL;
}

//#############################################
void PlgEchoRecv::Create(int num, uint32_t *out)
{
    nHist = num;
    hist  = out;
    in.Create(16);
    Add(&in);
}
//=============================================
void * PlgEchoRecv::threadFunc(void *)
{
    EchoMsg msg;

    //printf("To receive %ld\n", nHist);
    for(uint32_t i=0; i<nHist; i++){
        unitTestAssert(OK == in.Receive(&msg));
      //printf("%d\n", (int)msg.data);
        hist[msg.data]++;
    }

    return NULL;
}