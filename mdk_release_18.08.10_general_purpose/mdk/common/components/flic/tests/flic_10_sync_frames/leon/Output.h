///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugin
///

#ifndef __PLG_OUT_H__
#define __PLG_OUT_H__

#include <Flic.h>
#include <Receiver.h>
#include <UnitTestApi.h>
#include "myTypes.h"

template<typename T> class PlgOut : public ThreadedPlugin
{
  public:
    SReceiver<T> in;

    void Create(){
       in.Create(4);
       Add(&in);
    }

    void *threadFunc(void *)
    {
        int err;
        T m;

        while(Alive())
        {
            err = in.Receive(&m);
            if(!err){
              unitTestAssert(m.command == TEST_CMD);
              unitTestAssert(((uint8_t*)m.fCoco.ptr->base)[0] == VAL_COCO);
              unitTestAssert(((uint8_t*)m.fBobo.ptr->base)[0] == VAL_BOBO);

             //must do a manual release...
             //consumer might use fCoco in a TNF algo and release later...
              m.fCoco = nullptr;
              m.fBobo = nullptr;
            }
        }
        return NULL;
    }
};

#endif