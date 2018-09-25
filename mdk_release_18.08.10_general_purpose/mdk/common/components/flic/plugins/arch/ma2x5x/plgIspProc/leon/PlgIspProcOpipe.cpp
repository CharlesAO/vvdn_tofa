///
/// @file      PlgIspProcOpip.cpp
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
#include "PlgIspProcOpipe.h"
#include "IspCommonOpipe.h"


static void opipeIspEof(Opipe *p) {
    pthread_t waitingThr = (pthread_t)p->params[0];
    rtems_event_send(waitingThr, RTEMS_EVENT_1);
}

//#################################################################################################
void PlgIspProcOpipe::IspStart(OpipeMF *opF, ImgFrame *fInp, ImgFrame *fOut, icIspConfig *ispCfg)
{
    opipeCommon.LockMtx();
    {//=========================================
        Opipe *p = &opF->p;
        fetchIcIspConfig(p, ispCfg); //icIspConfig -> Opipe translation
        // just an auto compleate with default value if was not populated
        if(ispCfg->updnCfg0.hN & ispCfg->updnCfg0.hD
                & ispCfg->updnCfg0.vN & ispCfg->updnCfg0.vD) {
        }
        else {
            //assume scale params are not set if ona of them is 0, so use default 1 values
            ispCfg->updnCfg0.hN = ispCfg->updnCfg0.hD = ispCfg->updnCfg0.vN = ispCfg->updnCfg0.vD = 1;
        }
        // option to reduce size at 1/2
        if(this->down2xOn) {
            ispCfg->updnCfg0.hD = ispCfg->updnCfg0.hD << 1;
            ispCfg->updnCfg0.vD = ispCfg->updnCfg0.vD << 1;
        }
        polyUtils.computePolyfirParams(ispCfg);


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
    }//=========================================

    OpipeSetSizeMF(opF, fInp->fb.spec.width, fInp->fb.spec.height);

    uint32_t outW = (ispCfg->updnCfg0.hN * opF->p.width2  - 1)/ispCfg->updnCfg0.hD + 1;
    uint32_t outH = (ispCfg->updnCfg0.vN * opF->p.height2 - 1)/ispCfg->updnCfg0.vD + 1;


    opF->pIn->ddr.base    = (uint32_t)fInp->base;
    opF->pOutY->ddr.base  = (uint32_t)fOut->base;
    //
    opF->pOutUV->ddr.base = (uint32_t)fOut->base + outW * outH;

    opF->p.params[0] = (void*)pthread_self();
    OpipeStart(&opF->p);

    fOut->fb.spec.width  = opF->p.pUpfirdn0Cfg->oW;
    fOut->fb.spec.height = opF->p.pUpfirdn0Cfg->oH;
    fOut->fb.spec.stride = fOut->fb.spec.width;


    //ptr, strides ...
    fOut->fb.p1 = (unsigned char*) fOut->base;
    fOut->fb.p2 = (unsigned char*)((uint32_t)fOut->base + outW * outH);
    fOut->fb.p3 = (unsigned char*)((uint32_t)fOut->base + outW * outH + ((outW * outH)>>2));
    fOut->fb.spec.type = YUV420p;
    fOut->fb.spec.bytesPP =  1;
    // this field can be overwrite at app level if this isp is associated with still image
    fOut->categ     = FRAME_TYPE_PREVIEW;
}


//#################################################################################################
void PlgIspProcOpipe::IspBuildPipe(OpipeMF *opF)
{
    opipeCommon.Create();
    opipeCommon.LockMtx();
    opipeCommon.SetCircBufsAdr(&opF->in.cBufSigma, &opF->cBufDbyrY,
            &opF->cBufSharp, &opF->cBufLut, &opF->cBufPoly);
    OpipeCreateFull(opF, BPP(2));
    //opF->p.flags |= CLEAN_EXIT;
    opF->p.cbEndOfFrame = opipeIspEof;
    polyUtils.computePolyFirInit();
    opF->p.pUpfirdn0Cfg  = &updnCfg0;
    opF->p.pUpfirdn12Cfg = &updnCfg12;
    opipeCommon.UnlockMtx();
    nFrmDone = 0;
}

//#################################################################################################
void PlgIspProcOpipe::IspWait()
{
    rtems_event_set events;
    rtems_event_receive(RTEMS_EVENT_1, RTEMS_EVENT_ANY | RTEMS_WAIT, RTEMS_NO_TIMEOUT, &events);
    nFrmDone++;
    //printf("ce: 0x%x, 0x%x, 0x%x\n", GET_REG_WORD_VAL(SIPP_INT0_STATUS_ADR), GET_REG_WORD_VAL(SIPP_INT1_STATUS_ADR), GET_REG_WORD_VAL(SIPP_INT2_STATUS_ADR));
    {
        uint32_t locIbufDecIrqMask = opF.p.sippSrcIntEnMask
                | ((1<<SIPP_CC_ID))
                | ((1<<SIPP_MED_LUMA_ID));
        uint32_t locObufIncIrqMask = opF.p.sippSnkIntEnMask
                | ((1<<SIPP_SHARPEN_ID))
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
void PlgIspProcOpipe::IspDestroy() {
    //delete sharing resources mutex. if is not already deleted
    opipeCommon.Destroy();
}

//#################################################################################################
void PlgIspProcOpipe::fetchIcIspConfig(Opipe *p, icIspConfig *ic)
{
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
    p->pLutCfg        = &ic->gamma;
    p->pColConvCfg    = &ic->colorConvert;
    p->aeCfg          = &ic->aeAwbConfig;
    p->aeStats        =  ic->aeAwbStats;
    p->afCfg          = &ic->afConfig;
    p->afStats        =  ic->afStats;
    p->pUpfirdn0Cfg   = &ic->updnCfg0;
    p->pUpfirdn12Cfg  = &ic->updnCfg12;
}

