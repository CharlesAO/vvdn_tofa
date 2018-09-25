///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugins
///

#ifndef __TEST_PLG_ECHO_H__
#define __TEST_PLG_ECHO_H__

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <UnitTestApi.h>

#define N_ITERS 5
#define SEED    100

//#############################################
template <typename T> class PlgEchoSend : public ThreadedPlugin
{
  public:
   MSender<T> out;

   void Create() {Add(&out);}

   void *threadFunc(void *){
      T msg;
      msg.data = SEED;

      for(uint32_t i=0; i<N_ITERS; i++){
        rtems_task_wake_after(300);
        out.Send(&msg);
        msg.data++;
      }
      return NULL;
   }
};

//#############################################
template <typename T> class PlgEchoRecv : public ThreadedPlugin
{
  public:
   SReceiver<T> in;

   void Create(){
     in.Create(8);
     Add(&in);
   }

   void *threadFunc(void *)
   {
     for(uint32_t i=0; i<N_ITERS; i++){
        T msg;
        if(OK == in.Receive(&msg))
        {
         unitTestAssert(msg.data == SEED+i);
         printf("%ld %s\n", msg.data, msg.Info());
        }
     }
    return NULL;
   }
};

#endif