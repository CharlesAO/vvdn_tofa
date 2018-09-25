/**************************************************************************************************

 @File         : IpipeServerApi.h
 @Author       : MT
 @Brief        : Containing Ipipe2 server Lrt apy size
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :
 **************************************************************************************************/
#ifndef _IPIPE_SERVER_API_H_
#define _IPIPE_SERVER_API_H_


/**************************************************************************************************
 ~~~  Include standard types And General Plug-in used module
 **************************************************************************************************/
#include "ipipe.h"
#include "ipipeMsgQueue.h"
#include "IspCommon.h"

/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/

#ifndef MAX_NR_OF_CAPTURE_PENDING
#define MAX_NR_OF_CAPTURE_PENDING 8
#endif

/**************************************************************************************************
 ~~~  Basic project types definition
 **************************************************************************************************/
typedef struct {
    FrameT              *buffer;
    void                *config;
    icSourceInstance    source;
}TriggerCaptElement;

typedef struct {
    TriggerCaptElement  queue[MAX_NR_OF_CAPTURE_PENDING];
    int32_t queueIn;
    int32_t queueOut;
} TrigerCaptQue;

typedef struct {
    void            (*cbStartSource)(uint32_t sourceInstance, icSourceConfig  *sourceConfig);
    void            (*cbStopSource) (uint32_t sourceInstance);
    void            (*cbCfgDynamic) (uint32_t sourceInstance, icSourceConfigDynamic *dynCfg);
    FramePool       *pool; // frame pool associated with source, lock is defined just on sources
    IcQuerySource   sourceDescription;
} SourceServerCtrlT;

typedef struct {
    void (*cbConfigPlugin)(uint32_t ispInstance, void *cfg);
} PluginServerCtrl;

typedef struct {
    void                    (*cbIcSetup)(icCtrl *ctrl);
    void                    (*cbIcReset)(void);
    void                    (*cbIcTearDown)(void);
    SourceServerCtrlT       sourceServerCtrl[IPIPE_MAX_SOURCES_ALLOWED];
    PluginServerCtrl        pluginServerCtrl[IPIPE_MAX_ISP_ALlOWED];
    TrigerCaptQue           trigerCaptQue;
    icStatusCode            (*cbSourcesCommit)(icCtrl *ctrl);
    void                    (*cbDataWasSent) (FrameT * dataBuffer);
    uint32_t                memFree;
    void                    (*cbIcUserMsg)(void *eventStruct, uint32_t id);
} ipipeServerInfo;


/**************************************************************************************************
 ~~~  Exported Functions
 **************************************************************************************************/

extern volatile ipipeServerInfo  gServerInfo;
extern          icCtrl          *pSrvIcCtrl;

void setupIpipeServer();
void ipServerWasReset(void);
void ipServerWasTornDown(void);
void ipipeServerHandleInputEvent(icEvent *ev);
void ipServerSendData(FrameT *frame, uint32_t outId);
void ipServerSourceReady(icSourceInstance source);
void ipServerSourceStopped(icSourceInstance source);
void ipServerReadoutStart(icSourceInstance source, void * userData, uint32_t seqNo, icTimestamp  ts);
void ipServerLineHit(icSourceInstance source, void * userData, uint32_t seqNo, icTimestamp ts);
void ipServerReadoutEnd(icSourceInstance source, void * userData, uint32_t seqNo, icTimestamp  ts);
void ipServerIspStart(int32_t ispIdx, uint32_t seqNo, void *userData);
void ipServerIspEnd(int32_t ispIdx, uint32_t seqNo, void *userData);
void ipServerIspConfigAccepted(int32_t ispIdx, uint32_t seqNo, void *userData);
void ipServerIspStatsReady(int32_t ispIdx, uint32_t seqNo, void *userData);
void ipServerFrameLocked(icSourceInstance source, FrameT *frame);
int ipServerQueueGet(TriggerCaptElement **old);
void ipServerIspReportError(int32_t srcIdx, icSeverity severity, icError errorNo, void *userData);
// frames manager server capability
FrameT* ipServerFrameMgrCreateList(uint32_t nrOfFrame);
void ipServerFrameMgrAddBuffs(FrameT* frameList,
        uint32_t dataSizePl0, uint32_t dataSizePl1, uint32_t dataSizePl2);

void ipServerRegSourceQuery(uint32_t id, const char  *name, uint32_t attrs, uint32_t nrOfFramesInZsl,
        uint32_t outIdVid, uint32_t outIdStill, uint32_t outIdRaw, uint32_t outIdBin);
void ipServerRegIspQuery(uint32_t id, const char  *name, uint32_t attrs, uint32_t sourceId);
void ipServerRegOutputQuery(uint32_t id, const char  *name, uint32_t attrs, uint32_t dependentSources);
void ipServerRegUserPlgQuery(uint32_t id, const char  *name, uint32_t attrs);
void ipServerQueryAddChild(void* parentDescP, void* childDescP);
void ipServerSendUserMsgToLos(void *eventStruct, uint32_t id);

#endif
