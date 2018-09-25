///
/// @file      PlgIspCtrl.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     PlgIspCtrl header.
///

#ifndef __PLG_ISPCTRL_H__
#define __PLG_ISPCTRL_H__

#include <Flic.h>
#include <Receiver.h>
#include <Sender.h>
#include "ipipe.h"
#include "IspCommon.h"
#include "IspMsg.h"


#define MAX_ISP_INSTANCES  8
class PlgIspCtrl : public ThreadedPlugin
{
private:
    static PlgIspCtrl inst;
    PlgIspCtrl() {

    }
  public:
    MSender  <IspCmdMsg> outSrcCommand[IPIPE_MAX_SOURCES_ALLOWED];  //send commands to source
    MSender  <IspCmdMsg> outOutCmd;
    SReceiver<IspCmdMsg> inCtrlResponse;
  

    IspCmdMsg cmdIspMsgIn;

    // unused in this new approach bu keep in order to keep interface. TODO:delete it
    icCtrl ctrl;

    static PlgIspCtrl *instance()
    {
        return &inst;
    }
    void    *threadFunc(void *);
    void     Create();
};

#endif //__PLG_ISPCTRL_H__
