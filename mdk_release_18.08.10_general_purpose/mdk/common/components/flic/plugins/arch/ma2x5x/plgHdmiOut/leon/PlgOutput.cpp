///
/// @file      PlgOutput.cpp
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application for PlgOutput.
///            Platform(s) supported : ma2x5x
///

#include <stdio.h>
#include <Flic.h>
#include "PlgOutput.h"
#include <ImgFrame.h>
#include "PlgHdmiOut.h"
#include "MsgBuf.h"
#include "BehQueueOvr.h"

//#############################################
void PlgOutput::Create()
{
    lcdHdmiSendInit();
   //Create receivers:
    in.Create(8, QueueCmdOvr<ImgFramePtr>::Inst());
    Add(&in);
    frmCnt = 0;
}

//TBD: PlgOutput::Stop , Delete

//#############################################
void * PlgOutput::threadFunc(void *)
{
    while(Alive())
    {
        ImgFramePtr iFrm;
        int err = in.Receive(&iFrm);

        if(!err)
        {
         //printf("0x%lx\n", (uint32_t)iFrm.base);
         //lcdHdmiSendFrame(&iFrm, (uint32_t)((ImgFrame*)(iFrm.ptr))->userData);
           lcdHdmiSendFrame(&iFrm, iFrm->instNo);
           frmCnt++;
         //In MIPI/LCD EOF-irq, release the frame
        }
    }
    return NULL;
}
