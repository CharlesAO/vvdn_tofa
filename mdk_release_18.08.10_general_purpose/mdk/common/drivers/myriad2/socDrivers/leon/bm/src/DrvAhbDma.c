///
/// @file DrvAhbDma.c
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvAhbDma
/// @{
/// @brief     AHB DMA driver Implementation
///
/// Low level driver controlling the functions of the Myriad 2
/// AHB DMA Block
///



// 1: Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"
#include "registersMyriad.h"
#include "DrvRegUtils.h"
#include "DrvAhbDma.h"
#include "DrvIcb.h"
#include "assert.h"
#include <stdio.h>

// 2:  Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------
#define DRV_AHB_DMA_STATUS_MASK                0x03
#define DRV_AHB_DMA_SET_CHANNEL_STATUS(channel, status) (ahbDmaChannelControl[channel].channelStatus = status)

#define DRV_AHB_DMA_GET_CHANNEL_STATUS(channel) (ahbDmaChannelControl[channel].channelStatus)
typedef enum
{
    AHB_DMA_STATUS_CHANNEL_FREE = 0,
    AHB_DMA_STATUS_CHANNEL_TAKEN,
    AHB_DMA_STATUS_CHANNEL_RUNNING,
    AHB_DMA_STATUS_CHANNEL_UNKNOWN
} DRV_AHB_DMA_STATUS;

static struct {
    u8 irqSource;
    u8 status;
} ahbDmaControl;

static struct {
    u8 channelStatus;
    u8 gatherEnabled;
    u8 scatterEnabled;
} ahbDmaChannelControl[DRV_AHB_DMA_CHANNEL_NO];

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
ahbDmaHandler_t* ahbDmaHandler[DRV_AHB_DMA_CHANNEL_NO];

// 4: Static Local Data
// ----------------------------------------------------------------------------

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------
int DrvAhbDmaGetFreeChannel(s32* channel)
{
    u32 i = 0;
    ahbDmaDrvStatus_t status;

    if (ahbDmaControl.status == DRV_AHB_DMA_STAT_INITIALIZED)
    {
        do {
            if (DRV_AHB_DMA_GET_CHANNEL_STATUS(i) == AHB_DMA_STATUS_CHANNEL_FREE)
                break;
            i ++;
        } while(i < DRV_AHB_DMA_CHANNEL_NO);
        if (i == DRV_AHB_DMA_CHANNEL_NO)
            *channel = -1;
        *channel = i;
        status = AHB_DMA_DRV_SUCCESS;
    }
    else
    {
        status = AHB_DMA_DRV_NOT_INITIALIZED;
    }
    return status;
}

void DrvAhbDmaProcessIrq(void)
{
    u32 cfgRegVal;
    u32 intStatus;
    u32 errStatus;
    u32 channel;

    errStatus = GET_REG_WORD_VAL(DRV_AHB_DMA_STATUSERR);
    intStatus = GET_REG_WORD_VAL(DRV_AHB_DMA_STATUSBLOCK)|
                GET_REG_WORD_VAL(DRV_AHB_DMA_STATUSTFR)  |
                errStatus;

    //get the channel on which the interrupt was set
    channel = 0;
    while (intStatus > 0)
    {
        if (intStatus & 0x1)
        {
            cfgRegVal = GET_REG_WORD_VAL(DRV_AHB_DMA_CFG0 + channel * 0x58);

            //check if we have an error on the channel
            if (errStatus & (1 << channel))
            {
                // execute the callback with an error code
                if (ahbDmaHandler[channel]->transferDoneCallback)
                {
                    // Back to taken
                    DRV_AHB_DMA_SET_CHANNEL_STATUS(channel, AHB_DMA_STATUS_CHANNEL_TAKEN);
                    ahbDmaHandler[channel]->transferDoneCallback(channel, AHB_DMA_CHANNEL_ERROR);
                }
                // go to the next channel
                continue;
            }

            if (ahbDmaHandler[channel]->transferType == AHB_DMA_MULTI_BLOCK)
            {
                if (ahbDmaHandler[channel]->transferSize > ahbDmaHandler[channel]->blockSize)
                {
                    ahbDmaHandler[channel]->transferSize -= ahbDmaHandler[channel]->blockSize;
                }

                if (GET_REG_WORD_VAL(DRV_AHB_DMA_STATUSTFR) & (1 << channel))
                {
                    if (ahbDmaHandler[channel]->transferDoneCallback)
                    {
                        // Back to taken
                        DRV_AHB_DMA_SET_CHANNEL_STATUS(channel, AHB_DMA_STATUS_CHANNEL_TAKEN);
                        ahbDmaHandler[channel]->transferDoneCallback(channel, AHB_DMA_CHANNEL_SUCCESS);
                    }
                    // clear block and tfr interrupts
                    SET_REG_WORD(DRV_AHB_DMA_CLEARBLOCK, 1 << channel);
                    SET_REG_WORD(DRV_AHB_DMA_CLEARTFR, 1 << channel);
                }
                else if (ahbDmaHandler[channel]->transferSize <= ahbDmaHandler[channel]->blockSize)
                {
                    //lastBlock
                    // set reload bits to 0 for both source and destination, so that the hardware will automaticaly close the channel
                    SET_REG_WORD(DRV_AHB_DMA_CFG0 + channel * 0x58,
                    cfgRegVal & (~DRV_AHB_DMA_CFG_RELOAD_SRC)
                              & (~DRV_AHB_DMA_CFG_RELOAD_DST)
                        );
                    SET_REG_WORD(DRV_AHB_DMA_CLEARBLOCK, 1 << channel);
                }
                else {
                    //clear the block interrupt
                    SET_REG_WORD(DRV_AHB_DMA_CLEARBLOCK, 1 << channel);
                }
            }
            else {
                //clear block interrupt
                SET_REG_WORD(DRV_AHB_DMA_CLEARBLOCK, 1 << channel);

                if (GET_REG_WORD_VAL(DRV_AHB_DMA_STATUSTFR) & (1 << channel))
                {
                    // clear tfr interrupt
                    SET_REG_WORD(DRV_AHB_DMA_CLEARTFR, 1 << channel);

                    if (ahbDmaHandler[channel]->transferDoneCallback)
                    {
                        // Back to taken
                        DRV_AHB_DMA_SET_CHANNEL_STATUS(channel, AHB_DMA_STATUS_CHANNEL_TAKEN);
                        ahbDmaHandler[channel]->transferDoneCallback(channel, AHB_DMA_CHANNEL_SUCCESS);
                    }
                }
            }
        }
        channel++;
        intStatus >>= 1;
    }

}

static void drvAhbDmaIrqHandler(u32 source)
{
    UNUSED(source); // hush the compiler warning.

    DrvAhbDmaProcessIrq();
    DrvIcbIrqClear(IRQ_AHB_DMA);
}

int DrvAhbDmaChannelConfig(ahbDmaHandler_t* handler,
                   drvAhbDmaTrWidth_t dstTrWidth, drvAhbDmaTrWidth_t srcTrWidth,
                   drvAhbDmaInc_t dstInc, drvAhbDmaInc_t srcInc,
                   drvAhbDmaBurstSize_t dstBurstSize, drvAhbDmaBurstSize_t srcBurstSize,
                   drvAhbDmaCtlTransferType_t transferType, u32 priority)
{
    u32 ctlRegVal;
    u32 cfgRegVal;
    ahbDmaDrvStatus_t status;

    assert(dstTrWidth <= AHB_DMA_CTL_TR_WIDTH_32);
    assert(srcTrWidth <= AHB_DMA_CTL_TR_WIDTH_32);
    assert(dstInc <= AHB_DMA_CTL_NO_INC);
    assert(srcInc <= AHB_DMA_CTL_NO_INC);
    assert(dstBurstSize <= AHB_DMA_CTL_MSIZE_32);
    assert(srcBurstSize <= AHB_DMA_CTL_MSIZE_32);
    assert(transferType <= AHB_DMA_CTL_TT_FC_DP_P2P);
    assert(priority <= DRV_AHB_MAX_CH_PRIO);

    if (ahbDmaControl.status == DRV_AHB_DMA_STAT_INITIALIZED)
    {
        if (DRV_AHB_DMA_GET_CHANNEL_STATUS(handler->channel) == AHB_DMA_STATUS_CHANNEL_TAKEN)
        {
            handler->dstTransferWidth = dstTrWidth;
            handler->srcTransferWidth = srcTrWidth;
            ctlRegVal = DRV_AHB_DMA_CTL_INT_EN                           |
                    (dstTrWidth << DRV_AHB_DMA_CTL_DST_TR_WIDTH_POS) |
                    (srcTrWidth << DRV_AHB_DMA_CTL_SRC_TR_WIDTH_POS) |
                    (dstInc     << DRV_AHB_DMA_CTL_DINC_INC_POS)     |
                    (srcInc     << DRV_AHB_DMA_CTL_SINC_INC_POS)     |
                    (dstBurstSize << DRV_AHB_DMA_CTL_DST_MSIZE_POS)  |
                    (srcBurstSize << DRV_AHB_DMA_CTL_SRC_MSIZE_POS)  |
                    (transferType << DRV_AHB_DMA_CTL_TT_FC_POS)      |
                    DRV_AHB_DMA_CTL_DMS_1                            |
                    DRV_AHB_DMA_CTL_SMS_1;

            SET_REG_WORD(DRV_AHB_DMA_CTL0 + handler->channel * 0x58, ctlRegVal);

            cfgRegVal = (priority << DRV_AHB_DMA_CFG_CH_PRIOR_POS);
            SET_REG_WORD(DRV_AHB_DMA_CFG0 + handler->channel * 0x58, cfgRegVal);
            SET_REG_WORD(DRV_AHB_DMA_CFG0 + handler->channel * 0x58 + 4, 0);
            status = AHB_DMA_DRV_SUCCESS;
        }
        else
        {
            status = AHB_DMA_DRV_BUSY;
        }
    }
    else
    {
        status = AHB_DMA_DRV_NOT_INITIALIZED;
    }

    return status;
}

int DrvAhbDmaCloseChannel(ahbDmaHandler_t* handler)
{
    ahbDmaDrvStatus_t status;
    u32 regVal;

    assert(handler != NULL);
    if (ahbDmaControl.status == DRV_AHB_DMA_STAT_INITIALIZED)
    {

        if (DRV_AHB_DMA_GET_CHANNEL_STATUS(handler->channel) != AHB_DMA_STATUS_CHANNEL_RUNNING)
        {
            if (handler->transferDoneCallback != NULL)
            {
                //mask the interrupts for the closed channel
                regVal = GET_REG_WORD_VAL(DRV_AHB_DMA_MASKBLOCK);
                SET_REG_WORD(DRV_AHB_DMA_MASKBLOCK, regVal & ~(AHB_DMA_WE_AND_CHANNEL_BIT_MASK << handler->channel));
                regVal = GET_REG_WORD_VAL(DRV_AHB_DMA_MASKTFR);
                SET_REG_WORD(DRV_AHB_DMA_MASKTFR, regVal & ~(AHB_DMA_WE_AND_CHANNEL_BIT_MASK << handler->channel));
                regVal = GET_REG_WORD_VAL(DRV_AHB_DMA_MASKERR);
                SET_REG_WORD(DRV_AHB_DMA_MASKERR, regVal & ~(AHB_DMA_WE_AND_CHANNEL_BIT_MASK << handler->channel));
            }
            if (ahbDmaChannelControl[handler->channel].gatherEnabled)
            {
                DrvAhbDmaSrcGatherDisable(handler);
            }
            if (ahbDmaChannelControl[handler->channel].scatterEnabled)
            {
                DrvAhbDmaDstScatterDisable(handler);
            }
            DRV_AHB_DMA_SET_CHANNEL_STATUS(handler->channel, AHB_DMA_STATUS_CHANNEL_FREE);
            ahbDmaHandler[handler->channel] = NULL;

            status = AHB_DMA_DRV_SUCCESS;
        }
        else
        {
            status = AHB_DMA_DRV_BUSY;
        }
    }
    else
    {
        status = AHB_DMA_DRV_NOT_INITIALIZED;
    }
    return status;
}

int DrvAhbDmaOpenChannel(ahbDmaHandler_t* handler, u32 channel,
                         ahbDmaTransferDoneCallback_t* transferDoneCallback)
{
    u32 status;

    assert(handler != NULL);
    assert(channel < DRV_AHB_DMA_CHANNEL_NO);
    if (ahbDmaControl.status == DRV_AHB_DMA_STAT_INITIALIZED)
    {
        if (DRV_AHB_DMA_GET_CHANNEL_STATUS(channel) == AHB_DMA_STATUS_CHANNEL_FREE)
        {
            ahbDmaHandler[channel] = handler;
            handler->channel = channel;
            handler->transferDoneCallback = transferDoneCallback;
            DRV_AHB_DMA_SET_CHANNEL_STATUS(channel, AHB_DMA_STATUS_CHANNEL_TAKEN);
            status = AHB_DMA_DRV_SUCCESS;
        }
        else
        {
            status = AHB_DMA_DRV_BUSY;
        }
    }
    else
    {
        status = AHB_DMA_DRV_NOT_INITIALIZED;
    }

    return status;
}

int DrvAhbDmaOpen(void)
{
    ahbDmaDrvStatus_t status;

    if (ahbDmaControl.status == DRV_AHB_DMA_STAT_UNINITIALIZED)
    {
        ahbDmaControl.status = DRV_AHB_DMA_STAT_INITIALIZED;
        status = AHB_DMA_DRV_SUCCESS;
    }
    else
    {
        status = AHB_DMA_DRV_ALREADY_INITIALIZED;
    }
    return status;
}

int DrvAhbDmaConfigureIrq(u32 irqLevel,u32 dynIrqSource)
{
    ahbDmaDrvStatus_t status;

    assert(((dynIrqSource >= IRQ_DYNAMIC_0) && (dynIrqSource <= IRQ_DYNAMIC_11)) || (dynIrqSource == IRQ_AHB_DMA) || (dynIrqSource == 0));
    assert(irqLevel < NUM_INT_LEVELS);

    if (ahbDmaControl.status == DRV_AHB_DMA_STAT_INITIALIZED)
    {
        if (dynIrqSource)
            ahbDmaControl.irqSource = dynIrqSource;
        else
            ahbDmaControl.irqSource = IRQ_AHB_DMA;

        if (ahbDmaControl.irqSource != IRQ_AHB_DMA)
        {
            DrvIcbDisableIrq(ahbDmaControl.irqSource);
            DrvIcbIrqClear(ahbDmaControl.irqSource);
            DrvIcbSetupIrq(ahbDmaControl.irqSource, irqLevel, POS_LEVEL_INT, drvAhbDmaIrqHandler);
            //interrupt is routed to LRT
            DrvIcbDynamicIrqConfig(LOS_TO_LRT, IRQ_AHB_DMA, ahbDmaControl.irqSource, 1);
            DrvIcbEnableIrq(ahbDmaControl.irqSource);
        }
        else {
            DrvIcbDisableIrq(ahbDmaControl.irqSource);
            DrvIcbIrqClear(ahbDmaControl.irqSource);
            DrvIcbSetupIrq(ahbDmaControl.irqSource, irqLevel, POS_LEVEL_INT, drvAhbDmaIrqHandler);
            DrvIcbEnableIrq(ahbDmaControl.irqSource);
        }
        status = AHB_DMA_DRV_SUCCESS;
    }
    else
    {
        status = AHB_DMA_DRV_NOT_INITIALIZED;
    }

    return status;
}

int DrvAhbDmaInit(u32 irqLevel,u32 dynIrqSource)
{
    ahbDmaDrvStatus_t status;

    status = DrvAhbDmaOpen();

    if (status == AHB_DMA_DRV_SUCCESS)
    {
        return DrvAhbDmaConfigureIrq(irqLevel, dynIrqSource);
    }

    return status;
}


int DrvAhbDmaAutoreloadTransfer(ahbDmaHandler_t* handler, void* src, void* dst,
                        u32 transferSize, u32 blockSize, ahbDmaAutoreload_t reload)
{
    u32 regVal;
    ahbDmaDrvStatus_t status;

    assert(handler != NULL);
    assert(src != NULL);
    assert(dst != NULL);
    assert((blockSize >> (handler->srcTransferWidth)) < 4096);
    assert(reload <= AHB_DMA_CFG_RELOAD_BOTH);

    if (ahbDmaControl.status == DRV_AHB_DMA_STAT_INITIALIZED)
    {
        if (DRV_AHB_DMA_GET_CHANNEL_STATUS(handler->channel) == AHB_DMA_STATUS_CHANNEL_TAKEN)
        {
            DRV_AHB_DMA_SET_CHANNEL_STATUS(handler->channel, AHB_DMA_STATUS_CHANNEL_RUNNING);
            //clear interrupts
            SET_REG_WORD(DRV_AHB_DMA_CLEARBLOCK, 1 << handler->channel);
            SET_REG_WORD(DRV_AHB_DMA_CLEARTFR, 1 << handler->channel);
            SET_REG_WORD(DRV_AHB_DMA_CLEARERR, 1 << handler->channel);
            // unmask block, tfr and error interrupts
            regVal = GET_REG_WORD_VAL(DRV_AHB_DMA_MASKBLOCK);
            SET_REG_WORD(DRV_AHB_DMA_MASKBLOCK, regVal | (AHB_DMA_WE_AND_CHANNEL_BIT_MASK << handler->channel));
            regVal = GET_REG_WORD_VAL(DRV_AHB_DMA_MASKTFR);
            SET_REG_WORD(DRV_AHB_DMA_MASKTFR, regVal | (AHB_DMA_WE_AND_CHANNEL_BIT_MASK << handler->channel));
            regVal = GET_REG_WORD_VAL(DRV_AHB_DMA_MASKERR);
            SET_REG_WORD(DRV_AHB_DMA_MASKERR, regVal | (AHB_DMA_WE_AND_CHANNEL_BIT_MASK << handler->channel));

            //Write the starting source address in the SARx
            SET_REG_WORD(DRV_AHB_DMA_SAR0 + handler->channel * 0x58, (u32)src);
            // Write the destination address in the DARx
            SET_REG_WORD(DRV_AHB_DMA_DAR0 + handler->channel * 0x58, (u32)dst);
            // Program the LLPx register with 0
            SET_REG_WORD(DRV_AHB_DMA_LLP0 + handler->channel * 0x58, 0x00000000);

            handler->transferType = AHB_DMA_MULTI_BLOCK;
            handler->transferSize = transferSize;
            handler->blockSize = blockSize;
            SET_REG_WORD(DRV_AHB_DMA_CTL0 + handler->channel * 0x58 + 4, blockSize >> (handler->srcTransferWidth)); //BLOCK_TS - upper 32 bits

            regVal = GET_REG_WORD_VAL(DRV_AHB_DMA_CFG0 + handler->channel * 0x58);
            regVal |= reload << DRV_AHB_DMA_CFG_RELOAD_POS;
            SET_REG_WORD(DRV_AHB_DMA_CFG0 + handler->channel * 0x58, regVal);

            // Ensure that bit0 of the DmaCfgReg register is enabled before writing to ChEnReg:
            SET_REG_WORD(DRV_AHB_DMA_DMACFGREG,  DRV_AHB_DMA_DMACFGREG_EN);  //only bit0 matters (must be 1)
            SET_REG_WORD(DRV_AHB_DMA_CHENREG,    (1<<(handler->channel   ))  //CH_EN
                    |(1<<(handler->channel + 8)) //CH_enable write enable
            );
            status = AHB_DMA_DRV_SUCCESS;
        }
        else
        {
            status = AHB_DMA_DRV_BUSY;
        }
    }
    else
    {
        status = AHB_DMA_DRV_NOT_INITIALIZED;
    }
    return status;
}

int DrvAhbDmaSingleBlockTransfer(ahbDmaHandler_t* handler, void* src, void* dst, u32 transferSize)
{
    u32 cfgRegVal;
    u32 regVal;
    u32 tfr = 0, err = 0;
    ahbDmaDrvStatus_t status;

    assert(handler != NULL);
    assert(src != NULL);
    assert(dst != NULL);
    assert((transferSize >> (handler->srcTransferWidth)) < 4096);
    if (ahbDmaControl.status == DRV_AHB_DMA_STAT_INITIALIZED)
    {

        if (DRV_AHB_DMA_GET_CHANNEL_STATUS(handler->channel) == AHB_DMA_STATUS_CHANNEL_TAKEN)
        {
            DRV_AHB_DMA_SET_CHANNEL_STATUS(handler->channel, AHB_DMA_STATUS_CHANNEL_RUNNING);
            //clear interrupts
            SET_REG_WORD(DRV_AHB_DMA_CLEARTFR, 1 << handler->channel);
            SET_REG_WORD(DRV_AHB_DMA_CLEARBLOCK, 1 << handler->channel);
            SET_REG_WORD(DRV_AHB_DMA_CLEARERR, 1 << handler->channel);

            //Write the starting source address in the SARx
            SET_REG_WORD(DRV_AHB_DMA_SAR0 + handler->channel * 0x58, (u32)src);
            // Write the destination address in the DARx
            SET_REG_WORD(DRV_AHB_DMA_DAR0 + handler->channel * 0x58, (u32)dst);
            // Program the LLPx register with 0
            SET_REG_WORD(DRV_AHB_DMA_LLP0 + handler->channel * 0x58, 0x00000000);
    SET_REG_WORD(DRV_AHB_DMA_CLEARERR, 1 << handler->channel);

            handler->transferType = AHB_DMA_SINGLE_BLOCK;
            handler->transferSize = transferSize;
            handler->blockSize = transferSize;
            SET_REG_WORD(DRV_AHB_DMA_CTL0 + handler->channel * 0x58 + 4,
                    transferSize >> (handler->srcTransferWidth)); //BLOCK_TS - upper 32 bits

            // make sure that auto-reload is disabled
            cfgRegVal = GET_REG_WORD_VAL(DRV_AHB_DMA_CFG0 + handler->channel * 0x58);
            SET_REG_WORD(DRV_AHB_DMA_CFG0 + handler->channel * 0x58,
                    cfgRegVal & (~DRV_AHB_DMA_CFG_RELOAD_SRC)
                    & (~DRV_AHB_DMA_CFG_RELOAD_DST)
            );

            // Ensure that bit0 of the DmaCfgReg register is enabled before writing to ChEnReg:
            SET_REG_WORD(DRV_AHB_DMA_DMACFGREG,  DRV_AHB_DMA_DMACFGREG_EN);  //only bit0 matters (must be 1)
            regVal = GET_REG_WORD_VAL(DRV_AHB_DMA_CHENREG);
            SET_REG_WORD(DRV_AHB_DMA_CHENREG,  regVal | (1<<(handler->channel   ))  //CH_EN
                    | (1<<(handler->channel + 8)) //CH_enable write enable
            );

            if (handler->transferDoneCallback)
            {
                // unmask tfr and block interrupts
                regVal = GET_REG_WORD_VAL(DRV_AHB_DMA_MASKTFR);
                SET_REG_WORD(DRV_AHB_DMA_MASKTFR, regVal | (AHB_DMA_WE_AND_CHANNEL_BIT_MASK << handler->channel));
                regVal = GET_REG_WORD_VAL(DRV_AHB_DMA_MASKBLOCK);
                SET_REG_WORD(DRV_AHB_DMA_MASKBLOCK, regVal | (AHB_DMA_WE_AND_CHANNEL_BIT_MASK << handler->channel));
                regVal = GET_REG_WORD_VAL(DRV_AHB_DMA_MASKERR);
                SET_REG_WORD(DRV_AHB_DMA_MASKERR, regVal | (AHB_DMA_WE_AND_CHANNEL_BIT_MASK << handler->channel));
                status = AHB_DMA_DRV_SUCCESS;
            }
            else
            {
                // Poll for transfer done:
                // Once the transfer completes, hardware sets the interrupts and disables the channel
                do {
                    tfr = GET_REG_WORD_VAL(DRV_AHB_DMA_RAWTFR) & (1 << handler->channel);
                    err = GET_REG_WORD_VAL(DRV_AHB_DMA_RAWERR) & (1 << handler->channel);
                } while((tfr == 0) && (err == 0));
                if (err != 0)
                {
                    status = AHB_DMA_DRV_ERROR;
                }
                else
                {
                    status = AHB_DMA_DRV_SUCCESS;
                }
                // Channel is ready for another operation only
                DRV_AHB_DMA_SET_CHANNEL_STATUS(handler->channel, AHB_DMA_STATUS_CHANNEL_TAKEN);
            }
        }
        else
        {
            status = AHB_DMA_DRV_BUSY;
        }
    }
    else
    {
        status = AHB_DMA_DRV_NOT_INITIALIZED;
    }
    return status;
}

int DrvAhbDmaSetHs(ahbDmaHandler_t* handler, ahbDmaHsSel_t dstHsSel, ahbDmaHsSel_t srcHsSel,
                      u32 dstHwHs, u32 srcHwHs)
{
    ahbDmaDrvStatus_t status;

    assert(handler != NULL);
    assert(dstHsSel <= AHB_DMA_CFG_HS_SEL_SW);
    assert(srcHsSel <= AHB_DMA_CFG_HS_SEL_SW);
    if (ahbDmaControl.status == DRV_AHB_DMA_STAT_INITIALIZED)
    {
        if (DRV_AHB_DMA_GET_CHANNEL_STATUS(handler->channel) == AHB_DMA_STATUS_CHANNEL_TAKEN)
        {
            u32 cfgRegVal = GET_REG_WORD_VAL(DRV_AHB_DMA_CFG0 + handler->channel * 0x58);

            //clear the existing hardware handshaking settings
            cfgRegVal &= ~(DRV_AHB_DMA_CFG_HS_SEL_DST_MASK | DRV_AHB_DMA_CFG_HS_SEL_SRC_MASK);

            cfgRegVal |= (dstHsSel << DRV_AHB_DMA_CFG_HS_SEL_DST_POS) |
                    (srcHsSel << DRV_AHB_DMA_CFG_HS_SEL_SRC_POS);

            SET_REG_WORD(DRV_AHB_DMA_CFG0 + handler->channel * 0x58, cfgRegVal);
            SET_REG_WORD(DRV_AHB_DMA_CFG0 + handler->channel * 0x58 + 4, (dstHwHs << DRV_AHB_DMA_CFG_DEST_PER_POS) |
                    (srcHwHs << DRV_AHB_DMA_CFG_SRC_PER_POS));
            status = AHB_DMA_DRV_SUCCESS;
        }
        else
        {
            status = AHB_DMA_DRV_BUSY;
        }
    }
    else
    {
        status = AHB_DMA_DRV_NOT_INITIALIZED;
    }
    return status;
}

int DrvAhbDmaSrcGatherConfig(ahbDmaHandler_t* handler, u32 srcGatherCount, u32 srcGatherInterval)
{
    u32 ctlRegVal;
    ahbDmaDrvStatus_t status;

    assert(handler != NULL);
    // only channel 0 and 1 can use source gather feature
    assert(handler->channel < 2);
    if (ahbDmaControl.status == DRV_AHB_DMA_STAT_INITIALIZED)
    {
        if (DRV_AHB_DMA_GET_CHANNEL_STATUS(handler->channel) == AHB_DMA_STATUS_CHANNEL_TAKEN)
        {
            ctlRegVal = GET_REG_WORD_VAL(DRV_AHB_DMA_CTL0 + handler->channel * 0x58);
            // enable source gather
            ctlRegVal |= DRV_AHB_DMA_CTL_SRC_GATHER_EN;

            SET_REG_WORD(DRV_AHB_DMA_CTL0 + handler->channel * 0x58, ctlRegVal);

            SET_REG_WORD(DRV_AHB_DMA_SGR0 + handler->channel * 0x58,
                    (srcGatherInterval & 0xFFFFF)        | //20 bits field
                    ((srcGatherCount & 0xFFF) << 20));
            ahbDmaChannelControl[handler->channel].gatherEnabled = 1;
            status = AHB_DMA_DRV_SUCCESS;
        }
        else
        {
            status = AHB_DMA_DRV_BUSY;
        }
    }
    else
    {
        status = AHB_DMA_DRV_NOT_INITIALIZED;
    }
    return status;
}

int DrvAhbDmaSrcGatherDisable(ahbDmaHandler_t* handler)
{
    ahbDmaDrvStatus_t status;
    u32 ctlRegVal;

    assert(handler != NULL);
    if (ahbDmaControl.status == DRV_AHB_DMA_STAT_INITIALIZED)
    {
        if (DRV_AHB_DMA_GET_CHANNEL_STATUS(handler->channel) == AHB_DMA_STATUS_CHANNEL_TAKEN)
        {
            ctlRegVal = GET_REG_WORD_VAL(DRV_AHB_DMA_CTL0 + handler->channel * 0x58);
            ctlRegVal &= ~DRV_AHB_DMA_CTL_SRC_GATHER_EN;
            SET_REG_WORD(DRV_AHB_DMA_CTL0 + handler->channel * 0x58, ctlRegVal);
            ahbDmaChannelControl[handler->channel].gatherEnabled = 0;
            status = AHB_DMA_DRV_SUCCESS;
        }
        else
        {
            status = AHB_DMA_DRV_BUSY;
        }
    }
    else
    {
        status = AHB_DMA_DRV_NOT_INITIALIZED;
    }
    return status;
}

int DrvAhbDmaDstScatterConfig(ahbDmaHandler_t* handler, u32 dstScatterCount, u32 dstScatterInterval)
{
    u32 ctl0RegVal;
    ahbDmaDrvStatus_t status;

    assert(handler != NULL);
    // only channel 0 and 1 can use destination scatter feature
    assert(handler->channel < 2);
    if (ahbDmaControl.status == DRV_AHB_DMA_STAT_INITIALIZED)
    {
        if (DRV_AHB_DMA_GET_CHANNEL_STATUS(handler->channel) == AHB_DMA_STATUS_CHANNEL_TAKEN)
        {
            ctl0RegVal = GET_REG_WORD_VAL(DRV_AHB_DMA_CTL0 + handler->channel * 0x58);
            // enable destination scatter
            ctl0RegVal |= DRV_AHB_DMA_CTL_DST_SCATTER_EN;

            SET_REG_WORD(DRV_AHB_DMA_CTL0 + handler->channel * 0x58, ctl0RegVal);

            SET_REG_WORD(DRV_AHB_DMA_DSR0 + handler->channel * 0x58,
                    (dstScatterInterval & 0xFFFFF)        | //20 bits field
                    ((dstScatterCount & 0xFFF) << 20));
            ahbDmaChannelControl[handler->channel].scatterEnabled = 1;
            status = AHB_DMA_DRV_SUCCESS;
        }
        else
        {
            status = AHB_DMA_DRV_BUSY;
        }
    }
    else
    {
        status = AHB_DMA_DRV_NOT_INITIALIZED;
    }
    return status;
}

int DrvAhbDmaDstScatterDisable(ahbDmaHandler_t* handler)
{
    u32 ctlRegVal;
    ahbDmaDrvStatus_t status;

    assert(handler != NULL);
    if (ahbDmaControl.status == DRV_AHB_DMA_STAT_INITIALIZED)
    {
        if (DRV_AHB_DMA_GET_CHANNEL_STATUS(handler->channel) == AHB_DMA_STATUS_CHANNEL_TAKEN)
        {
            ctlRegVal = GET_REG_WORD_VAL(DRV_AHB_DMA_CTL0 + handler->channel * 0x58);
            ctlRegVal &= ~DRV_AHB_DMA_CTL_DST_SCATTER_EN;
            SET_REG_WORD(DRV_AHB_DMA_CTL0 + handler->channel * 0x58, ctlRegVal);
            ahbDmaChannelControl[handler->channel].scatterEnabled = 0;
            status = AHB_DMA_DRV_SUCCESS;
        }
        else
        {
            status = AHB_DMA_DRV_BUSY;
        }
    }
    else
    {
        status = AHB_DMA_DRV_NOT_INITIALIZED;
    }
    return status;
}

int DrvAhbDmaMemCopy(void* src, void* dst, s32 size)
{
    s32 channel;
    static ahbDmaHandler_t handler; //avoid dangling reference of handler to global variable ahbDmaHandler
    s32 transferedSize = 0;
    ahbDmaDrvStatus_t status;

    if (ahbDmaControl.status == DRV_AHB_DMA_STAT_INITIALIZED)
    {
        // get a free channel
        status =  DrvAhbDmaGetFreeChannel(&channel);
        if (status == AHB_DMA_DRV_SUCCESS)
        {
            if (channel == -1)
            {
                // channel not available
                return AHB_DMA_DRV_BUSY;
            }
        }
        else
        {
            return status;
        }
        status = DrvAhbDmaOpenChannel(&handler, channel, NULL);
        if (status != AHB_DMA_DRV_SUCCESS)
        {
            return status;
        }
        // Config channel
        status = DrvAhbDmaChannelConfig(&handler,
                AHB_DMA_CTL_TR_WIDTH_8,
                AHB_DMA_CTL_TR_WIDTH_8, AHB_DMA_CTL_INC, AHB_DMA_CTL_INC,
                AHB_DMA_CTL_MSIZE_8, AHB_DMA_CTL_MSIZE_8, AHB_DMA_CTL_TT_FC_D_M2M,
                AHB_DMA_DEFAULT_CHANEL_PRIO);

        if (status == AHB_DMA_DRV_SUCCESS)
        {
            while (size > 0)
            {
                if(size < DRV_AHB_MAX_BLOCK_SIZE)
                {
                    status = DrvAhbDmaSingleBlockTransfer(&handler, src + transferedSize, dst + transferedSize, size);
                    if (status != AHB_DMA_DRV_SUCCESS)
                    {
                        return status;
                    }
                    size = 0;
                    transferedSize += size;
                }
                else
                {
                    status = DrvAhbDmaSingleBlockTransfer(&handler, src + transferedSize, dst + transferedSize, DRV_AHB_MAX_BLOCK_SIZE);
                    if (status != AHB_DMA_DRV_SUCCESS)
                    {
                        return status;
                    }
                    size -= DRV_AHB_MAX_BLOCK_SIZE;
                    transferedSize += DRV_AHB_MAX_BLOCK_SIZE;
                }
            }
        }
        else
        {
            return status;
        }
        return DrvAhbDmaCloseChannel(&handler);
    }
    else
    {
        return AHB_DMA_DRV_NOT_INITIALIZED;
    }
}

int DrvAhbDmaCloseAllChannels(void)
{
    u32 channel;
    ahbDmaDrvStatus_t status = AHB_DMA_DRV_SUCCESS;

    if (ahbDmaControl.status == DRV_AHB_DMA_STAT_INITIALIZED)
    {
        for (channel = 0; channel < DRV_AHB_DMA_CHANNEL_NO; channel++)
        {
            if (ahbDmaHandler[channel] != NULL)
            {
                status = DrvAhbDmaCloseChannel(ahbDmaHandler[channel]);
                if (status != AHB_DMA_DRV_SUCCESS)
                {
                    return status;
                }
            }
        }
        ahbDmaControl.status = DRV_AHB_DMA_STAT_UNINITIALIZED;
    }
    return status;
}

int DrvAhbDmaClose(void)
{
    ahbDmaDrvStatus_t status = AHB_DMA_DRV_SUCCESS;

    status = DrvAhbDmaCloseAllChannels();
    if (status != AHB_DMA_DRV_SUCCESS)
    {
        return status;
    }
    DrvIcbDisableIrq(ahbDmaControl.irqSource);
    return status;
}

///@}
