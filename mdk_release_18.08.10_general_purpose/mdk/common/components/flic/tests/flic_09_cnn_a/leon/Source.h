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
#include "CnnFrame.h"

//NOTE:
//  This would be an ISP, or a Human detector forwarding ROIs ?

class PlgSrc : public ThreadedPlugin
{
  public:
    MReceiver<CnnFramePtr> inO;  //frame from frame-pool
    MSender  <CnnFramePtr> out;  //send frame to consumer
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
            CnnFramePtr fMsg;
          //pushing frames in the pipe at a certain rate ... (e.g. 30fps)
            sleep(1);

          //get a frame from frame pool
            err = inO.Receive(&fMsg);
            if(!err){
              CnnFrame *oFrm = fMsg.ptr;
              oFrm->fb.spec.width  = roiW;
              oFrm->fb.spec.height = roiH;
              oFrm->descPos     = 0;

            //push & release
              out.Send(&fMsg);
            }
        }
        return NULL;
    }
};

#endif