///
/// @file      PlgOutItf.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     PlgOutItf header.
///

#ifndef __PLG_OUTPUT_ITF_H__
#define __PLG_OUTPUT_ITF_H__

#include <Flic.h>
#include <Sender.h>
#include <Receiver.h>
#include <ImgFrame.h>
#include <FrameTypes.h>
#include "IspMsg.h"

class PlgOutItf : public ThreadedPlugin
{
  public:
    SReceiver <ImgFramePtr>in;
    MSender   <IspCmdMsg>outCmd;
    SReceiver <IspCmdMsg>inCmd;
    SemSync   sync;

    void  *threadFunc(void *);
    void   Create();
    void   Stop();

  private:
    static const int SendQMaxSize = 16;
    typedef struct {
        ImgFramePtr iFrm;
        FrameT      frm;
        uint32_t    frmQueStatus;
    } SendQ;
    SendQ sendQ[SendQMaxSize];
    void resetQ(void);
    SendQ* getAFreeElement(void);
    SendQ* releaseElmFromQue(FrameT *frm);
    ImgFramePtr     nullObj;   //for release
    ImgFrmFmt frmConvert[NONE+1];
    void convertFrmLutInit();
};

#endif //__PLG_OUTPUT_ITF_H__
