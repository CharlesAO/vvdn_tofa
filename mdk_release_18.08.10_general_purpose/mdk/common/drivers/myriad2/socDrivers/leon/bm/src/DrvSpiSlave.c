///
/// @file DrvSpiSlave.c
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvSpiSlave
/// @{
/// @brief     SPI Slave Functions.
///


// System Includes
// ----------------------------------------------------------------------------
#include <assert.h>
#include <stdio.h>

// Application Includes
// ----------------------------------------------------------------------------
#include "mv_types.h"
#include "registersMyriad.h"
#include "DrvSpiSlave.h"
#include "DrvIcb.h"
#include "DrvTimer.h"
#include "DrvAhbDma.h"
#include "DrvRegUtils.h"

// Source Specific #defines and types (typedef,enum,struct)
// ----------------------------------------------------------------------------

// Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Static Local Data
// ----------------------------------------------------------------------------

static spiHandler_t* spiHandler[SPI_DEVICE_NO] = {NULL, NULL, NULL};
static ahbDmaHandler_t rxDmaHandler[SPI_DEVICE_NO];
static ahbDmaHandler_t txDmaHandler[SPI_DEVICE_NO];
// keeps the spi device that uses each DMA channel
static u8 dmaToSpi[DRV_AHB_DMA_CHANNEL_NO];
static u32 dummy;

static void spiSlaveIrqHandler(u32 irq);

static void maskInterrupt(u32 base, u32 mask)
{
    u32 newMask;
    newMask = GET_REG_WORD_VAL(base + SPI_IMR_OFFSET) & ~mask;
    SET_REG_WORD(base + SPI_IMR_OFFSET, newMask);
}

static void unmaskInterrupt(u32 base, u32 mask)
{
    u32 newMask;
    newMask = GET_REG_WORD_VAL(base + SPI_IMR_OFFSET) | mask;
    SET_REG_WORD(base + SPI_IMR_OFFSET, newMask);
}

static void dmaTransferDone(u32 channel, ahbDmaChannelStatus_t status)
{
    UNUSED(status); // hush the compiler warning.

    u32 base;
    u32 regVal;
    u32 spiDev;

    spiDev = dmaToSpi[channel];
    //identify the spi device that used this channel and reenable the interrupts for it
    if (spiHandler[spiDev] != NULL)
    {
        base = getSpiBaseAddr(spiDev);

        if(spiHandler[spiDev]->txDmaHandler->channel == channel)
        {
            spiHandler[spiDev]->txState = TX_IDLE;
            //disable transmit dma
            regVal = GET_REG_WORD_VAL(base + SPI_DMACR_OFFSET);
            SET_REG_WORD(base + SPI_DMACR_OFFSET, regVal & (~SPI_DMACR_TDMAE));
            if (spiHandler[spiDev]->writeDoneCallback != NULL)
            {
                spiHandler[spiDev]->writeDoneCallback(spiHandler[spiDev]->txSize, spiHandler[spiDev]->txChannel,
                    spiHandler[spiDev]->txFlags, spiHandler[spiDev]->txBuffer);
            }
        }
        else if (spiHandler[spiDev]->rxDmaHandler->channel == channel)
        {
            spiHandler[spiDev]->rxState = RX_IDLE;
            //disable receive dma
            regVal = GET_REG_WORD_VAL(base + SPI_DMACR_OFFSET);
            SET_REG_WORD(base + SPI_DMACR_OFFSET, regVal & (~SPI_DMACR_RDMAE));

            //restore FIFO level to receive another header
            if (spiHandler[spiDev]->wordSizeBytes == 1)
            {
                SET_REG_WORD(base + SPI_RXFTLR_OFFSET, HEADER_SIZE - 1);
            }
            else
            {
                SET_REG_WORD(base + SPI_RXFTLR_OFFSET, (HEADER_SIZE >> 1) - 1);
            }
            if (spiHandler[spiDev]->readDoneCallback != NULL)
            {
                spiHandler[spiDev]->readDoneCallback(spiHandler[spiDev]->rxSize, spiHandler[spiDev]->rxChannel,
                        spiHandler[spiDev]->rxFlags, spiHandler[spiDev]->rxBuffer);
            }

            unmaskInterrupt(base, SPI_IMR_RXFIM);
        }
    }
}

int DrvSpiSlaveInit(spiHandler_t* handler,
                    wordSizeBytes_t wordSizeBytes,
                    dmaUsed_t useDma,
                    spiSlaveBlock_t device,
                    u32 cpol,
                    u32 cpha,
                    u32 interruptLevel,
                    spiWriteRequestCallback_t* writeCallback,
                    spiWriteDoneCallback_t* writeDoneCallback,
                    spiReadRequestCallback_t* readCallback,
                    spiReadDoneCallback_t* readDoneCallback
)
{
    int pil;
    u32 irqSource;
    u32 base;
    u32 frameSize;
    s32 rxDmaChannel, txDmaChannel;

    base = getSpiBaseAddr(device);
    spiHandler[device] = handler;
    handler->device = device;
    handler->rxState = RX_IDLE;
    handler->txState = TX_IDLE;
    handler->useDma = useDma;
    handler->wordSizeBytes = wordSizeBytes;
    if (!writeCallback || !readCallback)
    {
        //return with error if the read or write callbacks are not defined
        return -1;
    }
    handler->writeCallback = writeCallback;
    handler->writeDoneCallback = writeDoneCallback;
    handler->readCallback = readCallback;
    handler->readDoneCallback = readDoneCallback;
    //disable SPI block
    SET_REG_WORD(base + SPI_SSIENR_OFFSET, 0);

    frameSize = (wordSizeBytes == 1) ? SPI_ONE_BYTE_FRAME_SIZE : SPI_TWO_BYTES_FRAME_SIZE;
    //clear all interrupts
    GET_REG_WORD_VAL(base + SPI_ICR_OFFSET);

    if (handler->useDma)
    {
        DrvAhbDmaInit(SPI_DMA_INT_LEVEL, AHB_DMA_NO_LRT_ROUTING);
        handler->txDmaHandler = &txDmaHandler[handler->device];
        handler->rxDmaHandler = &rxDmaHandler[handler->device];
        DrvAhbDmaGetFreeChannel(&txDmaChannel);
        if (txDmaChannel >= 0)
        {
            DrvAhbDmaOpenChannel(handler->txDmaHandler, txDmaChannel, dmaTransferDone);
            dmaToSpi[txDmaChannel] = handler->device;
        }
        else
        {
            // dma channel not available; return an error
            return -2;
        }
        DrvAhbDmaGetFreeChannel(&rxDmaChannel);
        if (rxDmaChannel >= 0)
        {
            DrvAhbDmaOpenChannel(handler->rxDmaHandler, rxDmaChannel, dmaTransferDone);
            dmaToSpi[rxDmaChannel] = handler->device;
        }
        else
        {
            // dma channel not available; return an error
            return -2;
        }
    }

    SET_REG_WORD(base + SPI_CTRLR0_OFFSET, SPI_CTRLR0_SRL_NORMAL  |
                                           SPI_CTRLR0_SLV_OE_ENA  | // enable txd for slave
                                           SPI_CTRLR0_TMOD_TX_RX  | // set mode as both transmit and receive
                                           ((cpol & (u32)0x1) << SPI_CTRLR0_SCPOL_BIT)  |
                                           ((cpha & (u32)0x1) << SPI_CTRLR0_SCPH_BIT)   |
                                           SPI_CTRLR0_FRF_MOT_SPI   | // data frame format: Motorola SPI
                                           frameSize
                      );


    if (wordSizeBytes == 1)
    {
        SET_REG_WORD(base + SPI_RXFTLR_OFFSET, HEADER_SIZE - 1);
    }
    else
    {
        SET_REG_WORD(base + SPI_RXFTLR_OFFSET, (HEADER_SIZE >> 1) - 1);
    }

    SET_REG_WORD(base + SPI_TXFTRL_OFFSET, 0);
    //configure interrupts
    SET_REG_WORD(base + SPI_IMR_OFFSET, SPI_IMR_RXFIM); // Receive FIFO Full Interrupt

    //SET_REG_WORD(base + SPI_IMR_OFFSET, SPI_IMR_TXEIM); // Transmit FIFO empty Interrupt

    /// Disable all Interrupts
    pil = swcLeonSetPIL(15);
    irqSource = IRQ_SPI1 + device;
    // Disable ICB (Interrupt Control Block) while setting new interrupt
    DrvIcbDisableIrq(irqSource);
    // Clear any existing interrupts
    DrvIcbIrqClear(irqSource);
    // Configure interrupt handlers
    DrvIcbSetIrqHandler(irqSource, spiSlaveIrqHandler);
    // Configure interrupts
    DrvIcbConfigureIrq(irqSource, interruptLevel, POS_LEVEL_INT);
    // Trigger the interrupts
    DrvIcbEnableIrq(irqSource);
    // Can enable the interrupts now
    swcLeonSetPIL(pil);

    //enable SPI block
    SET_REG_WORD(base + SPI_SSIENR_OFFSET, 1);

    return 0;
}

static void txFifoEmptyHandler(spiHandler_t *handler)
{
    u32 base;
    u8 i;

    base = getSpiBaseAddr(handler->device);
    if (handler->txState == WRITE_DATA)
    {
        for (i = 0; (i < TX_FULL_LEVEL) && (handler->txSize > 0); i++)
        {
            if (handler->wordSizeBytes == 1)
            {
                SET_REG_WORD(base + SPI_DR_OFFSET, *(u8*)(handler->txBuffer));
            }
            else
            {
                SET_REG_WORD(base + SPI_DR_OFFSET, *(u16*)(handler->txBuffer));
            }
            handler->txSize -= handler->wordSizeBytes;
            handler->txBuffer += handler->wordSizeBytes;
        }
        if (handler->txSize == 0)
        {
            handler->txState = TX_IDLE;
            if (handler->writeDoneCallback != NULL)
            {
                handler->writeDoneCallback(handler->txSize, handler->txChannel, handler->txFlags, handler->txBuffer);
            }
        }
    }
}

static void rxFifoFullHandler(spiHandler_t* handler)
{
    u8 channel;
    u8 flags;
    u8 fifoLevel;
    u16 header;
    u16 size;
    u16 fifoEntries;
    u32 i;
    u32 base;

    base = getSpiBaseAddr(handler->device);

    if (handler->rxState == RX_IDLE)
    {
        // get header information
        if (handler->wordSizeBytes == 1)
        {
            size = GET_REG_WORD_VAL(base + SPI_DR_OFFSET);
            size |= GET_REG_WORD_VAL(base + SPI_DR_OFFSET) << 8;
            flags = GET_REG_WORD_VAL(base + SPI_DR_OFFSET);
            channel = GET_REG_WORD_VAL(base + SPI_DR_OFFSET);
        }
        else
        {
            size = GET_REG_WORD_VAL(base + SPI_DR_OFFSET);
            header = GET_REG_WORD_VAL(base + SPI_DR_OFFSET);
            flags = (u8)(header & 0xFF);
            channel = (u8)(header >> 8);
        }

        if (flags & TXN_MST_WRITE)
        {
            handler->rxSize = size;
            handler->rxBuffer = (void*)(handler->readCallback(handler->rxSize, channel, flags));
            handler->rxState = READ_DATA;

            //master wants to read
            if (!(handler->useDma))
            {
                // set the rx FIFO level for the next transfer
                fifoLevel = handler->wordSizeBytes == 1 ? handler->rxSize : (handler->rxSize >> 1);
                if (fifoLevel < RX_FULL_LEVEL)
                {
                    SET_REG_WORD(base + SPI_RXFTLR_OFFSET, fifoLevel - 1);
                }
                else {
                    SET_REG_WORD(base + SPI_RXFTLR_OFFSET, RX_FULL_LEVEL - 1);
                }
            }
            else
            {
                // receive threshold for DMA
                SET_REG_WORD(base + SPI_DMARDLR_OFFSET, SPI_DMA_RX_FULL_LEVEL - 1);
                // enable RX DMA
                SET_REG_WORD(base + SPI_DMACR_OFFSET, SPI_DMACR_RDMAE);

                DrvAhbDmaChannelConfig(handler->rxDmaHandler, handler->wordSizeBytes - 1, handler->wordSizeBytes - 1,
                                    AHB_DMA_CTL_INC, AHB_DMA_CTL_NO_INC,
                                    AHB_DMA_CTL_MSIZE_8, AHB_DMA_CTL_MSIZE_8,
                                    AHB_DMA_CTL_TT_FC_D_P2M, SPI_DMA_CH_PRIO);


                DrvAhbDmaSetHs(handler->rxDmaHandler, AHB_DMA_HS_HW, AHB_DMA_HS_HW, 0, SPI_DMA_HW_INT_RX_0 + (2 * handler->device));
                DrvAhbDmaSingleBlockTransfer(handler->rxDmaHandler, (void*)(base + SPI_DR_OFFSET),
                    (void*)handler->rxBuffer, handler->rxSize);

                maskInterrupt(base, SPI_IMR_RXFIM);
                maskInterrupt(base, SPI_IMR_TXEIM);
            }
        }
        else if (flags & TXN_MST_READ)
        {
            handler->rxState = READ_DUMMY;
            handler->rxSize = size;
            handler->txSize = size;
            handler->txBuffer = (void*)(handler->writeCallback(handler->txSize, channel, flags));

            // slave will write data to master
            handler->txState = WRITE_DATA;
            if (!(handler->useDma))
            {
                txFifoEmptyHandler(handler);
                // enable transmit FIFO empty Interrupt
                SET_REG_WORD(base + SPI_TXFTRL_OFFSET, TX_EMPTY_LEVEL);
                unmaskInterrupt(base, SPI_IMR_TXEIM);
                maskInterrupt(base, SPI_IMR_RXFIM);
            }
            else
            {
                // transmit threshold for DMA
                SET_REG_WORD(base + SPI_DMATDLR_OFFSET, SPI_DMA_TX_EMPTY_LEVEL);
                // receive threshold for DMA
                SET_REG_WORD(base + SPI_DMARDLR_OFFSET, SPI_DMA_RX_FULL_LEVEL - 1);

                // enable both TX and RX DMA
                SET_REG_WORD(base + SPI_DMACR_OFFSET, SPI_DMACR_TDMAE | SPI_DMACR_RDMAE);

                DrvAhbDmaChannelConfig(handler->txDmaHandler, handler->wordSizeBytes - 1, handler->wordSizeBytes - 1,
                            AHB_DMA_CTL_NO_INC, AHB_DMA_CTL_INC,
                            AHB_DMA_CTL_MSIZE_8, AHB_DMA_CTL_MSIZE_8,
                            AHB_DMA_CTL_TT_FC_D_M2P, SPI_DMA_CH_PRIO);
                DrvAhbDmaSetHs(handler->txDmaHandler, AHB_DMA_HS_HW, AHB_DMA_HS_HW, SPI_DMA_HW_INT_TX_0 + 2 * handler->device, 0);

                DrvAhbDmaSingleBlockTransfer(handler->txDmaHandler, handler->txBuffer,
                    (void*)(base + SPI_DR_OFFSET), handler->txSize);

                //receive dummy data from master
                DrvAhbDmaChannelConfig(handler->rxDmaHandler, handler->wordSizeBytes - 1, handler->wordSizeBytes - 1,
                            AHB_DMA_CTL_NO_INC, AHB_DMA_CTL_NO_INC,
                            AHB_DMA_CTL_MSIZE_8, AHB_DMA_CTL_MSIZE_8,
                            AHB_DMA_CTL_TT_FC_D_P2M, SPI_DMA_CH_PRIO);
                DrvAhbDmaSetHs(handler->rxDmaHandler, AHB_DMA_HS_HW, AHB_DMA_HS_HW, 0, SPI_DMA_HW_INT_RX_0 + 2 * handler->device);

                DrvAhbDmaSingleBlockTransfer(handler->rxDmaHandler, (void*)(base + SPI_DR_OFFSET),
                    (void*)&dummy, handler->txSize);

                maskInterrupt(base, SPI_IMR_RXFIM);
                maskInterrupt(base, SPI_IMR_TXEIM);
            }
        }
    }
    else if (handler->rxState == READ_DATA)
    {
        if(handler->useDma)
        {
            // receive threshold for DMA
            SET_REG_WORD(base + SPI_DMARDLR_OFFSET, SPI_DMA_RX_FULL_LEVEL - 1);
            // enable RX DMA
            SET_REG_WORD(base + SPI_DMACR_OFFSET, SPI_DMACR_RDMAE);

            DrvAhbDmaChannelConfig(handler->rxDmaHandler, handler->wordSizeBytes - 1, handler->wordSizeBytes - 1,
                                AHB_DMA_CTL_INC, AHB_DMA_CTL_NO_INC,
                                AHB_DMA_CTL_MSIZE_8, AHB_DMA_CTL_MSIZE_8,
                                AHB_DMA_CTL_TT_FC_D_P2M, SPI_DMA_CH_PRIO);

            DrvAhbDmaSetHs(handler->rxDmaHandler, AHB_DMA_HS_HW, AHB_DMA_HS_HW, 0, SPI_DMA_HW_INT_RX_0 + (2 * handler->device));

            DrvAhbDmaSingleBlockTransfer(handler->rxDmaHandler, (void*)(base + SPI_DR_OFFSET),
                (void*)handler->rxBuffer, handler->rxSize);

            maskInterrupt(base, SPI_IMR_RXFIM);
            maskInterrupt(base, SPI_IMR_TXEIM);
        }
        else
        {
            if (handler->rxSize < RX_FULL_LEVEL)
            {
                fifoEntries = handler->rxSize;
            }
            else
            {
                fifoEntries = RX_FULL_LEVEL;
            }
            for (i = 0; (i < fifoEntries) && (handler->rxSize > 0); i++)
            {
                if (handler->wordSizeBytes == 1)
                {
                    *(u8*)(handler->rxBuffer) = GET_REG_WORD_VAL(base + SPI_DR_OFFSET);
                }
                else
                {
                    *(u16*)(handler->rxBuffer) = GET_REG_WORD_VAL(base + SPI_DR_OFFSET);
                }

                handler->rxSize -= handler->wordSizeBytes;
                handler->rxBuffer += handler->wordSizeBytes;
            }
            if  (handler->rxSize == 0)
            {
                handler->rxState = RX_IDLE;
                //restore FIFO level to receive another header
                if (handler->wordSizeBytes == 1)
                {
                    SET_REG_WORD(base + SPI_RXFTLR_OFFSET, HEADER_SIZE - 1);
                }
                else
                {
                    SET_REG_WORD(base + SPI_RXFTLR_OFFSET, (HEADER_SIZE >> 1) - 1);
                }
                if (handler->readDoneCallback != NULL)
                {
                    handler->readDoneCallback(handler->rxSize, handler->rxChannel, handler->rxFlags, handler->rxBuffer);
                }
            }
            else
            {
                // set the rx FIFO level for the next transfer
                fifoLevel = handler->wordSizeBytes == 1 ? handler->rxSize : (handler->rxSize >> 1);
                if (fifoLevel < RX_FULL_LEVEL)
                {
                    SET_REG_WORD(base + SPI_RXFTLR_OFFSET, fifoLevel - 1);
                }
                else {
                    SET_REG_WORD(base + SPI_RXFTLR_OFFSET, RX_FULL_LEVEL - 1);
                }
            }
        }
    }
    else if (handler->rxState == READ_DUMMY)
    {
        fifoEntries = GET_REG_WORD_VAL(base + SPI_RXFLR_OFFSET);
        for (i = 0; (i < fifoEntries) && (handler->rxSize > 0); i++)
        {
            GET_REG_WORD_VAL(base + SPI_DR_OFFSET);
            handler->rxSize -= handler->wordSizeBytes;
        }
        if (handler->rxSize == 0)
        {
            handler->rxState = RX_IDLE;
        }
        else
        {
            fifoLevel = handler->wordSizeBytes == 1 ? handler->rxSize : (handler->rxSize >> 1);
            if (fifoLevel < RX_FULL_LEVEL)
            {
                SET_REG_WORD(base + SPI_RXFTLR_OFFSET, fifoLevel - 1);
            }
            else
            {
                SET_REG_WORD(base + SPI_RXFTLR_OFFSET, RX_FULL_LEVEL - 1);
            }
        }
    }
}

int DrvSpiClose(spiHandler_t *handler)
{
    u8 pollCount;
    u32 base;

    base = getSpiBaseAddr(handler->device);
    // if there are still an ongoing transfer wait until it is finished
    pollCount = 0;
    while ((GET_REG_WORD_VAL(base + SPI_SR_OFFSET) & SPI_SR_BUSY) && (pollCount < SPI_MAX_DISABLE_COUNT))
    {
        DrvTimerInit();
        DrvTimerSleepMicro(SPI_DISABLE_TIMEOUT_US);
        pollCount++;
    }
    if (pollCount < SPI_MAX_DISABLE_COUNT)
    {
        // disable SPI block
        SET_REG_WORD(base + SPI_SSIENR_OFFSET, 0);
        return 0;
    }
    //timeout error
    return 1;
}

static void spiSlaveIrqHandler(u32 source)
{
    u32 interrupt;
    u32 base;
    spiHandler_t *handler;

    handler = spiHandler[source - IRQ_SPI1];
    base = getSpiBaseAddr(handler->device);

    interrupt = GET_REG_WORD_VAL(base + SPI_ISR_OFFSET);

    if (interrupt & SPI_ISR_RXFIS)
    {
        rxFifoFullHandler(handler);
    }

    if (interrupt & SPI_ISR_TXEIS)
    {
        txFifoEmptyHandler(handler);
    }

    DrvIcbIrqClear(source);
}

///@}
