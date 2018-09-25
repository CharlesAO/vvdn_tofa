///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugin (buffer)
///

#include <Flic.h>
#include <Receiver.h>
#include <Sender.h>

#ifndef __PLG_BUF_H__
#define __PLG_BUF_H__

class PlgBuf : public ThreadedPlugin{
  public:
   SReceiver<int> in;
   MSender  <int> out;

   void Create()
   {
     in.Create(4);
     Add(&in);
     Add(&out);
   }

   void *threadFunc(void *)
   {
     while(Alive())
     {
        int val;
        if(OK == in.Receive(&val)){
           out.Send(&val);
        }
     }
     return NULL;
   }
};

#endif