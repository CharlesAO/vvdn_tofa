///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
/// @brief     Typical Opipe application helpers
///

#include "OpipeApps.h"
#include "OpipeInternals.h"

extern OpipeGlobal gOpipe;

//###############################################################################################
//Initialize a MipiRx pipeline that stores full output to DDR (packed or flat)
//###############################################################################################
void OpipeCreateRx(OpipeRx *pRx,
                   uint32_t width,        uint32_t height,
                   uint32_t mipiRxId,     uint32_t outFmt,
                   uint32_t oCircBufAddr, uint32_t oCircBufH)

{
   Opipe *p = &pRx->p;

   OpipeInit(p);

  //optionally override default "agent" with user config
   if(p->cdmaReq != gOpipe.cdmaReq)
      p->cdmaReq  = gOpipe.cdmaReq;

   p->width   = width;
   p->height  = height;
   p->format  = BAYER;

  //Specify enables for filters used in this pipe
   p->enMask =  (1<<mipiRxId);

   OpipeDetCfg(p);

  //Cmx circular buffer
   pRx->pOut = OpipeCfgBuff(p, mipiRxId, D_DMA|D_OUT, oCircBufAddr, oCircBufH, outFmt);
}

//###############################################################################################
//Initialize ISP Pre-Processor config (split scheme)
//###############################################################################################
void OpipeCreatePP(OpipePP *pp,
                   uint32_t width,        uint32_t height,
                   uint32_t mipiRxId,     uint32_t outFmt,
                   uint32_t oCircBufAddr, uint32_t oCircBufH)
{
   Opipe *p = &pp->p;

   OpipeInit(p);

  //optionally override default "agent" with user config
   if(p->cdmaReq != gOpipe.cdmaReq)
      p->cdmaReq  = gOpipe.cdmaReq;

   p->width   = width;
   p->height  = height;
   p->format  = BAYER;

  //Specify enables for filters used in this pipe
   p->enMask =  (1<<mipiRxId     ) |(1<<SIPP_SIGMA_ID)
               |(1<<SIPP_LSC_ID  ) |(1<<SIPP_RAW_ID  );

   OpipeDetCfg(p);

  //Cmx circular buffer
   pp->pOut = OpipeCfgBuff(p, SIPP_RAW_ID, D_DMA|D_OUT, oCircBufAddr, oCircBufH, outFmt);
}

//###############################################################################################
//Internal util that covers Main ISP, but also reused by Full pipe
//###############################################################################################
void OpipeCreateMF(OpipeMF *o,
                   uint32_t inSippID,  //must be SIPP_DBYR_ID or SIPP_SIGMA_ID
                   uint32_t inFmt)     //flat/packed modes
{
  // uint32_t k1, k2;
   Buf      inBuf = (inSippID == SIPP_DBYR_ID) ? o->in.cBufDbyr : o->in.cBufSigma; //same thing really...
   Opipe   *p = &o->p;

   OpipeInit(p);

  //optionally override default "agent" with user config
   if(p->cdmaReq != gOpipe.cdmaReq)
      p->cdmaReq  = gOpipe.cdmaReq;

   p->format = BAYER;

 //Filters that belong to Main ISP pipe
   p->enMask =  (1<<SIPP_DBYR_ID    ) |(1<<SIPP_DOGL_ID    ) |(1<<SIPP_LUMA_ID)
               |(1<<SIPP_SHARPEN_ID ) |(1<<SIPP_CGEN_ID    ) |(1<<SIPP_MED_ID )
               |(1<<SIPP_CHROMA_ID  ) |(1<<SIPP_CC_ID      ) |(1<<SIPP_LUT_ID );

   o->pIn   =  OpipeCfgBuff (p, inSippID,          D_DMA|D_IN, inBuf.base,         inBuf.h,          inFmt);

 //preview support requires DBYR input to be in CMX (http://dub30/bugzilla/show_bug.cgi?id=22238)
  if(o->cBufDbyrIn.base)
  {
    p->flags |= PREVIEW_ABLE;
   //Aux buffs
    o->pOutRaw = OpipeCfgBuff (p, SIPP_RAW_ID,          D_OUT, o->cBufDbyrIn.base, o->cBufDbyrIn.h,  BPP(2));
    o->pInDbyr = OpipeCfgBuff (p, SIPP_DBYR_ID,          D_IN, o->cBufDbyrIn.base, o->cBufDbyrIn.h,  BPP(2));
   //SwLink in between (TBD: break LLB connection)
    o->dbyrI   = OpipeSwLink(p, SIPP_RAW_ID, (1<<SIPP_DBYR_ID), SIPP_DBYR_ID);
  }

               OpipeCfgBuff (p, SIPP_DBYR_LUMA_ID,      D_OUT, o->cBufDbyrY.base,  o->cBufDbyrY.h,   BPP(2));
   o->pDbyrC = OpipeCfgBuff (p, SIPP_DOGL_ID,            D_IN, o->cBufDbyrY.base,  o->cBufDbyrY.h,   BPP(2)); //fp16
               OpipeCfgBuff (p, SIPP_MED_LUMA_ID,        D_IN, o->cBufDbyrY.base,  o->cBufDbyrY.h,   BPP(2)); //1xPlane needs to be read 3 times

               OpipeCfgBuff (p, SIPP_SHARPEN_ID,        D_OUT, o->cBufSharp.base,  o->cBufSharp.h,   BPP(2)); //fp16
               OpipeCfgBuff (p, SIPP_CC_ID,              D_IN, o->cBufSharp.base,  o->cBufSharp.h,   BPP(2));

  if(o->cBufPoly.base)
  {
    p->enMask |= (1<<SIPP_UPFIRDN0_ID) |(1<<SIPP_UPFIRDN1_ID);

    o->pOutLut = OpipeCfgBuff (p, SIPP_LUT_ID,            D_OUT, o->cBufLut.base,   o->cBufLut.h,   BPP(1)); //yuv444
                 OpipeCfgBuff (p, SIPP_UPFIRDN0_ID,        D_IN, o->cBufLut.base,   o->cBufLut.h,   BPP(1)); //Y
    o->pInUV   = OpipeCfgBuff (p, SIPP_UPFIRDN1_ID,        D_IN, o->cBufLut.base,   o->cBufLut.h,   BPP(1)); //UV (skip Y plane)

    o->pOutY   = OpipeCfgBuff (p, SIPP_UPFIRDN0_ID, D_DMA|D_OUT, o->cBufPoly.base,  o->cBufPoly.h,  BPP(1)); //Y
    o->pOutUV  = OpipeCfgBuff (p, SIPP_UPFIRDN1_ID, D_DMA|D_OUT, o->cBufPoly.base,  o->cBufPoly.h,  BPP(1)); //UV
  }
  else{
    o->pOutY   = OpipeCfgBuff (p, SIPP_LUT_ID,      D_DMA|D_OUT, o->cBufLut.base,   o->cBufLut.h,   BPP(1)); //yuv444
    o->pOutUV  = NULL;
  }

 //The DBYR->DOGL SW link (keep a reference to it, can patch it later...)
  o->dbyrO = OpipeSwLink(p, SIPP_DBYR_LUMA_ID, (1<<SIPP_DOGL_ID)|(1<<SIPP_MED_LUMA_ID), SIPP_DOGL_ID);
}

//###############################################################################################
//Internal util that covers Full ISP including LCA SW
//###############################################################################################
void OpipeCreateFLca(OpipeFLca *o,
                   uint32_t inSippID,  //must be SIPP_DBYR_ID or SIPP_SIGMA_ID
                   uint32_t inFmt)     //flat/packed modes
{
  // uint32_t k1, k2;
   Buf      inBuf = (inSippID == SIPP_DBYR_ID) ? o->in.cBufDbyr : o->in.cBufSigma; //same thing really...
   Opipe   *p = &o->p;

   OpipeInit(p);

  //optionally override default "agent" with user config
   if(p->cdmaReq != gOpipe.cdmaReq)
      p->cdmaReq  = gOpipe.cdmaReq;

   p->format = BAYER;

 //Filters that belong to Main ISP pipe
   p->enMask =  (1<<SIPP_DBYR_ID    ) |(1<<SIPP_DOGL_ID    ) |(1<<SIPP_LUMA_ID)
               |(1<<SIPP_SHARPEN_ID ) |(1<<SIPP_CGEN_ID    ) |(1<<SIPP_MED_ID )
               |(1<<SIPP_CHROMA_ID  ) |(1<<SIPP_CC_ID      ) |(1<<SIPP_LUT_ID );

   o->pIn   =  OpipeCfgBuff (p, inSippID,          D_DMA|D_IN, inBuf.base,         inBuf.h,          inFmt);

   //LCA
  o->dbyrI   = OpipeSwLink(p, SIPP_RAW_ID, (1<<SIPP_DBYR_ID), SIPP_DBYR_ID);

               OpipeCfgBuff (p, SIPP_DBYR_LUMA_ID,      D_OUT, o->cBufDbyrY.base,  o->cBufDbyrY.h,   BPP(2));
   o->pDbyrC = OpipeCfgBuff (p, SIPP_DOGL_ID,            D_IN, o->cBufDbyrY.base,  o->cBufDbyrY.h,   BPP(2)); //fp16
               OpipeCfgBuff (p, SIPP_MED_LUMA_ID,        D_IN, o->cBufDbyrY.base,  o->cBufDbyrY.h,   BPP(2)); //1xPlane needs to be read 3 times

               OpipeCfgBuff (p, SIPP_SHARPEN_ID,        D_OUT, o->cBufSharp.base,  o->cBufSharp.h,   BPP(2)); //fp16
               OpipeCfgBuff (p, SIPP_CC_ID,              D_IN, o->cBufSharp.base,  o->cBufSharp.h,   BPP(2));


  o->pRawO = OpipeCfgBuff (p, SIPP_RAW_ID,        D_OUT, o->oCbRaw.base,  o->oCbRaw.h, BPP(2));
  o->pDbyI = OpipeCfgBuff (p, SIPP_DBYR_ID,        D_IN, o->iCbDbyr.base,  o->iCbDbyr.h, BPP(2));

  if(o->cBufPoly.base)
  {
    p->enMask |= (1<<SIPP_UPFIRDN0_ID) |(1<<SIPP_UPFIRDN1_ID);

    o->pOutLut = OpipeCfgBuff (p, SIPP_LUT_ID,            D_OUT, o->cBufLut.base,   o->cBufLut.h,   BPP(1)); //yuv444
                 OpipeCfgBuff (p, SIPP_UPFIRDN0_ID,        D_IN, o->cBufLut.base,   o->cBufLut.h,   BPP(1)); //Y
    o->pInUV   = OpipeCfgBuff (p, SIPP_UPFIRDN1_ID,        D_IN, o->cBufLut.base,   o->cBufLut.h,   BPP(1)); //UV (skip Y plane)

    o->pOutY   = OpipeCfgBuff (p, SIPP_UPFIRDN0_ID, D_DMA|D_OUT, o->cBufPoly.base,  o->cBufPoly.h,  BPP(1)); //Y
    o->pOutUV  = OpipeCfgBuff (p, SIPP_UPFIRDN1_ID, D_DMA|D_OUT, o->cBufPoly.base,  o->cBufPoly.h,  BPP(1)); //UV
  }
  else{
    o->pOutY   = OpipeCfgBuff (p, SIPP_LUT_ID,      D_DMA|D_OUT, o->cBufLut.base,   o->cBufLut.h,   BPP(1)); //yuv444
    o->pOutUV  = NULL;
  }

 //The DBYR->DOGL SW link (keep a reference to it, can patch it later...)
  o->dbyrO = OpipeSwLink(p, SIPP_DBYR_LUMA_ID, (1<<SIPP_DOGL_ID)|(1<<SIPP_MED_LUMA_ID), SIPP_DOGL_ID);
}

//###############################################################################################
//Initialize ISP full pipeline
//###############################################################################################
void OpipeCreateFull(OpipeMF *o, uint32_t inFmt)
{
   OpipeCreateMF(o, SIPP_SIGMA_ID, inFmt);
 //Append extra filters:
   o->p.enMask |= (1<<SIPP_SIGMA_ID) |(1<<SIPP_LSC_ID) |(1<<SIPP_RAW_ID);
   OpipeDetCfg(&o->p);

}

//###############################################################################################
//Initialize ISP main pipeline
//###############################################################################################
void OpipeCreateMain(OpipeMF *o, uint32_t inFmt)
{
    OpipeCreateMF(o, SIPP_DBYR_ID, inFmt);
    OpipeDetCfg(&o->p);
}

//###############################################################################################
//Initialize ISP full pipeline
//###############################################################################################
void OpipeCreateFullLca(OpipeFLca *o, uint32_t inFmt)
{
    OpipeCreateFLca(o, SIPP_SIGMA_ID, inFmt);
 //Append extra filters:
   o->p.enMask |= (1<<SIPP_SIGMA_ID) |(1<<SIPP_LSC_ID) |(1<<SIPP_RAW_ID);

   OpipeDetCfg(&o->p);

   //LCA
   o->p.cfgMask &= ~(1<<SIPP_RAW_ID); //drop RAW->DBYR HW link
}

//###############################################################################################
// Change Main/Full pipe resolution
// WARNING: at this point, if POLYFIRs are included, their associated CFGs need to be set
//###############################################################################################
void OpipeSetSizeMF(OpipeMF *o, uint32_t iWidth, uint32_t iHeight)
{
    o->p.width = iWidth + 1;//force rescale
    OpipeSetRes(&o->p, iWidth, iHeight);//update width2/height2 first

  //Adjust UPFIRDN1 In/Out cmx.base addr
  if(o->cBufPoly.base)
  {
   //Adjust UPFIRDN1 input reference (MUST map on LUT's view of PlaneStride)
    o->pInUV->cmx.base  = o->pOutLut->cmx.base + o->pOutLut->cmx.height * o->p.width2;
   //Adjust UPFIRDN1 output (UV follow after UV plane)
    o->pOutUV->cmx.base = o->pOutY->cmx.base   + o->pOutY->cmx.height   * o->pOutY->cmx.lineStride;
  }
}

//###############################################################################################
// Change Main/Full pipe resolution
// WARNING: at this point, if POLYFIRs are included, their associated CFGs need to be set
//###############################################################################################
void OpipeSetSizeFLca(OpipeFLca *o, uint32_t iWidth, uint32_t iHeight)
{
    o->p.width = iWidth + 1;//force rescale
    OpipeSetRes(&o->p, iWidth, iHeight);//update width2/height2 first

  //Adjust UPFIRDN1 In/Out cmx.base addr
  if(o->cBufPoly.base)
  {
   //Adjust UPFIRDN1 input reference (MUST map on LUT's view of PlaneStride)
    o->pInUV->cmx.base  = o->pOutLut->cmx.base + o->pOutLut->cmx.height * o->p.width2;
   //Adjust UPFIRDN1 output (UV follow after UV plane)
    o->pOutUV->cmx.base = o->pOutY->cmx.base   + o->pOutY->cmx.height   * o->pOutY->cmx.lineStride;
  }
}

//###############################################################################################
//Set this before calling OpipeStart
void OpipeSetDoglCfg(OpipeMF *o, DoglCfg cfg)
{
  //If not already determined, figure out config words, we'll then override
    OpipeDetCfg(&o->p);

  //Without this, I get hangs on mode change
    OpipeHwReset(1<<26);

    CLR_REG_BITS_MASK(SIPP_INT0_ENABLE_ADR, o->p.sippSrcIntEnMask);
    CLR_REG_BITS_MASK(SIPP_INT1_ENABLE_ADR, o->p.sippSnkIntEnMask);
    CLR_REG_BITS_MASK(SIPP_INT2_ENABLE_ADR, o->p.sippSnkIntEnMask);

    o->dbyrO->prodMon.irqRatePow     = IRQ_RATE_POW;
    o->dbyrO->lastConsMon.irqRatePow = IRQ_RATE_POW;
    o->dbyrO->lastConsId             = SIPP_DOGL_ID;
    o->p.cbPreStart                  = NULL;

    switch(cfg)
    {
      //================================================================
      //This is hte default mode (best quality, slowest)
      case DOGL_ON_F16_CMX:
           o->p.cfgMask           &= ~(1<<SIPP_DBYR_LUMA_ID); //drop DBYR->DOGL
           o->p.cfgMask           |=  (1<<SIPP_DOGL_ID);      //en   DOGL->LUMA
           o->p.enMask            |=  (1<<SIPP_DOGL_ID); //enable DOGL

           o->dbyrO->allConsIdMask = (1<<SIPP_DOGL_ID)|(1<<SIPP_MED_LUMA_ID);
        break;
      //================================================================
      //2nd config in terms of quality, also fastest config
      case DOGL_ON_U8F_STREAM:
           o->p.cbPreStart                  =  OpipeForceU8fLuma;
           //[DON'T BREAK HERE], let it flow...
      //================================================================
      //Will get fixed in future
      case DOGL_ON_F16_STREAM:
           o->p.cfgMask           |=  (1<<SIPP_DBYR_LUMA_ID);//DBYR_LUMA->DOGL
           o->p.cfgMask           |=  (1<<SIPP_DOGL_ID);     //DOGL     ->LUMA
           o->p.enMask            |=  (1<<SIPP_DOGL_ID);     //enable DOGL

         //Note: DBYR_LUMA and DOGL will still get programmed, but
         //      will be useless step due to enabled OPIPE_CFG config
         //The only thing left to do: disable int-masks for filters
         //DBYR_LUMA oblf_inc will always trigger, but cancel it from EN_MASK:
           o->dbyrO->allConsIdMask          =  0x00000000;
           o->p.sippSnkIntEnMask           &= ~(1<<SIPP_DBYR_LUMA_ID);
           o->p.sippSrcIntEnMask           &= ~(1<<SIPP_DOGL_ID);
         //Critical overrides:

           o->dbyrO->prodMon.irqRatePow     =  0;
           o->dbyrO->lastConsMon.irqRatePow =  0;
      break;
      //================================================================
      case DOGL_OFF:
           o->p.cfgMask           &= ~(1<<SIPP_DBYR_LUMA_ID); //drop DBYR->DOGL
           o->p.cfgMask           &= ~(1<<SIPP_DOGL_ID);      //drop DOGL->LUMA
           o->p.enMask            &= ~(1<<SIPP_DOGL_ID);      //disable DOGL
           o->dbyrO->lastConsId    =      SIPP_LUMA_ID; //CRITICAL_OVERRIDE
           o->dbyrO->allConsIdMask =  (1<<SIPP_LUMA_ID)|(1<<SIPP_MED_LUMA_ID);
        break;
    }

    o->pDbyrC->unitID       = o->dbyrO->lastConsId;
    o->pDbyrC->sippBuffBase = gSippNfo[o->pDbyrC->unitID].iBase;
}

//###############################################################################################
//Full Isp with Dbyr tapout (Y or RGB) ability
//IMPORTANT NOTE: TAPOUT_EN macro needs to defined !
//###############################################################################################
void OpipeCreateDbyrTap(OpipeTap *o, TapParam param)     //flat/packed modes
{
   uint32_t inSippID = SIPP_SIGMA_ID;
   Buf      inBuf = (inSippID == SIPP_DBYR_ID) ? o->in.cBufDbyr : o->in.cBufSigma; //same thing really...
   Opipe   *p = &o->p;

   OpipeInit(p);

   p->format = BAYER;

 //Filters that belong to Main ISP pipe
   p->enMask =  (1<<SIPP_SIGMA_ID   ) |(1<<SIPP_LSC_ID     ) |(1<<SIPP_RAW_ID )
               |(1<<SIPP_DBYR_ID    ) |(1<<SIPP_DOGL_ID    ) |(1<<SIPP_LUMA_ID)
               |(1<<SIPP_SHARPEN_ID ) |(1<<SIPP_CGEN_ID    ) |(1<<SIPP_MED_ID )
               |(1<<SIPP_CHROMA_ID  ) |(1<<SIPP_CC_ID      ) |(1<<SIPP_LUT_ID )
               |(1<<SIPP_UPFIRDN0_ID) |(1<<SIPP_UPFIRDN1_ID);

   o->pIn   =  OpipeCfgBuff (p, inSippID,          D_DMA|D_IN, inBuf.base,         inBuf.h,          param.inFmt);

 /*TAP*/ if(param.mode == TAP_LUMA)
 /*TAP*/  o->pOutTap.y = OpipeCfgBuff(p, SIPP_DBYR_LUMA_ID, D_DMA|D_OUT, o->cBufDbyrY.base, o->cBufDbyrY.h, BPP(2));
 /*TAP*/ else{ //+ Dbyr.RGB to GenChroma buffer
 /*TAP*/                    OpipeCfgBuff(p, SIPP_DBYR_LUMA_ID,      D_OUT, o->cBufDbyrY.base,    o->cBufDbyrY.h,   BPP(2));
 /*TAP*/   o->pOutTap.rgb = OpipeCfgBuff(p, SIPP_DBYR_ID,     D_DMA|D_OUT, o->cBufDbyrRgb.base,  o->cBufDbyrRgb.h, param.bpp);
 /*TAP*/                    OpipeCfgBuff(p, SIPP_CGEN_ID,            D_IN, o->cBufDbyrRgb.base,  o->cBufDbyrRgb.h, param.bpp);
 /*TAP*/ }

               OpipeCfgBuff (p, SIPP_DOGL_ID,            D_IN, o->cBufDbyrY.base, o->cBufDbyrY.h, BPP(2)); //fp16
               OpipeCfgBuff (p, SIPP_MED_LUMA_ID,        D_IN, o->cBufDbyrY.base, o->cBufDbyrY.h, BPP(2)); //1xPlane needs to be read 3 times

               OpipeCfgBuff (p, SIPP_SHARPEN_ID,        D_OUT, o->cBufSharp.base, o->cBufSharp.h, BPP(2)); //fp16
               OpipeCfgBuff (p, SIPP_CC_ID,              D_IN, o->cBufSharp.base, o->cBufSharp.h, BPP(2)); //fp16

  o->pOutLut = OpipeCfgBuff (p, SIPP_LUT_ID,            D_OUT, o->cBufLut.base,   o->cBufLut.h,   BPP(1)); //yuv444
               OpipeCfgBuff (p, SIPP_UPFIRDN0_ID,        D_IN, o->cBufLut.base,   o->cBufLut.h,   BPP(1)); //Y
  o->pInUV   = OpipeCfgBuff (p, SIPP_UPFIRDN1_ID,        D_IN, o->cBufLut.base,   o->cBufLut.h,   BPP(1)); //UV (skip Y plane)

  o->pOutY   = OpipeCfgBuff (p, SIPP_UPFIRDN0_ID, D_DMA|D_OUT, o->cBufPoly.base,  o->cBufPoly.h,  BPP(1)); //Y
  o->pOutUV  = OpipeCfgBuff (p, SIPP_UPFIRDN1_ID, D_DMA|D_OUT, o->cBufPoly.base,  o->cBufPoly.h,  BPP(1)); //UV

 //The DBYR->DOGL SW link (keep a reference to it, can patch it later...)
  o->dbyrO = OpipeSwLink(p, SIPP_DBYR_LUMA_ID, (1<<SIPP_DOGL_ID)|(1<<SIPP_MED_LUMA_ID), SIPP_DOGL_ID);

 /*TAP*/ if(param.mode == TAP_RGB){
 /*TAP*/ //The DBYR->GenChroma SW link
 /*TAP*/  o->dbyrOrgb = OpipeSwLink(p, SIPP_DBYR_ID,      (1<<SIPP_CGEN_ID), SIPP_CGEN_ID);
 /*TAP*/ }

  OpipeDetCfg(&o->p);

  /*TAP*/ if(param.mode == TAP_RGB){
  /*TAP*/ //Cancel dbyr.RGB->GenChroma link
  /*TAP*/  p->cfgMask &= (~(1<<SIPP_DBYR_ID));
  /*TAP*/ }
}

//###############################################################################################
// Change Tap pipe resolution
// WARNING: at this point, if POLYFIRs are included, their associated CFGs need to be set
// TBD: same as MF resize, should refactor this code if more pipe variants show up...
//###############################################################################################
void OpipeSetSizeTap(OpipeTap *o, uint32_t iWidth, uint32_t iHeight)
{
    o->p.width = iWidth + 1;//force rescale
    OpipeSetRes(&o->p, iWidth, iHeight);//update width2/height2 first

   //Adjust UPFIRDN1 input reference (MUST map on LUT's view of PlaneStride)
    o->pInUV->cmx.base  = o->pOutLut->cmx.base + o->pOutLut->cmx.height * o->p.width2;
   //Adjust UPFIRDN1 output (UV follow after UV plane)
    o->pOutUV->cmx.base = o->pOutY->cmx.base   + o->pOutY->cmx.height   * o->pOutY->cmx.lineStride;
}

//#################################################################################################
//[Luma in] -> [Luma out] (PLANAR) Monochrome pipeline
//#################################################################################################
static void loc_setFmt(uint32_t reg, uint32_t fmt)
{
    uint32_t data = GET_REG_WORD_VAL(reg);
    data &= ~(0xF << SIPP_FO_OFFSET); //clear old format
    data |=   fmt << SIPP_FO_OFFSET;  //set   new format
    SET_REG_WORD(reg, data);
}
//==================================================================================================
//Patch buffer formats for u8f luma
static void Mono1PatchFormats(Opipe *p)
{
   //Temporaly disable filters
    CLR_REG_BITS_MASK(SIPP_CONTROL_ADR,p->enMask);
   //Set new formats
    loc_setFmt(O_CFG(SIPP_RAW_ID),  1/*u8f*/);
    loc_setFmt(I_CFG(SIPP_LUMA_ID), 1/*u8f*/);
   //Re-enable filters
    SET_REG_BITS_MASK(SIPP_CONTROL_ADR,p->enMask);
}
//==================================================================================================
void OpipeCreateLumaMono(OpipeLumaMono *o, uint32_t inFmt)
{
   Opipe *p = &o->p;
   OpipeInit(p);
   p->oPlanes[SIPP_LUT_ID] = 1;//override defaults !

 //p->width   = width;
 //p->height  = height;
   p->format  = PLANAR;

  //Specify enables for filters used in this pipe
   p->enMask =  (1<<SIPP_SIGMA_ID)
              | (1<<SIPP_LSC_ID)
              | (1<<SIPP_RAW_ID)
              | (1<<SIPP_LUMA_ID)
              | (1<<SIPP_SHARPEN_ID)
              | (1<<SIPP_LUT_ID);

   OpipeDetCfg(p);

  //Cmx circular buffers
   o->pIn  = OpipeCfgBuff(p, SIPP_SIGMA_ID,  D_DMA|D_IN, o->cBufInSig .base, o->cBufInSig .h, inFmt);
             OpipeCfgBuff(p, SIPP_RAW_ID,         D_OUT, o->cBufOutRaw.base, o->cBufOutRaw.h, BPP(1));
             OpipeCfgBuff(p, SIPP_LUMA_ID,         D_IN, o->cBufOutRaw.base, o->cBufOutRaw.h, BPP(1));
             OpipeCfgBuff(p, SIPP_SHARPEN_ID,     D_OUT, o->cBufOutUsm.base, o->cBufOutUsm.h, BPP(2)); //f16
             OpipeCfgBuff(p, SIPP_LUT_ID,          D_IN, o->cBufOutUsm.base, o->cBufOutUsm.h, BPP(2));
   o->pOut = OpipeCfgBuff(p, SIPP_LUT_ID,   D_DMA|D_OUT, o->cBufOutLut.base, o->cBufOutLut.h, BPP(1));

   o->usmO = OpipeSwLink(p, SIPP_SHARPEN_ID, (1<<SIPP_LUT_ID ), SIPP_LUT_ID );
   o->rawO = OpipeSwLink(p, SIPP_RAW_ID,     (1<<SIPP_LUMA_ID), SIPP_LUMA_ID);

  //Override default BPP formats
   o->p.cbPreStart = Mono1PatchFormats;
}

//#################################################################################################
//[Bayer in] -> [Luma out] Monochrome pipeline
//#################################################################################################
static void Mono2PatchDefaults(Opipe * p){
    UNUSED(p);
  //Patch Opipe defaults: no RGB outputs
    SET_REG_WORD(SIPP_DBYR_THRES_ADR  ,  (0<<25)   //DISABLE RGB out !!!
                                       | (1<<26)   //en Luma out
                                       | (1<<27)   //wr Y data to CMX
                                       | (0<<28)); //preview Gen
}
//==================================================================================================
void OpipeCreateBayerMono(OpipeBayerMono *o, uint32_t inFmt)
{
   Opipe *p = &o->p;
   OpipeInit(p);
   p->oPlanes[SIPP_LUT_ID] = 1;//override defaults !

 //p->width   = width;
 //p->height  = height;
   p->format  = BAYER;

  //Specify enables for filters used in this pipe
   p->enMask =  (1<<SIPP_SIGMA_ID)
              | (1<<SIPP_LSC_ID)
              | (1<<SIPP_RAW_ID)
              | (1<<SIPP_DBYR_ID)
              | (1<<SIPP_DOGL_ID)
              | (1<<SIPP_LUMA_ID)
              | (1<<SIPP_SHARPEN_ID)
              | (1<<SIPP_LUT_ID);

   OpipeDetCfg(p);

  //Cmx circular buffers
   o->pIn  = OpipeCfgBuff(p, SIPP_SIGMA_ID,     D_DMA|D_IN, o->cBufInSig  .base, o->cBufInSig  .h, inFmt);
             OpipeCfgBuff(p, SIPP_DBYR_LUMA_ID,      D_OUT, o->cBufOutDbyr.base, o->cBufOutDbyr.h, BPP(2)); //f16
             OpipeCfgBuff(p, SIPP_DOGL_ID,            D_IN, o->cBufOutDbyr.base, o->cBufOutDbyr.h, BPP(2)); //f16
             OpipeCfgBuff(p, SIPP_SHARPEN_ID,        D_OUT, o->cBufOutUsm .base, o->cBufOutUsm .h, BPP(2)); //f16
             OpipeCfgBuff(p, SIPP_LUT_ID,             D_IN, o->cBufOutUsm .base, o->cBufOutUsm .h, BPP(2)); //f16
   o->pOut = OpipeCfgBuff(p, SIPP_LUT_ID,      D_DMA|D_OUT, o->cBufOutLut .base, o->cBufOutLut .h, BPP(1)); //u8

   o->dbyrO = OpipeSwLink(p, SIPP_DBYR_LUMA_ID, (1<<SIPP_DOGL_ID), SIPP_DOGL_ID);
   o->usmO  = OpipeSwLink(p, SIPP_SHARPEN_ID,   (1<<SIPP_LUT_ID ), SIPP_LUT_ID );

  //Override default
   o->p.cbPreStart = Mono2PatchDefaults;
}

//#################################################################################################
//Default Opipe CFG registers (typical ISP config)
//#################################################################################################
uint32_t OpipeDefaultCfg(Opipe *p, uint32_t unitID)
{
    uint32_t ret = 0xDEADDEAD;

    switch(unitID)
    {
        //=====================================================================
        case SIPP_SIGMA_ID:{
            SigmaDnsCfg *sigCfg = p->pSigmaCfg;
            ret = (p->format          << 0) |
                  ((p->rawBits-1)     << 4) |
                  (sigCfg->noiseFloor << 8) ;
            break;
        }
        //=====================================================================
        case SIPP_RAW_ID: {
            RawCfg  *rawCfg = p->pRawCfg;
            ret = p->format                    |
                 (p->bayerPattern       <<  1) |
                 (1                     <<  3) |  //Gr/Gb imbalance enable
                 (1                     <<  4) |  //Hot/Cold pixel suppression enable
                 (0                     <<  7) |  //Luma histogram
                 ((p->rawBits - 1)      <<  8) |  //RAW_DATA_WIDTH
                 (1                     << 12) |  //Bayer 2x2 mode
                 (0                     << 13) |  //AF stats
                 (rawCfg->grgbImbalThr  << 16) |
                 (0                     << 24) |  //RGB histogram
                 (0                     << 27)  ; //Static pixel correction
            break;
        }
        //=====================================================================
        case SIPP_DBYR_ID: {
            #ifndef DBYR_CHROMA_BITS
            #define DBYR_CHROMA_BITS 12
            #endif

            //do this properly !!!
            #if !defined(TAPOUT_EN)
              #if(DBYR_CHROMA_BITS != 12)
                #error Invalid config
              #endif
            #endif

            DbyrCfg  *dbyrCfg = p->pDbyrCfg;
            ret =  p->bayerPattern
                |((p->rawBits    - 1)            <<  4)  // input pixel width
                |((DBYR_CHROMA_BITS - 1)         <<  8)  // output pixel width
                |(P_RGB                          << 12)  // rgb planes order
                |((p->oPlanes[SIPP_DBYR_ID] - 1) << 15)  // plane multiple
                |(dbyrCfg->dewormGradientMul     << 24);
            break;
        }
        //=====================================================================
        case SIPP_DOGL_ID: {
            #ifndef DOG_HEIGHT
            #define DOG_HEIGHT 15
            #endif

            LtmCfg *ltmCfg = p->pLtmCfg;
            DogCfg *dogCfg = p->pDogCfg;

          //Pick DOGL mode based on DOG strength
            uint32_t doglMode;
            if(dogCfg->strength>0) doglMode = 3;//DoG plus LTM (SLOW)
            else                   doglMode = 1;//LTM only     (FAST)

            ret = DOG_HEIGHT   << 26 |
                  ltmCfg->thr  << 14 |  //ISPC_LTM_FILTER_TH1
                            1  << 12 |  //' Don't propagate carry bit'
                  dogCfg->thr  <<  2 |  //ISPC_DOG_THR
                            doglMode ;
            break;
        }
        //=============================================
        case SIPP_SHARPEN_ID: {
            #define KERNEL_SIZE     7
            #define CLAMP_OUTPUT    1
            SharpenCfg *cfg = p->pSharpCfg;
            ret = KERNEL_SIZE       |
                  CLAMP_OUTPUT <<  3|
                  cfg->minThr  << 16;
            break;
        }
        //=============================================
        case SIPP_CGEN_ID: {
            ChromaGenCfg *cfg = p->pChrGenCfg;
            ret =                     ( 1 << 29) //'Don't propagate carry bit'
                   |((DBYR_CHROMA_BITS-1) << 24) //Input bit-width
                   | cfg->desatOffset     <<  8
                   | cfg->desatSlope      << 16
                   | cfg->pfrStrength;
            break;
        }
        //=============================================
        case  SIPP_MED_ID: {
            #define LUMA_SUBSAMPLE      1

            #define THRESH              0x1ff

            uint32_t  outSel, goFast;
            MedianCfg *cfg = p->pMedCfg;

            outSel = (cfg->kernelSize >> 1) * (cfg->kernelSize) + (cfg->kernelSize >> 1);
            goFast = (cfg->kernelSize  < 7) ? 1 : 0;

            ret =  goFast           << 30 |
                   LUMA_SUBSAMPLE   << 29 |
                   LUMA_BASED_BLEND << 28 |
                   THRESH           << 16 |
                   outSel           <<  8 |
                   cfg->kernelSize        ;
            break;
        }
        //=============================================
        case SIPP_CHROMA_ID: {
            #define PLANES_PER_CYCLE    3
            ChromaDnsCfg *cfg = p->pChromaDnsCfg;
            ret = cfg->hEnab                          |
                  cfg->limit                    <<  4 |
                  (PLANES_PER_CYCLE-1)          << 14 |
                  (cfg->greyDesatOffset & 0xff) << 16 |
                  (cfg->greyDesatSlope  & 0xff) << 24 ;
            break;
        }
        //=============================================
        case SIPP_CC_ID: {
            ret =  (1                        << 3)  //DISABLE 3D lut
                 |((p->oPlanes[SIPP_CC_ID]-1)<<24); //3 planes (program 3-1)
            break;
        }
        //=============================================
        case SIPP_LUT_ID: {
            #define FP16_MODE     1
            #define CHANNEL_MODE  1
            #define LOAD_EN       1
            #define CC_ENABLE     1
            ret =  FP16_MODE                        |
                   CHANNEL_MODE               <<  1 |
                  (p->oPlanes[SIPP_LUT_ID]-1) << 12 |
                // LOAD_EN                    << 14 |
                   CC_ENABLE                  << 16;
            break;
        }
        default:
        //ERROR: (no default config for filter:)
        break;
    };

    if(ret != 0xDEADDEAD)
       p->cfg[unitID] = ret;

    return ret;
}
