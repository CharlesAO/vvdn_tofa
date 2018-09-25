#include <string.h>
#include <DrvTimer.h>
#include <assert.h>
#include "TimeSyncMgr.h"
#include "PlgSrcIspApi.h"
#include "FrameMgrApi.h"
#include "Opipe.h"
#include "OpipeApps.h"
#include "PlgSrcPipeDef.h"

/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/

extern volatile uint32_t ispfc;
//##################################################################################
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

static void srcIspSetSize(OpipeRxIsp *o, uint32_t iWidth, uint32_t iHeight)
{
    o->p.width = iWidth + 1;//force rescale
    OpipeSetRes(&o->p, iWidth, iHeight);//update width2/height2 first

  //Adjust UPFIRDN1 In/Out cmx.base addr
  //Adjust UPFIRDN1 input reference (MUST map on LUT's view of PlaneStride)
   o->pInUV->cmx.base  = o->pOutLut->cmx.base + o->pOutLut->cmx.height * o->p.width2;
  //Adjust UPFIRDN1 output (UV follow after UV plane)
   o->pOutUV->cmx.base = o->pOutY->cmx.base   + o->pOutY->cmx.height   * o->pOutY->cmx.lineStride;
}
//##################################################################################
static void srcIspSetParams(PlgSrcIsp *obj)
{
    Opipe       *p  = &obj->pRxIsp.p;
    icIspConfig *ic =  obj->curIspCfg;

    fetchIcIspConfigLscLocal(p, ic); //icIspConfig -> Opipe translation
#ifndef SRC_ISP_AUTOSCALE // there is possible to not want to use internal factors
    computePolyfirParams(&obj->scale, ic);
#endif

    //Default CFG words:
    OpipeDefaultCfg(p, SIPP_SIGMA_ID  );
    OpipeDefaultCfg(p, SIPP_RAW_ID    );
    OpipeDefaultCfg(p, SIPP_DBYR_ID   );
    OpipeDefaultCfg(p, SIPP_DOGL_ID   );
    OpipeDefaultCfg(p, SIPP_SHARPEN_ID);
    OpipeDefaultCfg(p, SIPP_CGEN_ID   );
    OpipeDefaultCfg(p, SIPP_MED_ID    );
    OpipeDefaultCfg(p, SIPP_CHROMA_ID );
    OpipeDefaultCfg(p, SIPP_CC_ID     );
    OpipeDefaultCfg(p, SIPP_LUT_ID    );

  //Temporary check for patches
  // assert(ic->dog.strength == 0); //fast perf
  // assert((((uint32_t)ic->gamma.table)>>28) == 0x7); //lut must be in CMX
}

//##################################################################################
//We're guaranteed that we have a valid ISP config ready !!!
static void PrepareParams(PlgSrcIsp *obj)
{

     obj->curIspCfg = obj->nxtIspCfg;
     obj->nxtIspCfg = NULL; //mark we're done with this !

   //Fetch ISP params:
     srcIspSetParams(obj);
     srcIspSetSize  (&obj->pRxIsp, obj->frmSz.w, obj->frmSz.h);
}

//##################################################################################
//Pipe will start if: config AND output frames exist
void TryStartOpipe(PlgSrcIsp *obj)
{
   if(obj->nxtIspCfg)
   {
       FrameT *nxtOutFrm = FrameMgrAcquireFrame(obj->outputPools);
       if(nxtOutFrm)
       {
           uint32_t W,H;

          PrepareParams(obj); //need to compute out resolution first

          nxtOutFrm->appSpecificData = obj->curIspCfg;

          W = (obj->frmSz.w * obj->curIspCfg->updnCfg0.hN) / obj->curIspCfg->updnCfg0.hD;
          H = (obj->frmSz.h * obj->curIspCfg->updnCfg0.vN) / obj->curIspCfg->updnCfg0.vD;

          nxtOutFrm->tSize[3]          =  0;
          nxtOutFrm->tSize[0]          =  W*H;
          nxtOutFrm->tSize[1]          =  nxtOutFrm->tSize[0]/2;
          nxtOutFrm->tSize[2]          =  0;

//          nxtOutFrm->fbPtr[0]          =  nxtOutFrm->fbPtr[3] + nxtOutFrm->tSize[3];
          nxtOutFrm->fbPtr[1]          =  nxtOutFrm->fbPtr[0] + nxtOutFrm->tSize[0];
          nxtOutFrm->fbPtr[2]          =  NULL;

          obj->pRxIsp.pOutY ->ddr.base = (uint32_t)nxtOutFrm->fbPtr[0];
          obj->pRxIsp.pOutUV->ddr.base = (uint32_t)nxtOutFrm->fbPtr[1];
          obj->frame                   = nxtOutFrm;
          obj->pRxIsp.p.params[0]      = (void*)obj;
          obj->pRxIsp.p.params[1]      = (void*)nxtOutFrm;

          obj->rxCfg.sel01 = 0xF<<8 | obj->downshift;

          OpipeStart(&obj->pRxIsp.p);
          return;
       }
       else {
           if(obj->procesIspError) obj->procesIspError((void*)obj, ERR_PLGSRCISP_NO_OUT_BUF);
       }
   }
   else {
       if(obj->procesIspError) obj->procesIspError((void*)obj, ERR_PLGSRCISP_NO_ISP_CFG);
   }

    //Dummy sink (RX output disabled, no data gets written to CMX)
    obj->pRxSkip.p.params[0] = (void*)obj;
    obj->pRxSkip.p.params[1] = NULL;
    obj->rxCfg.sel01         = 0;  //sippRx WRITE disabled
    OpipeStart(&obj->pRxSkip.p);
}

//##################################################################################
void plgSrcIspOpipeAlloc(PlgSrcIsp *obj, MemPoolId id, icSourceSetup *src)
{
    OpipeRxIsp *o   = &obj->pRxIsp;
    uint32_t  wMain = src->maxWidth + 8; //+8 for padding purposes !!!

    o->cBufDbyrY.h = DBYR_Y_H;
    o->cBufSharp.h = SHARP_Y_H;
    o->cBufLut.h   = LUT_H;
    o->cBufPoly.h  = O_CBUFF_H;

  #if defined(MIPI_CMX_SIGMA)
    o->cBufMipiOut.h    = 2*IRQ_RATE;
    o->cBufMipiOut.base = (uint32_t)MemMgrAlloc(id, o->cBufMipiOut.h * wMain * 2); // bpp:2
  #endif

    o->cBufDbyrY.base = (uint32_t)MemMgrAlloc(id, o->cBufDbyrY.h * wMain * 2); // bpp:2(fp16)
    o->cBufSharp.base = (uint32_t)MemMgrAlloc(id, o->cBufSharp.h * wMain * 2); // bpp:2(fp16)
    o->cBufLut  .base = (uint32_t)MemMgrAlloc(id, o->cBufLut  .h * wMain * 3); // bpp:1(  u8) x 3 planes
    o->cBufPoly .base = (uint32_t)MemMgrAlloc(id, o->cBufPoly .h * wMain * 2); // bpp:1(  u8), but YUV (U,V ~ Y in size), thus 2x
  //TBD: derive wPoly properly based on source maxN, maxD
}

//##################################################################################
//Opipe line hit callback;
//check line indices and trigger appropriate Plugin callbacks
static void cbOpipeLineHit(Opipe *p)
{
    PlgSrcIsp *plg    = (PlgSrcIsp *)p->params[0];
    FrameT    *oFrame = (FrameT    *)p->params[1];

  //Note: moved SOF event in TryStartOpipe
    if(plg->sofEvent && (p->currHitLine == p->targetLine[SOF_IDX]))
        if(plg->sofEvent) plg->sofEvent((void*)plg, oFrame);
    if(plg->hitEvent && (p->currHitLine == p->targetLine[HIT_IDX]))
      {plg->hitEvent((void*)plg, oFrame);        }
}
//##################################################################################
//Opipe EOF callback: adjust associated Plugin Frame buffers
static void opipeIspEof(Opipe *p)
{
    PlgSrcIsp *obj = (PlgSrcIsp *)p->params[0];

    if(PLG_STATS_IDLE == obj->plg.status){
        // source was already stopped, this handler was call just because
        // fini happens by the time of dma interrupt happens
        // so nothing to be done here
        return;
    }
    if (1 == ispfc) {
        ispfc = 2;
        return;
    }
    // Statistics copied back to ddr. Start dma task
    icIspConfig * crtIspCfg = obj->curIspCfg;
    assert(crtIspCfg);
    if((crtIspCfg->aeAwbStats)||(crtIspCfg->afStats)) {
        plgIspStartStatsCopy(p, crtIspCfg->aeAwbStats, crtIspCfg->afStats);
    }
    FrameT *oFrame = (FrameT *)p->params[1]; //recently produced frame
    assert(oFrame != 0); //pipe was started with valid oBuf, so ...

    oFrame->stride[0] = p->pUpfirdn0Cfg->oW;
    oFrame->height[0] = p->pUpfirdn0Cfg->oH;
    oFrame->stride[1] = p->pUpfirdn12Cfg->oW<<1;//*2 (got 2 UV planes)
    oFrame->height[1] = p->pUpfirdn12Cfg->oH;

    TryStartOpipe(obj); //kick next frame

    //Timestamp recently produced frame
    FrameMgrAndAddTimeStamp(oFrame, TimeSyncMsgGetTimeUs());

    if(obj->eofEvent)
       obj->eofEvent((void*)obj, oFrame);

    FrameMgrProduceFrame   (oFrame); //update time info
}

//##################################################################################
//EOF routine for when we skipped current frame
static void opipeIspEof2(Opipe *p)
{
    PlgSrcIsp *obj = (PlgSrcIsp *)p->params[0];

    if(PLG_STATS_IDLE == obj->plg.status){
        // source was already stopped, this handler was call just because
        // fini happens by the time of dma interrupt happens
        // so nothing to be done here
        return;
    }

    if (1 == ispfc) {
        ispfc = 2;
        return;
    }
    TryStartOpipe(obj);

    //Must invoke EOF even if current frame was skipped, to notify users...
    if(obj->eofEvent)
       obj->eofEvent((void*)obj, NULL);
}

//##################################################################################
//OpipeRx creation and initial configuration
void plgSrcIspCreateOpipe(PlgSrcIsp *obj, icSourceConfig  *sCfg)
{
    OpipeRxIsp *pRxIsp  = &obj->pRxIsp;
    OpipeRx    *pRxSkip = &obj->pRxSkip;
    MipiRxCfg  *rxCfg   = &obj->rxCfg;

    //Figure out resolution based on crop window
    uint32_t stX  = sCfg->cropWindow.x1;
    uint32_t stY  = sCfg->cropWindow.y1;
    uint32_t imgW = sCfg->cropWindow.x2 - sCfg->cropWindow.x1;
    uint32_t imgH = sCfg->cropWindow.y2 - sCfg->cropWindow.y1;

    obj->frmSz.w = imgW; //remember resolution
    obj->frmSz.h = imgH;

    //Figure out sipp ID:
    uint32_t rxNo = sCfg->mipiRxData.recNrl - IC_SIPP_DEVICE0;
    uint32_t rxID = SIPP_MIPI_RX0_ID + rxNo;

    if (PLG_SRC_ISP_CREATED == obj->status) {
       //At this stage, the circular buffer MUST have been allocated
        assert(pRxIsp->cBufDbyrY.base != 0);
        assert(pRxIsp->cBufSharp.base != 0);
        assert(pRxIsp->cBufLut.base   != 0);
        assert(pRxIsp->cBufPoly.base  != 0);

        OpipeCreateRxIsp(pRxIsp,  rxID);
        OpipeCreateRx   (pRxSkip, imgW, imgH, rxID, 2, 0, 16);

        initLutCopyTask();
        initLscCopyTask();
        initStatsCopyTask();
        extern uint8_t gammaCopyWait;
        gammaCopyWait = 0; //CRITICAL not to wait on DMA-done in IRQ

        obj->status = PLG_SRC_ISP_INUSE;
        //Opipe Line-hit callbacks for current RX unit
        pRxIsp->p.cbLineHit           = cbOpipeLineHit;
        pRxIsp->p.triggerSinkId       = SIPP_UPFIRDN0_ID;
        pRxIsp->p.targetLine[SOF_IDX] = IRQ_RATE;

        pRxSkip->p.cbLineHit           = cbOpipeLineHit;
        pRxSkip->p.triggerSinkId       = rxID;
        pRxSkip->p.targetLine[SOF_IDX] = IRQ_RATE;
    }
    else {
        if(PLG_SRC_ISP_INUSE == obj->status) {
            //Assume that change resolution, so update it
            //Resolution updates (if needed)
            OpipeSetRes(&pRxIsp ->p, imgW, imgH);
            OpipeSetRes(&pRxSkip->p, imgW, imgH);
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
    pRxIsp->p.pMipiRxCfg[rxNo]  = rxCfg;
    pRxIsp->p.cbEndOfFrame      = opipeIspEof;

    pRxSkip->p.pMipiRxCfg[rxNo] = rxCfg;
    pRxSkip->p.cbEndOfFrame     = opipeIspEof2;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Configs must be set BEFORE starting the filter !!!
    pRxIsp ->p.rawBits      = sCfg->bitsPerPixel;
    pRxIsp ->p.bayerPattern = sCfg->bayerFormat;
    pRxSkip->p.rawBits      = sCfg->bitsPerPixel;
    pRxSkip->p.bayerPattern = sCfg->bayerFormat;

    //single input bayer plane maps on window-0
    #define RX_WIN 0
    rxCfg->xStartWidth [RX_WIN] = (imgW<<16) | stX;
    rxCfg->yStartHeight[RX_WIN] = (imgH<<16) | stY;

    // Receiver can optionally downshift.
    obj->downshift = mipiDataTypeBpp(sCfg->mipiRxData.dataType) - sCfg->bitsPerPixel;
    assert(obj->downshift >= 0); //TODO: Handle promotion.

    rxCfg->mask[RX_WIN]         = (1<<sCfg->bitsPerPixel)-1;
}
