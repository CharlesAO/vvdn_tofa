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
   #if (LOG)
   uint32_t buff[N_MSG];
   #endif
   void Create(uint32_t n){
      num = n;
      Add(&out, ".out");
      #if (LOG)
       memset(buff, 0x88, sizeof(buff));
      #endif
   }
   virtual void *threadFunc(void *)
   {
       for(uint32_t i=0; i<num; i++)
       {
         T m;
         m.data = SEED + i;
         #if (LOG)
         buff[i] = m.data;
         #endif
         out.Send(&m);
       }
       return NULL;
   }
};

//##################################################
template<typename T> class PlgB : public ThreadedPlugin{
  public:
   SReceiver<T> in;
   MSender  <T> out;
   #if (LOG)
   uint32_t buff[N_MSG];
   #endif
   uint32_t num;

   void Create(uint32_t n){
      num = n;
      in.Create(2);
      Add(&in);
      Add(&out);
      #if (LOG)
       memset(buff, 0x77, sizeof(buff));
      #endif
   }
   void *threadFunc(void *){
      for(uint32_t i=0; i<num; i++)
      {
        T m;
        assert(OK == in.Receive(&m));
        assert(m.data == SEED + i);
        #if (LOG)
        buff[i] = m.data;
        #endif
        assert(OK == out.Send  (&m));
      }
      return NULL;
   }
};

//##################################################
template<typename T> class PlgC : public ThreadedPlugin{
  public:
   SReceiver<T> in;
   uint32_t num, err;
   #if (LOG)
   uint32_t buff[N_MSG];
   #endif
   void Create(uint32_t n){
      num = n;
      err = 0;
      in.Create(2);
      Add(&in, ".in");
      #if (LOG)
       memset(buff, 0x66, sizeof(buff));
      #endif
   }
   void *threadFunc(void *){
      for(uint32_t i=0; i<num; i++)
      {
        T m;
        if(OK == in.Receive(&m)){
          #if (LOG)
          buff[i] = m.data;
          #endif
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
