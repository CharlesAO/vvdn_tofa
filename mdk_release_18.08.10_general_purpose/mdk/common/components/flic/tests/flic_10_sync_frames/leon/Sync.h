///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test sync plugin
///

#ifndef __SYNC_H__
#define __SYNC_H__

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include "myTypes.h"

//##########################################################
template<typename T1, typename T2, typename T3> class PlgSync : public ThreadedPlugin
{
   public:
   SReceiver<PoPtr<T1>> inA;
   SReceiver<PoPtr<T2>> inB;
   MSender  <T3>        out;

   //=========================================
   void Create(){
      inA.Create(4);
      inB.Create(4);
      Add(&inA, ".inA");
      Add(&inB, ".inB");
      Add(&out, ".out");
   }

   //=========================================
   void * threadFunc(void *)
   {
       uint32_t  err, run = 0;

       while(Alive())
       {
           PoPtr<T1> fInA;
           PoPtr<T2> fInB;

           //1) Wait for both inputs
           err  = 0;
           err += inA.Receive(&fInA);
           err += inB.Receive(&fInB);
           if(!err)
           {
               if(1){ //say we check that TSs are close ehough
                    printf("Got L/R frame (run %ld)\n", run++);
                    T3 m;
                    m.fCoco   = fInA;
                    m.fBobo   = fInB;
                    m.command = TEST_CMD;

                  //3)forward results
                    out.Send(&m);
               }
               else{
                  //frames TSs are too different, just discard
               }
           }
       }//fInA, fInB get released here.
       return NULL;
   }
};

#endif
