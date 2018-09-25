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

#define SEED 0x1A2B0000

//##################################################
template<typename T> class PlgA : public ThreadedPlugin{
  public:
   MSender<T> out;
   uint32_t num;
   void Create(uint32_t n){
      num = n;
      Add(&out, ".out");
   }
   virtual void *threadFunc(void *)
   {
       for(uint32_t i=0; i<num; i++)
       {
         T m;
         m.data = SEED + i;
         out.Send(&m);
       }
       return NULL;
   }
};

//##################################################
template<typename T> class PlgC : public ThreadedPlugin{
  public:
   SReceiver<T> in;
   uint32_t num, err;
   void Create(uint32_t n){
      num = n;
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
          if(m.data != SEED + i) err++;
          #if (PRINT)
           printf("%.8s:%ld\n", (char*)&name, m.data);
          #endif
        }
      }
      return NULL;
   }
};

#endif
