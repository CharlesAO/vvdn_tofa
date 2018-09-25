///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
/// @brief     Opipe - runtime
///

#include <DrvIcb.h>
#include "Opipe.h"
#include "OpipeInternals.h"
#include "OpipeDefs.h"

extern OpipeGlobal gOpipe; //used by ISRs

//Given remaining(left) lines to be processed, figure out how many we can do in next step
//(a step contains up to IRQ_RATE lines, and it generally is IRQ_RATE lines, exception
// is at bottom of the frame when image height is not multiple of IRQ_RATE)
#define GET_N_LINES(left, I_RATE) ((left >= I_RATE) ? I_RATE : left)

//#################################################################################################
//Return next descriptor in cicular fashion:
dmaTransactionList* getNextDmaDsc(DBuffer *db)
{
    dmaTransactionList* ret = &db->ddr.dmaDsc[db->ddr.curDsc];

    db->ddr.curDsc++;
    if(db->ddr.curDsc >= N_DESCS)
       db->ddr.curDsc  = 0;

    return ret;
}

//#################################################################################################
//Internal usage: when missed interrupts are detected terminate pipe to avoid permanent hang
#if defined(OPIPE_RUNTIME_CHECKS)
static void OpipeTerminate(Opipe *o)
{
    o->flags &= ~(PIPE_RUNNING); //what OpipeFinished would do
    //+disable filters, they get renabled at start
    o->nMissedIrq++;

    if(o->cbEndOfFrame)
       o->cbEndOfFrame(o);
}
#endif

//#################################################################################################
//When a DMA-out finishes the transaction, decrement corresponding SIPP filter OBFL
//#################################################################################################
void dmaSinkIrqHandler(dmaTransactionList* ListPtr, void* userContext)
{
    uint32_t  k;
    DBuffer  *sink   = (DBuffer*)userContext;
    Opipe    *o      = (Opipe*)sink->pipeRef;
    uint32_t  nLines = ListPtr->userData0; //how many lines were transfered with this desc

   #if defined(TAPOUT_EN)
   //Purpose: last of (Sink or SwLink) need to decrement the common HW parent.
   //If Sink got no sibling swLink, or we got one but doneCnt is at least >= current doneCnt,
   //then can decrement associated parent filter; it means DMA is last consumer
    sink->doneCnt++;
    uint32_t canDec = (sink->link == NULL) ? 1 : (sink->link->doneCnt >= sink->doneCnt);
    if(canDec)
   #endif
    {
      for(k=0; k<nLines; k++)
       SET_REG_WORD(SIPP_OBFL_DEC_ADR, 1<<sink->unitID);
    }

    sink->ddr.cpLines += nLines;

   #if defined(OPIPE_RUNTIME_CHECKS)
    o->nDmaPending--;

    if((o->flags & LOST_IRQ) && (o->nDmaPending == 0))
    {
        OpipeTerminate(o);
        return;
    }
   #endif

   //If current sink finished, check for Pipe Finish
    if(sink->ddr.cpLines >= sink->ddr.height)
    {
      if(o->cbEndOfFrame)
       if(OpipeFinished(o))
         o->cbEndOfFrame(o);
    }
}

#if defined(OPIPE_CROP)
//#################################################################################################
//return 1 if current sink does NOT crop
static int SinkDoesNotCrop (DBuffer *sink)
{
    if(sink->crop.h == 0)  return 1;
    else                   return 0;
}

//#################################################################################################
//Returns number of lines to copy to output buffer
static int CropTransactions(DBuffer *sink, dmaTransactionList *dmaD)
{
   //Figure out numLines and offset
    uint16_t nOutLines = 0; //no DMA-out by default
    uint16_t lnOffset  = 0; //default value

   //Crop isn't enabled for this sink if (Height == 0)
    if(sink->crop.h == 0)
       return 0;

  //TBD: factor out some of the terms, see if generated code improves !

  //[First transaction]
    if((sink->crop.tlcY >= sink->ddr.lineNo                ) &&
       (sink->crop.tlcY <  sink->ddr.lineNo + sink->irqRate))
    {
        nOutLines = sink->ddr.lineNo + sink->irqRate - sink->crop.tlcY;
        lnOffset  = sink->irqRate - nOutLines;
    }
  //[Last transaction]
    else if((sink->crop.tlcY + sink->crop.h >= sink->ddr.lineNo                ) &&
            (sink->crop.tlcY + sink->crop.h <  sink->ddr.lineNo + sink->irqRate))
    {
        nOutLines = sink->crop.tlcY + sink->crop.h - sink->ddr.lineNo;
      //Note: lnOffset remains ZERO
    }
  //[Inner transaction]
    else if((sink->ddr.lineNo > sink->crop.tlcY               ) &&
            (sink->ddr.lineNo < sink->crop.tlcY + sink->crop.h))
    {
        nOutLines = sink->irqRate;
      //Note: lnOffset remains ZERO
    }

    if(nOutLines){
      //Patch descriptor
       dmaD->dst = (void*)(sink->ddr.base + (sink->crop.lineNo          )*sink->crop.lStride);
       dmaD->src = (void*)(sink->cmx.base + (sink->cmx.lineNo + lnOffset)*sink->cmx.lineStride + sink->crop.tlcX);
       dmaD->dst_stride = sink->crop.lStride;
       dmaD->dst_width  = sink->crop.w;
       dmaD->src_width  = sink->crop.w;
       dmaD->length     = sink->crop.w * nOutLines;
       dmaD->dst_plane_stride = sink->crop.pStride;
       sink->crop.lineNo += nOutLines;//progress for next run
      //Note: dmaD->userData0 (nLines) remains as is as it's used to decrement producer BFL
      //Note: DMA gets stared when exiting this routine
    }
    else{
      //Do NOT start DMA as there's nothing to copy !!!
      //Just call "dmaSinkIrqHandler" to finish up default logic, decrement HW BFL
      #if defined(OPIPE_RUNTIME_CHECKS)
       o->nDmaPending++;
      #endif
       dmaSinkIrqHandler(dmaD, sink);
    }

  //printInt((nOutLines<<16) | lnOffset);
    return nOutLines;
}
#endif //defined(OPIPE_CROP)

//#################################################################################################
//re http://dub30/bugzilla/show_bug.cgi?id=22709, due to the order in which EOF and OBFL_INC
//get serviced, always check that a sink didn't already fisish.
//It's possible for EOF to be serviced after OBFL_INC irq but OBFL_INC irq already sovled the sink.
void handleObflInc(uint32_t status)
{
   uint32_t i, k;

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //loop through all [SW-LINK-PRODUCERS]
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  for(i=0; i<gOpipe.nSwLinks; i++)
  {
    SwLink *swLnk = gOpipe.swCtrl[i];
    Opipe   *o    = (Opipe*)swLnk->pipeRef;
    uint32_t left = swLnk->prodMon.height - swLnk->prodMon.curLine;  //remaining till EOF

   //Skip if no lines left or pipe inactive
    if(!left                     ) continue;
    if(!(o->flags & PIPE_RUNNING)) continue;

    if(status & (1<<swLnk->prodId))
    {
        #if defined(TAPOUT_EN)
        // Only clear parent IRQ if sibling sink is not set;
        // else we need to let message reach SINKS loop below
        if(swLnk->sink == NULL)
        #endif
        {
          SET_REG_WORD(SIPP_INT1_CLEAR_ADR, 1<<swLnk->prodId); //clear flag
        }

        uint32_t nLines = GET_N_LINES(left, swLnk->prodMon.irqRate); //num lines produced by DBYR
        swLnk->prodMon.curLine += nLines;

     #if defined(SW_LINK_EXT)
       if(swLnk->cbIbflIncHook)                //start inner block, when it's done, only then will
          swLnk->cbIbflIncHook(swLnk, nLines); //increment SW-Link consumers IBFL-s
       else
     #endif
       for(k=0; k<nLines; k++)
         SET_REG_WORD(SIPP_IBFL_INC_ADR, swLnk->allConsIdMask);
    }
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //loop through all SINKS from all pipes
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  for(i=0; i<gOpipe.nSinks; i++)
  {
     DBuffer *sink = gOpipe.sinks[i];
     Opipe *o      = (Opipe*)sink->pipeRef;
     uint32_t left = sink->ddr.height - sink->ddr.lineNo;  //remaining till EOF

   //Current sink already finished, skip
    if(!left) continue;

   //If current sink belongs to an inactive pipe, skip
    if(!(o->flags & PIPE_RUNNING))
       continue;

   //If current sink fired
    if(status & (1<<sink->unitID))
    {
     //Current sink fired:
      SET_REG_WORD(SIPP_INT1_CLEAR_ADR, 1<<sink->unitID);

     //Figure out how many lines we need to copy out
      uint32_t nLines = GET_N_LINES(left,sink->irqRate); //transferred in current kick

    #if defined(OPIPE_RUNTIME_CHECKS)
      sink->liloCnt += nLines;

     //If filter EOF (LO becomes HEIGHT, then quickly turns ZERO at EOF), but SW count is smaller,
     //then we missed data (Leon couldn't handle interrupts in time)
      k = (GET_REG_WORD_VAL(gSippNfo[sink->unitID].oCtx) & 0xFFFF);
      if((k == 0) && (sink->liloCnt != sink->ddr.height))
      {
        //Mark the missed IRQ situation and disable filters
          o->flags |= LOST_IRQ;
          CLR_REG_BITS_MASK(SIPP_CONTROL_ADR,o->enMask);

        //Clear all pending interrupts from this pipe
          SET_REG_WORD(SIPP_INT1_CLEAR_ADR, o->enMask);
          SET_REG_WORD(SIPP_INT2_CLEAR_ADR, o->enMask);
          status &= ~(o->enMask);

        //if DMAs aren't pending, we won't start new ones, can just kill all
          if(o->nDmaPending == 0)
             OpipeTerminate(o);

          continue; //move to next task (might be a sipp_rx that needs to be serviced
      }
    #endif

      dmaTransactionList *dmaD = getNextDmaDsc(sink);

     //Update DST/SRC member and start transaction (no wait)
      dmaD->dst = (void*)(sink->ddr.base + sink->ddr.lineNo * sink->ddr.lineStride);
      dmaD->src = (void*)(sink->cmx.base + sink->cmx.lineNo * sink->cmx.lineStride);
      dmaD->length    = dmaD->src_width * nLines;
      dmaD->userData0 = nLines;

     #if defined(OPIPE_CROP)
      if(SinkDoesNotCrop (sink) ||     //if sink doesn't crop, we always copy
         CropTransactions(sink, dmaD)) //if sink crops and have data to copy
     #else
      //else, always copy !
     #endif
     {
        #if defined(OPIPE_RUNTIME_CHECKS)
         o->nDmaPending++;
        #endif
         DrvCmxDmaStartTaskAsync(dmaD, dmaSinkIrqHandler, sink);
     }

     //Progress SRC/DST buffers for next run
      sink->ddr.lineNo += nLines;              //DST-ddr
      sink->cmx.lineNo += nLines;              //SRC-cmx
      if(sink->cmx.lineNo == sink->cmx.height) //SRC-cmx
         sink->cmx.lineNo = 0;                 //SRC-cmx

     #if !defined(OPIPE_CANCEL_LN_CB)
     //Line callbacks (after "lineNo" update, as reflects better the filter
     //state, i.e.: filter produced the lines, just not copied yet to DDR)
      {
        if((o->cbLineHit) && (o->triggerSinkId == sink->unitID))
         if((o->targetLine[0] == sink->ddr.lineNo)||
            (o->targetLine[1] == sink->ddr.lineNo)||
            (o->targetLine[2] == sink->ddr.lineNo)||
            (o->targetLine[3] == sink->ddr.lineNo))
           {
               o->currHitLine = sink->ddr.lineNo;
               o->cbLineHit(o);
           }
      }
     #endif
    }
  }
}

//#################################################################################################
//patch for http://dub30/bugzilla/show_bug.cgi?id=22709
void oPipeSippEofIsr(uint32_t irqSrc)
{
    uint32_t status2 = GET_REG_WORD_VAL(SIPP_INT2_STATUS_ADR);
    uint32_t status3 = status2; //backup original reg

  //At EOF, handle all sinks that aren't already finished (i.e. left != 0)
   if(status2 & SIPP_MIPI_RX_MASK) {
     //will clear mipiRX status bits if RX has se01 ==0, so that
     //handleObflInc doesn't try to handle this too
       void rxRofSkipAdvance(uint32_t *status);
       rxRofSkipAdvance(&status2);
   }
    handleObflInc(status2);
    SET_REG_WORD(SIPP_INT2_CLEAR_ADR, status3);
    DrvIcbIrqClear(irqSrc);
}

//#################################################################################################
//Sinks:
void oPipeSippObflIncIsr(uint32_t irqSrc)
{
  handleObflInc (GET_REG_WORD_VAL(SIPP_INT1_STATUS_ADR));
  DrvIcbIrqClear(irqSrc);
}

//#################################################################################################
//When a DMA-in finishes the transaction, increment corresponding SIPP filter IBFL
//#################################################################################################
void dmaSourceIrqHandler(dmaTransactionList* ListPtr, void* userContext)
{
    uint32_t  k;
    DBuffer  *src    = (DBuffer*)userContext;
    uint32_t  nLines = ListPtr->userData0;

    for(k=0; k<nLines; k++)
        SET_REG_WORD(SIPP_IBFL_INC_ADR, 1<<src->unitID);

    src->ddr.cpLines += nLines;

   #if defined(OPIPE_RUNTIME_CHECKS)
    ((Opipe*)src->pipeRef)->nDmaPending--;
   #endif
}

//#################################################################################################
//Sources
void oPipeSippIbflDecIsr(uint32_t irqSrc)
{
  uint32_t i, k, status;
  status = GET_REG_WORD_VAL(SIPP_INT0_STATUS_ADR);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //loop through all [SW-LINK-LAST_CONSUMMERs]
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  for(i=0; i<gOpipe.nSwLinks; i++)
  {
    SwLink *swLnk = gOpipe.swCtrl[i];
    Opipe  *o     = (Opipe*)swLnk->pipeRef;

   //If current swLink belongs to an inactive pipe, skip
     if(!(o->flags & PIPE_RUNNING)) continue;

    //In this case DOGL is the last DBYR.Y consumer, so DOGL will decrement DBYR.outFill
    //(might need to make this dynamic in future, i.e. DOGL or MED ?)
     if(status & (1<<swLnk->lastConsId))
     {
        SET_REG_WORD(SIPP_INT0_CLEAR_ADR, 1<<swLnk->lastConsId); //clear flag
      //Tell PRODUCER lines are consumed by decrementing its OBFL
        uint32_t left   = swLnk->lastConsMon.height - swLnk->lastConsMon.curLine;
        uint32_t nLines = GET_N_LINES(left,swLnk->lastConsMon.irqRate);
        swLnk->lastConsMon.curLine += nLines;

       #if defined(TAPOUT_EN)
       //Purpose: last of swLink.lastCons or DmaSink is to decrement parent OBFL
       //If no sibling sink exist, then Sink.lastCons is the unique consumer, and can dec parent
        swLnk->doneCnt++;
        uint32_t canDec = (swLnk->sink == NULL) ? 1 : (swLnk->sink->doneCnt >= swLnk->doneCnt);
        if(canDec)
       #endif
       {
         #if defined(SW_LINK_EXT)
          if(swLnk->cbObflDecHook)                //only later will when inner block finishes
             swLnk->cbObflDecHook(swLnk, nLines); //will decrement OBFL
          else
         #endif
          {
           for(k=0; k<nLines; k++)
             SET_REG_WORD(SIPP_OBFL_DEC_ADR, 1<<swLnk->prodId);
          }
       }
     }
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //loop through all sources from all pipes
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  for(i=0; i<gOpipe.nSources; i++)
  {
     DBuffer *src = gOpipe.sources[i];

   //If current source belongs to an inactive pipe, skip
     Opipe   *o = (Opipe*)src->pipeRef;
     if(!(o->flags & PIPE_RUNNING))
        continue;

     if(status & (1<<src->unitID))
     {//Current source fired:

      //Clear the source
       SET_REG_WORD(SIPP_INT0_CLEAR_ADR, 1<<src->unitID);

      //Re-Trigger new DMA-in if data is still to be fed
      if(src->ddr.lineNo < src->ddr.height)
      {
         uint32_t left   = src->ddr.height - src->ddr.lineNo; //remaining till EOF
         uint32_t nLines = GET_N_LINES(left, src->irqRate);   //transfered in current kick

         dmaTransactionList *dmaD = getNextDmaDsc(src);

        //Patch DST/SRC/LEN members and start transaction (no wait)
         dmaD->src = (void*)(src->ddr.base + src->ddr.lineNo * src->ddr.lineStride);
         dmaD->dst = (void*)(src->cmx.base + src->cmx.lineNo * src->cmx.lineStride);
         dmaD->length    = dmaD->src_width * nLines;
         dmaD->userData0 = nLines;
        #if defined(OPIPE_RUNTIME_CHECKS)
         o->nDmaPending++;
        #endif
         DrvCmxDmaStartTaskAsync(dmaD, dmaSourceIrqHandler, src);

        //Progress SRC/DST buffers for next run
         src->ddr.lineNo += nLines;             //SRC-ddr
         src->cmx.lineNo += nLines;             //DST-cmx
         if(src->cmx.lineNo == src->cmx.height) //DST-cmx
            src->cmx.lineNo = 0;                //DST-cmx
       }
     }
  }

  DrvIcbIrqClear(irqSrc);
}

//#################################################################################################
void startSources(Opipe *p)
{
  uint32_t i, run, curPil;

  #ifndef INITIAL_RUNS
  #define INITIAL_RUNS 2
  #endif

  for(i=0; i < p->nSrcs; i++)
  {
     DBuffer *src = p->srcs[i];
     dmaTransactionList *dmaD = getNextDmaDsc(src);

     for(run=0; run<INITIAL_RUNS; run++)
     {
        dmaD->src = (void*)(src->ddr.base + src->ddr.lineNo * src->ddr.lineStride);
        dmaD->dst = (void*)(src->cmx.base + src->cmx.lineNo * src->cmx.lineStride);

      //Critical section: initial posting is done at application level,
      //must not allow other interrutps (calling DrvCmxDmaStartListTask) to break this one
        curPil = swcLeonGetPIL();
        swcLeonSetPIL(15);
        DrvCmxDmaStartListTask(dmaD);
        swcLeonSetPIL(curPil);

        DrvCmxDmaWaitTask     (dmaD);

       //move to next line group
        src->ddr.lineNo  += src->irqRate;  //SRC(ddr) : cur line in InputFullBuff
        src->cmx.lineNo  += src->irqRate;  //DST(cmx) : circular smaller buffer; no need for wrap checks as should NOT wrap at this stage
        src->ddr.cpLines += src->irqRate;
     }

    //Now set fill level; this will make filter start due to:
    // a) available input data
    // b) empty output buffer
     SET_REG_WORD(gSippNfo[src->unitID].iFc, SIPP_CTXUP_BIT_MASK | (INITIAL_RUNS * src->irqRate));
  }
}

//#################################################################################################
void cfgSources(Opipe *p)
{
  uint32_t i, j;

  for(i=0; i < p->nSrcs; i++)
  {
     DBuffer *src     = p->srcs[i];
     src->cmx.lineNo  = 0;
     src->ddr.curDsc  = 0;
     src->ddr.lineNo  = 0;
     src->ddr.cpLines = 0;
   #if defined(OPIPE_RUNTIME_CHECKS)
     src->liloCnt     = 0;
   #endif

     //Make all descriptors identical, will just update SRC/DST at runtime...
     for(j=0; j<N_DESCS; j++)
     {
        dmaTransactionList *dmaD = &src->ddr.dmaDsc[j];
      //Fill all CMXDMA descriptor fields
      //link and cfg are already set; src/dst will be set in startSources
        dmaD->src_width        = src->ddr.lineW;                //line_size
        dmaD->dst_width        = dmaD->src_width;               //line_size
        dmaD->length           = dmaD->src_width * src->irqRate;//full size = line_size * IRQ_RATE
        dmaD->no_planes        = src->nPlanes - 1;
        dmaD->src_stride       = src->ddr.lineStride;
        dmaD->dst_stride       = src->cmx.lineStride;
        dmaD->src_plane_stride = src->ddr.lineStride * src->ddr.height;
        dmaD->dst_plane_stride = src->cmx.lineStride * src->cmx.height;
     }

     SET_REG_WORD(gSippNfo[src->unitID].iIR, src->irqRatePow);
  }
}

//#################################################################################################
void cfgSinks(Opipe *p)
{
  uint32_t i, j;

  for(i=0; i < p->nSnks; i++)
  {
    DBuffer *sink     = p->snks[i];
    sink->cmx.lineNo  = 0;
    sink->ddr.curDsc  = 0;
    sink->ddr.lineNo  = 0;
    sink->ddr.cpLines = 0;

  #if defined(OPIPE_RUNTIME_CHECKS)
    sink->liloCnt     = 0;
  #endif

  #if defined(TAPOUT_EN)
    sink->doneCnt     = 0;
  #endif

  #if defined(OPIPE_CROP)
    sink->crop.lineNo = 0;
  #endif

    //Make all descriptors identical, will just update SRC/DST at runtime...
    for(j=0; j<N_DESCS; j++)
    {
        dmaTransactionList *dmaD = &sink->ddr.dmaDsc[j];
       //Fill all CMXDMA descriptor fields
       //link and cfg are already set; src/dst will be set in startSources
        dmaD->src_width        = sink->cmx.lineW;                 //line_size
        dmaD->dst_width        = dmaD->src_width;                 //line_size
        dmaD->length           = dmaD->src_width * sink->irqRate; //full size = line_size * IRQ_RATE
        dmaD->no_planes        = sink->nPlanes - 1;
        dmaD->src_stride       = sink->cmx.lineStride;
        dmaD->dst_stride       = sink->ddr.lineStride;
        dmaD->src_plane_stride = sink->cmx.lineStride * sink->cmx.height;
        dmaD->dst_plane_stride = sink->ddr.lineStride * sink->ddr.height;
    }

    SET_REG_WORD(gSippNfo[sink->unitID].oIR, sink->irqRatePow);
  }
}

//#################################################################################################
void cfgSwLinks(Opipe *p)
{
   uint32_t i;

   for(i=0; i < p->nSwLinks; i++)
   {
       SwLink *lnk = &p->swCtrl[i];

       lnk->prodMon.    curLine = 0;
       lnk->lastConsMon.curLine = 0;
     #if defined(TAPOUT_EN)
       lnk->doneCnt             = 0;
     #endif

       //allConsIdMask == 0 indicates disabled link !
       if(lnk->allConsIdMask)
       {
           p->sippSrcIntEnMask |= (1<<lnk->lastConsId); //sources treat IBFL-dec
           p->sippSnkIntEnMask |= (1<<lnk->prodId);     //sinks   treat OBFL-inc
       }

       SET_REG_WORD(gSippNfo[lnk->prodId    ].oIR, lnk->prodMon.    irqRatePow);//obfl_inc
       SET_REG_WORD(gSippNfo[lnk->lastConsId].iIR, lnk->lastConsMon.irqRatePow);//ibfl_dec
   }
}

//#################################################################################################
//Handle SippRx EOF when frame is skipped (i.e. sel01 == 0)
//author: marius.truica@movidius.com
void rxRofSkipAdvance(uint32_t *status)
{
    uint32_t rxStatus = *status & SIPP_MIPI_RX_MASK;
    uint32_t i;

    //loop through all SINKS from all pipes
    for(i=0; i<gOpipe.nSinks; i++)
    {
        DBuffer *sink = gOpipe.sinks[i];
        Opipe *o      = (Opipe*)sink->pipeRef;

        //If current sink belongs to an inactive pipe, skip
        if(!(o->flags & PIPE_RUNNING))
            continue;

        // just rx are watched
        if((rxStatus & (1<<sink->unitID)))
        {
            uint32_t  unitNo = sink->unitID - SIPP_MIPI_RX0_ID; //results in 0,1,2,3
            MipiRxCfg *param = o->pMipiRxCfg[unitNo];

            if(0 == param->sel01)
            {
                *status = *status & (~(1<<sink->unitID));
                // this Rx filter skip in this moment the frame, will be just eof irq
                sink->ddr.cpLines = sink->ddr.height;
                if(o->cbEndOfFrame)
                    if(OpipeFinished(o))
                        o->cbEndOfFrame(o);
            }
            // frame already finished execution, eof cb triggered,
            //so clear status to avoid additional call of obflInc
            if(0 == sink->ddr.lineNo) {
                *status = *status & (~(1<<sink->unitID));
            }
        }
    }
}
