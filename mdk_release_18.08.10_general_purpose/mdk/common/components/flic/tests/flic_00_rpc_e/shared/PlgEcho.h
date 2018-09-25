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
template<typename T> class PlgA : public ThreadedPlugin{
  public:
   MReceiver<MyFrmPtr> in; //pull from Pool
   MSender  <T>        out;
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
         T m;
         MyFrmPtr frmIn;
         assert(OK == in.Receive(&frmIn));
         #if (PTR_IN_MSG) //---------------------------
           m.frm  = frmIn;
           m.data       = s + i;
           m.frm->seqNo = s + i*2;
           out.Send(&m);
         #else //--------------------------------------
           UNUSED(m);
           frmIn->seqNo = s + i*2;
           out.Send(&frmIn);
         #endif //-------------------------------------
       }
       return NULL;
   }
};

//#########################################################
template<typename T> class PlgB : public ThreadedPlugin{
  public:
   SReceiver<T> in;
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
        T m;
        if(OK == in.Receive(&m))
        {
        #if (PTR_IN_MSG) //---------------------------------
          if(          m.data       != s + i  ) err++;
          if((uint32_t)m.frm->seqNo != s + i*2) err++;
          if(delay) rtems_task_wake_after(delay);
          #if (PRINT)
            printf("DAT_:%.8s:%lx\n", (char*)&name, m.data);
          #endif
        #else //--------------------------------------------
          if((uint32_t)m->seqNo     != s + i*2) err++;
          if(delay) rtems_task_wake_after(delay);
          #if (PRINT)
            printf("SEQ_:%.8s:%x\n", (char*)&name, m->seqNo);
           #endif
        #endif //-------------------------------------------
        }
      }
      return NULL;
   }
};

#endif