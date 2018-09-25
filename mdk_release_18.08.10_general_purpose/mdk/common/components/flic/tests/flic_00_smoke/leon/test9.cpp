///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Timed Send/Receive
///

#include <Flic.h>
#include <MsgBuf.h>
#include <BehQueue.h>
#include <Sender.h>
#include <Receiver.h>
#include <Pool.h>
#include <MemAllocator.h>
#include <ImgFrame.h>
#include <UnitTestApi.h>

#define ONE_MS (1000*1000) //[ns]

class Msg {
  public:
    uint32_t d;
};

long int tickInNs = 0; //RTEMS tick [ns]

//###############################################################
//assert that lhs >= rhs
void assertGte(timespec *lhs, timespec *rhs)
{
  timespec tick, rhs2;

 //early exit
  if( rtems_timespec_greater_than(lhs, rhs) ||
      rtems_timespec_equal_to    (lhs, rhs) )
  {
    printf("[sec=%2lld nsec=%9ld] >= [sec=%2lld nsec=%9ld]\n",
         lhs->tv_sec, lhs->tv_nsec,
         rhs->tv_sec, rhs->tv_nsec);
    return; //OK
  }

  //Things are roughly OK, but expiration seems to happen slightly
  //faster than timeOut (within 1xRTEMS TICK)
  tick.tv_sec  = 0;
  tick.tv_nsec = tickInNs;
  rtems_timespec_subtract(&tick, rhs, &rhs2);

  printf("[sec=%2lld nsec=%9ld] >= [sec=%2lld nsec=%9ld]\n",
         lhs->tv_sec, lhs->tv_nsec,
         rhs2.tv_sec, rhs2.tv_nsec);

  assert( rtems_timespec_equal_to    (lhs, &rhs2) ||
          rtems_timespec_greater_than(lhs, &rhs2) );
}

//###############################################################
//Timed MSend/SReceive test
//###############################################################
void test9_a()
{
   Msg m;

   timespec start, end, delta;
   timespec timeOut;

   for(int i=0; i<10; i++)
   {
     MSender  <Msg> send;
     SReceiver<Msg> recv;
     recv.Create(4);
     send.Link(&recv);

     timeOut.tv_sec  = 0;
     timeOut.tv_nsec = (i+12) * ONE_MS;

   //=======================================================
   //Slave receiver buff empty, a receiver should timeout
     clock_gettime(CLOCK_REALTIME, &start);
     assert(OK != recv.Receive(&m, &timeOut));
     clock_gettime(CLOCK_REALTIME, &end);
     rtems_timespec_subtract(&start, &end, &delta);
     assertGte(&delta, &timeOut); //check elapsed time

   //=======================================================
   //Fill receiver buff
     m.d = 10; assert(OK == send.Send(&m));
     m.d = 11; assert(OK == send.Send(&m));
     m.d = 12; assert(OK == send.Send(&m));
     m.d = 13; assert(OK == send.Send(&m));

   //Another timed Send should fail:
     clock_gettime(CLOCK_REALTIME, &start);
     assert(OK != send.Send(&m, &timeOut));
     clock_gettime(CLOCK_REALTIME, &end);
     rtems_timespec_subtract(&start, &end, &delta);
     assertGte(&delta, &timeOut); //check elapsed time

     recv.Destroy();
     recv.Delete ();
   }
}

//###############################################################
//Timed MReceive/SSender test
//###############################################################
#define N_FRM 4 //frames in pool

void test9_b()
{
  timespec start, end, delta;
  timespec timeOut;

  for(int k=0; k<10; k++)
  {
    Pool     <ImgFrame>    pool;
    MReceiver<ImgFramePtr> recv;

    pool.Create(&HeapAlloc, N_FRM, 16);
    assert(pool.FreeCnt()==N_FRM);
    pool.out.Link(&recv);

    {
      ImgFramePtr elem[N_FRM], im;

     //===========================================
     //Can pop N frames "instantly" (say 1ms) :
      timeOut.tv_sec  = 0;
      timeOut.tv_nsec = ONE_MS;
      for(int i=0; i<N_FRM; i++)
      {
        //Pulling from Pool:
         clock_gettime(CLOCK_REALTIME, &start);
         assert(OK == recv.Receive(&elem[i], &timeOut, REL));
         clock_gettime(CLOCK_REALTIME, &end);
         rtems_timespec_subtract(&start, &end, &delta);
         assertGte(&timeOut, &delta); //check delta < timeOut
      }

     //===========================================
     //Next pull should fail as Pool is empty
        timeOut.tv_nsec = 50 * ONE_MS;
        clock_gettime(CLOCK_REALTIME, &start);
        assert(OK != recv.Receive(&im, &timeOut, REL));
        clock_gettime(CLOCK_REALTIME, &end);
        rtems_timespec_subtract(&start, &end, &delta);
        assertGte(&delta, &timeOut); //check elapsed time

     //===========================================
     //Forcefully return one frame back to pool
        elem[0] = nullptr;
     //Next pull should succeed:
        timeOut.tv_nsec = 50 * ONE_MS;
        clock_gettime(CLOCK_REALTIME, &start);
        assert(OK == recv.Receive(&im, &timeOut, REL));
        clock_gettime(CLOCK_REALTIME, &end);
        rtems_timespec_subtract(&start, &end, &delta);
        assertGte(&timeOut, &delta); //check delta < timeOut

      // frame ptrs get released here ...
    }

    pool.Stop  ();
    pool.Delete();
  }
}

//compute 1xTICK period in ns :
//number of NS in a sec / num_ticks_per_sec
static void calcTickInNs(){
  tickInNs = 1000*1000*1000 / rtems_clock_get_ticks_per_second();
}

//###############################################################
void test9()
{
    calcTickInNs();
    test9_a();
    test9_b();
}
