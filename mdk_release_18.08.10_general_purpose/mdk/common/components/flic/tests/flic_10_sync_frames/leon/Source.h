///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Test plugin
///

#ifndef __SOURCE_H__
#define __SOURCE_H__

#include <Flic.h>
#include <Receiver.h>
#include <Sender.h>

template<typename T> class PlgSrc : public ThreadedPlugin
{
  public:
    MReceiver<PoPtr<T>> inO;  //frame from frame-pool
    MSender  <PoPtr<T>> out;  //send frame to consumer
    uint32_t  val;  //memsets frames with val

    void Create(int v) {
        val = v;
        Add(&inO);
        Add(&out);
    }

    void *threadFunc(void *)
    {
        int err;
        while(Alive())
        {
            PoPtr<T> fMsg;
          //pushing frames at a certain rate
            rtems_task_wake_after(300);

          //get a frame from frame pool
            err = inO.Receive(&fMsg);
            if(!err)
            {
              //TBD: dynamic_cast here
              //T *oFrm = (T *)fMsg.frm;

              //Say camera writes the frame
                memset(fMsg.ptr->base, val, fMsg.ptr->size);

              //push to consumers
                out.Send(&fMsg);
            }
        }
        return NULL;
    }
};

#endif