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
#include "OpipeApps.h"
#include <ImgFrame.h>
#include <IspMsg.h>

#include "PlgIspProcOpipe.h"

class PlgIspProc : public ThreadedPlugin
{
  public:
    SReceiver<IspInputMsg>  inI;  //in [Input ImgFramePtr ]
    MReceiver<ImgFramePtr>  inO;  //in [Output frame]
    MSender  <ImgFramePtr>  outF; //out ImgFramePtr
    MSender  <IspCmdMsg> outE; //outEvents

    uint32_t  id;
    virtual void *threadFunc(void *);
    virtual void Create(uint32_t insId, uint32_t downscale2xOn = 0);
    virtual void Stop();
    virtual void Delete();
    PlgIspProcOpipe plgOpipe;
  private:
    uint32_t seqOutNr;
    void sendIspEndEvent(void *userData, uint32_t seqNr);
    void sendIspStartEvent(void *userData, uint32_t seqNr);
    void sendIspErrorEvent(void *userData, uint32_t seqNr);
};

#endif
