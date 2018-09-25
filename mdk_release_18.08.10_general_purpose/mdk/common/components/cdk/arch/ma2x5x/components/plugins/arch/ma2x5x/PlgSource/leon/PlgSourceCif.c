/**************************************************************************************************

 @File         : PlgSourceCif.c
 @Author       : MT
 @Brief        : Contain Source Cif receiver configuration plug-in interface
 Date          : 01 - March - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :

 **************************************************************************************************/

/**************************************************************************************************
 ~~~ Included types first then APIs from other modules
 **************************************************************************************************/
#include "TimeSyncMgr.h"
#include "PlgSourceApi.h"
#include "FrameMgrApi.h"
#include <string.h>
#include <DrvTimer.h>
#include "swcLeonUtils.h"
#include "Opipe.h"
#include <DrvRegUtils.h>
#include <registersMyriad.h>
#include "swcWhoAmI.h"
#include "DrvCpr.h"
#include "DrvGpio.h"
#include "DrvMipi.h"
#include "DrvCif.h"
#include "DrvMss.h"
#include "assert.h"
#include "ipipeDbg.h"
#include "PlgSourceCif.h"

/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/
#define PLG_CIF_NR_OF_INSTANCES          2
// module irq priority
#define IPIPE_MIPIRX_IRQ_PRIORITY        4

/**************************************************************************************************
 ~~~  Local variables
 **************************************************************************************************/
static void IpipeMipiRxCif0Handler(uint32_t source);
static void IpipeMipiRxCif1Handler(uint32_t source);

static RecCtrlT      recCtrl[PLG_CIF_NR_OF_INSTANCES] = {
        {IC_CIF0_DEVICE4, CIF1_BASE_ADR, CIF1_INT_CLEAR_ADR, CIF1_INT_ENABLE_ADR, IRQ_CIF0, &IpipeMipiRxCif0Handler},
        {IC_CIF1_DEVICE5, CIF2_BASE_ADR, CIF2_INT_CLEAR_ADR, CIF2_INT_ENABLE_ADR, IRQ_CIF1, &IpipeMipiRxCif1Handler}
};
static sDrvMipiCifConfig    cifCfg[PLG_CIF_NR_OF_INSTANCES];
static PlgSource            *plgSourceCifPlg[PLG_CIF_NR_OF_INSTANCES];

/**************************************************************************************************
 ~~~ Imported functions declarations
 **************************************************************************************************/

/**************************************************************************************************
 ~~~ Local File function declarations
 **************************************************************************************************/
static inline void IpipeMipiRxCifEofHandler(uint32_t sourceD);
static inline void IpipeMipiRxCifHandler(uint32_t cifId);

/**************************************************************************************************
 ~~~ Exported Functions Implementation
 **************************************************************************************************/
void PlgSourceCifCfg(icSourceConfig  *srcCfg, PlgSource *plug) {
    uint32_t cifId = PLGCIF_GET_ID(srcCfg->mipiRxData.recNrl);
    plgSourceCifPlg[cifId] = plug;
    plug->inProcessFrame = FrameMgrAcquireFrame(plug->outputPools);
    plug->producedFrame  = 0;
    plug->mipiCtrlEofIdx = 0;
    plug->receiverEofIdx = 0;
    assert(0 != plug->inProcessFrame); // your pipe not allocate even 1 output buffer. Or stop source was not made proper
    //printf("save %p %d rawImg_10bpp_%dx%d_P400.by\n", nxtOutFrm->fbPtr[0], nxtOutFrm->stride[0]*nxtOutFrm->height[0]>>1, nxtOutFrm->stride[0]>>1, nxtOutFrm->height[0]>>1);
    cifCfg[cifId].cifModule             = recCtrl[cifId].BASE_ADR;
    cifCfg[cifId].cifDmaAddress         = (uint32_t)plug->inProcessFrame->fbPtr[0];// dma will not be enabled until a valid frame will be set
    cifCfg[cifId].cifWidth              = srcCfg->cameraOutputSize.w;
    cifCfg[cifId].cifHeight             = srcCfg->cameraOutputSize.h;
    cifCfg[cifId].cifWinPosX            = srcCfg->cropWindow.x1;
    cifCfg[cifId].cifWinPosY            = srcCfg->cropWindow.y1;
    cifCfg[cifId].cifWinWidth           = srcCfg->cropWindow.x2 - srcCfg->cropWindow.x1;
    cifCfg[cifId].cifWinHeight          = srcCfg->cropWindow.y2 - srcCfg->cropWindow.y1;
    cifCfg[cifId].cifInputFormatCfg     = D_CIF_INFORM_FORMAT_RGB_BAYER;
    cifCfg[cifId].cifPreviewFormatCfg   = D_CIF_PREV_OUTF_RGB_BY;
    if (srcCfg->bitsPerPixel <= 8) {
        cifCfg[cifId].cifInputFormatCfg     |= D_CIF_INFORM_DAT_SIZE_8;
        cifCfg[cifId].cifPreviewFormatCfg   |= D_CIF_PREV_RAW_BPP_1;
        cifCfg[cifId].cifDmaBpp = 1;
    }
    else { // raw10, raw12, raw14
        cifCfg[cifId].cifInputFormatCfg     |= D_CIF_INFORM_DAT_SIZE_16;
        cifCfg[cifId].cifPreviewFormatCfg   |= D_CIF_PREV_RAW_BPP_2;
        cifCfg[cifId].cifDmaBpp = 2;
    }
    cifCfg[cifId].cifInputInterfaceCfg  = D_CIF_IN_PIX_CLK_CPR;
    cifCfg[cifId].cifOutputFormatCfg    = 0;
    cifCfg[cifId].cifDmaWidth           = cifCfg[cifId].cifWinWidth;
    cifCfg[cifId].cifDmaHeight          = cifCfg[cifId].cifWinHeight;
    cifCfg[cifId].cifDmaCtrl            = D_CIF_DMA_ENABLE | D_CIF_DMA_AXI_BURST_16 | D_CIF_DMA_V_STRIDE_EN; //D_CIF_DMA_AUTO_RESTART_PING_PONG
    cifCfg[cifId].cifDmaStride          = cifCfg[cifId].cifWinWidth * cifCfg[cifId].cifDmaBpp;
    cifCfg[cifId].cifGenCtrl            = D_CIF_CTRL_ENABLE| D_CIF_CTRL_RGB_OUT_EN | D_CIF_CTRL_WINDOW_EN;

    // TODO: implement it
    //SET_REG_WORD((recCtrl[cifId].BASE_ADR+CIF_LINE_COUNT_OFFSET), (cifCfg[cifId].cifWinPosY+80));

    SET_REG_WORD(recCtrl[cifId].INT_CLEAR_ADR, 0xFFFFFFFF);
    SET_REG_WORD(recCtrl[cifId].INT_ENABLE_ADR, D_CIF_INT_DMA0_DONE | D_CIF_INT_DMA0_OVER); //TODO: add D_CIF_INT_LINE_COMPARE
    DrvIcbSetupIrq(recCtrl[cifId].irqSourceDefault, IPIPE_MIPIRX_IRQ_PRIORITY, POS_EDGE_INT, recCtrl[cifId].handler);
    DrvCifConfig(&cifCfg[cifId]);
}

void PlgSourceCifStop( PlgSource *plug) {
    uint32_t cifId = PLGCIF_GET_ID(plug->mipiRxData.recNrl);
    SET_REG_WORD(recCtrl[cifId].INT_ENABLE_ADR, 0);
    SET_REG_WORD(recCtrl[cifId].INT_CLEAR_ADR, 0xFFFFFFFF);
    DrvIcbIrqClear( recCtrl[cifId].irqSourceDefault);
    DrvIcbDisableIrq( recCtrl[cifId].irqSourceDefault);
}

// start of frame callback
void IpipeMipiRxCifSof(uint32_t sourceD) {
    PlgSource *plg = plgSourceCifPlg[sourceD];
    if(plg->sofEvent){
        plg->sofEvent((void*)plg, plg->inProcessFrame);
        return;
    }
}

void IpipeMipiRxCifEof(uint32_t sourceD) {
    PlgSource *plg = plgSourceCifPlg[sourceD];
    if(plg->eofEvent){
        plg->eofEvent((void*)plg, plg->producedFrame);
        return;
    }
}

// TODO: implement this feature
void IpipeMipiRxCifLineHit(uint32_t sourceD) {
    PlgSource *plg = plgSourceCifPlg[sourceD];
    if(plg->hitEvent){
        plg->hitEvent((void*)plg, plg->inProcessFrame);
        return;
    }
}

void producedFrameCifCall(PlgSource *plug) {
    if(plug->receiverEofIdx == plug->mipiCtrlEofIdx) {
        if(plug->producedFrame) { //handle SKIP-ed frames
            //Timestamp recently produced frame
            FrameMgrAndAddTimeStamp(plug->producedFrame, TimeSyncMsgGetTimeUs());
        }
        //Must invoke EOF even if current frame was skipped, to notify users...
        if(plug->eofEvent)
            plug->eofEvent((void*)plug, plug->producedFrame);

        //New frame produced, call the consumers
        if(plug->producedFrame) {
            FrameMgrProduceFrame (plug->producedFrame);
        }
    }
    else {
        uint32_t dif;
        if(plug->receiverEofIdx < plug->mipiCtrlEofIdx)
            dif = plug->mipiCtrlEofIdx - plug->receiverEofIdx;
        else
            dif = plug->receiverEofIdx - plug->mipiCtrlEofIdx;
        assert(dif <= 1);
    }
}

/**************************************************************************************************
 ~~~ Local Functions Implementation
 **************************************************************************************************/
//
static inline void IpipeMipiRxCifEofHandler(uint32_t sourceD) {
    PlgSource *plg = plgSourceCifPlg[sourceD];
    if(PLG_STATS_IDLE == plg->plg.status){
        // source was already stopped, this handler was call just because
        // fini happens by the time of dma interrupt happens so nothing to be done here
        return;
    }
    // protection wrong cif configuration. line height is probably bigger that the image,
    // or sensor is running before receiver configuration
    //assert(plg->receiverEofIdx == plg->mipiCtrlEofIdx);
    plg->producedFrame = plg->inProcessFrame;
    plg->receiverEofIdx++;
    //Get new output frame for next run...
    plg->inProcessFrame = FrameMgrAcquireFrame(plg->outputPools);
    SET_REG_WORD(recCtrl[sourceD].INT_CLEAR_ADR, D_CIF_INT_DMA0_DONE | D_CIF_INT_EOF);
    if(plg->inProcessFrame) {
        // enable next frame receiver
        DrvCifDma0RestartPP(recCtrl[sourceD].BASE_ADR,
                (uint32_t)plg->inProcessFrame->fbPtr[0],
                (uint32_t)plg->inProcessFrame->fbPtr[0]);
        SET_REG_WORD(recCtrl[sourceD].INT_ENABLE_ADR, D_CIF_INT_DMA0_DONE | D_CIF_INT_DMA0_OVER); //TODO: add D_CIF_INT_LINE_COMPARE
    }
    else {
        // disable next frame receiver
        CLR_REG_BITS_MASK(recCtrl[sourceD].BASE_ADR + CIF_DMA0_CFG_OFFSET, D_CIF_DMA_ENABLE);
        CLR_REG_BITS_MASK(recCtrl[sourceD].BASE_ADR + CIF_CONTROL_OFFSET, D_CIF_CTRL_ENABLE);
        SET_REG_WORD(recCtrl[sourceD].BASE_ADR + CIF_FIFO_FLUSH_OFFSET, 1); //flush ALL 3 DMAs FIFO (!)
        while(((GET_REG_WORD_VAL(recCtrl[sourceD].BASE_ADR + CIF_DMA0_CFG_OFFSET)) & D_CIF_DMA_ACTIVITY_MASK));
        SET_REG_BITS_MASK(recCtrl[sourceD].BASE_ADR + CIF_CONTROL_OFFSET, D_CIF_CTRL_ENABLE);
        SET_REG_WORD(recCtrl[sourceD].INT_ENABLE_ADR, D_CIF_INT_EOF | D_CIF_INT_DMA0_OVER); //TODO: add D_CIF_INT_LINE_COMPARE
    }

    DrvIcbIrqClear(recCtrl[sourceD].irqSourceDefault);

    producedFrameCifCall(plg);
}


//
static inline void IpipeMipiRxCifHandler(uint32_t cifId) {
    uint32_t status = GET_REG_WORD_VAL((recCtrl[cifId].BASE_ADR+CIF_INT_STATUS_OFFSET));
    if(status & D_CIF_INT_DMA0_OVER) {
        assert(0);// TODO add protection, reset
    }
    if(status & (D_CIF_INT_DMA0_DONE | D_CIF_INT_EOF)) {
        IpipeMipiRxCifEofHandler(cifId);
    }
}
//
static void IpipeMipiRxCif0Handler(uint32_t source) {
    UNUSED(source);
    IpipeMipiRxCifHandler(0);
}
//
static void IpipeMipiRxCif1Handler(uint32_t source) {
    UNUSED(source);
    IpipeMipiRxCifHandler(1);
}

