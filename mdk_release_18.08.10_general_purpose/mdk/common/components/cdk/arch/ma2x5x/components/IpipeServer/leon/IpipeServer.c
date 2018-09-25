#include <stdio.h>
#include <string.h>                          //memset
#include <assert.h>
#if defined(__sparc)
#include "DrvIcb.h"
#include "DrvLeonL2C.h"
#include "DrvTimer.h"
#else
#endif


#include "IpipeServerApi.h"
#include "TimeSyncMgr.h"
#include "FrameMgrApi.h"
#include "MemMgrApi.h"
#include "ipipeUtils.h"

#ifndef MEM_FRAMES_STRUCT_SIZE
#define MEM_FRAMES_STRUCT_SIZE    (1024*20)
#endif

#ifndef IPIPE_SERVER_PRINTF
#define IPIPE_SERVER_PRINTF(...)
#endif


//#############################################################################
icCtrl              gIcCtrl = {.appId=1};  //Shared with OS Leon ! NO BSS section for it
icCtrl*             pSrvIcCtrl = (icCtrl*)0xBEEFDEAD; //ptr that bypasses L2
volatile            ipipeServerInfo  gServerInfo;
static MemPoolId    ddrMemPoolId = NO_VALID_MEM;
static MemPoolId    cmxMemPoolId = NO_VALID_MEM;
static uint8_t      memFramesStructuresBuf[MEM_FRAMES_STRUCT_SIZE]={1}; //! No Bss section for it

//#############################################################################

void scanInputEvents(uint32_t unused);
void ipServerAddExtraZslFrames(icSourceInstance sourceId, uint32_t ZSLframesToAdd);

// Internal debug code, disabled by default.
//#define ENABLE_DBG_LOCAL
#ifdef ENABLE_DBG_LOCAL
#define DBG_MAX_MESSAGE 4048
volatile uint32_t dbgMsgBuf[DBG_MAX_MESSAGE] = {1};
volatile uint32_t dbgMsgIdx = 1;
static inline void dbgMAddMes(uint32_t mesage) {
    dbgMsgBuf[dbgMsgIdx] = mesage;
    dbgMsgIdx++;
    //printf("dbgMsgIdx: %d", dbgMsgIdx);
    //printf("dbgMsgIdx: %d", dbgMsgIdx);
    assert(dbgMsgIdx < DBG_MAX_MESSAGE);
    //if(dbgMsgIdx == DBG_MAX_MESSAGE) {
    //    dbgMsgIdx = 0;
    //}
}
#else
#define dbgMAddMes(X)
#endif

// "*ev" needs to be obtained previously with "ipipeQueueGetNextOutputSlot"
static inline void ipipeQueueSendToClientEvent(icEvent *ev) {
    IPIPE_SERVER_PRINTF("RT: ipipeQueueSendToClientEvent \n");
    ev->ctrl |= IC_EVENT_CTRL_OWNED; //consummer owns this from now on
#if defined(__sparc)
    DrvIcbIrqRemoteTrigger(INTER_CPU_INT);
#else
    extern void ipipeClientISR(uint32_t unused);
    ipipeClientISR(0);
#endif
}

static int queuePut(FrameT *buffer, void *config, icSourceInstance source) {
    if(gServerInfo.trigerCaptQue.queueIn == (( gServerInfo.trigerCaptQue.queueOut - 1 +
            MAX_NR_OF_CAPTURE_PENDING) % MAX_NR_OF_CAPTURE_PENDING)) {
        return -1; /* Queue Full*/
    }
    gServerInfo.trigerCaptQue.queue[gServerInfo.trigerCaptQue.queueIn].buffer = buffer;
    gServerInfo.trigerCaptQue.queue[gServerInfo.trigerCaptQue.queueIn].config = config;
    gServerInfo.trigerCaptQue.queue[gServerInfo.trigerCaptQue.queueIn].source = source;
    gServerInfo.trigerCaptQue.queueIn = (gServerInfo.trigerCaptQue.queueIn + 1) % MAX_NR_OF_CAPTURE_PENDING;
    return 0; // No errors
}

int ipServerQueueGet(TriggerCaptElement **old) {
    if(gServerInfo.trigerCaptQue.queueIn == gServerInfo.trigerCaptQue.queueOut) {
        return -1; /* Queue Empty - nothing to get*/
    }
    //printf("ipServerQueueGet \n");
    *old = (TriggerCaptElement *)&gServerInfo.trigerCaptQue.queue[gServerInfo.trigerCaptQue.queueOut];
    gServerInfo.trigerCaptQue.queueOut = (gServerInfo.trigerCaptQue.queueOut + 1) % MAX_NR_OF_CAPTURE_PENDING;
    return 0; // No errors
}


//#############################################################################
// RT-Leon needs to call this after BOOT
// At the end, will signal to OS-Leon that it's ready for messaging)
void setupIpipeServer()
{
    //Get & use a pointer that bypasses L2 cache
    pSrvIcCtrl = (icCtrl *)ipipeGetNoCacheAddr((uint32_t)&gIcCtrl);
    assert(IPIPE_UNINITIALIZED == pSrvIcCtrl->status);

#if defined(__sparc)
    DrvTimerInit();
    //Init ICB side for RT-Leon
    DrvIcbIrqClear(INTER_CPU_INT);
    swcLeonSetPIL(0);
    DrvIcbSetupIrq(INTER_CPU_INT, INTER_CPU_INT_LEVEL, POS_EDGE_INT, scanInputEvents);
#endif
    //Reset SIPP subsystem
    //CLR_REG_BITS_MASK(MSS_RSTN_CTRL_ADR, 1<<MSS_CP_SPMP);
    //SET_REG_BITS_MASK(MSS_RSTN_CTRL_ADR, 1<<MSS_CP_SPMP);
    //SET_REG_WORD     (MSS_SIPP_CLK_CTRL_ADR , 0xffffffff);

    //app specific initialisation, after will be available application description
    // init frames buffer allocated by los
    MemMgrInit();
    ddrMemPoolId = MemMgrAddPool(pSrvIcCtrl->framepoolBase,     pSrvIcCtrl->framepoolSize);
    cmxMemPoolId = MemMgrAddPool((void*)memFramesStructuresBuf, sizeof(memFramesStructuresBuf));
    if(gServerInfo.cbIcSetup) {
        gServerInfo.cbIcSetup(pSrvIcCtrl);
    }
    gServerInfo.trigerCaptQue.queueIn = gServerInfo.trigerCaptQue.queueOut = 0;
    pSrvIcCtrl->status = IPIPE_INITIALIZED;
    icEvent *ev = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
    ev->ctrl    = IC_EVENT_TYPE_LEON_RT_READY;
    ipipeQueueSendToClientEvent(ev);
}

void ipServerRegSourceQuery(uint32_t id, const char  *name, uint32_t attrs, uint32_t nrOfFramesInZsl,
        uint32_t outIdVid, uint32_t outIdStill, uint32_t outIdRaw, uint32_t outIdBin) {
    pSrvIcCtrl->icPipelineDescription.icQuerySource[id] =
            (IcQuerySource*)MemMgrAlloc(cmxMemPoolId, sizeof(IcQuerySource));
    assert(0 != pSrvIcCtrl->icPipelineDescription.icQuerySource[id]);// out of memory
    memset((void*)pSrvIcCtrl->icPipelineDescription.icQuerySource[id], 0, sizeof(IcQuerySource));
    // plugin description. TODO: this have to be internally in the plug-in ?
    strcpy(pSrvIcCtrl->icPipelineDescription.icQuerySource[id]->plgDefine.name, name);
    pSrvIcCtrl->icPipelineDescription.icQuerySource[id]->plgDefine.idx  = id;
    pSrvIcCtrl->icPipelineDescription.icQuerySource[id]->plgDefine.type = IC_QUERY_TYPE_SOURCE;
    // information used by los isp control code
    pSrvIcCtrl->icPipelineDescription.icQuerySource[id]->attrs = attrs;
    pSrvIcCtrl->icPipelineDescription.icQuerySource[id]->zslNumFrames = nrOfFramesInZsl;
    pSrvIcCtrl->icPipelineDescription.icQuerySource[id]->output.videoOutputId    = outIdVid;
    pSrvIcCtrl->icPipelineDescription.icQuerySource[id]->output.stillOutputId    = outIdStill;
    pSrvIcCtrl->icPipelineDescription.icQuerySource[id]->output.rawOutputId      = outIdRaw;
    pSrvIcCtrl->icPipelineDescription.icQuerySource[id]->output.metadataOutputId = outIdBin;
}

void ipServerRegIspQuery(uint32_t id, const char  *name, uint32_t attrs, uint32_t sourceId) {
    pSrvIcCtrl->icPipelineDescription.icQueryIsp[id] =
            (IcQueryIsp*) MemMgrAlloc(cmxMemPoolId, sizeof(IcQueryIsp));
    assert(0 != pSrvIcCtrl->icPipelineDescription.icQueryIsp[id]);// out of memory
    memset((void*)pSrvIcCtrl->icPipelineDescription.icQueryIsp[id], 0, sizeof(IcQueryIsp));
    // plugin description. TODO: this have to be internally in the plug-in ?
    strcpy(pSrvIcCtrl->icPipelineDescription.icQueryIsp[id]->plgDefine.name, name);
    pSrvIcCtrl->icPipelineDescription.icQueryIsp[id]->plgDefine.idx  = id;
    pSrvIcCtrl->icPipelineDescription.icQueryIsp[id]->plgDefine.type = IC_QUERY_TYPE_ISP;
    //
    pSrvIcCtrl->icPipelineDescription.icQueryIsp[id]->attrs = attrs;
    pSrvIcCtrl->icPipelineDescription.icQueryIsp[id]->sourceDepId = sourceId;
    pSrvIcCtrl->icPipelineDescription.icQueryIsp[id]->outputType  = IC_IPIPE_YUV_420_B8;
}

void ipServerRegOutputQuery(uint32_t id, const char  *name, uint32_t attrs, uint32_t dependentSources) {
    pSrvIcCtrl->icPipelineDescription.icQueryOutput[id] =
            (IcQueryOutput*)MemMgrAlloc(cmxMemPoolId, sizeof(IcQueryOutput));
    assert(0 != pSrvIcCtrl->icPipelineDescription.icQueryOutput[id]);// out of memory
    memset((void*)pSrvIcCtrl->icPipelineDescription.icQueryOutput[id], 0, sizeof(IcQueryOutput));
    // plugin description. TODO: this have to be internally in the plug-in ?
    strcpy(pSrvIcCtrl->icPipelineDescription.icQueryOutput[id]->plgDefine.name, name);
    pSrvIcCtrl->icPipelineDescription.icQueryOutput[id]->plgDefine.idx  = id;
    pSrvIcCtrl->icPipelineDescription.icQueryOutput[id]->plgDefine.type = IC_QUERY_TYPE_OUTPUT;
    //
    pSrvIcCtrl->icPipelineDescription.icQueryOutput[id]->attrs = attrs;
    pSrvIcCtrl->icPipelineDescription.icQueryOutput[id]->dependentSources = dependentSources;
    //
}

void ipServerRegUserPlgQuery(uint32_t id, const char  *name, uint32_t attrs) {
    pSrvIcCtrl->icPipelineDescription.icQueryPlg[id] =
            (IcQueryPlg*)MemMgrAlloc(cmxMemPoolId, sizeof(IcQueryPlg));
    assert(0 != pSrvIcCtrl->icPipelineDescription.icQueryPlg[id]);// out of memory
    memset((void*)pSrvIcCtrl->icPipelineDescription.icQueryPlg[id], 0, sizeof(IcQueryPlg));
    // plugin description. TODO: this have to be internally in the plug-in ?
    strcpy(pSrvIcCtrl->icPipelineDescription.icQueryPlg[id]->plgDefine.name, name);
    pSrvIcCtrl->icPipelineDescription.icQueryPlg[id]->plgDefine.idx  = id;
    pSrvIcCtrl->icPipelineDescription.icQueryPlg[id]->plgDefine.type = IC_QUERY_TYPE_USERPLG;
    //
    pSrvIcCtrl->icPipelineDescription.icQueryPlg[id]->attrs = attrs;
}

void ipServerQueryAddChild(void* parentDescP, void* childDescP) {
    IcQueryPlg *parent = (IcQueryPlg *)parentDescP;
    IcQueryPlg *child = (IcQueryPlg *)childDescP;
    parent->plgLink.child[parent->plgLink.nrChilds++] = &child->plgDefine;
    assert(parent->plgLink.nrChilds <= MAX_PLUGINS_CHILD_NR);
}
//#############################################################################
// Normal-Priority Input Events handling
void ipipeServerHandleInputEvent(icEvent *ev) {
    icEvent *intEv;
    icStatusCode statusCode;
    FrameT *lockFrame;
    uint32_t inst;
    dbgMAddMes(ev->ctrl & IC_EVENT_CTRL_TYPE_MASK);
    switch(ev->ctrl & IC_EVENT_CTRL_TYPE_MASK)
    {
    case IC_EVENT_TYPE_SETUP_SOURCE:
        IPIPE_SERVER_PRINTF("RT: IC_EVENT_TYPE_SETUP_SOURCE \n");
        assert(IPIPE_INITIALIZED == pSrvIcCtrl->status);
        assert(IPIPE_SRC_UNINITIALIZED == pSrvIcCtrl->source[ev->u.configureSource.sourceInstance].sourceStatus);
        pSrvIcCtrl->source[ev->u.configureSource.sourceInstance].sourceStatus = IPIPE_SRC_SETUP;
        // the source configuration was copied by LOS, RT need to take them, invalidating cache
#if defined(__sparc)
        DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_INVALIDATE,0);
#endif
        break;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    case IC_EVENT_TYPE_SETUP_SOURCES_COMMIT:
        IPIPE_SERVER_PRINTF("RT: IC_EVENT_TYPE_SETUP_SOURCES_COMMIT \n");
        assert(IPIPE_INITIALIZED == pSrvIcCtrl->status);
        uint32_t x;
        for (x = 0; x < IPIPE_MAX_SOURCES_ALLOWED; x++)
        {
            if(IPIPE_SRC_SETUP == pSrvIcCtrl->source[x].sourceStatus) {
                ipServerFrameMgrAddBuffs(
                                gServerInfo.sourceServerCtrl[x].pool->frames,
                               (pSrvIcCtrl->source[x].sourceSetup.maxWidth *
                                pSrvIcCtrl->source[x].sourceSetup.maxHeight *
                                pSrvIcCtrl->source[x].sourceSetup.maxBpp) >> 3, // div by 8 size in bytes
                                0, 0);
            }
        }
        assert(NULL != gServerInfo.cbSourcesCommit);
        statusCode = gServerInfo.cbSourcesCommit(pSrvIcCtrl);
        pSrvIcCtrl->status = IPIPE_CONFIGURED;
        intEv = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
        intEv->ctrl    = IC_EVENT_TYPE_SETUP_SOURCES_RESULT;
        intEv->u.configSourcesResult.status = statusCode;
        intEv->u.configSourcesResult.memFree = gServerInfo.memFree;
        ipipeQueueSendToClientEvent(intEv);
        break;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    case IC_EVENT_TYPE_CONFIG_SOURCE:
        IPIPE_SERVER_PRINTF("RT: IC_EVENT_TYPE_CONFIG_SOURCE \n");
        // the source configuration was copied by LOS, RT need to take them, invalidating cache
#if defined(__sparc)
        DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_INVALIDATE,0);
#endif
        break;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    case IC_EVENT_TYPE_START_SOURCE:
        IPIPE_SERVER_PRINTF("RT: IC_EVENT_TYPE_START_SOURCE \n");
        if(gServerInfo.sourceServerCtrl[ev->u.start.sourceInstance].cbStartSource) {
            assert(IPIPE_SRC_CONFIGURED == pSrvIcCtrl->source[ev->u.start.sourceInstance].sourceStatus);
            assert(IPIPE_CONFIGURED == pSrvIcCtrl->status);
            gServerInfo.sourceServerCtrl[ev->u.start.sourceInstance].cbStartSource(
                    ev->u.start.sourceInstance,
                    &pSrvIcCtrl->source[ev->u.start.sourceInstance].sourceConfig);
            pSrvIcCtrl->source[ev->u.start.sourceInstance].sourceStatus = IPIPE_SRC_IN_USE;
        }
        break;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    case IC_EVENT_TYPE_STOP_SOURCE:
        IPIPE_SERVER_PRINTF("RT: IC_EVENT_TYPE_STOP_SOURCE \n");
        if(gServerInfo.sourceServerCtrl[ev->u.stop.sourceInstance].cbStopSource) {
            assert(IPIPE_SRC_IN_USE == pSrvIcCtrl->source[ev->u.stop.sourceInstance].sourceStatus);
            assert(IPIPE_CONFIGURED == pSrvIcCtrl->status);
            gServerInfo.sourceServerCtrl[ev->u.stop.sourceInstance].cbStopSource(ev->u.stop.sourceInstance);
        }
        break;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    case IC_EVENT_TYPE_CONFIG_ISP:
        IPIPE_SERVER_PRINTF("RT: IC_EVENT_TYPE_CONFIG_ISP \n");
        if(gServerInfo.pluginServerCtrl[ev->u.configureIsp.ispInstance].cbConfigPlugin) {
            gServerInfo.pluginServerCtrl[ev->u.configureIsp.ispInstance].cbConfigPlugin(ev->u.configureIsp.ispInstance, (void*)ev->u.configureIsp.pConfigStruct);
        }
        break;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    case IC_EVENT_TYPE_CONFIG_SOURCE_DYNAMIC:
        IPIPE_SERVER_PRINTF("RT: IC_EVENT_TYPE_CONFIG_SOURCE_DYNAMIC \n");
        //if callback exists configureSourceDynamic.sourceInstance
        inst = ev->u.configureSourceDynamic.sourceInstance;
        if(gServerInfo.sourceServerCtrl[inst].cbCfgDynamic) {
            assert(IPIPE_SRC_IN_USE == pSrvIcCtrl->source[inst].sourceStatus);
            assert(IPIPE_CONFIGURED == pSrvIcCtrl->status);
            //invoke the callback
            gServerInfo.sourceServerCtrl[inst].cbCfgDynamic(
                    inst,
                    ev->u.configureSourceDynamic.pDynConfig);
        }
        break;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    case IC_EVENT_TYPE_LOCK_ZSL:
        //printf("RT: IC_EVENT_TYPE_LOCK_ZSL \n");
        lockFrame =  FrameMgrLockFrame(
                gServerInfo.sourceServerCtrl[ev->u.lockZSL.sourceInstance].pool,
                ev->u.lockZSL.frameSel,
                (ev->u.lockZSL.flags & IC_LOCKZSL_TS_RELATIVE));
        intEv = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
        intEv->ctrl    = IC_EVENT_TYPE_ZSL_LOCKED;
        intEv->u.buffLockedZSL.sourceInstance = ev->u.lockZSL.sourceInstance;
        intEv->u.buffLockedZSL.buffZsl = (FrameT *)ipipeGetNoCacheAddr((uint32_t)lockFrame);
        intEv->u.buffLockedZSL.userData = NULL; // we don't assume buffers in ZSL queue has valid user data
        ipipeQueueSendToClientEvent(intEv);
        break;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    case IC_EVENT_TYPE_UNLOCK_ZSL:
        IPIPE_SERVER_PRINTF("RT: IC_EVENT_TYPE_UNLOCK_ZSL \n");
        FrameMgrUnlockFrame((FrameT *)ipipeGetCacheAddr((uint32_t)ev->u.unlockZSL.buff));
        break;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    case IC_EVENT_TYPE_CAPTURE:
        IPIPE_SERVER_PRINTF("RT: IC_EVENT_TYPE_CAPTURE \n");
        lockFrame = (FrameT *)ipipeGetCacheAddr((uint32_t)ev->u.capture.buff);
        assert(0 == queuePut(lockFrame, ev->u.capture.pConfigStruct,
                ev->u.capture.sourceInstance));
        if(IC_CAPTURE_KEEP_ZSL_LOCKED & ev->u.capture.flags) { // after trigger will run, buffer will remain lock
            FrameMgrIncreaseNrOfConsumer(lockFrame, 1);
        }
        //TODO: For now send raw at each frame until there is LOS support for this flag
        //if(IC_CAPTURE_SEND_RAW & ev->u.capture.flags) {
        if(lockFrame)
        { // option available just if frame exist
            FrameMgrIncreaseNrOfConsumer(lockFrame, 1);
            ipServerSendData(lockFrame, pSrvIcCtrl->icPipelineDescription.icQuerySource[ev->u.capture.sourceInstance]->output.rawOutputId);
        }
        //}

        break;
    case IC_EVENT_TYPE_ZSL_ADD:
        IPIPE_SERVER_PRINTF("RT: IC_EVENT_TYPE_ZSL_ADD \n");
        ipServerAddExtraZslFrames(ev->u.addZSL.sourceInstance, ev->u.addZSL.addZSLframesNo);
        break;
    case IC_EVENT_TYPE_OUTPUT_DATA_RECEIVED:
        IPIPE_SERVER_PRINTF("RT: IC_EVENT_TYPE_OUTPUT_DATA_RECEIVED \n");
        if(gServerInfo.cbDataWasSent) {
            dbgMAddMes((uint32_t)0xFFFFFFFF);
            // Lrt use frames in cache context. Los have to read it, but not change.
            // The pointer will be moved back in catchable address
            gServerInfo.cbDataWasSent((FrameT *)ipipeGetCacheAddr((uint32_t)ev->u.dataWasSent.dataBufStruct));
        }
        break;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    case IC_EVENT_TYPE_TEAR_DOWN:
        IPIPE_SERVER_PRINTF("RT: IC_EVENT_TYPE_TEAR_DOWN \n");
        // reset the application and after stop RT processor
        if(gServerInfo.cbIcTearDown) {
            gServerInfo.cbIcTearDown();
        }
        break;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    case IC_EVENT_TYPE_USER:
        IPIPE_SERVER_PRINTF("RT: IC_EVENT_TYPE_USER \n");
        // if user message function is initialized, the call it
        if(gServerInfo.cbIcUserMsg) {
            gServerInfo.cbIcUserMsg(ev->u.generalEvent.eventStruct, ev->u.generalEvent.id);
        }
        break;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

    default:
        // undefined message come
        printf("ev->ctrl = %d\n", (int)ev->ctrl);
        assert((ev->ctrl & IC_EVENT_CTRL_TYPE_MASK) == IC_EVENT_TYPE_TEAR_DOWN);
        break;
    }
    //Mark event as consummed
    ev->ctrl = 0; //clears both ID and ownership Mem-POOL:
}


// Parse events from Client side till no more are found
void scanInputEvents(uint32_t unused) {
    icEvent ev;

    UNUSED(unused);

    while(ipipeQueueGetEvent(&pSrvIcCtrl->eventQOut, &ev)) {
        ipipeServerHandleInputEvent(&ev);
    }
}

//
int32_t ipServerGetTrigerCaptEvent (TriggerCaptElement *old) {
    if(gServerInfo.trigerCaptQue.queueIn == gServerInfo.trigerCaptQue.queueOut) {
        return -1; /* Queue Empty - nothing to get*/
    }
    *old = gServerInfo.trigerCaptQue.queue[gServerInfo.trigerCaptQue.queueOut];
    gServerInfo.trigerCaptQue.queueOut = (gServerInfo.trigerCaptQue.queueOut + 1) %
            MAX_NR_OF_CAPTURE_PENDING;

    return 0; // No errors
}

//
void ipServerWasTornDown(void) {
    icEvent *intEv;
    // move sources in uninitialised state
    memset((void*)pSrvIcCtrl->source, 1, sizeof(pSrvIcCtrl->source));
    pSrvIcCtrl->status = IPIPE_UNINITIALIZED;
    //
    intEv = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
    intEv->ctrl    = IC_EVENT_TYPE_TORN_DOWN;
    ipipeQueueSendToClientEvent(intEv);
}

//
void ipServerSourceReady(icSourceInstance source) {
    icEvent *intEv;
    intEv = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
    intEv->ctrl    = IC_EVENT_TYPE_SOURCE_READY;
    intEv->u.sourceEvent.sourceInstance = source;
    ipipeQueueSendToClientEvent(intEv);
}

void ipServerSourceStopped(icSourceInstance source) {
    icEvent *intEv;
    pSrvIcCtrl->source[source].sourceStatus = IPIPE_SRC_SETUP;
    intEv = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
    intEv->ctrl    = IC_EVENT_TYPE_SOURCE_STOPPED;
    intEv->u.sourceEvent.sourceInstance = source;
    ipipeQueueSendToClientEvent(intEv);
}

//
void ipServerSendData(FrameT *frame, uint32_t outId) {
    icEvent *intEv;
    intEv = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
    //Get & use a pointer that bypasses L2 cache
    intEv->u.sentOutput.dataBufStruct   = (FrameT *)ipipeGetNoCacheAddr((uint32_t)frame);
    intEv->u.sentOutput.outputId        = outId;
    intEv->ctrl    = IC_EVENT_TYPE_SEND_OUTPUT_DATA;
    dbgMAddMes((uint32_t)0x88888888);
    ipipeQueueSendToClientEvent(intEv);
}

//
void ipServerReadoutStart(icSourceInstance source, void * userData, uint32_t seqNo, icTimestamp  ts) {
    icEvent *intEv;
    intEv = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
    intEv->u.lineEvent.sourceInstance   = source;
    intEv->u.lineEvent.userData         = userData;
    intEv->u.lineEvent.seqNo            = seqNo;
    intEv->u.lineEvent.ts               = ts;
    intEv->ctrl                         = IC_EVENT_TYPE_READOUT_START;
    ipipeQueueSendToClientEvent(intEv);
}

void ipServerLineHit(icSourceInstance source, void * userData, uint32_t seqNo, icTimestamp ts) {
    icEvent *intEv;
    intEv = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
    intEv->u.lineEvent.sourceInstance   = source;
    intEv->u.lineEvent.userData         = userData;
    intEv->u.lineEvent.seqNo            = seqNo;
    intEv->u.lineEvent.ts               = ts;
    intEv->ctrl                         = IC_EVENT_TYPE_LINE_REACHED;
    ipipeQueueSendToClientEvent(intEv);
}

//
void ipServerReadoutEnd(icSourceInstance source, void * userData, uint32_t seqNo, icTimestamp  ts) {
    icEvent *intEv;
    intEv = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
    intEv->u.lineEvent.sourceInstance 	= source;
    intEv->u.lineEvent.userData         = userData;
    intEv->u.lineEvent.seqNo 			= seqNo;
    intEv->u.lineEvent.ts 				= ts;
    intEv->ctrl    						= IC_EVENT_TYPE_READOUT_END;
    ipipeQueueSendToClientEvent(intEv);
}

//
void ipServerIspStart(int32_t ispIdx, uint32_t seqNo, void *userData) {
    icEvent *intEv;
    intEv = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
    intEv->u.ispEvent.ispInstance   = ispIdx;
    intEv->u.ispEvent.seqNo         = seqNo;
    intEv->u.ispEvent.userData      = userData;
    intEv->ctrl                     = IC_EVENT_TYPE_ISP_START;
    ipipeQueueSendToClientEvent(intEv);
}

//
void ipServerIspEnd(int32_t ispIdx, uint32_t seqNo, void *userData) {
    icEvent *intEv;
    intEv = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
    intEv->u.ispEvent.ispInstance   = ispIdx;
    intEv->u.ispEvent.seqNo         = seqNo;
    intEv->u.ispEvent.userData      = userData;
    intEv->ctrl                     = IC_EVENT_TYPE_ISP_END;
    ipipeQueueSendToClientEvent(intEv);
}

//
void ipServerIspConfigAccepted(int32_t ispIdx, uint32_t seqNo, void *userData) {
    icEvent *intEv;
    intEv = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
    intEv->u.ispEvent.ispInstance   = ispIdx;
    intEv->u.ispEvent.seqNo         = seqNo;
    intEv->u.ispEvent.userData      = userData;
    intEv->ctrl                     = IC_EVENT_TYPE_ISP_CONFIG_ACCEPTED;
    ipipeQueueSendToClientEvent(intEv);
}

void ipServerIspStatsReady(int32_t ispIdx, uint32_t seqNo, void *userData) {
    icEvent *intEv;
    intEv = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
    intEv->u.ispEvent.ispInstance   = ispIdx;
    intEv->u.ispEvent.seqNo         = seqNo;
    intEv->u.ispEvent.userData      = userData;
    intEv->ctrl                     = IC_EVENT_TYPE_STATS_READY;
    ipipeQueueSendToClientEvent(intEv);
}

void ipServerFrameLocked(icSourceInstance source, FrameT *frame) {
    icEvent *intEv;
    intEv = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
    intEv->ctrl    = IC_EVENT_TYPE_ZSL_LOCKED;
    intEv->u.buffLockedZSL.sourceInstance = source;
    intEv->u.buffLockedZSL.buffZsl = (FrameT *)ipipeGetNoCacheAddr((uint32_t)frame);
    if((frame) && (intEv->u.buffLockedZSL.buffZsl->appSpecificData)) {
        intEv->u.buffLockedZSL.userData =
                ((icIspConfig *)intEv->u.buffLockedZSL.buffZsl->appSpecificData)->userData;
    } else {
        // this should be an error condition - we produce a frame without proper configuration
        // but we have to initialize all message fields
        intEv->u.buffLockedZSL.userData = NULL;
    }
    ipipeQueueSendToClientEvent(intEv);
}
FrameT* ipServerFrameMgrCreateList(uint32_t nrOfFrame) {
    uint32_t i;
    assert(nrOfFrame > 0);
    FrameT* framesList;
    FrameT* frames = (FrameT*)MemMgrAlloc(cmxMemPoolId, sizeof(FrameT));
    if(NULL == frames) return 0;
    memset((void*)frames, 0, sizeof(FrameT));
    framesList = frames;
    for (i = 1; i < nrOfFrame; i++) {
        FrameT* framesN = (FrameT*)MemMgrAlloc(cmxMemPoolId, sizeof(FrameT));
        if(NULL == frames) return 0;
        memset((void*)framesN, 0, sizeof(FrameT));
        framesList->next = framesN;
        framesList = framesN;
    }
    return frames;
}

void ipServerFrameMgrAddBuffs(FrameT* frameList,
        uint32_t dataSizePl0, uint32_t dataSizePl1, uint32_t dataSizePl2) {
    FrameT* framesN = frameList;
    uint32_t totalSize = dataSizePl0 + dataSizePl1 + dataSizePl2;
    void *tmp_buf = NULL;
    while (framesN) {
        tmp_buf = MemMgrAlloc(ddrMemPoolId, totalSize);
        framesN->fbPtr[0] = tmp_buf;
        framesN->fbPtr[1] = tmp_buf + dataSizePl0;
        framesN->fbPtr[2] = tmp_buf + dataSizePl0 + dataSizePl1;

        framesN->tSize[0] = dataSizePl0;
        framesN->tSize[1] = dataSizePl1;
        framesN->tSize[2] = dataSizePl2;

        framesN = framesN->next;
    }
}

void ipServerIspReportError(int32_t srcIdx, icSeverity severity, icError errorNo, void *userData)
{
    icEvent *intEv;
    intEv = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
    intEv->u.error.sourceInstance    = srcIdx;
    intEv->u.error.errorNo           = errorNo;
    intEv->u.error.severity          = severity;
    intEv->u.error.userData          = userData;
    intEv->ctrl                      = IC_EVENT_TYPE_ERROR;
    ipipeQueueSendToClientEvent(intEv);
}

void ipServerAddExtraZslFrames(icSourceInstance sourceId, uint32_t ZSLframesToAdd) {
    icEvent *intEv;
    FrameT* newFrame;
    uint32_t i;

    intEv = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
    if(gServerInfo.sourceServerCtrl[sourceId].pool) {
        if(ZSLframesToAdd>0) {
            gServerInfo.memFree = MemMgrGetFreeMem(ddrMemPoolId);
            uint32_t maxFrameSz = (pSrvIcCtrl->source[sourceId].sourceSetup.maxWidth *
                                   pSrvIcCtrl->source[sourceId].sourceSetup.maxHeight *
                                   pSrvIcCtrl->source[sourceId].sourceSetup.maxBpp) >> 3;// div by 8 size in bytes

            if (gServerInfo.memFree > ((maxFrameSz * ZSLframesToAdd) )){
                for(i=0; i<ZSLframesToAdd; i++){
                    newFrame = (FrameT*)MemMgrAlloc(cmxMemPoolId, sizeof(FrameT));
                    if(NULL == newFrame) return;
                    memset((void*)newFrame, 0, sizeof(FrameT));
                    ipServerFrameMgrAddBuffs(newFrame,
                            maxFrameSz,
                            0, 0);
                    FrameMgrAddFrameToPool(gServerInfo.sourceServerCtrl[sourceId].pool, newFrame);
                    pSrvIcCtrl->icPipelineDescription.icQuerySource[sourceId]->zslNumFrames++;
                }
                intEv->u.addZSLresult.status = IC_ADD_ZSL_SUCCESS;
            }
            else{
                intEv->u.addZSLresult.status = IC_ADD_ZSL_NOT_ENOUGH_MEM;
            }
        }
        else {
            intEv->u.addZSLresult.status = IC_ADD_ZSL_FIND_STATUS;
        }
    }
    else{
        intEv->u.addZSLresult.status = IC_ADD_ZSL_BAD_STATE;
    }
    gServerInfo.memFree = MemMgrGetFreeMem(ddrMemPoolId);
    intEv->u.addZSLresult.memFree = gServerInfo.memFree;
    intEv->u.addZSLresult.totalNoOfZSLframes = pSrvIcCtrl->icPipelineDescription.icQuerySource[sourceId]->zslNumFrames;
    intEv->ctrl    = IC_EVENT_TYPE_ZSL_ADD_RESULT;
    ipipeQueueSendToClientEvent(intEv);
}

void ipServerSendUserMsgToLos(void *eventStruct, uint32_t id) {
    icEvent *intEv;
    intEv = ipipeQueueGetNextOutputSlot(&pSrvIcCtrl->eventQIn);
    intEv->u.generalEvent.eventStruct  = eventStruct;
    intEv->u.generalEvent.id           = id;
    intEv->ctrl                        = IC_EVENT_TYPE_USER;
    ipipeQueueSendToClientEvent(intEv);
}
