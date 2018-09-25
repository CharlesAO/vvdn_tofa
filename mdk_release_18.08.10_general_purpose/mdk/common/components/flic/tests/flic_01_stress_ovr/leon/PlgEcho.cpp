///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugins
///

#include "PlgEcho.h"
#include <BehQueueOvr.h>

//#############################################
void PlgEchoSend::Create(int data)
{
    what = data;
    Add(&out);
}
//=============================================
void * PlgEchoSend::threadFunc(void *)
{
    sent = 0;

    while(Alive())
    {
      EchoMsg msg;
      msg.data = what;
      out.Send(&msg);
      sent++;
      rtems_task_wake_after(9 + what/2);
    }

    return NULL;
}

//#############################################
void PlgEchoRecv::Create(int num, uint32_t *out)
{
    nHist = num;
    hist  = out;
    in.Create(32, QueueCmdOvr<EchoMsg>::Inst());
    Add(&in);
}
//=============================================
void * PlgEchoRecv::threadFunc(void *)
{
    printf("To receive %ld\n", nHist);
    for(uint32_t i=0; i<nHist; i++)
    {
        EchoMsg msg;
        if(OK == in.Receive(&msg))
        {
          assert(msg.data < N_HIST);
         //printf("[%ld], %d\n", i, (int)msg.data);
          rtems_task_wake_after(1);
          hist[msg.data]++;
        }
    }

    return NULL;
}