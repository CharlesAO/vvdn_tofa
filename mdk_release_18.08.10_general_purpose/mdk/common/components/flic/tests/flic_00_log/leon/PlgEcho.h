///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugins
///

#ifndef __PLG_ECHO_H__
#define __PLG_ECHO_H__

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include "Data.h"
#include <UnitTestApi.h>

#define N_MSG 4
#define DATA(i) (i*100)

//#####################################
class PlgSend : public ThreadedPlugin{
  public:
   MSender<MyMsg> out;
   void Create(){
       Add(&out, ".out");
   }
   virtual void *threadFunc(void *)
   {
       MyMsg m;
       for(uint32_t i=0; i< N_MSG; i++){
          ///*LOG*/if(i & 0x1)
          ///*LOG*/  log->Print("<Odd sent no %d>\n", i);
          m.data = DATA(i);
          out.Send(&m);
       }
       return NULL;
   }
};

//#####################################
class PlgRecv : public ThreadedPlugin{
  public:
   SReceiver<MyMsg> in;
   void Create(){
      in.Create(2);
      Add(&in);
      in.name = Flic::Name(".in");
   }
   void *threadFunc(void *){
      MyMsg m;
      for(uint32_t i=0; i< N_MSG; i++)
        if(OK == in.Receive(&m)){
          // /*LOG*/if((i & 0x1)==0)
          // /*LOG*/  log->Print("{even Recv no %d}\n", i);
          unitTestAssert(m.data == DATA(i));
        }
      return NULL;
   }
};

#endif