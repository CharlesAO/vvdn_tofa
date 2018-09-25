///
/// @file
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Dummy source plugin
///

#include "PlgSrc.h"
#include <ImgFrame.h>
#include "cfg.h"

//#############################################
void PlgSrc::Create()
{
    Add(&inO); Add(&out);
}

//#############################################
void * PlgSrc::threadFunc(void *)
{
    for(int i=0; i<T_FRM; i++)
    {
        ImgFramePtr fMsg;
      //Get a frame from frame pool
        if(OK == inO.Receive(&fMsg))
        {
           rtems_task_wake_after(50);
           out.Send(&fMsg);
        }
    }

    return NULL;
}
