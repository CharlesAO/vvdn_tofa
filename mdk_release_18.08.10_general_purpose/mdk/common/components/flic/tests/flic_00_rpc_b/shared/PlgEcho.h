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

//##################################################
template<typename T> class PlgA : public ThreadedPlugin{
  public:
   MSender<T> out;
   uint32_t num, s;

   void Create(uint32_t n, uint32_t seed){
      num = n;
      s   = seed;
      Add(&out, ".out");
   }
   virtual void *threadFunc(void *)
   {
       T m;
       for(uint32_t i=0; i<num; i++)
       {
         m.data = s + i;
         out.Send(&m);
       }
       return NULL;
   }
};

//##################################################
template<typename T> class PlgB : public ThreadedPlugin{
  public:
   SReceiver<T> in;
   uint32_t num, s, delay, err;

   void Create(uint32_t n, uint32_t seed, uint32_t dly){
      num = n;
      err = 0;
      delay = dly;
      s  = seed;
      in.Create(2);
      Add(&in, ".in");
   }
   void *threadFunc(void *){
      T m;
      for(uint32_t i=0; i<num; i++)
      {
        if(OK == in.Receive(&m)){
          if(m.data != s + i) err++;
          if(delay) rtems_task_wake_after(delay);
         #if (PRINT)
          printf("%.8s:%lx\n", (char*)&name, m.data);
         #endif
        }
      }
      return NULL;
   }
};

#endif
