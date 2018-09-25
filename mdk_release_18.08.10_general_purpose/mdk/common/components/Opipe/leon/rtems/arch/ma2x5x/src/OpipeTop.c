///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
/// @brief     Opipe - user API
///

#include <rtems.h>
#include "Opipe.h"
#include "OpipeInternals.h"
#include "assert.h"

OpipeGlobal gOpipe; //used by ISRs

//#################################################################################################
const SippInfo gSippNfo[] =
{
    /*SIPP_SIGMA_ID    */ {cfgSigma    , I_BASE(SIPP_SIGMA_ID    ), O_BASE(SIPP_SIGMA_ID    ), I_FC(SIPP_SIGMA_ID   ), I_IR(SIPP_SIGMA_ID   ), O_IR(SIPP_SIGMA_ID    ), COND(I_CTX(SIPP_SIGMA_ID   ), O_CTX(SIPP_SIGMA_ID   ) ) },
    /*SIPP_LSC_ID      */ {cfgLsc      , I_BASE(SIPP_LSC_ID      ), O_BASE(SIPP_LSC_ID      ), I_FC(SIPP_LSC_ID     ), I_IR(SIPP_LSC_ID     ), O_IR(SIPP_LSC_ID      ), COND(I_CTX(SIPP_LSC_ID     ), O_CTX(SIPP_LSC_ID     ) ) },
    /*SIPP_RAW_ID      */ {cfgRaw      , I_BASE(SIPP_RAW_ID      ), O_BASE(SIPP_RAW_ID      ), I_FC(SIPP_RAW_ID     ), I_IR(SIPP_RAW_ID     ), O_IR(SIPP_RAW_ID      ), COND(I_CTX(SIPP_RAW_ID     ), O_CTX(SIPP_RAW_ID     ) ) },
    /*SIPP_DBYR_ID     */ {cfgDbyr     , I_BASE(SIPP_DBYR_ID     ), O_BASE(SIPP_DBYR_ID     ), I_FC(SIPP_DBYR_ID    ), I_IR(SIPP_DBYR_ID    ), O_IR(SIPP_DBYR_ID     ), COND(I_CTX(SIPP_DBYR_ID    ), O_CTX(SIPP_DBYR_ID    ) ) },
    /*SIPP_DOGL_ID     */ {cfgDogLtm   , I_BASE(SIPP_DOGL_ID     ), O_BASE(SIPP_DOGL_ID     ), I_FC(SIPP_DOGL_ID    ), I_IR(SIPP_DOGL_ID    ), O_IR(SIPP_DOGL_ID     ), COND(I_CTX(SIPP_DOGL_ID    ), O_CTX(SIPP_DOGL_ID    ) ) },
    /*SIPP_LUMA_ID     */ {cfgLumaDns  , I_BASE(SIPP_LUMA_ID     ), O_BASE(SIPP_LUMA_ID     ), I_FC(SIPP_LUMA_ID    ), I_IR(SIPP_LUMA_ID    ), O_IR(SIPP_LUMA_ID     ), COND(I_CTX(SIPP_LUMA_ID    ), O_CTX(SIPP_LUMA_ID    ) ) },
    /*SIPP_SHARPEN_ID  */ {cfgSharpen  , I_BASE(SIPP_SHARPEN_ID  ), O_BASE(SIPP_SHARPEN_ID  ), I_FC(SIPP_SHARPEN_ID ), I_IR(SIPP_SHARPEN_ID ), O_IR(SIPP_SHARPEN_ID  ), COND(I_CTX(SIPP_SHARPEN_ID ), O_CTX(SIPP_SHARPEN_ID ) ) },
    /*SIPP_CGEN_ID     */ {cfgGenChr   , I_BASE(SIPP_CGEN_ID     ), O_BASE(SIPP_CGEN_ID     ), I_FC(SIPP_CGEN_ID    ), I_IR(SIPP_CGEN_ID    ), O_IR(SIPP_CGEN_ID     ), COND(I_CTX(SIPP_CGEN_ID    ), O_CTX(SIPP_CGEN_ID    ) ) },
    /*SIPP_MED_ID      */ {cfgMedian   , I_BASE(SIPP_MED_ID      ), O_BASE(SIPP_MED_ID      ), I_FC(SIPP_MED_ID     ), I_IR(SIPP_MED_ID     ), O_IR(SIPP_MED_ID      ), COND(I_CTX(SIPP_MED_ID     ), O_CTX(SIPP_MED_ID     ) ) },
    /*SIPP_CHROMA_ID   */ {cfgChromaDns, I_BASE(SIPP_CHROMA_ID   ), O_BASE(SIPP_CHROMA_ID   ), I_FC(SIPP_CHROMA_ID  ), I_IR(SIPP_CHROMA_ID  ), O_IR(SIPP_CHROMA_ID   ), COND(I_CTX(SIPP_CHROMA_ID  ), O_CTX(SIPP_CHROMA_ID  ) ) },
    /*SIPP_CC_ID       */ {cfgColComb  , I_BASE(SIPP_CC_ID       ), O_BASE(SIPP_CC_ID       ), I_FC(SIPP_CC_ID      ), I_IR(SIPP_CC_ID      ), O_IR(SIPP_CC_ID       ), COND(I_CTX(SIPP_CC_ID      ), O_CTX(SIPP_CC_ID      ) ) },
    /*SIPP_LUT_ID      */ {cfgLut      , I_BASE(SIPP_LUT_ID      ), O_BASE(SIPP_LUT_ID      ), I_FC(SIPP_LUT_ID     ), I_IR(SIPP_LUT_ID     ), O_IR(SIPP_LUT_ID      ), COND(I_CTX(SIPP_LUT_ID     ), O_CTX(SIPP_LUT_ID     ) ) },
    /*SIPP_EDGE_OP_ID  */ {NULL        , I_BASE(SIPP_EDGE_OP_ID  ), O_BASE(SIPP_EDGE_OP_ID  ), I_FC(SIPP_EDGE_OP_ID ), I_IR(SIPP_EDGE_OP_ID ), O_IR(SIPP_EDGE_OP_ID  ), COND(I_CTX(SIPP_EDGE_OP_ID ), O_CTX(SIPP_EDGE_OP_ID ) ) },
    /*SIPP_CONV_ID     */ {cfgConv     , I_BASE(SIPP_CONV_ID     ), O_BASE(SIPP_CONV_ID     ), I_FC(SIPP_CONV_ID    ), I_IR(SIPP_CONV_ID    ), O_IR(SIPP_CONV_ID     ), COND(I_CTX(SIPP_CONV_ID    ), O_CTX(SIPP_CONV_ID    ) ) },
    /*SIPP_HARRIS_ID   */ {cfgHarris   , I_BASE(SIPP_HARRIS_ID   ), O_BASE(SIPP_HARRIS_ID   ), I_FC(SIPP_HARRIS_ID  ), I_IR(SIPP_HARRIS_ID  ), O_IR(SIPP_HARRIS_ID   ), COND(I_CTX(SIPP_HARRIS_ID  ), O_CTX(SIPP_HARRIS_ID  ) ) },
    /*SIPP_UPFIRDN0_ID */ {cfgUpfirdn0 , I_BASE(SIPP_UPFIRDN0_ID ), O_BASE(SIPP_UPFIRDN0_ID ), I_FC(SIPP_UPFIRDN0_ID), I_IR(SIPP_UPFIRDN0_ID), O_IR(SIPP_UPFIRDN0_ID ), COND(I_CTX(SIPP_UPFIRDN0_ID), O_CTX(SIPP_UPFIRDN0_ID) ) },
    /*SIPP_UPFIRDN1_ID */ {cfgUpfirdn1 , I_BASE(SIPP_UPFIRDN1_ID ), O_BASE(SIPP_UPFIRDN1_ID ), I_FC(SIPP_UPFIRDN1_ID), I_IR(SIPP_UPFIRDN1_ID), O_IR(SIPP_UPFIRDN1_ID ), COND(I_CTX(SIPP_UPFIRDN1_ID), O_CTX(SIPP_UPFIRDN1_ID) ) },
    /*SIPP_UPFIRDN2_ID */ {cfgUpfirdn2 , I_BASE(SIPP_UPFIRDN2_ID ), O_BASE(SIPP_UPFIRDN2_ID ), I_FC(SIPP_UPFIRDN2_ID), I_IR(SIPP_UPFIRDN2_ID), O_IR(SIPP_UPFIRDN2_ID ), COND(I_CTX(SIPP_UPFIRDN2_ID), O_CTX(SIPP_UPFIRDN2_ID) ) },
    /*SIPP_MIPI_TX0_ID */ {NULL        , I_BASE(SIPP_MIPI_TX0_ID ), O_BASE(SIPP_DBYR_LUMA_ID),            0x00000000 ,            0x00000000 , O_IR(SIPP_DBYR_LUMA_ID), COND(                      0,                       0 ) },
    /*SIPP_MIPI_TX1_ID */ {NULL        ,                0x00000000,               0x00000000,             0x00000000 ,            0x00000000 ,             0x00000000 , COND(                      0,                       0 ) },
    /*SIPP_MIPI_RX0_ID */ {cfgMipiRx0  ,                0x00000000, O_BASE(SIPP_MIPI_RX0_ID ),            0x00000000 ,            0x00000000 , O_IR(SIPP_MIPI_RX0_ID ), COND(                      0, O_CTX(SIPP_MIPI_RX0_ID) ) },
    /*SIPP_MIPI_RX1_ID */ {cfgMipiRx1  , I_BASE(SIPP_MED_LUMA_ID ), O_BASE(SIPP_MIPI_RX1_ID ),            0x00000000 ,            0x00000000 , O_IR(SIPP_MIPI_RX1_ID ), COND(                      0, O_CTX(SIPP_MIPI_RX1_ID) ) },
    /*SIPP_MIPI_RX2_ID */ {cfgMipiRx2  , I_BASE(SIPP_CC_CHROMA_ID), O_BASE(SIPP_MIPI_RX2_ID ), I_FC(SIPP_CC_CHROMA_ID),I_IR(SIPP_CC_CHROMA_ID),O_IR(SIPP_MIPI_RX2_ID ), COND(I_CTX(SIPP_CC_CHROMA_ID),O_CTX(SIPP_MIPI_RX2_ID) ) },
    /*SIPP_MIPI_RX3_ID */ {cfgMipiRx3  ,                0x00000000, O_BASE(SIPP_MIPI_RX3_ID ),            0x00000000 ,            0x00000000 , O_IR(SIPP_MIPI_RX3_ID ), COND(                      0, O_CTX(SIPP_MIPI_RX3_ID) ) },

};

//#################################################################################################
uint32_t gOpipeErrCode;

void opipeError(uint32_t errCode)
{
    gOpipeErrCode = errCode;
    assert(gOpipeErrCode == 0);
}

//#################################################################################################
//Return line width in Bytes, considering available formats
uint32_t getLnWidthInBytes(int widthInPixels, int format)
{
    uint32_t width = widthInPixels;

    switch(format){
       case SIPP_FMT_8BIT : //normal formats (unpacked)
       case SIPP_FMT_16BIT:
       case SIPP_FMT_32BIT:  width = width * format;  break;
       case SIPP_FMT_PACK10: width = width * 5 / 4;   break; //10bit pack
       case SIPP_FMT_PACK12: width = width * 3 / 2;   break; //12bit pack
    }

    return width;
}

//#################################################################################################
//Reset internal stuff
void OpipeReset()
{
    gOpipeErrCode     = OPIPE_SUCCESS;
  //opipeInitMemPool();

   //tbd: use a memset instead, then do associations...
    gOpipe.unitsInUse = 0x00000000;
    gOpipe.nSources   = 0;
    gOpipe.nSinks     = 0;
    gOpipe.cdmaReq    = CDMA_DEF_REQ;
    gOpipe.nPipes     = 0;
    gOpipe.nSwLinks   = 0;
   // - clear counters
   // - assign default cfg routines
    DrvCmxDmaInitDefault();

   //Setup default sipp IRQ handlers, user can override !!!
   if(swcWhoAmI() == PROCESS_LEON_RT)
   {
     DrvIcbSetupIrq(IRQ_SIPP_0, SIPP_IRQ_LEVEL, POS_LEVEL_INT, oPipeSippIbflDecIsr); //ibfl_dec
     DrvIcbSetupIrq(IRQ_SIPP_1, SIPP_IRQ_LEVEL, POS_LEVEL_INT, oPipeSippObflIncIsr); //obfl_inc
     DrvIcbSetupIrq(IRQ_SIPP_2, SIPP_IRQ_LEVEL, POS_LEVEL_INT, oPipeSippEofIsr    ); //eof
   }
   else{
     DrvIcbDynamicIrqConfig (LRT_TO_LOS, IRQ_SIPP_0, IRQ_DYNAMIC_0, 0x1);
     DrvIcbDynamicIrqConfig (LRT_TO_LOS, IRQ_SIPP_1, IRQ_DYNAMIC_1, 0x1);
     DrvIcbDynamicIrqConfig (LRT_TO_LOS, IRQ_SIPP_2, IRQ_DYNAMIC_2, 0x1);

     DrvIcbSetupIrq(IRQ_DYNAMIC_0, SIPP_IRQ_LEVEL, POS_LEVEL_INT, oPipeSippIbflDecIsr);
     DrvIcbSetupIrq(IRQ_DYNAMIC_1, SIPP_IRQ_LEVEL, POS_LEVEL_INT, oPipeSippObflIncIsr);
     DrvIcbSetupIrq(IRQ_DYNAMIC_2, SIPP_IRQ_LEVEL, POS_LEVEL_INT, oPipeSippEofIsr    );
   }

   //By default, enable MEDIAN LUMA-REFERENCE priority
   //App can override later
    SET_REG_WORD(MSS_SUPER_RD_PRI_ADR, (1 << SIPP_MED_LUMA_ID)); //fastest option
}

//#################################################################################################
//Set default number of planes as per Opipe ISP
//After Pipe is initialized, user can customize nubmer of planes at app level
static void opipeDefaultNumPlanes(Opipe *p)
{
   p->iPlanes[SIPP_SIGMA_ID  ]   = p->oPlanes[SIPP_SIGMA_ID  ] = 1;
   p->iPlanes[SIPP_LSC_ID    ]   = p->oPlanes[SIPP_LSC_ID    ] = 1;
   p->iPlanes[SIPP_RAW_ID    ]   = p->oPlanes[SIPP_RAW_ID    ] = 1;
   p->iPlanes[SIPP_DOGL_ID   ]   = p->oPlanes[SIPP_DOGL_ID   ] = 1;
   p->iPlanes[SIPP_LUMA_ID   ]   = p->oPlanes[SIPP_LUMA_ID   ] = 1;
   p->iPlanes[SIPP_SHARPEN_ID]   = p->oPlanes[SIPP_SHARPEN_ID] = 1;

   p->iPlanes[SIPP_DBYR_ID  ] = 1; p->oPlanes[SIPP_DBYR_ID   ] = 3;  p->oPlanes[SIPP_DBYR_LUMA_ID] = 1;
   p->iPlanes[SIPP_CGEN_ID  ] =    p->oPlanes[SIPP_CGEN_ID   ] = 3;
   p->iPlanes[SIPP_MED_ID   ] =    p->oPlanes[SIPP_MED_ID    ] = 3;  p->iPlanes[SIPP_MED_LUMA_ID]  = 3;
   p->iPlanes[SIPP_CHROMA_ID] =    p->oPlanes[SIPP_CHROMA_ID ] = 3;
   p->iPlanes[SIPP_CC_ID    ] = 1; p->oPlanes[SIPP_CC_ID     ] = 3;  p->iPlanes[SIPP_CC_CHROMA_ID] = 3;
   p->iPlanes[SIPP_LUT_ID   ] = 1; p->oPlanes[SIPP_LUT_ID    ] = 3; //in will get programmed as 1xChannel

   p->iPlanes[SIPP_CONV_ID    ] = p->oPlanes[SIPP_CONV_ID    ] = 1;
   p->iPlanes[SIPP_EDGE_OP_ID ] = p->oPlanes[SIPP_EDGE_OP_ID ] = 1;
   p->iPlanes[SIPP_HARRIS_ID  ] = p->oPlanes[SIPP_HARRIS_ID  ] = 1;
   p->iPlanes[SIPP_UPFIRDN0_ID] = p->oPlanes[SIPP_UPFIRDN0_ID] = 1; //Y
   p->iPlanes[SIPP_UPFIRDN1_ID] = p->oPlanes[SIPP_UPFIRDN1_ID] = 2; //UV
   p->iPlanes[SIPP_UPFIRDN2_ID] = p->oPlanes[SIPP_UPFIRDN2_ID] = 1; //not used usually, but put a default

   p->oPlanes[SIPP_MIPI_RX0_ID] = 1; //no Input
   p->oPlanes[SIPP_MIPI_RX1_ID] = 1; //no Input
   p->oPlanes[SIPP_MIPI_RX2_ID] = 1; //no Input
   p->oPlanes[SIPP_MIPI_RX3_ID] = 1; //no Input

   p->iPlanes[SIPP_MIPI_TX0_ID] = 1; //no Output
   p->iPlanes[SIPP_MIPI_TX1_ID] = 1; //no Output
}

//#################################################################################################
void OpipeInit(Opipe *p)
{
  #if 0
   memset(p, 0, sizeof(Opipe ));
  #else
  { //TBD: align Opipe struct and do 64bit clear
     uint32_t i;
     uint8_t *ptr = (uint8_t*)p;
     for(i=0; i<sizeof(Opipe); i++)
       ptr[i] = 0;
  }
  #endif

   {
    uint32_t i;
    for(i=0; i<sizeof(p->cfg)/sizeof(p->cfg[0]); i++)
        p->cfg[i] = OP_UNINITIALIZED;
   }

   opipeDefaultNumPlanes(p);
 //p->flags  |= CLEAN_EXIT;     //clean IRQs by default
   p->cdmaReq = CDMA_DEF_REQ;   //default CMXDMA agent
   p->id      = gOpipe.nPipes++;//incrementing id
}

//#################################################################################################
// - check a single mipiRX->Sigma link exists
// - check subpipe continuity
// - check filters are not alredy in use
// -

//#################################################################################################
//Figure out OPIPE_CFG mask (if a Parent and associated Consumer are both ENABLED)
//en = filter enable mask
void OpipeDetCfg(Opipe *p)
{
    uint32_t en = p->enMask;
    uint32_t cfg = 0;

  //Early exit if already computed
    if(p->flags & DETERMINED_CFG)
       return;

  //Typical links [OUTPUT] -------------------------> [INPUT]
    if ((en & (1<<SIPP_SIGMA_ID  )) && (en & (1<<SIPP_LSC_ID    )))   cfg |= (1<<SIPP_SIGMA_ID);     //SIGMA-out -> LSC-in
    if ((en & (1<<SIPP_LSC_ID    )) && (en & (1<<SIPP_RAW_ID    )))   cfg |= (1<<SIPP_LSC_ID);       //LSC-out   -> RAW-in
  //Only try to enable is PREVIEW ability is disabled !
   if(!(p->flags & PREVIEW_ABLE)){
    if ((en & (1<<SIPP_RAW_ID    )) && (en & (1<<SIPP_DBYR_ID   )))   cfg |= (1<<SIPP_RAW_ID);       //RAW-out   -> DBYR-in
   }

   #if 0
   //this link is FAULTY in fp16 mode, so don't enable by default, see http://dub30/bugzilla/show_bug.cgi?id=22593
   //(however works ok in u8f mode)
    if ((en & (1<<SIPP_DBYR_ID   )) && (en & (1<<SIPP_DOGL_ID   )))   cfg |= (1<<SIPP_DBYR_LUMA_ID); //Dbyr.Luma -> DogLtm
   #endif

    if ((en & (1<<SIPP_DBYR_ID   )) && (en & (1<<SIPP_CGEN_ID   )))   cfg |= (1<<SIPP_DBYR_ID);      //Dbyr.Rgb  -> GenChroma
    if ((en & (1<<SIPP_DOGL_ID   )) && (en & (1<<SIPP_LUMA_ID   )))   cfg |= (1<<SIPP_DOGL_ID);      //DogLtm    -> LumaDns
    if ((en & (1<<SIPP_LUMA_ID   )) && (en & (1<<SIPP_SHARPEN_ID)))   cfg |= (1<<SIPP_LUMA_ID);      //LumaDns   -> Sharpen
    if ((en & (1<<SIPP_CGEN_ID   )) && (en & (1<<SIPP_MED_ID    )))   cfg |= (1<<SIPP_CGEN_ID);      //GenChroma -> Med
    if ((en & (1<<SIPP_MED_ID    )) && (en & (1<<SIPP_CHROMA_ID )))   cfg |= (1<<SIPP_MED_ID);       //Med       -> ChromaDns
    if ((en & (1<<SIPP_SHARPEN_ID)) && (en & (1<<SIPP_CC_ID     )))   cfg |= (1<<SIPP_SHARPEN_ID);   //Sharpen   -> CC-Luma in
    if ((en & (1<<SIPP_CHROMA_ID )) && (en & (1<<SIPP_CC_ID     )))   cfg |= (1<<SIPP_CHROMA_ID);    //ChromaDns -> CC-Chroma in
    if ((en & (1<<SIPP_CC_ID     )) && (en & (1<<SIPP_LUT_ID    )))   cfg |= (1<<SIPP_CC_ID);        //CC        -> LUT

  //Sigma can be driven by ONE(any) of the MipiRX instance
    if(en & (1<<SIPP_SIGMA_ID))
    { //If none is active, we'll get a |= 0x0, so nothing wrong
       cfg |= en & ((1<<SIPP_MIPI_RX0_ID)|(1<<SIPP_MIPI_RX1_ID)|
                    (1<<SIPP_MIPI_RX2_ID)|(1<<SIPP_MIPI_RX3_ID));
    }

  //Special case for LUT->UPFIRDN (need to enable both LUT and desired UPFIRDN filter)
  //LUT can drive any number of upfirdn instances
    if (en & (1<<SIPP_LUT_ID))
    {
        uint32_t upfirDnEn = en & ((1<<SIPP_UPFIRDN0_ID)|
                                   (1<<SIPP_UPFIRDN1_ID)|
                                   (1<<SIPP_UPFIRDN2_ID));

        if(upfirDnEn)
           cfg |= (1<<SIPP_LUT_ID) | upfirDnEn;
    }

    p->cfgMask = cfg;
    p->flags  |= DETERMINED_CFG;
}

//#################################################################################################
static DBuffer * opipeAppendSource(Opipe *p, DBuffer *newSrc, uint32_t unitID)
{
    uint32_t j;

    newSrc->dir = D_IN;

  //We're about to use another source, check we don't exceed array size
    if(p->nSrcs+1 > MAX_SOURCES)
        opipeError(OPIPE_ERR_TOO_MANY_SRC);

  //Register source with current pipe, but also globally
    p->srcs       [p->nSrcs       ++] = newSrc; //local  ref
    gOpipe.sources[gOpipe.nSources++] = newSrc; //global ref

  //Fill in the data struct
    newSrc->pipeRef        = (void*)p;
    newSrc->unitID         = unitID;

  //Init CMXDMA descriptor
    for(j=0; j<N_DESCS; j++)
     DrvCmxDmaTransactionBriefInit(DrvCmxDmaInitRequester(p->cdmaReq),
                                   DMA_2D_TRANSACTION,
                                   &newSrc->ddr.dmaDsc[j]);

  //Update interrupt mask
    p->sippSrcIntEnMask |= (1<<unitID);

    return newSrc;
}

//#################################################################################################
static DBuffer * opipeAppendSink(Opipe *p, DBuffer *newSink, uint32_t unitID)
{
    uint32_t j;

    newSink->dir = D_OUT;

  //We're about to use another sink, check we don't exceed array size
    if(p->nSnks+1 > MAX_SINKS)
        opipeError(OPIPE_ERR_TOO_MANY_SNK);

  //Register sink with current pipe, but also globally
    p->snks     [p->nSnks     ++] = newSink; //local ref
    gOpipe.sinks[gOpipe.nSinks++] = newSink; //global ref

  //Fill in the data struct
    newSink->pipeRef        = (void*)p;
    newSink->unitID         = unitID;

  //Init CMXDMA descriptor
    for(j=0; j<N_DESCS; j++)
      DrvCmxDmaTransactionBriefInit(DrvCmxDmaInitRequester(p->cdmaReq),
                                    DMA_2D_TRANSACTION,
                                    &newSink->ddr.dmaDsc[j]);

  //Update interrupt mask
    p->sippSnkIntEnMask   |= (1<<unitID);

    return newSink;
}

//#################################################################################################
//Appends a new buffer configuration
DBuffer * OpipeCfgBuff(Opipe    *p,
                       uint32_t  unitID,
                       uint32_t  flags,
                       uint32_t  cmxBuff,
                       uint32_t  cmxBuffH,
                       uint32_t  format)
{
  //Get ref to next buffer
    DBuffer *buf    = &p->cmxBuffs[p->nCmxBuffs];

    buf->unitID         = unitID;
    buf->cmx.height     = cmxBuffH;
    buf->fmt            = format;
    buf->irqRatePow     = IRQ_RATE_POW;
    buf->irqRate        = IRQ_RATE;

  //Figure out SIPP buffer BASE addr (In or Out)
    if(flags & D_IN) {//input
      buf->sippBuffBase = gSippNfo  [unitID].iBase;
      buf->nPlanes      = p->iPlanes[unitID];
      buf->dir          = D_IN;
    }
    else             {//output
      buf->sippBuffBase = gSippNfo  [unitID].oBase;
      buf->nPlanes      = p->oPlanes[unitID];
      buf->dir          = D_OUT;
    }

   //If CMX  buff not specified, allocate from internal mempool
   // if(0 == cmxBuff)
   //    cmxBuff = opipeAllocBuff(buf->cmx.lineStride * cmxBuffH * buf->nPlanes);
   // else
    {
      //If buff specified, make sure it's 8Bytes aligned !
      if(cmxBuff & 0x7)
        opipeError(OPIPE_ERR_UNALIGNED_CBUF);
    }
    buf->cmx.base  = cmxBuff;

    p->nCmxBuffs++;

   //=====================================================================
   //Figure out internally if it's input and ouput and append to Source or Sink
   if(flags & D_DMA)
   {
       if(flags & D_IN) opipeAppendSource(p, buf, unitID);
       else             opipeAppendSink  (p, buf, unitID);
   }

   return buf;
}

//#################################################################################################
//Notes: 1) All consumer heights need to be the same !
//       2) A producer can fill multiple consumers (e.g. DOGL fills LUMA and MED_LUMA_REF),
//          but just the last consumer will decrement producer out level once data is consumed
SwLink* OpipeSwLink(Opipe   *p,
                    uint32_t prodID,
                    uint32_t allConsMask,
                    uint32_t lastConsID)
{
    //TBD: check we don't exceed max range !
    SwLink *lnk        = &p->swCtrl[p->nSwLinks++];
    gOpipe.swCtrl[gOpipe.nSwLinks++] = lnk; //global ref

    lnk->pipeRef       = p;
    lnk->prodId        = prodID;
    lnk->allConsIdMask = allConsMask;
    lnk->lastConsId    = lastConsID;

    lnk->prodMon.    irqRatePow = IRQ_RATE_POW;
    lnk->lastConsMon.irqRatePow = IRQ_RATE_POW;

    lnk->prodMon.    irqRate = IRQ_RATE;
    lnk->lastConsMon.irqRate = IRQ_RATE;

   #if defined(TAPOUT_EN)
   //When pipe is created, entire struct is memset(0)
   //threfore sink->link = NULL by default

   //Search SINKS and keep a reference
    uint32_t i;
    for(i=0; i<p->nSnks; i++)
    {
      DBuffer *curSink = p->snks[i];
      if(curSink->unitID == prodID)
      {
       //Create the cross reference:
         lnk->sink     = curSink;
         curSink->link = lnk;
       //printInt(0xC0FEE111);
         break;
      }
    }
   #endif

    return lnk;
}

//#################################################################################################
//HW resets SIPP filters
void OpipeHwReset(uint32_t mask)
{
   rtems_interrupt_level level;
   rtems_interrupt_disable(level);

  #if 0
  //Reset ALL sipp filters:
   CLR_REG_BITS_MASK(MSS_RSTN_CTRL_ADR     , 1<<MSS_CP_SPMP);
   SET_REG_BITS_MASK(MSS_RSTN_CTRL_ADR     , 1<<MSS_CP_SPMP);
  #else
  //Reset individual filters
   CLR_REG_BITS_MASK(MSS_SIPP_RSTN_CTRL_ADR, mask);
   SET_REG_BITS_MASK(MSS_SIPP_RSTN_CTRL_ADR, mask);
  #endif
  //Must include MSS_SP_SIPP_ABPSLV here as sometimes default clk configs don't set it.
   SET_REG_BITS_MASK(MSS_SIPP_CLK_CTRL_ADR , mask | (1<<MSS_SP_SIPP_ABPSLV));
   rtems_interrupt_enable(level);
}
//#################################################################################################
//For filters that belong to current pipeline, check that
//a) associated config data structures exist
//b) custom filter buffs (e.g. LSG-gain-map, LUT-table) are 64bit aligned

static void checkConfigs(Opipe *p)
{
    uint32_t en = p->enMask;

    if(en & (1<<SIPP_SIGMA_ID)){
       if(!p->pBlcCfg  ) opipeError(OPIPE_ERR_MISSING_CFG);
       if(!p->pSigmaCfg) opipeError(OPIPE_ERR_MISSING_CFG);
    }

    if(en & (1<<SIPP_LSC_ID)){
       if(!p->pLscCfg)                                 opipeError(OPIPE_ERR_MISSING_CFG);
       else {
           if(((uint32_t)p->pLscCfg->pLscTable) & 0x7) opipeError(OPIPE_ERR_UNALIGNED_FBUF);
           if(((uint32_t)p->pLscCfg->lscStride) & 0x3) opipeError(OPIPE_ERR_UNALIGNED_GM_S);
       }
    }

    if(en & (1<<SIPP_LUT_ID)){
              if(!p->pLutCfg)                         opipeError(OPIPE_ERR_MISSING_CFG);
         else if(((uint32_t)p->pLutCfg->table) & 0x7) opipeError(OPIPE_ERR_UNALIGNED_FBUF);
    }

    if(en & (1<<SIPP_RAW_ID)){
       if(!p->pRawCfg)                    opipeError(OPIPE_ERR_MISSING_CFG);
     //stats buffs should be null if not set by user, so below checks won't harm
       if(((uint32_t)p->aeStats ) & 0x7)  opipeError(OPIPE_ERR_UNALIGNED_FBUF);
       if(((uint32_t)p->afStats ) & 0x7)  opipeError(OPIPE_ERR_UNALIGNED_FBUF);
       if(((uint32_t)p->histLuma) & 0x7)  opipeError(OPIPE_ERR_UNALIGNED_FBUF);
       if(((uint32_t)p->histRgb ) & 0x7)  opipeError(OPIPE_ERR_UNALIGNED_FBUF);
    }

    if(en & (1<<SIPP_DOGL_ID)){
        //might have just one of the modes here... ?
        if((!p->pLtmCfg)||(!p->pDogCfg))  opipeError(OPIPE_ERR_MISSING_CFG);
    }

    if(en & (1<<SIPP_LUMA_ID)){
        if(!p->pLumaDnsCfg   ) opipeError(OPIPE_ERR_MISSING_CFG);
        if(!p->pLumaDnsRefCfg) opipeError(OPIPE_ERR_MISSING_CFG);
    }

    if((en & (1<<SIPP_SHARPEN_ID )) && (!p->pSharpCfg    )) opipeError(OPIPE_ERR_MISSING_CFG);
    if((en & (1<<SIPP_CGEN_ID    )) && (!p->pChrGenCfg   )) opipeError(OPIPE_ERR_MISSING_CFG);
    if((en & (1<<SIPP_MED_ID     )) && (!p->pMedCfg      )) opipeError(OPIPE_ERR_MISSING_CFG);
    if((en & (1<<SIPP_CHROMA_ID  )) && (!p->pChromaDnsCfg)) opipeError(OPIPE_ERR_MISSING_CFG);
    if((en & (1<<SIPP_CC_ID      )) && (!p->pColCombCfg  )) opipeError(OPIPE_ERR_MISSING_CFG);
    if((en & (1<<SIPP_UPFIRDN0_ID)) && (!p->pUpfirdn0Cfg )) opipeError(OPIPE_ERR_MISSING_CFG);
    if((en & (1<<SIPP_UPFIRDN1_ID)) && (!p->pUpfirdn12Cfg)) opipeError(OPIPE_ERR_MISSING_CFG);
    if((en & (1<<SIPP_UPFIRDN2_ID)) && (!p->pUpfirdn12Cfg)) opipeError(OPIPE_ERR_MISSING_CFG);
    if((en & (1<<SIPP_MIPI_RX0_ID)) && (!p->pMipiRxCfg[0])) opipeError(OPIPE_ERR_MISSING_CFG);
    if((en & (1<<SIPP_MIPI_RX1_ID)) && (!p->pMipiRxCfg[1])) opipeError(OPIPE_ERR_MISSING_CFG);
    if((en & (1<<SIPP_MIPI_RX2_ID)) && (!p->pMipiRxCfg[2])) opipeError(OPIPE_ERR_MISSING_CFG);
    if((en & (1<<SIPP_MIPI_RX3_ID)) && (!p->pMipiRxCfg[3])) opipeError(OPIPE_ERR_MISSING_CFG);
    if((en & (1<<SIPP_CONV_ID    )) && (!p->pConvCfg     )) opipeError(OPIPE_ERR_MISSING_CFG);
    //TBD: harris, edge...
}

//#################################################################################################
//Start a pipe:
//Return values: 0 for success, else error codes....
uint32_t OpipeStart(Opipe *p)
{
    uint32_t i;
    rtems_interrupt_level level;
  //=====================================================================
  //Disable filters of interest
    rtems_interrupt_disable(level);
    CLR_REG_BITS_MASK(SIPP_CONTROL_ADR, p->enMask);
    rtems_interrupt_enable(level);
  //Reset internal states (including icbl/ocbl) of used filters:
    OpipeHwReset(p->enMask);

  //=====================================================================
  //Figure out & program Opipe links; it's important to do this early
  //so that filters don't accidentally start
    if((p->flags & DETERMINED_CFG) == 0)
       OpipeDetCfg(p);

    rtems_interrupt_disable(level);
    SET_REG_BITS_MASK(SIPP_OPIPE_CFG_ADR, p->cfgMask);
    rtems_interrupt_enable(level);

  //=====================================================================
  //Resolution updates (if needed)
    OpipeSetRes(p, p->width, p->height);

    checkConfigs (p);

  //=====================================================================
  //Program the filters:
    for(i = SIPP_SIGMA_ID; i <= SIPP_MIPI_RX3_ID; i++)
    {
       if((p->enMask & (1<<i)) && (gSippNfo[i].cfgFn != NULL))
          gSippNfo[i].cfgFn(p);
    }

  //=====================================================================
  //Program defined CMX buffers to proper SIPP I/O buffer defs
    for(i=0; i<p->nCmxBuffs; i++)
    {
      DBuffer *buf = &p->cmxBuffs[i];
      uint32_t nPlanes = buf->nPlanes;

     //Or here the cfg[SIPP_LUT_ID] & channel enabled
      if(buf->sippBuffBase == I_BASE(SIPP_LUT_ID))
         nPlanes = 1;//channel mode

      progCmxBuff(buf->sippBuffBase,
                  buf->cmx.base,
                  buf->cmx.lineStride,
                  buf->cmx.height,
                       nPlanes,
                  buf->fmt);
    }

  //=====================================================================
  //If Med is used, set the Luma stride = 0
    if(p->enMask & (1<<SIPP_MED_ID))
      SET_REG_WORD(I_PS(SIPP_MED_LUMA_ID), 0); //so that all 3 planes map on the single Y plane

  //Or-Enable filters of interest
    rtems_interrupt_disable(level);
    SET_REG_BITS_MASK(SIPP_CONTROL_ADR,p->enMask);
    rtems_interrupt_enable(level);

  //=====================================================================
    cfgSources  (p);
    cfgSinks    (p);
    cfgSwLinks  (p);

  //=====================================================================
  //Enable ints for Sources and Sinks
  //(might need mutex protection here ????)
    rtems_interrupt_disable(level);

    // Bugzilla 27952: Clear left-over interrupts before enabling them
    SET_REG_WORD(SIPP_INT0_CLEAR_ADR, p->sippSrcIntEnMask);
    SET_REG_WORD(SIPP_INT1_CLEAR_ADR, p->sippSnkIntEnMask);
    SET_REG_WORD(SIPP_INT2_CLEAR_ADR, p->sippSnkIntEnMask);

    SET_REG_BITS_MASK(SIPP_INT0_ENABLE_ADR, p->sippSrcIntEnMask);
    SET_REG_BITS_MASK(SIPP_INT1_ENABLE_ADR, p->sippSnkIntEnMask);
    SET_REG_BITS_MASK(SIPP_INT2_ENABLE_ADR, p->sippSnkIntEnMask);//patch for not working obfl_inc

    rtems_interrupt_enable(level);
  //Patch time....
    if(p->cbPreStart)
       p->cbPreStart(p);

  #if 0
    /*DBG*/ printInt(0x12345678);
    /*DBG*/ printInt(p->cfgMask);
    /*DBG*/ printInt(p->enMask);
    /*DBG*/ printInt(p->sippSrcIntEnMask);
    /*DBG*/ printInt(p->sippSnkIntEnMask);
  #endif

  #if defined(OPIPE_RUNTIME_CHECKS)
    p->nDmaPending = 0;
    p->flags      &= ~LOST_IRQ;
  #endif

    p->flags |=  PIPE_RUNNING;
    startSources(p); //the actual start
    return 0;
}
//#################################################################################################
uint32_t OpipeDisable(Opipe *p)
{
   //If pipe is running, ret some error codes...
   #if defined(OPIPE_RUNTIME_CHECKS)
    if(p->nDmaPending         ) return 1;
   #endif
    if(p->flags & PIPE_RUNNING) return 2;

   //Else, atomic operations ...
    rtems_interrupt_level level;
    rtems_interrupt_disable(level);

   //Disable own filters and links
    CLR_REG_BITS_MASK(SIPP_CONTROL_ADR,   p->enMask);
    CLR_REG_BITS_MASK(SIPP_OPIPE_CFG_ADR, p->cfgMask);

   //Disable associated interrupts
    CLR_REG_BITS_MASK(SIPP_INT0_ENABLE_ADR, p->sippSrcIntEnMask);
    CLR_REG_BITS_MASK(SIPP_INT1_ENABLE_ADR, p->sippSnkIntEnMask);
    CLR_REG_BITS_MASK(SIPP_INT2_ENABLE_ADR, p->sippSnkIntEnMask);//patch for not working obfl_inc

    rtems_interrupt_enable(level);
    return 0; //success
}

//#################################################################################################
//A pipe finished when all its sinks copied all lines to destination
uint32_t OpipeFinished(Opipe *p)
{
    uint32_t i, done = 1;

   #if defined(OPIPE_RUNTIME_CHECKS)
    if(p->nDmaPending)
       return 0;
   #endif

    for(i=0; i < p->nSnks; i++)
     if(p->snks[i]->ddr.cpLines < p->snks[i]->ddr.height){
         done = 0; //mark we're not done yet
         break;
     }

    if(done){
      //clear running flag
       p->flags &= ~(PIPE_RUNNING);

       if(p->flags & CLEAN_EXIT){
         OpipeDisable(p);
       }
    }

    return done;
}

//#################################################################################################
uint32_t OpipeWait(Opipe *p)
{
    while(!OpipeFinished(p)){
        NOP;NOP;NOP;NOP;NOP; NOP;NOP;NOP;NOP;NOP;
        NOP;NOP;NOP;NOP;NOP; NOP;NOP;NOP;NOP;NOP;
        NOP;NOP;NOP;NOP;NOP; NOP;NOP;NOP;NOP;NOP;
        NOP;NOP;NOP;NOP;NOP; NOP;NOP;NOP;NOP;NOP;
    }

    return 0;//some error code? (e.g. if not started...)
}

//#################################################################################################
//Filter main path finished, but stats-writing is still in progress
//so wait till all status bits of interest are set !
//http://dub30/bugzilla/show_bug.cgi?id=21484
void OpipeWaitForRawStats()
{
   uint32_t status1, status2;
   uint32_t mask1,   mask2;

   mask1 =   (1<<SIPP_STATS_ID);
   mask2 =   (1<<SIPP_AF_STATS_ID)
            |(1<<SIPP_LUMA_HIST_ID)
            |(1<<SIPP_RGB_HIST_ID);
   do{
     status1 = GET_REG_WORD_VAL(SIPP_INT1_STATUS_ADR);
     status2 = GET_REG_WORD_VAL(SIPP_INT2_STATUS_ADR);
     NOP;NOP;NOP;NOP;NOP;
     if((status1 & mask1) || (status2 & mask2))
         break;
   }while(1);

  //TBD: clear bits next...
}

//private helper
extern UpfirdnCfg* getUpfirdnOutRes(Opipe *pipe, uint32_t unitID, uint32_t *outW, uint32_t *outH);

//#################################################################################################
//"front filters" = filters above DBYR
static int isFrontFilter(uint32_t unitId)
{
    return (/*(unitId >= SIPP_SIGMA_ID   ) &&*/ (unitId <  SIPP_DBYR_ID    )) ||
           (  (unitId >= SIPP_MIPI_RX0_ID) &&   (unitId <= SIPP_MIPI_RX3_ID));
}

//#################################################################################################
static void detSecondaryRes(Opipe *p)
{
    p->width2  = p->width;
    p->height2 = p->height;

    //If preview is enabled, filters below debayer operate on quartered input
    if(p->flags & GEN_PREVIEW){
       p->width2  = p->width2  >> 1;
       p->height2 = p->height2 >> 1;
    }

    if(p->fullW == 0) p->fullW = p->width;
    if(p->fullH == 0) p->fullH = p->height;
}

//#################################################################################################
//Set new input resolution (width and height) in pixels
//output resolution depends on end filters and resize factors
//When this is invoked, it's assumed polyfir scale factors are properly set
//(if polyfir filtres are used at all)
void OpipeSetRes(Opipe *p, uint32_t newW, uint32_t newH)
{
    uint32_t i;
    uint32_t oUpfirdnH[3];//polyfir out height

  //Early exit (TBD: factor genPreview status)
    if((p->oldW == p->width) && (p->oldH == p->height))
      return;

  //Else, resolution is truly new
    p->width  = newW;
    p->height = newH;
    detSecondaryRes(p); //update width2, height2

  //================================================================
  //Update all CMX buffers
    for(i=0; i<p->nCmxBuffs; i++)
    {
       DBuffer *buf    = &p->cmxBuffs[i];
       uint32_t unitId = buf->unitID;
       uint32_t w; //buffer width

       //Top buffers inherit "width"
       if(isFrontFilter(unitId) || ((unitId==SIPP_DBYR_ID)&&(buf->dir==D_IN)))
       {
            w = p->width;
       }
       //Filters below debayer inherit "width2"
       else{ //Special case for polyFir output buffs
           if((unitId >= SIPP_UPFIRDN0_ID)&&(unitId <= SIPP_UPFIRDN2_ID)&&(buf->dir == D_OUT))
              getUpfirdnOutRes(p, unitId, &w, &oUpfirdnH[unitId-SIPP_UPFIRDN0_ID]);
           else
              w = p->width2;
       }

       buf->cmx.lineW      = getLnWidthInBytes(w, buf->fmt);
       buf->cmx.lineStride = DW_ALIGN(buf->cmx.lineW);
     //by default, DDR settings match:
       buf->ddr.lineW      = buf->cmx.lineW;
       buf->ddr.lineStride = buf->ddr.lineW;
    }

  //================================================================
  //Update sink heights
  for(i=0; i < p->nSnks; i++){
    DBuffer *sink = p->snks[i];
    if(isFrontFilter(sink->unitID))
        sink->ddr.height = p->height;
    //Special case for polyfir sinks (where output res is a function of resize params N & D
    //("p->pUpfirdn12Cfg" will factor the UV420 resize !!!)
    else if((sink->unitID >= SIPP_UPFIRDN0_ID)&&(sink->unitID <= SIPP_UPFIRDN2_ID))
        sink->ddr.height = oUpfirdnH[sink->unitID-SIPP_UPFIRDN0_ID];
    else if(sink->unitID == SIPP_CGEN_ID) //TBD: or other chroma filters...
        sink->ddr.height = p->height2>>1;
    else
        sink->ddr.height = p->height2;
  }

  //================================================================
  //Update sources heights
  for(i=0; i < p->nSrcs; i++){
    DBuffer *src = p->srcs[i];
    if(isFrontFilter(src->unitID) || (src->unitID == SIPP_DBYR_ID))
         src->ddr.height = p->height ;
    else src->ddr.height = p->height2;
  }

  //================================================================
  //Update swLinks heights
   for(i=0; i < p->nSwLinks; i++)
   {
      uint32_t h;
      SwLink *lnk = &p->swCtrl[i];

      if(isFrontFilter(lnk->prodId)) h = p->height;
      else                           h = p->height2;
      //TBD: polyfir ...

      p->swCtrl[i].prodMon.    height = h;//OUT_RES(prodID)
      p->swCtrl[i].lastConsMon.height = h;//IN_RES(lastConsID)
   }

   p->oldW = p->width;
   p->oldH = p->height;
  //OpipeStart will call "cfgSources", "cfgSinks" so sinks and sources will use
  //new cmx buffer line width
}

//#################################################################################################
static void setFmt(uint32_t reg, uint32_t fmt)
{
  uint32_t data = GET_REG_WORD_VAL(reg);
  data &= ~(0xF << SIPP_FO_OFFSET); //clear old format
  data |=   fmt << SIPP_FO_OFFSET;  //set   new format
  SET_REG_WORD(reg, data);
}

//#################################################################################################
//Patch buffer formats for u8f luma
void OpipeForceU8fLuma(Opipe *p)
{
   //Temporaly disable filters
    CLR_REG_BITS_MASK(SIPP_CONTROL_ADR,p->enMask);

   //TBD: check bits are set in enMask !!!
    setFmt(O_CFG(SIPP_DBYR_LUMA_ID), 1/*u8f*/);
    setFmt(I_CFG(SIPP_DOGL_ID),      1/*u8f*/);
    setFmt(O_CFG(SIPP_DOGL_ID),      1/*u8f*/);
    setFmt(I_CFG(SIPP_MED_LUMA_ID),  1/*u8f*/);
    setFmt(O_CFG(SIPP_SHARPEN_ID),   1/*u8f*/);
    setFmt(I_CFG(SIPP_CC_ID),        1/*u8f*/);

   //Re-enable filters
    SET_REG_BITS_MASK(SIPP_CONTROL_ADR,p->enMask);
}

