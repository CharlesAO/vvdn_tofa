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

typedef struct
{
   ImgFramePtr ptr[2];
   uint32_t    data;
}SyncPtrs;

inline bool operator==(const SyncPtrs& lhs, const SyncPtrs& rhs)
{
   if((lhs.data == rhs.data)) return true ; //....
   else                       return false;
}


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
   SReceiver<ImgFramePtr> in0;
   SReceiver<ImgFramePtr> in1;
   MSender  <SyncPtrs   > out;
   uint32_t num, i, err;

   void Create(uint32_t n){
      num = n;
      in0.Create(2);
      in1.Create(2);
      Add(&in0, ".in0");
      Add(&in1, ".in1");
      Add(&out);
   }
   void *threadFunc(void *){
      for(i=0; i<num; i++)
      {
        ImgFramePtr m0, m1;
        SyncPtrs    oMsg;

        if(OK == in0.Receive(&m0))
         if(OK == in1.Receive(&m1))
         {
           oMsg.ptr[0] = m0;
           oMsg.ptr[1] = m1;
           oMsg.data   = i;

           out.Send(&oMsg);
         }
      }
      return NULL;
   }
};
//#########################################################
class PlgC : public ThreadedPlugin{
  public:
   SReceiver<SyncPtrs> in;
   uint32_t num, s, i, err;

   void Create(uint32_t n, uint32_t seed){
      num = n;
      s   = seed;
      err = 0;
      in.Create(2);
      Add(&in, ".in");
   }
   void *threadFunc(void *){
      for(i=0; i<num; i++)
      {
        SyncPtrs m;

        if(OK == in.Receive(&m))
        {
           if((uint32_t)m.ptr[0]->seqNo != s + i*2) err++;
           if((uint32_t)m.ptr[1]->seqNo != s + i*2) err++;
           if(          m.data          != i      ) err++;
        }
      }
      return NULL;
   }
};


#endif