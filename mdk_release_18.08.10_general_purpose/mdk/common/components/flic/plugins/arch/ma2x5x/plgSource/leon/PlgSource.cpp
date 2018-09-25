///
/// @file      PlgSource.cpp
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Source file for common HGL functions.
///            Platform(s) supported : ma2x5x
///

// 1: Includes
// ----------------------------------------------------------------------------
#include <stdio.h>
#include "PlgSource.h"
#include <ImgFrame.h>
#include <DrvGpio.h>
#include <DrvMipi.h>
#include <DrvMss.h>
#include "OpipeInternals.h"
#include "sched.h"
#include "DrvTimer.h"

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define HS_MIPI_CTRL_OFSET_ 0x100
#ifndef DESTROY_MESSAGE_SRC
#define DESTROY_MESSAGE_SRC (-1)
#endif
// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern void plgSrcCreateRxOpipe(PlgSource *obj, icSourceConfig  *sCfg, uint32_t outFmt);
extern void plgSrcPurgeCBufsMem(void);
// 4: Static Local Data
// ----------------------------------------------------------------------------
int ddd[8] = {0, 0, 0, 0, 0, 0, 0, 0};
// 5: Static Function Prototypes
// ----------------------------------------------------------------------------


// 6: Functions Implementation
// ----------------------------------------------------------------------------
//#################################################################################################
void PlgSource::Create(icSourceInstance instId)
{
    inCommand.Create(8);

    Add(&inO); Add(&inCommand);
    Add(&out); Add(&outCommand);
    Add(&outStl);

    plgStatus = PLG_STATS_IDLE;
    status    = PLG_SOURCE_CREATED;
    srcId     = instId;
    //no implicit callbacks
    eofEvent = &eofEventSrc;
    sofEvent = &sofEventSrc;
    hitEvent = &lnHitEventSrc;
    lineHitNo = 0xFFFFFFFF;
    hitEvent = NULL;
    srcCfg   = NULL;
    frameCnt = 0;
    strIdx   = 0;
    nullObj = NULL;
    //PlgSourceCmxAlloc here...
}

void PlgSource::Delete() {
    IPlugin::Delete();
    plgSrcPurgeCBufsMem();
}
//#################################################################################################
void * PlgSource::threadFunc(void *) {
    rtems_object_set_name(RTEMS_SELF, "PlgSource");
    int   err = 0;
    IspCmdMsg inCmdMsg;
    IspCmdMsg outCmdMsg;
    threadOn = 0;

    // Block que zsl frames
    init();

    //
    err += inCommand.Receive(&inCmdMsg);
    if(err) goto cleanExit;
    assert(IC_EVENT_TYPE_SETUP_SOURCE == (inCmdMsg.cmdMsg.ctrl & IC_EVENT_CTRL_TYPE_MASK));
    assert(srcId == inCmdMsg.cmdMsg.u.setupSourceMsg.sourceInstance);
    assert(inCmdMsg.cmdMsg.u.setupSourceMsg.srcSetup != 0);
    srcSetup = inCmdMsg.cmdMsg.u.setupSourceMsg.srcSetup;

    nextIspCfg = NULL;
    // start Source involve a fix succession of messages if is not respect, an assert will be send
    // teardown can be call at any time, if special situation are meet,
    //but normally have to be call after stop
    while(Alive()) {
        // wait for config message have to be send 1 isp config before start the sensor
        err += inCommand.Receive(&inCmdMsg);
        if(err) goto cleanExit;
        assert(IC_EVENT_TYPE_CONFIG_ISP == (inCmdMsg.cmdMsg.ctrl & IC_EVENT_CTRL_TYPE_MASK));
        ispCfg = (icIspConfig*)inCmdMsg.cmdMsg.u.configureIsp.pConfigStruct;
        assert(ispCfg);
        //
        err += inCommand.Receive(&inCmdMsg);
        if(err) goto cleanExit;

        if(IC_EVENT_TYPE_CONFIG_SOURCE != (inCmdMsg.cmdMsg.ctrl & IC_EVENT_CTRL_TYPE_MASK)) {
            printf("E:%lu\n", (inCmdMsg.cmdMsg.ctrl & IC_EVENT_CTRL_TYPE_MASK));
        }
        assert(IC_EVENT_TYPE_CONFIG_SOURCE == (inCmdMsg.cmdMsg.ctrl & IC_EVENT_CTRL_TYPE_MASK));
        assert(inCmdMsg.cmdMsg.u.configureSource.srcCfg != 0);
        srcCfg = inCmdMsg.cmdMsg.u.configureSource.srcCfg;

        // start sensor receiver
        err = inCommand.Receive(&inCmdMsg);
        if(err) goto cleanExit;
        assert(IC_EVENT_TYPE_START_SOURCE == (inCmdMsg.cmdMsg.ctrl & IC_EVENT_CTRL_TYPE_MASK));

        //Source start sequence
        mipiRxData = srcCfg->mipiRxData;
        plgStatus  = PLG_STATS_RUNNING;

        //TBD: Wait for source config, start events
        err = inO.Receive(&outFrame); //TBD: TryReceive
        if(err) goto cleanExit;
        iImg = (ImgFrame*)outFrame.ptr;
        assert(iImg);
        iImg->userData = ispCfg;

        startSourceThread();

        // source ready
        outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_SOURCE_READY;
        outCmdMsg.cmdMsg.u.start.sourceInstance = srcId;
        err = outCommand.Send(&outCmdMsg);
        if(err) goto cleanExit;

        while((Alive()) && (PLG_STATS_RUNNING == plgStatus)) {
            // TODO: try receive, and take last isp config, and release the old ones by sending a
            // message possible other message to be send here, have to be take in consideration
            err = inCommand.Receive(&inCmdMsg);
            if(err) goto cleanExit;
            switch (inCmdMsg.cmdMsg.ctrl & IC_EVENT_CTRL_TYPE_MASK) {
            case IC_EVENT_TYPE_CONFIG_ISP:
                assert(this->srcId == inCmdMsg.cmdMsg.u.configureIsp.ispInstance);
                if(nextIspCfg != 0) {
                    // Send skip isp config message
                    outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_ERROR;
                    outCmdMsg.cmdMsg.u.error.sourceInstance = srcId;
                    outCmdMsg.cmdMsg.u.error.errorNo = IC_ERROR_RT_CFG_SKIPPED;
                    outCmdMsg.cmdMsg.u.error.severity = IC_SEVERITY_NORMAL;
                    outCmdMsg.cmdMsg.u.error.userData = nextIspCfg->userData;
                    err = outCommand.Send(&outCmdMsg);
                    if(err) goto cleanExit;
                }
                nextIspCfg = (icIspConfig*)inCmdMsg.cmdMsg.u.configureIsp.pConfigStruct;
                break;
            case IC_EVENT_TYPE_STOP_SOURCE:
                assert(this->srcId == inCmdMsg.cmdMsg.u.stop.sourceInstance);
                plgStatus = PLG_STATS_IDLE;
                // not necesarry, in PLG_SRC_TIMEOUT_SEC second source will automatically shut
                // down if sensor stop streaming
                //if(1 == inCmdMsg.cmdMsg.u.stop.stopImmediate) // not use,
                assert(OK == pthread_join(srcThread, NULL));
                threadOn = 0;
                // stop the source
                // send fini command was made info
                outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_SOURCE_STOPPED;
                outCmdMsg.cmdMsg.u.sourceEvent.sourceInstance = srcId;
                outCommand.Send(&outCmdMsg);
                stopSrc();
                nextIspCfg = NULL;
                lineHitNo = 0xFFFFFFFF;
                break;
            case IC_EVENT_TYPE_CONFIG_SOURCE_DYNAMIC:
                this->lineHitNo = inCmdMsg.cmdMsg.u.configureSourceDynamic.pDynConfig->notificationLine;
                break;
            case IC_EVENT_TYPE_LOCK_ZSL:
            {
                assert(this->srcId == inCmdMsg.cmdMsg.u.lockZSL.sourceInstance);
                uint64_t frameSel = inCmdMsg.cmdMsg.u.lockZSL.frameSel;
                uint32_t tsRel = inCmdMsg.cmdMsg.u.lockZSL.flags;
                ImgFramePtr* iim = queSearchLockFrame(frameSel, tsRel);
                if(iim) { // frame was locked
                    IspCmdMsg outCmdMsg;
                    outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_ZSL_LOCKED;
                    outCmdMsg.cmdMsg.u.buffLockedZSL.sourceInstance = srcId;
                    outCmdMsg.cmdMsg.u.buffLockedZSL.buffZsl = iim->ptr->base;
                    outCmdMsg.cmdMsg.u.buffLockedZSL.userData = (((icIspConfig *)(iim->ptr->userData))->userData);
                    outCmdMsg.cmdMsg.u.buffLockedZSL.seqNo = iim->ptr->seqNo;
                    outCmdMsg.cmdMsg.u.buffLockedZSL.ts = iim->ptr->ts;
                    outCommand.Send(&outCmdMsg);
                    if(err) goto cleanExit;
                }
                else { // error frame was not possible to be lock
                    outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_ERROR;
                    outCmdMsg.cmdMsg.u.error.sourceInstance = srcId;
                    outCmdMsg.cmdMsg.u.error.errorNo = IC_ERROR_NO_ZSL_BUFFS_AVAILABLE;
                    outCmdMsg.cmdMsg.u.error.severity = IC_SEVERITY_NORMAL;
                    outCmdMsg.cmdMsg.u.error.userData = NULL;
                    err = outCommand.Send(&outCmdMsg);
                    if(err) goto cleanExit;
                }
            }
            break;
            case IC_EVENT_TYPE_UNLOCK_ZSL:
                break;
            case IC_EVENT_TYPE_CAPTURE:
            {
                assert(lockBuffSize>0);// try to triger a capture but no frame was lock
                void *base = inCmdMsg.cmdMsg.u.capture.buff;
                int32_t inxForTrigBuf = -1;
                for (int i = 0; i < lockBuffSize; i++) {
                    if(base == imgLockBufs[i].ptr->base) {
                        inxForTrigBuf = i;
                        break;
                    }
                }
                if(-1 != inxForTrigBuf) {
                    Atomic a;
                    a.Enter();
                    ImgFramePtr iim;
                    iim = imgLockBufs[inxForTrigBuf];
                    if(IC_CAPTURE_KEEP_ZSL_LOCKED & inCmdMsg.cmdMsg.u.capture.flags) {
                        // not release the bufer from que, nothing to do
                        printf("trigMadeNotRelease\n");
                    }
                    else {
                        // relese from que
                        imgLockBufs[inxForTrigBuf] = nullObj;
                        for(int i = inxForTrigBuf; i < lockBuffSize-1; i++) {
                            assert(0);
                            imgLockBufs[i] = imgLockBufs[i+1];
                        }
                        imgLockBufs[lockBuffSize-1] = nullObj;
                        lockBuffSize--;
                    }
                    {
                        assert(ispCfg);
                        IspInputMsg sendMacro;
                        sendMacro.ispCfg = (icIspConfig*)inCmdMsg.cmdMsg.u.capture.pConfigStruct;
                        sendMacro.img = iim;
                        if(outStl.TrySend(&sendMacro)) {
                            assert(0); // TODO: implement frame Skipp
                        }
                        sendMacro.img = nullObj;
                    }
                    iim = nullObj;
                    a.Leave();
                }
                else {
                    //send error
                    outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_ERROR;
                    outCmdMsg.cmdMsg.u.error.sourceInstance = srcId;
                    outCmdMsg.cmdMsg.u.error.errorNo = IC_ERROR_TRIGGER_NOT_EXISTING_BUFF;
                    outCmdMsg.cmdMsg.u.error.severity = IC_SEVERITY_NORMAL;
                    outCmdMsg.cmdMsg.u.error.userData = NULL;
                    err = outCommand.Send(&outCmdMsg);
                    if(err) goto cleanExit;
                }
            }
            break;
            default:
                assert(0); //invalid message received
            }
        }
    }
    cleanExit:
    plgStatus = PLG_STATS_IDLE;
    if(threadOn) {
        assert(OK == pthread_join(srcThread, NULL));
        stopSrc();
    }
    nextIspCfg = NULL;
    lineHitNo = 0xFFFFFFFF;
    plgStatus = PLG_STATS_IDLE;
    status    = PLG_SOURCE_NOTMADE;
    releaseAllFrms();
    return NULL;
}


ImgFramePtr* PlgSource::queSearchLockFrame(uint64_t frameSel, int32_t tsRel) {

    ImgFramePtr  *chosedFrm = NULL;
    int bestMatchIdx = -1;
    Atomic a;
    a.Enter();
    if(tsRel) { // selection base on relative time
        int64_t  whatedTimeStamp = (int64_t)frameSel;
        int64_t  bestDif         = INT64_MAX;
        for(int i = 0; i < circBufSize; i++) {
            assert(imgCircBuf[i].ptr->ts>0);
            int64_t frameTimeStamp = (int64_t)imgCircBuf[i].ptr->ts;
            int64_t diff = frameTimeStamp - whatedTimeStamp;
            if(diff < 0) diff = -diff;
            if(diff < bestDif) {
                bestDif = diff;
                bestMatchIdx = i;
            }
        }
    }
    else {
        int32_t     whatedSeq = (int32_t)frameSel;
        int32_t    bestDif    = INT32_MAX;
        for(int i = 0; i < circBufSize; i++) {
            int32_t seq = (int64_t)imgCircBuf[i].ptr->seqNo;
            int32_t diff = seq - whatedSeq;
            if(diff < 0) diff = -diff;
            if(diff < bestDif) {
                bestDif = diff;
                bestMatchIdx = i;
            }
        }
    }
    if(-1 != bestMatchIdx) {
        assert(circBufSize>=1);
        imgLockBufs[lockBuffSize] = imgCircBuf[bestMatchIdx];
        imgCircBuf[bestMatchIdx] = nullObj;

        chosedFrm = &imgLockBufs[lockBuffSize];
        lockBuffSize++;
        for(int i = bestMatchIdx; i < circBufSize-1; i++) {
            imgCircBuf[i] = imgCircBuf[i+1];
        }
        imgCircBuf[circBufSize-1] = nullObj;
        circBufSize--;

    }
    a.Leave();
    return chosedFrm;
}

//#################################################################################################
void PlgSource:: startSourceThread() {
    // Src thread will be associated with max priority, losing eof is not acceptable and can
    // generate unexpected crash
    pthread_attr_t attr;
    assert(OK == pthread_attr_init           (&attr));
    assert(OK == pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED));
    assert(OK == pthread_attr_setschedpolicy (&attr, SCHED_RR));
    //printf("MAX= = %d \n",sched_get_priority_max(SCHED_RR));
    sched_param param;
    assert(OK == pthread_attr_getschedparam (&attr, &param));
    //printf("CrtPri= = %d \n",param.sched_priority);
    param.sched_priority = sched_get_priority_max(SCHED_RR);
    assert(OK == pthread_attr_setschedparam (&attr, &param));
    assert(OK == pthread_create(&srcThread, &attr, srcIrqHandlerFuncGenCal, this));
    assert(OK == pthread_attr_destroy   (&attr));

    //Build Rx Opipe and set the config buff
    plgSrcCreateRxOpipe(this, srcCfg, outFmt);
    pRx.pOut->ddr.base = (uint32_t)iImg->base;
    //References needed in Opipe EOF handler to update plugin info
    pRx.p.params[0] = (void*)this;
    pRx.p.params[1] = (void*)iImg;
    rxCfg.sel01 = 0xF<<8;
    OpipeStart(&pRx.p);
    mipiPhyCfg(srcCfg, this);
    DrvMssConnectMipiToDevice(mipiRxData.controllerNo, DRV_MSS_SIPP_RX);
}
//#################################################################################################
// hi priority thread that will take care about isp eof
void* PlgSource::srcIrqHandlerFunc(void *) {
    rtems_object_set_name(RTEMS_SELF, "PlgSourceSrcIrqThr");
    threadOn = 1;
    while(PLG_STATS_IDLE != plgStatus) {
        rtems_event_set events;
        rtems_interval  ticks = (PLG_SRC_TIMEOUT_SEC*rtems_clock_get_ticks_per_second());
        rtems_status_code rc = RTEMS_SUCCESSFUL;
        rc = rtems_event_receive(RTEMS_EVENT_2 | RTEMS_EVENT_3 | RTEMS_EVENT_4,
                        RTEMS_WAIT | RTEMS_EVENT_ANY, ticks, &events);

        ImgFrame  *frm = (ImgFrame*)this->pRx.p.params[1];

        if (PLG_STATS_IDLE == plgStatus) { // stop command received
            if(frm) {
                this->pRx.p.params[1] = NULL;
                outFrame = nullObj; //Release(); //we're done with this !
            }
            return NULL;
        }
        if(RTEMS_TIMEOUT == rc) {
            IspCmdMsg outCmdMsg;
            // most probably the sensor remain block. Or not work, the camera need to be restarted
            // this can be associated and with stop command
            plgStatus = PLG_STATS_IDLE;
            // Send skip isp config message
            outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_ERROR;
            outCmdMsg.cmdMsg.u.error.sourceInstance = srcId;
            outCmdMsg.cmdMsg.u.error.errorNo = IC_ERROR_SENSOR_SOURCE_STOP_TIMEOUT;
            outCmdMsg.cmdMsg.u.error.severity = IC_SEVERITY_RESET;
            outCmdMsg.cmdMsg.u.error.userData = NULL;//nextIspCfg->userData;
            outCommand.Send(&outCmdMsg);
        }
        else {
            if(RTEMS_SUCCESSFUL == rc) {
                if((events & RTEMS_EVENT_2)) {
                    ddd[srcId]++;
                    assert(frm == outFrame.ptr);
                    if(frm) {
                        ((ImgFrame*)outFrame.ptr)->fb.spec.width  = srcCfg->cameraOutputSize.w;
                        ((ImgFrame*)outFrame.ptr)->fb.spec.height = srcCfg->cameraOutputSize.h;
                        ((ImgFrame*)outFrame.ptr)->fb.spec.stride = srcCfg->cameraOutputSize.w;
                        { // send, construct send buffer
                            assert(ispCfg);
                            IspInputMsg sendMacro;
                            sendMacro.ispCfg = ispCfg;
                            sendMacro.img = outFrame;
                            if(out.TrySend(&sendMacro)) {
                                assert(0); // TODO: implement frame Skipp
                            }
                            sendMacro.img = nullObj;
                        }
                        if(eofEvent)
                            eofEvent((void*)this, outFrame.ptr);
                        { // ZSL capability
                            Atomic a;
                            a.Enter();
                            if((circBufSize+lockBuffSize)<allZslBufsSize) {
                                if (IC_PIPECTL_ZSL_LOCK & (((icIspConfig *)(outFrame.ptr->userData))->pipeControl)) {
                                    imgLockBufs[lockBuffSize] = outFrame;
                                    lockBuffSize++;
                                    // frame was Locked
                                    IspCmdMsg outCmdMsg;
                                    outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_ZSL_LOCKED;
                                    outCmdMsg.cmdMsg.u.buffLockedZSL.sourceInstance = srcId;
                                    outCmdMsg.cmdMsg.u.buffLockedZSL.buffZsl = outFrame.ptr->base;
                                    outCmdMsg.cmdMsg.u.buffLockedZSL.userData = (((icIspConfig *)(outFrame.ptr->userData))->userData);
                                    outCmdMsg.cmdMsg.u.buffLockedZSL.seqNo = outFrame.ptr->seqNo;
                                    outCmdMsg.cmdMsg.u.buffLockedZSL.ts = outFrame.ptr->ts;
                                    outCommand.Send(&outCmdMsg);
                                }
                                else {
                                    assert(circBufSize<2);
                                    assert(imgCircBuf[circBufSize].ptr==NULL);
                                    imgCircBuf[circBufSize] = outFrame;
                                    circBufSize++;
                                }
                            }
                            else {
                                if(circBufSize>0) {
                                    imgCircBuf[0] = nullObj;
                                    for(int i = 0; i < circBufSize-1; i++) {
                                        imgCircBuf[i] = imgCircBuf[i+1];
                                    }
                                    imgCircBuf[circBufSize-1]=nullObj;
                                    circBufSize--;
                                    if (IC_PIPECTL_ZSL_LOCK & (((icIspConfig *)(outFrame.ptr->userData))->pipeControl)) {
                                        assert(0);
                                        imgLockBufs[lockBuffSize] = outFrame;
                                        lockBuffSize++;
                                        // frame was Locked
                                        IspCmdMsg outCmdMsg;
                                        outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_ZSL_LOCKED;
                                        outCmdMsg.cmdMsg.u.buffLockedZSL.sourceInstance = srcId;
                                        outCmdMsg.cmdMsg.u.buffLockedZSL.buffZsl = outFrame.ptr->base;
                                        outCmdMsg.cmdMsg.u.buffLockedZSL.userData = (((icIspConfig *)(outFrame.ptr->userData))->userData);
                                        outCmdMsg.cmdMsg.u.buffLockedZSL.seqNo = outFrame.ptr->seqNo;
                                        outCmdMsg.cmdMsg.u.buffLockedZSL.ts = outFrame.ptr->ts;
                                        outCommand.Send(&outCmdMsg);
                                    }
                                    else {
                                        assert(circBufSize<2);
                                        assert(imgCircBuf[circBufSize].ptr==NULL);
                                        imgCircBuf[circBufSize] = outFrame;
                                        circBufSize++;
                                    }
                                }
                                else {
                                    IspCmdMsg outCmdMsg;
                                    // error message, zsl buff is full
                                    // this frame will not be keep in history and will not be lock
                                    outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_ERROR;
                                    outCmdMsg.cmdMsg.u.error.sourceInstance = srcId;
                                    outCmdMsg.cmdMsg.u.error.errorNo = IC_ERROR_NO_ZSL_BUFFS_AVAILABLE;
                                    outCmdMsg.cmdMsg.u.error.severity = IC_SEVERITY_NORMAL;
                                    outCmdMsg.cmdMsg.u.error.userData = NULL;
                                    outCommand.Send(&outCmdMsg);
                                }
                            }
                            a.Leave();
                        }
                        outFrame = nullObj ;//.Release(); //we're done with this !
                        strIdx++;
                    }
                    //Remember currently generated frame, will need to mark it produced in the end
                    // update frame counter
                    frameCnt++;
                    // handle new frame and eof event
                    if(NULL == nextIspCfg) {
                        assert(outFrame.ptr == NULL);
                        IspCmdMsg outCmdMsg;
                        // Skip Frame because not have Isp Config
                        this->pRx.p.params[1] = (void*)NULL;
                        rxCfg.sel01 = 0;
                        // Send skip isp config message
                        outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_ERROR;
                        outCmdMsg.cmdMsg.u.error.sourceInstance = srcId;
                        outCmdMsg.cmdMsg.u.error.errorNo = IC_ERROR_RT_CFG_MISSING;
                        outCmdMsg.cmdMsg.u.error.severity = IC_SEVERITY_NORMAL;
                        outCmdMsg.cmdMsg.u.error.userData = NULL;//nextIspCfg->userData;
                        outCommand.Send(&outCmdMsg);
                    }
                    else {
                        //Get new output frame for next run...
                        if(inO.TryReceive(&outFrame)) {
                            IspCmdMsg outCmdMsg;
                            // Skip Next frame because not have output buffer to write
                            this->pRx.p.params[1] = (void*)NULL;
                            rxCfg.sel01 = 0;
                            outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_ERROR;
                            outCmdMsg.cmdMsg.u.error.sourceInstance = srcId;
                            outCmdMsg.cmdMsg.u.error.errorNo = IC_ERROR_RT_OUT_BUFFERS_NOT_AVAILABLE;
                            outCmdMsg.cmdMsg.u.error.severity = IC_SEVERITY_NORMAL;
                            outCmdMsg.cmdMsg.u.error.userData = NULL;//nextIspCfg->userData;
                            outCommand.Send(&outCmdMsg);
                        }
                        else {
                            ispCfg = nextIspCfg;
                            nextIspCfg = NULL;
                            ((ImgFrame*)(outFrame.ptr))->userData = ispCfg;
                            outFrame.ptr->seqNo = frameCnt;
                            this->pRx.p.params[0] = (void*)this; //TBD: this is redundant, remove it
                            this->pRx.p.params[1] = (void*)((ImgFrame*)(outFrame.ptr));
                            rxCfg.sel01 = 0xF<<8 | downshift;
                            pRx.pOut->ddr.base = (uint32_t)(((ImgFrame*)outFrame.ptr)->base);
                        }
                    }
                    //Starting new frame (nxtOutFrm)
                    SetLineHit(lineHitNo);
                    OpipeStart(&pRx.p);
                }
                // this events will not be trigger if frame is skipped
                // no protection is associated
                if((events & RTEMS_EVENT_3)) {
                    getTimeStamps(((uint64_t*)&frm->ts));
                    if(sofEvent)
                        sofEvent((void*)this, frm);
                }
                if((events & RTEMS_EVENT_4)) {
                    if(hitEvent)
                        hitEvent((void*)this, frm);
                }
            }
            else {
                // Unexpected, unknown  error received
                assert(0);
            }
        }
    }
    return NULL;
}
//#################################################################################################
void* PlgSource::srcIrqHandlerFuncGenCal(void *This) {
    ((PlgSource *)This)->srcIrqHandlerFunc(This);
    return NULL;
}
//#################################################################################################
void PlgSource::SetLineHit(uint32_t lineNo) {
    if((mipiRxData.recNrl >= IC_SIPP_DEVICE0) && (mipiRxData.recNrl <= IC_SIPP_DEVICE3) ) {
        //Snap value to be multiple of IRQ-RATE, else won't be detected by Opipe
        pRx.p.targetLine[HIT_IDX_SRC] = (lineNo + (IRQ_RATE-1)) & (~(IRQ_RATE-1));
        //TBD: check if exceeds height ?
    }
}
//#################################################################################################
void PlgSource::sofEventSrc(void *plg, ImgFrame *frame) {
    PlgSource *obj    = (PlgSource *)plg;
    IspCmdMsg outCmdMsg;
    outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_READOUT_START;
    outCmdMsg.cmdMsg.u.lineEvent.sourceInstance = (icSourceInstance)obj->srcId;
    outCmdMsg.cmdMsg.u.lineEvent.userData = ((icIspConfig *)frame->userData)->userData;
    getTimeStamps(&outCmdMsg.cmdMsg.u.lineEvent.ts);
    outCmdMsg.cmdMsg.u.lineEvent.seqNo = obj->strIdx;//frame->seqNo; TODO: invalid values at first run from frmae.seqNr
    if(DESTROY_MESSAGE_SRC == obj->outCommand.TrySend(&outCmdMsg)) {
        printf("se03\n");
    }
}
//#################################################################################################
void PlgSource::eofEventSrc(void *plg, ImgFrame *frame) {
    UNUSED(frame);
    PlgSource *obj    = (PlgSource *)plg;
    IspCmdMsg outCmdMsg;
    outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_READOUT_END;
    outCmdMsg.cmdMsg.u.lineEvent.sourceInstance = (icSourceInstance)obj->srcId;
    outCmdMsg.cmdMsg.u.lineEvent.userData =  ((icIspConfig *)frame->userData)->userData;
    getTimeStamps(&outCmdMsg.cmdMsg.u.lineEvent.ts);
    outCmdMsg.cmdMsg.u.lineEvent.seqNo = obj->strIdx; //frame->seqNo;
    if(DESTROY_MESSAGE_SRC == obj->outCommand.TrySend(&outCmdMsg)) {
        printf("se02\n");
    }
}
//#################################################################################################
void PlgSource::lnHitEventSrc(void *plg, ImgFrame *frame) {
    UNUSED(frame);
    PlgSource *obj    = (PlgSource *)plg;
    IspCmdMsg outCmdMsg;
    outCmdMsg.cmdMsg.ctrl = IC_EVENT_TYPE_LINE_REACHED;
    outCmdMsg.cmdMsg.u.lineEvent.sourceInstance = (icSourceInstance)obj->srcId;
    outCmdMsg.cmdMsg.u.lineEvent.userData = ((icIspConfig *)frame->userData)->userData;
    getTimeStamps(&outCmdMsg.cmdMsg.u.lineEvent.ts);
    outCmdMsg.cmdMsg.u.lineEvent.seqNo = obj->strIdx; //frame->seqNo;
    if(DESTROY_MESSAGE_SRC == obj->outCommand.TrySend(&outCmdMsg)) {
        printf("se01\n");
    }
}
//#################################################################################################
void PlgSource::mipiPhyCfg(icSourceConfig *mipiRxConfiguration, PlgSource *obj) {
    UNUSED(obj);
    // set mipi controler
    tDrvMipiControllerConfigStruct rxConfig;
    rxConfig.controllerNo      = (drvMipiCtrlNo)mipiRxConfiguration->mipiRxData.controllerNo;
    rxConfig.type              = MIPI_CSI;
    rxConfig.direction         = MIPI_RX;
    rxConfig.noLanes           = mipiRxConfiguration->mipiRxData.noLanes;
    rxConfig.laneRateMbps      = mipiRxConfiguration->mipiRxData.laneRateMbps;
    rxConfig.rxPacket.dataMode = MIPI_D_MODE_0;
    rxConfig.rxPacket.dataType = (drvMipiDataType)mipiRxConfiguration->mipiRxData.dataType;
    rxConfig.width             = mipiRxConfiguration->cameraOutputSize.w;
    rxConfig.height            = mipiRxConfiguration->cameraOutputSize.h;
    //rxConfig->mipiCtrlCfg.refClkKHz         = mipiRxConfiguration->sensorSpec->idealRefFreq; // TODO: ask !
    assert(DRV_MIPI_CFG_DONE == DrvMipiInit(&rxConfig));
}
//#################################################################################################
void PlgSource::stopSrc() {
    PlgSource *obj    = this;
    // stop source
    // disable controler in order to fix sensor hank after a reset
    CLR_REG_BITS_MASK(MIPI0_HS_CTRL_ADDR + obj->srcCfg->mipiRxData.controllerNo * HS_MIPI_CTRL_OFSET_ ,1);
    uint32_t disableCtrlMask = (1<<obj->srcCfg->mipiRxData.controllerNo);
    CLR_REG_BITS_MASK(MSS_MIPI_CFG_ADR, disableCtrlMask);
    // clear conection Mipi-SippRx
    CLR_REG_BITS_MASK(MSS_MIPI_SIPP_CFG_ADR, (0x3F<<(obj->srcCfg->mipiRxData.recNrl-IC_SIPP_DEVICE0)));
    // reset sipp Rx to, in order to not remain old interrupts
    OpipeHwReset(obj->pRx.p.enMask);
}


void PlgSource::init() {
    allZslBufsSize = MAX_ZSL_BUF_SIZE;
    circBufSize = MAX_ZSL_BUF_SIZE;
    lockBuffSize = 0;
    for(int i = 0; i<circBufSize; i++) {
        ImgFramePtr iiimg;
        if(inO.TryReceive(&iiimg)) {
            assert(0); // Incorrect number of zsl buffers allocated, is bigger that total numbers of bufers
        }
        imgCircBuf[i] = iiimg;
    }
}

//#################################################################################################
//Must release all persistently referenced frames
//else, on destroy frames will "return" to potentially destroyed pools
//causing issues
void PlgSource::releaseAllFrms()
{
   outFrame = nullptr;
   for(int i=0; i<MAX_ZSL_BUF_SIZE; i++)
   {
     imgCircBuf [i] = nullptr;
     imgLockBufs[i] = nullptr;
   }
}
