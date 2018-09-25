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


#define SEED(i) (0xAB000000 + 20000 * i)

//#########################################################
template<typename T> class PlgA : public ThreadedPlugin{
  public:
   MReceiver<MyFrmPtr> in; //pull from Pool
   MSender  <T>        out;
   uint32_t num, s;

   void Create(uint32_t n, uint32_t seed){
      num = n;
      s   = seed;
      Add(&in,  ".in" );
      Add(&out, ".out");
   }
   virtual void *threadFunc(void *)
   {
       for(uint32_t i=0; i<num; i++)
       {
         MyFrmPtr frmIn;
         assert(OK == in.Receive(&frmIn));
         frmIn->seqNo = s + i;
         out.Send(&frmIn);
       }
       return NULL;
   }
};

//#########################################################
template<typename T> class PlgB : public ThreadedPlugin{
  public:
   SReceiver<T> in;
   MSender  <T> out;
   uint32_t num;

   void Create(uint32_t n){
      num = n;
      in.Create(2);
      Add(&in);
      Add(&out);
   }
   void *threadFunc(void *){
      for(uint32_t i=0; i<num; i++)
      {
        T m;
        assert(OK == in.Receive(&m));
        //assert(m.data == SEED + i);
        assert(OK == out.Send  (&m));
      }
      return NULL;
   }
};

//#########################################################
template<typename T> class PlgC : public ThreadedPlugin{
  public:
   SReceiver<T> in;
   uint32_t num, s, err;

   void Create(uint32_t n, uint32_t seed){
      num = n;
      s   = seed;
      err = 0;
      in.Create(2);
      Add(&in, ".in");
   }
   void *threadFunc(void *){
      for(uint32_t i=0; i<num; i++)
      {
        T m;
        if(OK == in.Receive(&m))
        {
          if((uint32_t)m->seqNo != s + i) err++;
          #if (PRINT)
           printf("PlgC : %x\n", m->seqNo);
          #endif
        }
      }
      printf("C_END\n");
      return NULL;
   }
};

#endif