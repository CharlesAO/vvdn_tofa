#include <assert.h>
#include "Opipe.h"
#include "OpipeInternals.h"
#include "PlgSrcPipeDef.h"

extern OpipeGlobal gOpipe;

//###############################################################################################
void OpipeCreateRxIsp(OpipeRxIsp *o, uint32_t mipiRxId)
{
   Opipe *p = &o->p;

   OpipeInit(p);

  //optionally override default "agent" with user config
   if(p->cdmaReq != gOpipe.cdmaReq)
      p->cdmaReq  = gOpipe.cdmaReq;

   p->format  = BAYER;

   p->enMask =  (1<<mipiRxId    )
               |(1<<SIPP_SIGMA_ID   ) |(1<<SIPP_LSC_ID     ) |(1<<SIPP_RAW_ID )
               |(1<<SIPP_DBYR_ID    ) |(1<<SIPP_DOGL_ID    ) |(1<<SIPP_LUMA_ID)
               |(1<<SIPP_SHARPEN_ID ) |(1<<SIPP_CGEN_ID    ) |(1<<SIPP_MED_ID )
               |(1<<SIPP_CHROMA_ID  ) |(1<<SIPP_CC_ID      ) |(1<<SIPP_LUT_ID )
               |(1<<SIPP_UPFIRDN0_ID) |(1<<SIPP_UPFIRDN1_ID);

   #if defined(MIPI_CMX_SIGMA)
               OpipeCfgBuff (p, mipiRxId,             D_OUT, o->cBufMipiOut.base, o->cBufMipiOut.h,BPP(2));
               OpipeCfgBuff (p, SIPP_SIGMA_ID,         D_IN, o->cBufMipiOut.base, o->cBufMipiOut.h,BPP(2));
   #endif
               OpipeCfgBuff (p, SIPP_DBYR_LUMA_ID,      D_OUT, o->cBufDbyrY.base,  o->cBufDbyrY.h, BPP(2));
               OpipeCfgBuff (p, SIPP_DOGL_ID,            D_IN, o->cBufDbyrY.base,  o->cBufDbyrY.h, BPP(2)); //fp16
               OpipeCfgBuff (p, SIPP_MED_LUMA_ID,        D_IN, o->cBufDbyrY.base,  o->cBufDbyrY.h, BPP(2)); //1xPlane needs to be read 3 times

               OpipeCfgBuff (p, SIPP_SHARPEN_ID,        D_OUT, o->cBufSharp.base,  o->cBufSharp.h, BPP(2)); //fp16
               OpipeCfgBuff (p, SIPP_CC_ID,              D_IN, o->cBufSharp.base,  o->cBufSharp.h, BPP(2));

    o->pOutLut = OpipeCfgBuff (p, SIPP_LUT_ID,            D_OUT, o->cBufLut.base,  o->cBufLut.h,   BPP(1)); //yuv444
                 OpipeCfgBuff (p, SIPP_UPFIRDN0_ID,        D_IN, o->cBufLut.base,  o->cBufLut.h,   BPP(1)); //Y
    o->pInUV   = OpipeCfgBuff (p, SIPP_UPFIRDN1_ID,        D_IN, o->cBufLut.base,  o->cBufLut.h,   BPP(1)); //UV (skip Y plane)

    o->pOutY   = OpipeCfgBuff (p, SIPP_UPFIRDN0_ID, D_DMA|D_OUT, o->cBufPoly.base, o->cBufPoly.h,  BPP(1)); //Y
    o->pOutUV  = OpipeCfgBuff (p, SIPP_UPFIRDN1_ID, D_DMA|D_OUT, o->cBufPoly.base, o->cBufPoly.h,  BPP(1)); //UV

 //The DBYR->DOGL SW link
  OpipeSwLink(p, SIPP_DBYR_LUMA_ID, (1<<SIPP_DOGL_ID)|(1<<SIPP_MED_LUMA_ID), SIPP_DOGL_ID);

  OpipeDetCfg(&o->p);

  #if defined(MIPI_CMX_SIGMA)
   //Break MipiRx-Sigma HW connection
    p->cfgMask &= ~(1<<mipiRxId);
   //Create SW link
    OpipeSwLink(p, mipiRxId, (1<<SIPP_SIGMA_ID), SIPP_SIGMA_ID);
  #endif
}
