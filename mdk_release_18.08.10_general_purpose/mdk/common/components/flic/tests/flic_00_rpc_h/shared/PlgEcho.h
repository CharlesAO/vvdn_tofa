///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test dummy plugins
///

#ifndef __PLG_ECHO_H__
#define __PLG_ECHO_H__

#include "testConfig.h"
#include <UnitTestApi.h>


#define SEED(i) (0xAB000000 + 10000 * i)

//#########################################################
class PlgA : public ThreadedPlugin{
  public:
   MReceiver<ImgFramePtr> in; //pull from Pool
   MSender  <ImgFramePtr> out;
   uint32_t num, s, i;

   void Create(uint32_t n, uint32_t seed){
      num = n;
      s   = seed;
      Add(&in,  ".in" );
      Add(&out, ".out");
   }
   virtual void *threadFunc(void *)
   {
       for(i=0; i<num; i++)
       {
         ImgFramePtr frmIn;
         assert(OK == in.Receive(&frmIn));
         frmIn->seqNo = s + i*2;
         out.Send(&frmIn);
       }
       return NULL;
   }
};

//#########################################################
class PlgB : public ThreadedPlugin{
  public:
   SReceiver<ImgFramePtr> in;
   uint32_t num, s, delay, i, err;

   void Create(uint32_t n, uint32_t seed, uint32_t dly){
      num = n;
      delay = dly;
      s  = seed;
      in.Create(2);
      err = 0;
      Add(&in, ".in");
   }
   void *threadFunc(void *){
      for(i=0; i<num; i++)
      {
        ImgFramePtr m;
        if(OK == in.Receive(&m))
        {
          if((uint32_t)m->seqNo != s + i*2) err++;
          if(delay) rtems_task_wake_after(delay);
        }
      }
      return NULL;
   }
};

#endif