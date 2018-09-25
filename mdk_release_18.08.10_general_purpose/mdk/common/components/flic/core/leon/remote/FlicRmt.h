///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Inter Leon comms - plugin helpers
///

#if defined(FLIC_2LEONS)

#ifndef __FLIC_RMT_H__
#define __FLIC_RMT_H__

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <FlicIpc.h>

//########################################################################
template<typename T> class RmtI;

template<typename T> class RmtO : public ThreadedPlugin //Stub
{
  public:
    uint32_t cnt;
    SReceiver<T>  in;
    RmtI     <T> *rmtI; //on other Leon
    sem_t         resp;
    void Create(RmtI<T> *_rmt, int nrMsg=16);
    void Stop();
    virtual void *threadFunc(void*);
};

template<typename T> class RmtI : public ThreadedPlugin //Skel
{
  public:
    uint32_t cnt;
    sem_t       inCall;
    MSender<T>  out;
    RmtO   <T> *rmtO; //on other Leon
    void Create(RmtO<T> *_rmt);
    void Stop();
    virtual void *threadFunc(void*);
};

//########################################################################
//Implementation
//########################################################################

//========================================================================
template<typename T> void RmtO<T>::Create(RmtI<T> *_rmt, int nrMsg)
{
    rmtI = _rmt;
    assert(OK == sem_init(&resp, 0, 0));
    in.Create(nrMsg);
    Add(&in, ".in");
    if(0 == schParam.sched_priority) {
        schParam.sched_priority = FLIC_IPC_DEFAULT_THREAD_PRIORITY;
    }
    cnt = 0;
}
//========================================================================
template<typename T> void RmtO<T>::Stop()
{
    IPlugin::Stop();
    sem_destroy(&resp);
}
//========================================================================
template<typename T> void * RmtO<T>::threadFunc(void *)
{
    int rc;
    SetThreadName("RmtO");
    while(Alive())
    {
       T  msg;
       rc = in.Receive(&msg);
       cnt++;
       if(OK == rc)
       {
        //0)Make a copy of the message in non-cached space !!!
          uint8_t    TmpBuff [DCALGN(sizeof(T))] ALIGNED(64);    //   aligned storage
          uint8_t * pTmpBuff = (uint8_t*)NonCachedAddr(&TmpBuff);//NC aligned storage
          T       * pNcMsg   = (T*)pTmpBuff;
          new(pTmpBuff) T(msg);//invoke copy-constructor on the NC storage !!!
        //Read-back & check
          if(FLIC_READBACK){
            T *rdBack = pNcMsg;
            while(!(*rdBack == msg))
             {NOP;NOP;NOP;NOP;NOP;}
          }
        //1)Remote call
          RmiCtx ctx;
          ctx.semToWake = &rmtI->inCall;
          ctx.params[0] = (uint32_t)pNcMsg;
          FlicRmt::RmiCall(&ctx); //waits for ACK
        //2)Wait for RESP
          sem_wait(&resp);
        //Kill the NC object clone (storage is on stack, will be cleared soon)
          pNcMsg->~T();
          rc = inpCall->params[0];
        //printf("rc = %d\n", rc);
          FlicRmt::RmiAck(); //ack only after resp is taken
       }
    }
    return NULL;
}
//########################################################################
template<typename T> void RmtI<T>::Create(RmtO<T> *_rmt)
{
    rmtO = _rmt;
    Add(&out, ".out");
    assert(OK == sem_init(&inCall, 0, 0));
    if(0 == schParam.sched_priority) {
        schParam.sched_priority = FLIC_IPC_DEFAULT_THREAD_PRIORITY;
    }
    cnt = 0;
}
//========================================================================
template<typename T> void RmtI<T>::Stop()
{
    IPlugin::Stop();
    sem_destroy(&inCall);
}
//========================================================================
template<typename T> void * RmtI<T>::threadFunc(void *)
{
    int rc;
    SetThreadName("RmtI");
    while(Alive())
    {
       rc = sem_wait(&inCall);
       cnt++;
       if(OK == rc)
       {
        //1) Get the param
          T *msg = (T*)inpCall->params[0];
        //2) Ack the call
          FlicRmt::RmiAck();
        //3) Send might be blocking here...
          T  msgD = *msg; //just grow refCnt
          rc = out.Send(&msgD);
        //4) Return rc to caller
          RmiCtx ctx;
          ctx.semToWake = &rmtO->resp;
          ctx.params[0] = rc;
          FlicRmt::RmiCall(&ctx); //waits for ACK
       }
    }
    return NULL;
}

#endif
#endif
