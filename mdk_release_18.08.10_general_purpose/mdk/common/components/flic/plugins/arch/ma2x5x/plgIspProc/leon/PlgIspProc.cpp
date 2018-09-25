///
/// @file      PlgIspProc.cpp
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application for PlgIspProc.
///            Platform(s) supported : ma2x5x
///

#include "PlgIspProc.h"
#include <ImgFrame.h>
#include "IspCommonOpipe.h"
#include "IspMsg.h"

//#############################################
void PlgIspProc::Create(uint32_t insId, uint32_t downscale2xOn) {
    //Create receivers
    inI.Create(8); //in frames
  //inO.Create(8); //out frames
    Add(&inI );Add(&inO);
    Add(&outF);Add(&outE);
    id = insId;
    seqOutNr = 0;
    plgOpipe.down2xOn = downscale2xOn;
    plgOpipe.IspBuildPipe(&plgOpipe.opF);
}

void PlgIspProc::Delete() {
    IPlugin::Delete();
    plgOpipe.IspDestroy();
}

//#############################################
void PlgIspProc::Stop(){
    IPlugin::Stop();
}

uint32_t ispDD[2] = {0,0};

//#############################################
void * PlgIspProc::threadFunc(void *) {
    rtems_object_set_name(RTEMS_SELF, "PlgIspProc");
    while(Alive()) {
        int       err = 0;
        IspInputMsg  fInp;
        ImgFramePtr  fOut;
        //1) Wait on inputs to be available
        err += inI.Receive(&fInp);
        err += inO.Receive(&fOut);
        //2) Process
        if(!err) {
            ImgFrame *iImg = fInp.img.ptr; //TBD: use get()
            ImgFrame *oImg = fOut.ptr; //TBD: use get()
            // frame for isp need to come with ispConfig inside received related with source sof
            assert(iImg->userData);
            ispDD[0]++;
            sendIspStartEvent(fInp.ispCfg->userData, seqOutNr);
            plgOpipe.IspStart(&plgOpipe.opF, iImg, oImg, fInp.ispCfg);
            plgOpipe.IspWait ();
            ispDD[1]++;
            //3) Done
            // send OutE message, to inform that isp config was consumed and statistics was generated
            sendIspEndEvent(fInp.ispCfg->userData, seqOutNr);
            seqOutNr++;
            oImg->instNo = id; // populate frame user data with isp id information
            //TODO: replace it, Guzzi unexpected behavior, disable output for some reason
            if (IC_PIPECTL_MIPI_ENABLE & (fInp.ispCfg->pipeControl)) {
                outF.Send(&fOut); //Forward output
            } else {
                // this is not an error, is a functionality,
                //I not send this as error, but can be add for informations, and to know that firs frame will not be send
                // for example
                //sendIspErrorEvent(fInp.ispCfg->userData, seqOutNr);
                //Nothig, release is made automaticaly //assert(0);
            }
            fInp.img = NULL;
        }
    }
    return NULL;
}

void PlgIspProc::sendIspEndEvent(void *userData, uint32_t seqNr) {
    IspCmdMsg outCmdMsg;
    outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_ISP_END;
    outCmdMsg.cmdMsg.u.ispEvent.ispInstance = id;
    outCmdMsg.cmdMsg.u.ispEvent.seqNo = seqNr;
    outCmdMsg.cmdMsg.u.ispEvent.userData = userData;
    outE.Send(&outCmdMsg);
}

void PlgIspProc::sendIspStartEvent(void *userData, uint32_t seqNr) {
    IspCmdMsg outCmdMsg;
    outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_ISP_START;
    outCmdMsg.cmdMsg.u.ispEvent.ispInstance = id;
    outCmdMsg.cmdMsg.u.ispEvent.seqNo = seqNr;
    outCmdMsg.cmdMsg.u.ispEvent.userData = userData;
    outE.Send(&outCmdMsg);
}

void PlgIspProc::sendIspErrorEvent(void *userData, uint32_t seqNr) {
    IspCmdMsg outCmdMsg;
    UNUSED(seqNr);
    outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_ERROR;
    outCmdMsg.cmdMsg.u.error.sourceInstance = id;
    outCmdMsg.cmdMsg.u.error.errorNo = IC_ERROR_ISP_OUT_SKIP_3A_REQUEST;
    outCmdMsg.cmdMsg.u.error.severity = IC_SEVERITY_NORMAL;
    outCmdMsg.cmdMsg.u.error.userData = userData;
    outE.Send(&outCmdMsg);
}

