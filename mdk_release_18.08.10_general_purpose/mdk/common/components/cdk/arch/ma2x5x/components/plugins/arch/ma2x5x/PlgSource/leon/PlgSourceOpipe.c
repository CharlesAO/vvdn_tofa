/**************************************************************************************************

 @File         : PlgSourceOpipe.c
 @Author       : AL
 @Brief        : Contain sippRx source side
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :

 **************************************************************************************************/

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include <string.h>
#include <DrvTimer.h>
#include <assert.h>
#include <swcLeonUtils.h>
#include "TimeSyncMgr.h"
#include "ipipeDbg.h"
#include "ipipeUtils.h"
#include "PlgSourceApi.h"
#include "FrameMgrApi.h"
#include "MemMgrApi.h"
#include "Opipe.h"
#include "OpipeApps.h"

/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/
#ifndef RX_CBUF_MUL_H //Opipe specific
#define RX_CBUF_MUL_H 5
#endif

//Required CMX circular buffer height
#define RX_CBUFF_H (RX_CBUF_MUL_H*IRQ_RATE)

/**************************************************************************************************
 ~~~  Local variables
 **************************************************************************************************/

//SippRx pointers to circular output buffers in CMX
//out of 6 sources, we can have max 4 pointer set as there are 4 RX
uint8_t *cSippRx[IPIPE_MAX_SOURCES_ALLOWED] = {0,0,0,0,0,0};


/**************************************************************************************************
 ~~~ Functions Implementation
 **************************************************************************************************/
static int mipiDataTypeBpp(int dataType){
    switch (dataType){
        case IC_IPIPE_RAW_10:
            return 10;
        case IC_IPIPE_RAW_12:
            return 12;
        case IC_IPIPE_RAW_14:
            return 14;
        default:
            //Unsupported data type.
            assert(0);
            break;
    }
}

void plgSrcOpipeAlloc(uint32_t no, MemPoolId id, icSourceSetup *src)
{
    uint32_t lineSz = lineWidthInBytes(src->maxWidth, src->maxBpp);
  //CRITICAL: within this buffer, lines get aligned (to 8 Bytes as SIPP requires)
  //          so +8 on line size
    lineSz     += 8;
    cSippRx[no] = (uint8_t*)MemMgrAlloc(id, RX_CBUFF_H * lineSz);
}

//Opipe line hit callback;
//check line indices and trigger appropriate Plugin callbacks
void cbOpipeLineHit(Opipe *p)
{
  PlgSource *plg    = (PlgSource *)p->params[0];
  FrameT    *oFrame = (FrameT    *)p->params[1];

  if(plg->sofEvent && (p->currHitLine == p->targetLine[SOF_IDX_SRC]))
    {plg->sofEvent((void*)plg, oFrame); return;}

  if(plg->hitEvent && (p->currHitLine == p->targetLine[HIT_IDX_SRC]))
    {plg->hitEvent((void*)plg, oFrame);        }
}

//Opipe EOF callback: adjust associated Plugin Frame buffers
static void opipeIspEof(Opipe *p)
{
    PlgSource *obj = (PlgSource *)p->params[0];

    if(PLG_STATS_IDLE == obj->plg.status){
        // source was already stopped, this handler was call just because
        // fini happens by the time of dma interrupt happens
        // so nothing to be done here
        return;
    }
    FrameT    *oFrame = (FrameT    *)p->params[1];
    FrameT *nxtOutFrm = FrameMgrAcquireFrame(obj->outputPools);
    p->params[0] = (void*)obj;
    //Get new output frame for next run...
    p->params[1] = (void*)nxtOutFrm;
    obj->frame   = nxtOutFrm;
    if(nxtOutFrm){
        obj->rxCfg.sel01 = 0xF<<8 | obj->downshift;
        obj->pRx.pOut->ddr.base = (uint32_t)nxtOutFrm->fbPtr[0];
    }
    else {
        obj->rxCfg.sel01 = 0;
    }

    #if 0
   //Optimization: ditch cbLineHit if no callbacks are enabled
    if((obj->sofEvent == NULL) &&
       (obj->hitEvent == NULL) &&
       (obj->eofEvent == NULL))
      pRx->p.cbLineHit = NULL;
    #endif

    //Starting new frame (nxtOutFrm)
    OpipeStart(p);

    //Timestamp recently produced frame
    if(oFrame){ //handle SKIP-ed frames
       FrameMgrAndAddTimeStamp(oFrame, TimeSyncMsgGetTimeUs());
    }
    //Must invoke EOF even if current frame was skipped, to notify users...
    if(obj->eofEvent)
       obj->eofEvent((void*)obj, oFrame);

    //New frame produced, updating time informations
    if(oFrame){ //handle SKIP-ed frames
        FrameMgrProduceFrame   (oFrame);
    }
}

//OpipeRx creation and initial configuration
void plgSrcCreateRxOpipe(PlgSource *obj, icSourceConfig  *sCfg, uint32_t outFmt)
{
    OpipeRx   *pRx   = &obj->pRx;
    MipiRxCfg *rxCfg = &obj->rxCfg;

    //Figure out resolution based on crop window
    uint32_t stX  = sCfg->cropWindow.x1;
    uint32_t stY  = sCfg->cropWindow.y1;
    uint32_t imgW = sCfg->cropWindow.x2 - sCfg->cropWindow.x1;
    uint32_t imgH = sCfg->cropWindow.y2 - sCfg->cropWindow.y1;

    //Figure out sipp ID:
    uint32_t rxNo = sCfg->mipiRxData.recNrl - IC_SIPP_DEVICE0;
    uint32_t rxID = SIPP_MIPI_RX0_ID + rxNo;

    if (PLG_SOURCE_CREATED == obj->status) {
       //At this stage, the circular buffer MUST have been allocated
        assert(cSippRx[obj->srcId] != 0);
        OpipeCreateRx(pRx, imgW, imgH,
                      rxID, outFmt,
                      (uint32_t)cSippRx[obj->srcId], RX_CBUFF_H);
        obj->status = PLG_SOURCE_INUSE;
        //Opipe Line-hit callbacks for current RX unit
        pRx->p.cbLineHit           = cbOpipeLineHit;
        pRx->p.triggerSinkId       = rxID;
        pRx->p.targetLine[SOF_IDX_SRC] = IRQ_RATE;
    }
    else {
        if(PLG_SOURCE_INUSE == obj->status) {
            //Assume that change resolution, so update it
            //Resolution updates (if needed)
            OpipeSetRes(&pRx->p, imgW, imgH);
            // not possible to change board design and to assign other source to other receiver
            assert(sCfg->mipiRxData.recNrl == obj->mipiRxData.recNrl);
        }
        else {
            // try to start a source but not create is first,
            // or a memory corruption overwrite plug-in structure
            assert(0);
        }
    }

    //Set param pointers after create (i.e. post OpipeInit)
    pRx->p.pMipiRxCfg[rxNo] = rxCfg;
    pRx->p.cbEndOfFrame     = opipeIspEof;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Configs must be set BEFORE starting the filter !!!
    pRx->p.rawBits      = sCfg->bitsPerPixel;
    pRx->p.bayerPattern = sCfg->bayerFormat;

    //Enable packed output
    if((outFmt == SIPP_FMT_PACK10)||(outFmt == SIPP_FMT_PACK12))
    rxCfg->cfg                  = 1<<3;

    //single input bayer plane maps on window-0
    #define RX_WIN 0
    rxCfg->xStartWidth [RX_WIN] = (imgW<<16) | stX;
    rxCfg->yStartHeight[RX_WIN] = (imgH<<16) | stY;
    rxCfg->sel01                =  0xF<<8; //WIN0 selection : all even/odd lines and columns

    // Receiver can optionally downshift.
    obj->downshift = mipiDataTypeBpp(sCfg->mipiRxData.dataType) - sCfg->bitsPerPixel;
    assert(obj->downshift >= 0); //TODO: Handle promotion.

    rxCfg->mask[RX_WIN]         = (1<<sCfg->bitsPerPixel)-1;
}
