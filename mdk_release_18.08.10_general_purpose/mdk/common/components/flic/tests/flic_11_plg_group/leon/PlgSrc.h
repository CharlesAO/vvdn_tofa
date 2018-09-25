///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugin (Source)
///

#include <Flic.h>
#include <Sender.h>

#ifndef __PLG_SRC_H__
#define __PLG_SRC_H__

class PlgSrc : public ThreadedPlugin{
  public:
   MSender<int> out;

   void Create(){ Add(&out, ".out"); }

   void *threadFunc(void *)
   {
     int val = SEED;
     while(Alive())
     {
         rtems_task_wake_after(50);
         out.Send(&val);
         val++;
     }
     return NULL;
   }
};

#endif