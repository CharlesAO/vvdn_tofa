///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dummy output
///

#ifndef __PLG_OUT_H__
#define __PLG_OUT_H__

#include <Flic.h>
#include <Receiver.h>
#include "test.h"

template<class T> class PlgOut : public ThreadedPlugin
{
  public:
    SReceiver<PoPtr<T>> in;

    void Create(){
       in.Create(4);
       Add(&in);
    }

    void *threadFunc(void *)
    {
        int err;
        while(Alive())
        {
            PoPtr<T> fMsg;
            err = in.Receive(&fMsg);
            if(!err){
              T *f = fMsg.ptr;
              printf("Out: age = %d, gender = %d\n", f->age, f->gender);
              unitTestAssert(f->age    == TEST_AGE);
              unitTestAssert(f->gender == TEST_GEN);
            }
        }
        return NULL;
    }
};

#endif