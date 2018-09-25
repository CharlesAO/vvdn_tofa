///
/// @file      PlgIspMonoOpip.cpp
/// @copyright All code copyright Movidius Ltd 2017, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Opipe Interface for the Isp flic Plugin.
///            Platform(s) supported : ma2x5x
///

#include <rtems.h>
#include "Opipe.h"
#include "OpipeApps.h"
#include <ImgFrame.h>
#include "ImgTypes.h"
#include "PlgIspMonoOpipe.h"
#include "IspCommonOpipe.h"


uint16_t PlgIspMonoOpipe::locGammaMono[PLG_ISP_MAX_GAMMA_SIZE]  __attribute__((section(".cmx.opipe"))) __attribute__((aligned(8)));
//SECTION(PLG_ISP_GAMMA_SECT) ALIGNED(8);
LutCfg   PlgIspMonoOpipe::locLutMonoCfg = {0, 0, {0, 0}, (uint16_t*)locGammaMono};

static void opipeIspEof(Opipe *p) {
    pthread_t waitingThr = (pthread_t)p->params[0];
    rtems_event_send(waitingThr, RTEMS_EVENT_1);
}

//#################################################################################################
void PlgIspMonoOpipe::IspStart(OpipeBayerMono *opF, ImgFrame *fInp, ImgFrame *fOut, icIspConfig *ispCfg) {
    opipeCommon.LockMtx();
    Opipe *p = &opF->p;
    plgIspMonoBayerSetParams(p, ispCfg);

    // Set image size
    opF->p.width = fInp->fb.spec.width;
    opF->p.height = fInp->fb.spec.height;

    opF->pIn->ddr.base    = (uint32_t)fInp->base;
    opF->pOut->ddr.base  = (uint32_t)fOut->base;
    //
    opF->p.params[0] = (void*)pthread_self();
    OpipeStart(&opF->p);

    fOut->fb.spec.width  = fInp->fb.spec.width;
    fOut->fb.spec.height = fInp->fb.spec.height;
    fOut->fb.spec.stride = fOut->fb.spec.width;

    //ptr, strides ...
    fOut->fb.p1 = (unsigned char*) fOut->base;
    fOut->fb.p2 = NULL;
    fOut->fb.p3 = NULL;
    fOut->fb.spec.type = RAW8;
    fOut->fb.spec.bytesPP =  1;
    // this field can be overwrite at app level if this isp is associated with still image
    fOut->categ     = FRAME_TYPE_PREVIEW;
}


//#################################################################################################
void PlgIspMonoOpipe::IspBuildPipe(OpipeBayerMono *opF) {
    opipeCommon.Create();
    opipeCommon.LockMtx();
    opipeCommon.SetCircBufsAdr(&opF->cBufInSig, &opF->cBufOutDbyr,
            &opF->cBufOutUsm, &opF->cBufOutLut, NULL);
    OpipeCreateBayerMono(opF, BPP(2));
    //opF->p.flags |= CLEAN_EXIT;
    //initLutCopyTask();
    opF->p.cbEndOfFrame = opipeIspEof;
    opipeCommon.UnlockMtx();
    nFrmDone = 0;
}

//#################################################################################################
void PlgIspMonoOpipe::IspWait() {
    rtems_event_set events;
    rtems_event_receive(RTEMS_EVENT_1, RTEMS_EVENT_ANY | RTEMS_WAIT, RTEMS_NO_TIMEOUT, &events);
    nFrmDone++;
    //printf("me: 0x%x, 0x%x, 0x%x\n", GET_REG_WORD_VAL(SIPP_INT0_STATUS_ADR), GET_REG_WORD_VAL(SIPP_INT1_STATUS_ADR), GET_REG_WORD_VAL(SIPP_INT2_STATUS_ADR));
    {
        uint32_t locIbufDecIrqMask = opF.p.sippSrcIntEnMask;
        uint32_t locObufIncIrqMask = opF.p.sippSnkIntEnMask
                | ((1<<SIPP_STATS_ID))
                | ((1<<SIPP_AF_STATS_ID))
                | ((1<<SIPP_LUMA_HIST_ID))
                | ((1<<SIPP_RGB_HIST_ID));
        rtems_interrupt_level level;
        rtems_interrupt_disable(level);

        SET_REG_WORD(SIPP_INT0_CLEAR_ADR, locIbufDecIrqMask);
        SET_REG_WORD(SIPP_INT1_CLEAR_ADR, locObufIncIrqMask);
        SET_REG_WORD(SIPP_INT2_CLEAR_ADR, opF.p.sippSnkIntEnMask);

        //Disable own filters and links
        CLR_REG_BITS_MASK(SIPP_CONTROL_ADR,   opF.p.enMask);
        CLR_REG_BITS_MASK(SIPP_OPIPE_CFG_ADR, opF.p.cfgMask);

        //Disable associated interrupts
        CLR_REG_BITS_MASK(SIPP_INT0_ENABLE_ADR, opF.p.sippSrcIntEnMask);
        CLR_REG_BITS_MASK(SIPP_INT1_ENABLE_ADR, opF.p.sippSnkIntEnMask);
        CLR_REG_BITS_MASK(SIPP_INT2_ENABLE_ADR, opF.p.sippSnkIntEnMask);//patch for not working obfl_inc

        rtems_interrupt_enable(level);
    }
    opipeCommon.UnlockMtx();
}

//#################################################################################################
void PlgIspMonoOpipe::IspDestroy() {
    //delete sharing resources mutex. if is not already deleted
    opipeCommon.Destroy();
}

//#################################################################################################
void PlgIspMonoOpipe::fetchIcIspConfig(Opipe *p, icIspConfig *ic) {
    // Function transfers information from ic -> p.
    // The gamma lut requires only one channel.
    uint32_t i;

    p->format       = BAYER;
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
    p->pLutCfg        = &locLutMonoCfg; //Tell Opipe to use local LutCfg instead of &ic->gamma;
    p->pColConvCfg    = &ic->colorConvert;
    p->aeCfg          = &ic->aeAwbConfig;
    p->aeStats        = ic->aeAwbStats;
    p->afCfg          = &ic->afConfig;
    p->afStats        =  ic->afStats;
    p->pUpfirdn0Cfg   = &ic->updnCfg0;
    p->pUpfirdn12Cfg  = &ic->updnCfg12;

    // LUT. Only one channel is used
    locLutMonoCfg.size       = ic->gamma.size;
    locLutMonoCfg.rgnSize[0] = ic->gamma.rgnSize[0];
    locLutMonoCfg.rgnSize[1] = ic->gamma.rgnSize[1];
    for(i = 0; i < locLutMonoCfg.size; i++)
    {
        locGammaMono[i] = ic->gamma.table[i*4];
    }
}


void PlgIspMonoOpipe::plgIspMonoBayerSetParams(Opipe *p, icIspConfig *ic) {

#define GRGB_IMB_EN          0 //Gr/Gb imbalance enable
#define BAD_PIXEL_FIX_EN     1 //Hot/Cold pixel suppression enable
#define LUMA_HIST_EN         0 //Luma histogram enable
#define GAIN_MODE            1 //Bayer 2x2 mode
#define AF_STATS_EN          0 //AF stats
#define RGB_HIST_EN          0 //RGB histogram enable
#define SDC_EN               0 //Static pixel correction
#define RAW_CFG             (p->format                <<  0) |\
        (p->bayerPattern          <<  1) |\
        (GRGB_IMB_EN              <<  3) |\
        (BAD_PIXEL_FIX_EN         <<  4) |\
        (LUMA_HIST_EN             <<  7) |\
        ((p->rawBits - 1)         <<  8) |\
        (GAIN_MODE                << 12) |\
        (AF_STATS_EN              << 13) |\
        (p->pRawCfg->grgbImbalThr << 16) |\
        (RGB_HIST_EN              << 24) |\
        (SDC_EN                   << 27)

#define FP16_MODE            1 // FP16 mode
#define CHANNEL_MODE         0 // Channel mode
#define LUT_CFG             (FP16_MODE                   << 0) |\
        (CHANNEL_MODE                << 1) |\
        ((p->oPlanes[SIPP_LUT_ID]-1) << 12)


    //icIspConfig -> Opipe translation
    fetchIcIspConfig(p, ic);

    // Configure SIPP filters
    OpipeDefaultCfg(p, SIPP_SIGMA_ID  );
    OpipeDefaultCfg(p, SIPP_DBYR_ID   );
    OpipeDefaultCfg(p, SIPP_DOGL_ID   );
    OpipeDefaultCfg(p, SIPP_SHARPEN_ID);

    p->cfg[SIPP_RAW_ID]   = RAW_CFG;

    //Opipe internally will force progPlanes = 1;
    p->iPlanes[SIPP_LUT_ID] = 1; //just 1 plane
    p->oPlanes[SIPP_LUT_ID] = 1; //just 1 plane
    p->cfg[SIPP_LUT_ID] = LUT_CFG;
}
