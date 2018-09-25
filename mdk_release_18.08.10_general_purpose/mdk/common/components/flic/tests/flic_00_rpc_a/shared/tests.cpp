#include <Flic.h>
#include <FlicRmt.h>
#include <RmtBarr.h>
#include "testConfig.h"

//######################################################
void test_0(RmtBarr *barArr)
{
   printf("test_0\n");
   for(int i=0; i<RUNS; i++)
   {
   #if !defined(LEON_RT)
    if(PRINT) printf("\nIter %d\n",i);
   #endif
     for(int b=0; b<N; b++)
     {
      barArr[b].Wait();
     #if !defined(LEON_RT)
      if(PRINT)printf("...barrier %d \n", b);
     #endif
      if(SLEEP) usleep(SLEEP+i*SDELTA);
     }
   }
}

//######################################################
void * testFn(void * param)
{
  RmtBarr *bar = (RmtBarr*)param;
  for(int i=0; i<RUNS; i++)
  {
    bar->Wait();
    if(SLEEP) usleep(SLEEP+i*SDELTA);
  }
  return NULL;
}
//======================================================
static pthread_t thread[N];
void test_1(RmtBarr *barArr)
{
   printf("test_1\n");

   for(int i=0; i<N; i++){
    pthread_attr_t attr;
    assert(OK == pthread_attr_init           (&attr));
    assert(OK == pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED));
    assert(OK == pthread_attr_setschedpolicy (&attr, SCHED_RR));
    assert(OK == pthread_create(&thread[i],   &attr, testFn, &barArr[i]));
    assert(OK == pthread_attr_destroy        (&attr));
   }

   for(int i=0; i<N; i++)
    pthread_join(thread[i], NULL);
}