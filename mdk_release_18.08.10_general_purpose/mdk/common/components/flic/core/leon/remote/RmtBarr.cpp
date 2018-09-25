///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Inter Leon comms - Barrier thread
///

#if defined(FLIC_2LEONS)

#include <RmtBarr.h>
//#include <rtems/bspIo.h> //for printk

sem_t            _barrWakeSem = 0; //== SEM_FAILED
static pthread_t _barThread;

#ifndef RMT_BAR_THR_PRI
#define RMT_BAR_THR_PRI 0 //not defined
#endif

//##############################################################################
//Thread that handles all barrier objects
void * barrThreadFunc(void * arg)
{
  UNUSED(arg);
  pthread_setname_np(RTEMS_SELF, "RmtBarrThrd");

  while(1){
    if(OK == sem_wait(&_barrWakeSem))
    {
      assert(OK == sem_post((sem_t*)inpCall->params[0]));
      FlicRmt::RmiAck();
    }
    else break;
  }

  return NULL;
}

//##############################################################################
void RmtBarr::Init()
{
   //Must create semaphore outside thread, as incomming IRQ might immediately
   //try and increment it, even if thread is not yet scheduled to run
   assert(OK == sem_init(&_barrWakeSem,0,0));

   pthread_attr_t attr;
   assert(OK == pthread_attr_init           (&attr));
   assert(OK == pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED));
   assert(OK == pthread_attr_setschedpolicy (&attr, SCHED_RR));

   if(RMT_BAR_THR_PRI){
     struct sched_param schParam;
     schParam.sched_priority = RMT_BAR_THR_PRI;
     assert(OK == pthread_attr_setschedparam  (&attr, &schParam));
   } //else, will just use default RTEMS thread priority

   assert(OK == pthread_create(&_barThread, &attr, &barrThreadFunc, NULL));
   assert(OK == pthread_attr_destroy   (&attr));
}

//##############################################################################
void RmtBarr::DeInit()
{
   assert(OK == sem_destroy(&_barrWakeSem));
   pthread_join(_barThread, NULL);
}

//ugly export
void BarInit(){RmtBarr::Init  ();}
void BarKill(){RmtBarr::DeInit();}

#endif //FLIC_2LEONS