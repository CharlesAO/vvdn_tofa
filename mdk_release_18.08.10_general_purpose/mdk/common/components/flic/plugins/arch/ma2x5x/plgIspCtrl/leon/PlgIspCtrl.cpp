///
/// @file      PlgIspCtrl.cpp
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application for PlgIspCtrl.
///            Platform(s) supported : ma2x5x
///

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>


#include "PlgIspCtrl.h"
#include <ImgFrame.h>
#include "IspCommon.h"


PlgIspCtrl PlgIspCtrl::inst;

//###################################################################################


//#############################################
void PlgIspCtrl::Create(void)
{
   //Create receivers:
    inCtrlResponse.Create(16);
    Add(&inCtrlResponse);
    Add(&outOutCmd);
    for(int i = 0; i < IPIPE_MAX_SOURCES_ALLOWED; i++) {
        Add(&outSrcCommand[i]);
    }
}

//#################################################################################################
//########################## IPIPE.H interface implementation #####################################
//#################################################################################################
icCtrl *icSetup(int32_t appID,    uint32_t ddrStart,  uint32_t ddrSize) {
    UNUSED(ddrStart);
    UNUSED(ddrSize);
    PlgIspCtrl *g = PlgIspCtrl::instance();
    memset((void*)&g->ctrl, 0, sizeof(g->ctrl));
    g->ctrl.appId = appID;
    return &g->ctrl;
}

// TODO: implement it
int32_t icTeardown              (icCtrl *ctrl) {
    //PlgIspCtrl *g = PlgIspCtrl::instance();
    //g->pl->Stop();
    UNUSED(ctrl);
    return 0;
}

int32_t icLockZSL               (icCtrl *ctrl, icSourceInstance source, uint64_t frameSel, icLockZSLFlags flags) {
    UNUSED(ctrl);
    PlgIspCtrl *g = PlgIspCtrl::instance();
    IspCmdMsg cmdIspMsg;
    cmdIspMsg.cmdMsg.u.lockZSL.sourceInstance      = source;
    cmdIspMsg.cmdMsg.u.lockZSL.frameSel            = frameSel;
    cmdIspMsg.cmdMsg.u.lockZSL.flags               = flags;
    cmdIspMsg.cmdMsg.ctrl                          = IC_EVENT_TYPE_LOCK_ZSL; //type of event
    g->outSrcCommand[source].Send(&cmdIspMsg);
    return 0;
}

int32_t icTriggerCapture        (icCtrl *ctrl, icSourceInstance source, void * buff,void *cfg, icCaptureFlags flags) {
    UNUSED(ctrl);
    PlgIspCtrl *g = PlgIspCtrl::instance();
    IspCmdMsg cmdIspMsg;
    cmdIspMsg.cmdMsg.u.capture.sourceInstance      = source;
    cmdIspMsg.cmdMsg.u.capture.pConfigStruct       = cfg;
    cmdIspMsg.cmdMsg.u.capture.buff                = buff;
    cmdIspMsg.cmdMsg.u.capture.flags               = flags;
    cmdIspMsg.cmdMsg.ctrl                          = IC_EVENT_TYPE_CAPTURE; //type of event
    g->outSrcCommand[source].Send(&cmdIspMsg);
    return 0;
}

void icSetupSource (icCtrl *ctrl, icSourceInstance srcIdx, icSourceSetup *setup) {
    UNUSED(ctrl);
    PlgIspCtrl *g = PlgIspCtrl::instance();
    IspCmdMsg cmdIspMsg;
    memcpy(&g->ctrl.source[srcIdx].sourceSetup, setup, sizeof(icSourceSetup));
    cmdIspMsg.cmdMsg.ctrl = IC_EVENT_TYPE_SETUP_SOURCE;
    cmdIspMsg.cmdMsg.u.setupSourceMsg.sourceInstance = srcIdx;
    cmdIspMsg.cmdMsg.u.setupSourceMsg.srcSetup = &g->ctrl.source[srcIdx].sourceSetup;
    g->outSrcCommand[srcIdx].Send(&cmdIspMsg);
}


void    icConfigureSource       (icCtrl *ctrl, icSourceInstance srcIdx, icSourceConfig *config) {
    UNUSED(ctrl);
    PlgIspCtrl *g = PlgIspCtrl::instance();
    IspCmdMsg cmdIspMsg;
    memcpy(&g->ctrl.source[srcIdx].sourceConfig, config, sizeof(icSourceConfig));
    cmdIspMsg.cmdMsg.ctrl = IC_EVENT_TYPE_CONFIG_SOURCE;
    cmdIspMsg.cmdMsg.u.configureSource.sourceInstance = srcIdx;
    cmdIspMsg.cmdMsg.u.configureSource.srcCfg = &g->ctrl.source[srcIdx].sourceConfig;
    g->outSrcCommand[srcIdx].Send(&cmdIspMsg);
}

void    icSetupSourcesCommit    (icCtrl *ctrl) {
    UNUSED(ctrl);
}

int32_t icStartSource           (icCtrl *ctrl, icSourceInstance source) {
    UNUSED(ctrl);
    PlgIspCtrl *g = PlgIspCtrl::instance();
    IspCmdMsg cmdIspMsg;
    cmdIspMsg.cmdMsg.ctrl = IC_EVENT_TYPE_START_SOURCE;
    cmdIspMsg.cmdMsg.u.start.sourceInstance = source;
    g->outSrcCommand[source].Send(&cmdIspMsg);
    return 0;
}

int32_t icStopSource            (icCtrl *ctrl, icSourceInstance source) {
    UNUSED(ctrl);
    PlgIspCtrl *g = PlgIspCtrl::instance();
    IspCmdMsg cmdIspMsg;
    cmdIspMsg.cmdMsg.ctrl = IC_EVENT_TYPE_STOP_SOURCE;
    cmdIspMsg.cmdMsg.u.stop.sourceInstance = source;
    cmdIspMsg.cmdMsg.u.stop.stopImmediate = 0;
    g->outSrcCommand[source].Send(&cmdIspMsg);
    UNUSED(source);
    return (0);
}

int32_t icConfigureIsp          (icCtrl *ctrl, int32_t ispIdx, void *cfg) {
    UNUSED(ctrl);
    PlgIspCtrl *g = PlgIspCtrl::instance();
    IspCmdMsg cmdIspMsg;
    cmdIspMsg.cmdMsg.ctrl = IC_EVENT_TYPE_CONFIG_ISP;
    cmdIspMsg.cmdMsg.u.configureIsp.ispInstance = ispIdx;
    cmdIspMsg.cmdMsg.u.configureIsp.pConfigStruct = (uint32_t)cfg;
    cmdIspMsg.cmdMsg.u.configureIsp.seqNo = 0; // TODO: review what is use this for
    g->outSrcCommand[ispIdx].Send(&cmdIspMsg);
    return (0);
}

void    icDataReceived          (icCtrl *ctrl, FrameT   *dataBufStruct) {
    UNUSED(ctrl);
    PlgIspCtrl *g = PlgIspCtrl::instance();
    IspCmdMsg cmdOutMsg;
    cmdOutMsg.cmdMsg.ctrl = IC_EVENT_TYPE_OUTPUT_DATA_RECEIVED;
    cmdOutMsg.cmdMsg.u.dataWasSent.dataBufStruct = dataBufStruct;
    g->outOutCmd.Send(&cmdOutMsg);

}

//#############################################
//NOTES:
//1) Guzzi manages config frame pool => no need for a config pool @ wrapper level
//2) Config gets released by Guzzi when it receives IC_EVENT_TYPE_ISP_END
//   => IcIspConfig doesn't need to inherit Message base class ???

// this is interface Guzzi - Ipipe thread.
// In order to allow develop stand alone functionality, another thread will be internally created and will control

// sensors and will generate configs.


int32_t icGetEvent(icCtrl *ctrl, icEvent *ev) {
    UNUSED(ctrl);
    int   err = 0;
    PlgIspCtrl *g = PlgIspCtrl::instance();
    err = g->inCtrlResponse.Receive(&g->cmdIspMsgIn);
    if(err) {
        return(1);
    }
    else {
        memcpy((void*)ev, (void*)&g->cmdIspMsgIn.cmdMsg, sizeof (*ev));
        return(0);
    }
}
void * PlgIspCtrl::threadFunc(void *)
{
    rtems_object_set_name(RTEMS_SELF, "PlgIspCtrl");
    //regi
    //PlgIspCtrl *g = PlgIspCtrl::instance();

    while(Alive())
    {

        //inCtrlResponse.Receive(&g->cmdIspMsg,sizeof(g->cmdIspMsg));

        sleep(5);

        // startCam
        // wait cam to start
        //
        //
      //1) Some latency in generating the config ...
        //rtems_task_wake_after(FRAME_TIME);
      //2) Send config

    }

    return NULL;
}
