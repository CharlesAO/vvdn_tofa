///
/// @file      PlgIspProc.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     PlgIspProc header.
///

#ifndef __PLG_ISP_PROC_H__
#define __PLG_ISP_PROC_H__

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include "IspCommon.h"
#include <ImgFrame.h>
#include <IspMsg.h>

class PlgIspProc : public ThreadedPlugin
{
  public:
    static pthread_t waitingThr;
    SReceiver<ImgFramePtr> inI;  //in [Input FrameMsg ]
    MReceiver<ImgFramePtr> inO;  //in [Output frame]
    MSender  <ImgFramePtr> outF; //out FrameMsg
    MSender  <IspCmdMsg>   outE; //outEvents

    uint32_t id;
    virtual void *threadFunc(void *);
    virtual void Create(uint32_t insId);
    virtual void Stop();
    virtual void Delete();
    
    //sippIsp specific
    IspPipeline ispPipe; //sipp filters
    PipeConfig  iCfg; //guzzi->sipp params
    
  private:
    void sendIspEndEvent(void *userData);
};

#endif
