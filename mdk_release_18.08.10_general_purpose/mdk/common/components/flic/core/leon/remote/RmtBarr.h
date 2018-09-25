///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Inter Leon sync barrier
///

#if defined(FLIC_2LEONS)

#ifndef __RMT_BARRIER_H__
#define __RMT_BARRIER_H__

#include <FlicRmt.h>
#include <rtems/bspIo.h>

class RmtBarr
{
  private:
    sem_t  syncSem;
    sem_t *wakeSem;
    RmtBarr *rmt;

  //public:
   //pthread_t thread;

  public:
   uint32_t hitCnt; //debug

   static void Init();
   static void DeInit();

   void Create(RmtBarr *remote)
   {
       hitCnt  = 0;
       rmt     = remote;
       extern sem_t _barrWakeSem;
       wakeSem = &_barrWakeSem;
       assert(_barrWakeSem);
       assert(OK == sem_init(&syncSem, 0, 0));
       assert(isNonCachedAddr(this));
   }
   void Destroy()
   {
       sem_destroy(&syncSem);
   }
   void Wait() //limitation: don't invoke .Wait() on same object from multiple threads
   {
       RmiCtx ctx;
       ctx.semToWake =            rmt->wakeSem; //wakes RmtBarr::thread
       ctx.params[0] = (uint32_t)&rmt->syncSem; //will be posted by RmtBarr::thread
       FlicRmt::RmiCall(&ctx); //waits for ACK

       sem_wait(&syncSem);
       hitCnt++;
   }
};

#endif
#endif
