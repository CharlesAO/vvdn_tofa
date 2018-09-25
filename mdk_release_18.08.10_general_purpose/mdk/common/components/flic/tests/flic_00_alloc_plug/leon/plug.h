///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugins
///

#ifndef __PLUG_H__
#define __PLUG_H__

#include <Sender.h>
#include <Receiver.h>
#include <semaphore.h>

struct Cmd{
    uint32_t data;
};

class MyPlugin : public IPlugin
{
  public:
   SReceiver<Cmd> r1;
   SReceiver<Cmd> r2;
   SReceiver<Cmd> r3;

   MSender  <Cmd> s1;
   MSender  <Cmd> s2;

  //Additional RTEMS resources
   sem_t     aSem;

  //Methods:
   void Create()
   {
      r1.Create(8);
      r2.Create(8);
      r3.Create(8);
      assert(0 == sem_init(&aSem, 0, 0));

    //Book IOs to rely on automatic Destroy/Delete
    //(also needed for indexed linking)
      Add(&r1);Add(&r2);Add(&r3);
      Add(&s1);Add(&s2);

    //assert((nRecs + nSndr) == (3+2));
   }

   void Stop(){
       IPlugin::Stop();//kill all booked IOs
     //Kill additional resources
       sem_destroy(&aSem);
   }

};

#endif