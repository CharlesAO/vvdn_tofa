///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugin (Sink)
///

#ifndef __PLG_SNK_H__
#define __PLG_SNK_H__

#include <Flic.h>
#include <Receiver.h>
#include <UnitTestApi.h>

class PlgSnk : public ThreadedPlugin{
  public:
   SReceiver<int> in;

   void Create(){ in.Create(4); Add(&in); }

   void *threadFunc(void *)
   {
     int exp = SEED;
     while(Alive())
     {
        int val;
        if(OK == in.Receive(&val)){
         unitTestAssert(val == exp);
         exp++;
        }
     }
     //make sure it ran a few iterations...
     unitTestAssert(exp > SEED);
     return NULL;
   }
};

#endif