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
#include <BehQueueOvr.h>

#define W_V(x) (x+100)
#define H_V(x) (x+200)

//#########################################################
class PlgA : public ThreadedPlugin{
  public:
   MReceiver<ImgFramePtr> in; //pull from Pool
   MSender  <ImgFramePtr> out;
   uint32_t num, i;

   void Create(uint32_t n){
      num = n;
      Add(&in,  ".in" );
      Add(&out, ".out");
   }
   virtual void *threadFunc(void *)
   {
       for(i=0; i<num; i++)
       {
         ImgFramePtr frmIn;
         assert(OK == in.Receive(&frmIn));
       //set some members for checks
         frmIn->seqNo          = i;
         frmIn->fb.spec.width  = W_V(i);
         frmIn->fb.spec.height = H_V(i);
         out.Send(&frmIn);
       }
       return NULL;
   }
};

//#########################################################
class PlgB : public ThreadedPlugin{
  public:
   SReceiver<ImgFramePtr> in;
   uint32_t num, delay, i, err, nRec;

   void Create(uint32_t n, uint32_t dly){
      num   = n;
      delay = dly;

    //in.Create(2);
      in.Create(2, QueueCmdOvr<ImgFramePtr>::Inst());

      err   = 0;
      nRec  = 0;
      Add(&in, ".in");
   }
   void *threadFunc(void *)
   {
      bool done = false;
      while(!done)
      {
        ImgFramePtr m;
        if(OK == in.Receive(&m))
        {
          nRec++; //<= num

          if((uint32_t)m->seqNo == num-1) done = true;

          uint32_t j = (uint32_t)m->seqNo;
          if(m->fb.spec.width  != W_V(j)) err++;
          if(m->fb.spec.height != H_V(j)) err++;

          if(delay) usleep(delay);
        }
      }
      return NULL;
   }
};

#endif