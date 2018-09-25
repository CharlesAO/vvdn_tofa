///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dummy source
///

#ifndef __SOURCE_H__
#define __SOURCE_H__

#include <Flic.h>
#include <Receiver.h>
#include <Sender.h>

template <typename T> class PlgSrc : public ThreadedPlugin
{
  public:
    MReceiver<PoPtr<T>> inO;  //frame from frame-pool
    MSender  <PoPtr<T>> out;  //send frame to consumer
    uint32_t  roiW;
    uint32_t  roiH;

    void Create()
    {
        Add(&inO);
        Add(&out);
    }

    void *threadFunc(void *)
    {
        int err;

        while(Alive())
        {
            PoPtr<T> fMsg;
          //pushing frames in the pipe at a certain rate ... (e.g. 30fps)
            sleep(1);

          //get a frame from frame pool
            err = inO.Receive(&fMsg);
            if(!err){
              T *oFrm = fMsg.ptr;
              oFrm->fb.spec.width  = roiW;
              oFrm->fb.spec.height = roiH;

            //push
              out.Send(&fMsg);
            }
        }
        return NULL;
    }
};

#endif