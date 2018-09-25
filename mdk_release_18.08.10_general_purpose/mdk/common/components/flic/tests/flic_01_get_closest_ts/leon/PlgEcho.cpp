///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugins
///

#include "PlgEcho.h"
#include <BehQueue.h>
#include <UnitTestApi.h>

#define N_MSG 4
#define TIMESTAMP(x) (1000*(x+1))

//Must tell FLIC how to get TS from an arbitrary message struct
template<> uint64_t ClosestTsGet<EchoMsg>(EchoMsg *m)
{
    return m->ts;
};

//#############################################
void PlgEchoSend::Create() {Add(&out);}

void * PlgEchoSend::threadFunc(void *)
{
    EchoMsg msg;

    for(int i=0; i<N_MSG; i++)
    {
      msg.ts = TIMESTAMP(i);
      out.Send(&msg);
    }

    return NULL;
}

//#############################################
void PlgEchoRecv::Create()
{
    in.Create(N_MSG);
    Add(&in);
}
//=============================================
void * PlgEchoRecv::threadFunc(void *)
{
    int      i;
    EchoMsg  msg;
    ClosestTsCmd<EchoMsg> req;

  //Wait till Sender push all messages
    while(in.mq.Count() != N_MSG)
      rtems_task_wake_after(100);

   //Peek into the message queue
    for(i=0; i<N_MSG; i++)
       printf("Peek msg[%d].ts = %lld\n", i, in.mq.mssg[i]->ts);

    if(1)
    {
       //====================================
       //Pop messages in backwards order
        for(i=N_MSG-1; i>=0; i--){
          req.targetTs = TIMESTAMP(i)-5; //requested TS
          if(OK == in.Receive(&msg, &req)) {
             printf("Popped %lld (requested %lld)\n", msg.ts, req.targetTs);
             unitTestAssert(msg.ts == (uint32_t)TIMESTAMP(i));
          }
          else
             unitTestAssert(0);
        }
    }
    else{
       //====================================
       //Pop messages in Queue order
        for(i=0; i<N_MSG; i++){
          if(OK == in.Receive(&msg))
             printf("Popped ts = %lld\n", msg.ts);
          else
             unitTestAssert(0);
        }
    }

  //Queue is now empty, a TryReceive should fail
    unitTestAssert(in.TryReceive(&msg) != 0);

    return NULL;
}