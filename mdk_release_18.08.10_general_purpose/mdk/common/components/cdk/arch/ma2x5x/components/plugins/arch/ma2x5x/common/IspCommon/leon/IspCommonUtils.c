#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "DrvTimer.h"
#include "swcLeonUtils.h"
#include "Opipe.h"
#include "PlgTypes.h"
#include "IspCommon.h"
#include "IspCommonUtils.h"
#include "ipipeDbg.h"
#include "ipipeUtils.h"

/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/
#ifndef PLG_ISP_MAX_GAMMA_SIZE
#define PLG_ISP_MAX_GAMMA_SIZE (512*4*2)
#endif

#ifndef PLG_ISP_MAX_LSC_SIZE
#define PLG_ISP_MAX_LSC_SIZE (64*44)
#endif

#ifndef PLG_ISP_MAX_AE_NO_PATCH
#define PLG_ISP_MAX_AE_NO_PATCH (32*32)
#endif

#ifndef PLG_ISP_MAX_AF_NO_PATCH
#define PLG_ISP_MAX_AF_NO_PATCH (8*8)
#endif
#ifndef PLG_ISP_GAMMA_SECT
#define PLG_ISP_GAMMA_SECT ".cmx.bss.gammaTable"
#endif

#ifndef PLG_ISP_LSC_SECT
#define PLG_ISP_LSC_SECT ".cmx.bss.lscTable"
#endif

#ifndef PLG_ISP_AE_STATS_SECT
#define PLG_ISP_AE_STATS_SECT ".cmx.bss.aestats"
#endif

#ifndef PLG_ISP_AF_STATS_SECT
#define PLG_ISP_AF_STATS_SECT ".cmx.bss.afstats"
#endif
/**************************************************************************************************
 ~~~  Local variables
 **************************************************************************************************/

uint8_t  locGamma[PLG_ISP_MAX_GAMMA_SIZE] SECTION(PLG_ISP_GAMMA_SECT) ALIGNED(8);
LutCfg   locLutCfg  = {0, 0, {0, 0}, (uint16_t*)locGamma};

//Workaround for: http://dub30/bugzilla/show_bug.cgi?id=22777
//CMXDMA task list to copy LUT(s) from DDR to CMX (where HW can load them up).
//For now it's just gammaLut, but 3D lut follows...
dmaTransactionList_t dmaLutTasks[1];
uint8_t              gammaCopyWait = 1; //wait for LUT copy to finish (won't wait in RX driven ISPs)

//Mem alloc related:
uint8_t  *ogcBuff[24]; //opipe global circular buffer ptr

// Lsc tablead read from ddr introduce a delay that crash opipe, is is connected directly to source
uint16_t ispcLscMeshLoc[PLG_ISP_MAX_LSC_SIZE] SECTION(PLG_ISP_LSC_SECT) ALIGNED(8);
LscCfg locLscCfg  = {0, 0, 0, (uint16_t*)ispcLscMeshLoc};
dmaTransactionList_t dmaLscTasks[1];
uint8_t              lscCopyWait = 1;

AeAwbPatchStats  aeAwbPatchStatsLoc[PLG_ISP_MAX_AE_NO_PATCH] SECTION(PLG_ISP_AE_STATS_SECT) ALIGNED(8);
AfPatchStats     afPatchStatsLoc[PLG_ISP_MAX_AF_NO_PATCH] SECTION(PLG_ISP_AF_STATS_SECT) ALIGNED(8);
dmaTransactionList_t dmaStatsTasks[2];
//
/**************************************************************************************************
 ~~~  Local functions
 **************************************************************************************************/
 //Initialize a CMXDMA descriptor for 1D
static void setLutCopyDmaTask(dmaTransactionList *dmaD, void *pDst, uint32_t len)
{
    DrvCmxDmaTransactionBriefInit(DrvCmxDmaInitRequester(3),
                                  DMA_1D_TRANSACTION,
                                  dmaD);

    dmaD->src              = 0;   //will update at runtime
    dmaD->dst              = pDst;//this is constant
    dmaD->src_width        = len; //line_size
    dmaD->dst_width        = len; //line_size
    dmaD->length           = len;
    dmaD->no_planes        = 0;   //0+1 = 1 "plane"
   //Strides are don't care
    dmaD->src_stride       = dmaD->dst_stride       = 0;
    dmaD->src_plane_stride = dmaD->dst_plane_stride = 0;
}

/**************************************************************************************************
 ~~~ Exported Functions Implementation
 **************************************************************************************************/

void initLutCopyTask()
{
  //Bug 22777 :Init DMA task for backing LUTs in CMX
    setLutCopyDmaTask(&dmaLutTasks[0], (void*)locGamma, sizeof(locGamma));
  #if 0
  //When adding 3D lut:
    setLutCopyDmaTask (&dmaLutTasks[1], (void*)loc3dLut, sizeof(loc3dLut));
    DrvCmxDmaLinkTasks(&dmaLutTasks[0], 1, &dmaLutTasks[1]);
  #endif
}

void initLscCopyTask()
{
    dmaTransactionList *dmaD = &dmaLscTasks[0];
    DrvCmxDmaTransactionBriefInit(DrvCmxDmaInitRequester(3),
            DMA_1D_TRANSACTION,
            dmaD);
    dmaD->src              = 0;   //will update at runtime
    dmaD->dst              = (void*)ispcLscMeshLoc;//this is constant
    dmaD->src_width        = PLG_ISP_MAX_LSC_SIZE; //line_size
    dmaD->dst_width        = PLG_ISP_MAX_LSC_SIZE; //line_size
    dmaD->length           = PLG_ISP_MAX_LSC_SIZE;
    dmaD->no_planes        = 0;   //0+1 = 1 "plane"
    //Strides are don't care
    dmaD->src_stride       = dmaD->dst_stride       = 0;
    dmaD->src_plane_stride = dmaD->dst_plane_stride = 0;
}

void initStatsCopyTask(void)
{
    dmaTransactionList *dmaD = &dmaStatsTasks[0];
    DrvCmxDmaTransactionBriefInit(DrvCmxDmaInitRequester(3),
            DMA_1D_TRANSACTION,
            dmaD);
    dmaD->src              = (void*)aeAwbPatchStatsLoc;//this is constant
    dmaD->dst              = (void*)0;//will update at runtime
    dmaD->src_width        = 0; //will update at runtime
    dmaD->dst_width        = 0; //will update at runtime
    dmaD->length           = 0;//will update at runtime
    dmaD->no_planes        = 0;   //0+1 = 1 "plane"
    //Strides are don't care
    dmaD->src_stride       = dmaD->dst_stride       = 0;
    dmaD->src_plane_stride = dmaD->dst_plane_stride = 0;

    dmaD = &dmaStatsTasks[1];
    DrvCmxDmaTransactionBriefInit(DrvCmxDmaInitRequester(3),
            DMA_1D_TRANSACTION,
            dmaD);
    dmaD->src              = (void*)afPatchStatsLoc;//this is constant
    dmaD->dst              = (void*)0;//will update at runtime
    dmaD->src_width        = 0; //will update at runtime
    dmaD->dst_width        = 0; //will update at runtime
    dmaD->length           = 0;//will update at runtime
    dmaD->no_planes        = 0;   //0+1 = 1 "plane"
    //Strides are don't care
    dmaD->src_stride       = dmaD->dst_stride       = 0;
    dmaD->src_plane_stride = dmaD->dst_plane_stride = 0;

}

void plgIspStartStatsCopy(Opipe *p, AeAwbPatchStats* pCfgAeStats, AfPatchStats* pCfgAfStats) {
    uint32_t curPil;
    uint32_t aeSize = 0;
    uint32_t afSize = 0;
    uint32_t statsEnabledBitMask = 0;
    if(pCfgAeStats && p->aeCfg) { // the ae stats are required
        aeSize = p->aeCfg->nPatchesX * p->aeCfg->nPatchesY * sizeof(AeAwbPatchStats);
        if(aeSize) {
            // there is assumption about max buffer size,PLG_ISP_MAX_AE_NO_PATCH can be overwrite from Makefile
            assert((p->aeCfg->nPatchesX * p->aeCfg->nPatchesY) <= PLG_ISP_MAX_AE_NO_PATCH);
            dmaStatsTasks[0].dst = (void*)ipipeGetNoCacheAddr((uint32_t)pCfgAeStats);
            dmaStatsTasks[0].src_width        = aeSize; //line_size
            dmaStatsTasks[0].dst_width        = aeSize; //line_size
            dmaStatsTasks[0].length           = aeSize;
            statsEnabledBitMask |= 1;
        }
    }
    if (pCfgAfStats && p->afCfg) { // the af stats are required
        afSize = p->afCfg->nPatchesX * p->afCfg->nPatchesY * sizeof(AfPatchStats);
        if(afSize) {
            // there is assumption about max buffer size, PLG_ISP_MAX_AF_NO_PATCH can be overwrite from Makefile
            assert((p->afCfg->nPatchesX * p->afCfg->nPatchesY) <= PLG_ISP_MAX_AF_NO_PATCH);
            dmaStatsTasks[1].dst = (void*)ipipeGetNoCacheAddr((uint32_t)pCfgAfStats);
            dmaStatsTasks[1].src_width        = afSize; //line_size
            dmaStatsTasks[1].dst_width        = afSize; //line_size
            dmaStatsTasks[1].length           = afSize;
            statsEnabledBitMask |= 2;
        }
    }
    switch (statsEnabledBitMask) {
    case 3:
        DrvCmxDmaLinkTasks(&dmaStatsTasks[0], 1, &dmaStatsTasks[1]);
        curPil = swcLeonSetPIL(15);
        DrvCmxDmaStartListTask(&dmaStatsTasks[0]);
        swcLeonSetPIL(curPil);
        break;
    case 1:
        curPil = swcLeonSetPIL(15);
        DrvCmxDmaStartListTask(&dmaStatsTasks[0]);
        swcLeonSetPIL(curPil);
        break;
    case 2:
        curPil = swcLeonSetPIL(15);
        DrvCmxDmaStartListTask(&dmaStatsTasks[1]);
        swcLeonSetPIL(curPil);
        break;
    default:
        // not stats required
        break;
    }
}
void fetchIcIspConfig(Opipe *p, icIspConfig *ic) {
    p->rawBits      = ic->pipelineBits;
    p->bayerPattern = ic->bayerOrder;

    //Filter specific
    p->pBlcCfg        = &ic->blc;
    p->pSigmaCfg      = &ic->sigma;
    p->pLscCfg        = &ic->lsc;
    p->pRawCfg        = &ic->raw;
    p->pDbyrCfg       = &ic->demosaic;
    p->pLtmCfg        = &ic->ltm;
    p->pDogCfg        = &ic->dog;
    p->pLumaDnsCfg    = &ic->lumaDenoise;
    p->pLumaDnsRefCfg = &ic->lumaDenoiseRef;
    p->pSharpCfg      = &ic->sharpen;
    p->pChrGenCfg     = &ic->chromaGen;
    p->pMedCfg        = &ic->median;
    p->pChromaDnsCfg  = &ic->chromaDenoise;
    p->pColCombCfg    = &ic->colorCombine;
    p->pLutCfg        = &locLutCfg; //Tell Opipe to use local LutCfg instead of &ic->gamma;
    p->pColConvCfg    = &ic->colorConvert;
    p->aeCfg          = &ic->aeAwbConfig;
    p->aeStats        = ic->aeAwbStats;
    p->afCfg          = &ic->afConfig;
    p->afStats        =  ic->afStats;
    p->pUpfirdn0Cfg   = &ic->updnCfg0;
    p->pUpfirdn12Cfg  = &ic->updnCfg12;

   if(p->pLutCfg)
   {//Workaround for bug 22777
      uint32_t curPil;

      locLutCfg       = ic->gamma;
      locLutCfg.table = (uint16_t*)locGamma;
    #if 0
      memcpy((void*)ipipeGetNoCacheAddr((uint32_t)locGamma), (void*)ipipeGetNoCacheAddr((uint32_t)ic->gamma.table), sizeof(locGamma));
    #else
      dmaLutTasks[0].src = (void*)ipipeGetNoCacheAddr((uint32_t)ic->gamma.table);//(void*)ic->gamma.table;
    //TBD: update size for what's really needed from ic->gamma.size !!!
    //dmaLutTasks[1].src = (void*)ipipeGetNoCacheAddr((uint32_t)ic->ic->3D_lut); //TBD

     /*ATOMIC*/ curPil = swcLeonSetPIL(15);
     /*ATOMIC*/ DrvCmxDmaStartListTask(&dmaLutTasks[0]);
     /*ATOMIC*/ swcLeonSetPIL(curPil);

      if(gammaCopyWait)
       DrvCmxDmaWaitTask(&dmaLutTasks[0]);
    #endif
   }

   //TBD: append 3D lut separately....
}

void fetchIcIspConfigLscLocal(Opipe *p, icIspConfig *ic) {
    p->rawBits      = ic->pipelineBits;
    p->bayerPattern = ic->bayerOrder;

    //Filter specific
    p->pBlcCfg        = &ic->blc;
    p->pSigmaCfg      = &ic->sigma;
    p->pLscCfg        = &locLscCfg; //Tell Opipe to use local locLscCfg instead of &ic->lsc;
    p->pRawCfg        = &ic->raw;
    p->pDbyrCfg       = &ic->demosaic;
    p->pLtmCfg        = &ic->ltm;
    p->pDogCfg        = &ic->dog;
    p->pLumaDnsCfg    = &ic->lumaDenoise;
    p->pLumaDnsRefCfg = &ic->lumaDenoiseRef;
    p->pSharpCfg      = &ic->sharpen;
    p->pChrGenCfg     = &ic->chromaGen;
    p->pMedCfg        = &ic->median;
    p->pChromaDnsCfg  = &ic->chromaDenoise;
    p->pColCombCfg    = &ic->colorCombine;
    p->pLutCfg        = &locLutCfg; //Tell Opipe to use local LutCfg instead of &ic->gamma;
    p->pColConvCfg    = &ic->colorConvert;
    p->aeCfg          = &ic->aeAwbConfig;
    p->aeStats        = aeAwbPatchStatsLoc;
    p->afCfg          = &ic->afConfig;
    p->afStats        = afPatchStatsLoc;
    p->pUpfirdn0Cfg   = &ic->updnCfg0;
    p->pUpfirdn12Cfg  = &ic->updnCfg12;

    if (p->afCfg->nPatchesX == 0 || p->afCfg->nPatchesY == 0){
        p->afStats        = NULL;
    }
    if(p->aeCfg && ic->aeAwbStats) // check if local allocated memory cover stats size, memory corruption
        assert((p->aeCfg->nPatchesX * p->aeCfg->nPatchesY) <= PLG_ISP_MAX_AE_NO_PATCH);
    if(p->afCfg && ic->afStats) // check if local allocated memory cover stats size,  memory corruption
        assert((p->afCfg->nPatchesX * p->afCfg->nPatchesY) <= PLG_ISP_MAX_AF_NO_PATCH);
    {
        uint32_t curPil;
        uint32_t lscSize = ic->lsc.lscStride * ic->lsc.lscHeight * sizeof(uint16_t);

        locLscCfg           = ic->lsc;
        locLscCfg.pLscTable = ispcLscMeshLoc;
        locLutCfg       = ic->gamma;
        locLutCfg.table = (uint16_t*)locGamma;

#if 0
        memcpy((void*)ipipeGetNoCacheAddr((uint32_t)locGamma), (void*)ipipeGetNoCacheAddr((uint32_t)ic->gamma.table), sizeof(locGamma));
        memcpy((void*)ipipeGetNoCacheAddr((uint32_t)ispcLscMeshLoc), (void*)ipipeGetNoCacheAddr((uint32_t)ic->lsc.pLscTable),lscSize);
#else
        dmaLutTasks[0].src = (void*)ipipeGetNoCacheAddr((uint32_t)ic->gamma.table);//(void*)ic->gamma.table;
        //TBD: update size for what's really needed from ic->gamma.size !!!
        //dmaLutTasks[1].src = (void*)ipipeGetNoCacheAddr((uint32_t)ic->ic->3D_lut); //TBD

        dmaLscTasks[0].src = (void*)ipipeGetNoCacheAddr((uint32_t)ic->lsc.pLscTable);
        dmaLscTasks[0].src_width        = lscSize; //line_size
        dmaLscTasks[0].dst_width        = lscSize; //line_size
        dmaLscTasks[0].length           = lscSize;

        DrvCmxDmaLinkTasks(&dmaLscTasks[0], 1, &dmaLutTasks[0]);

        /*ATOMIC*/ curPil = swcLeonSetPIL(15);
        /*ATOMIC*/ DrvCmxDmaStartListTask(&dmaLscTasks[0]);
        /*ATOMIC*/ swcLeonSetPIL(curPil);

        if(gammaCopyWait)
            DrvCmxDmaWaitTask(&dmaLutTasks[0]);
#endif
    }
}
//##########################################################################################
void SetCropRegion(DBuffer  *pOutY,
                   DBuffer  *pOutUV, //optional for yuv400 cases
                   YuvFmt    fmt,
                   icRect    cropRgn)
{
  #if defined(OPIPE_CROP)
   //Make start coord even number, due to UV subsampling...
    cropRgn.x1 &=( ~1);
    cropRgn.y1 &=( ~1);

   //Luma crop
    pOutY->crop.tlcX    =  cropRgn.x1;
    pOutY->crop.tlcY    =  cropRgn.y1;
    pOutY->crop.w       = (cropRgn.x2 - cropRgn.x1);
    pOutY->crop.h       = (cropRgn.y2 - cropRgn.y1);
    pOutY->crop.lStride = pOutY->crop.w;
    pOutY->crop.pStride = 0;//UNUSED (got a single Y plane)

   //Chroma Crop
   if(pOutUV != NULL)
   {
     switch(fmt)
     {
       case FMT_420: pOutUV->crop.tlcX    = pOutY->crop.tlcX    >> 1;
                     pOutUV->crop.tlcY    = pOutY->crop.tlcY    >> 1;
                     pOutUV->crop.w       = pOutY->crop.w       >> 1;
                     pOutUV->crop.h       = pOutY->crop.h       >> 1;
                     pOutUV->crop.lStride = pOutY->crop.lStride >> 1;
                     break;

       case FMT_422: pOutUV->crop.tlcX    = pOutY->crop.tlcX    >> 1;
                     pOutUV->crop.tlcY    = pOutY->crop.tlcY        ;
                     pOutUV->crop.w       = pOutY->crop.w       >> 1;
                     pOutUV->crop.h       = pOutY->crop.h           ;
                     pOutUV->crop.lStride = pOutY->crop.lStride >> 1;
                     break;

       case FMT_444: pOutUV->crop         = pOutY->crop;
                     break;
     }

   //All formats :
     pOutUV->crop.pStride = pOutUV->crop.w * pOutUV->crop.h;
   }
 #else
   UNUSED(pOutY);
   UNUSED(pOutUV);
   UNUSED(fmt);
   UNUSED(cropRgn);
   assert(0); //one must compile with OPIPE_CROP defined to be able to use this feature
 #endif
}
