///
/// @file      PlgSource.h
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     PlgSource header.
///

#ifndef __PLG_SOURCE_H__
#define __PLG_SOURCE_H__

#include <Flic.h>
#include <Receiver.h>
#include <Sender.h>
#include <Opipe.h>
#include <OpipeApps.h>
#include <ImgFrame.h>
#include "ipipe.h"
#include "IspCommon.h"
#include "IspMsg.h"

#ifndef PLG_SRC_TIMEOUT_SEC
#define PLG_SRC_TIMEOUT_SEC 3
#endif

#ifndef MAX_ZSL_BUF_SIZE
#define MAX_ZSL_BUF_SIZE 2
#endif

//TBD: standardize this ...
typedef enum {
    PLG_STATS_IDLE    = 0,
    PLG_STATS_RUNNING = 1,
}PlgStatus;

typedef enum {
    PLG_SOURCE_NOTMADE = 0,
    PLG_SOURCE_CREATED = 1,
    PLG_SOURCE_INUSE   = 2
}PlgSourceStatus;

typedef enum {
    SOF_IDX_SRC = 0,
    EOF_IDX_SRC = 1,
    HIT_IDX_SRC = 2
}LineRefs;

//===========================================================
class PlgSource : public ThreadedPlugin
{
  public:
    MReceiver<ImgFramePtr> inO;  //frame from frame-pool
    MSender  <IspInputMsg> out;  //send frame to consumer
    MSender  <IspInputMsg> outStl;  //send frame to consumer

    SReceiver<IspCmdMsg> inCommand;
    MSender  <IspCmdMsg> outCommand;

    OpipeRx      pRx;
    MipiRxCfg    rxCfg; //used when source is a SIPP-RX block
    icMipiConfig mipiRxData;
    ImgFramePtr     outFrame;  //cur output frame
    ImgFramePtr     nullObj;   //for release
    PlgStatus    plgStatus;
    PlgSourceStatus status;

    icSourceConfig  *srcCfg;
    icSourceSetup   *srcSetup;
    uint32_t         outFmt;
    // config that will be attached to the current frame
    icIspConfig     *ispCfg;
    icIspConfig     *nextIspCfg;

    uint32_t  srcId;
    uint32_t  frameCnt;
    int       downshift; //Downshift data at receiver if necessary.

   //Specific component interface
    void   (*eofEvent)(void *plg, ImgFrame *frame); //End-Of-Frame
    void   (*sofEvent)(void *plg, ImgFrame *frame); //Start-Of-Frame
    void   (*hitEvent)(void *plg, ImgFrame *frame); //Line-Hit
    uint32_t lineHitNo;

    void    *threadFunc(void *);
    void     Create(icSourceInstance instId);
    void     Delete();
    void     SetLineHit(uint32_t lineNo);
    void*    srcIrqHandlerFunc(void*);
    pthread_t srcThread;
    uint32_t  threadOn;
  private:
    ImgFrame *iImg;
    uint32_t strIdx;
    static void sofEventSrc(void *plg, ImgFrame *frame); //Start-Of-Frame
    static void eofEventSrc(void *plg, ImgFrame *frame); //End-Of-Frame
    static void lnHitEventSrc(void *plg, ImgFrame *frame); //lineHit
    void  mipiPhyCfg(icSourceConfig *mipiRxConfiguration, PlgSource *obj);
    void  stopSrc(void);
    void  startSourceThread(void);
    void  releaseAllFrms();
    static void* srcIrqHandlerFuncGenCal(void *This);

    // zsl capability
    void init();
    int allZslBufsSize;
    int circBufSize;
    ImgFramePtr imgCircBuf[MAX_ZSL_BUF_SIZE];
    int lockBuffSize;
    ImgFramePtr imgLockBufs[MAX_ZSL_BUF_SIZE];
    ImgFramePtr* queSearchLockFrame(uint64_t frameSel, int32_t tsRel);
};

#endif
