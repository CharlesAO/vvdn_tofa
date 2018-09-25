///
/// @file DrvSpiMaster.c
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup DrvSpiMaster
/// @{
/// @brief     SPI master Functions.
///


// System Includes
// ----------------------------------------------------------------------------
#include <assert.h>
#include <stdio.h>
#include "DrvSpiMaster.h"
#include "registersMyriad.h"
#include "DrvIcb.h"
#include "DrvRegUtils.h"

static spiHandler_t* spiHandler[SPI_DEVICE_NO];

static void spiMasterHandler(u32 unused);
static void txFifoEmptyHandler(spiHandler_t* handler);
static void rxFifoFullHandler(spiHandler_t* handler);

void DrvSpiMasterEnableSlave(spiHandler_t* mstHandler, spiSlaveBlock_t slaveNo)
{
    u32 base = getSpiBaseAddr(mstHandler->device);
    SET_REG_WORD(base + SPI_SER_OFFSET, 1 << slaveNo);
}

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

/// @todo code similar to slaveInit: needs refractoring
int DrvSpiMasterInit(spiHandler_t* handler,
                    wordSizeBytes_t wordSizeBytes,
                    dmaUsed_t useDma,
                    spiSlaveBlock_t device,
                    u32 cpol,
                    u32 cpha,
                    u32 clkDivider,
                    u32 interruptLevel,
                    spiWriteRequestCallback_t* writeCallback,
                    spiWriteDoneCallback_t* writeDoneCallback,
                    spiReadRequestCallback_t* readCallback,
                    spiReadDoneCallback_t* readDoneCallback
)
{
    UNUSED(useDma); // hush the compiler warning.
    
    int pil;
    u32 irqSource;
    u32 base;
    u32 mask;
    u32 frameSize;

    base = getSpiBaseAddr(device);
    spiHandler[device] = handler;
    handler->device = device;
    handler->rxState = RX_IDLE;
    handler->txState = TX_IDLE;
    handler->wordSizeBytes = wordSizeBytes;
    handler->writeCallback = writeCallback;
    handler->writeDoneCallback = writeDoneCallback;
    handler->readCallback = readCallback;
    handler->readDoneCallback = readDoneCallback;

    //configure SPI block as master
    mask = GET_REG_WORD_VAL(CPR_GEN_CTRL_ADR) & 0xffffff8f;
    SET_REG_WORD(CPR_GEN_CTRL_ADR, mask | (0x1 << (handler->device + 4)));

    //disable SPI block
    SET_REG_WORD(base + SPI_SSIENR_OFFSET, 0);

    frameSize = (wordSizeBytes == 1) ? SPI_ONE_BYTE_FRAME_SIZE : SPI_TWO_BYTES_FRAME_SIZE;
    //clear all interrupts
    GET_REG_WORD_VAL(base + SPI_ICR_OFFSET);

    SET_REG_WORD(base + SPI_CTRLR0_OFFSET, SPI_CTRLR0_SRL_NORMAL  |
                                           SPI_CTRLR0_SLV_OE_ENA  | // enable txd for slave
                                           SPI_CTRLR0_TMOD_TX_RX  | // set mode as both transmit and receive
                                           ((cpol & (u32)0x1) << SPI_CTRLR0_SCPOL_BIT)  |
                                           ((cpha & (u32)0x1) << SPI_CTRLR0_SCPH_BIT)   |
                                           SPI_CTRLR0_FRF_MOT_SPI | // data frame format: Motorola SPI
                                           frameSize
                      );

    //set baud rate
    SET_REG_WORD(base + SPI_BAUDR_OFFSET, clkDivider);

    SET_REG_WORD(base + SPI_RXFTLR_OFFSET, (HEADER_SIZE >> 1) - 1);
    //configure interrupts
    SET_REG_WORD(base + SPI_IMR_OFFSET, 0); // mask all interrupts

    /// Disable all Interrupts
    pil = swcLeonSetPIL(15);
    irqSource = IRQ_SPI1 + device;
    // Disable ICB (Interrupt Control Block) while setting new interrupt
    DrvIcbDisableIrq(irqSource);
    // Clear any existing interrupts
    DrvIcbIrqClear(irqSource);
    // Configure interrupt handlers
    DrvIcbSetIrqHandler(irqSource, spiMasterHandler);
    // Configure interrupts
    DrvIcbConfigureIrq(irqSource, interruptLevel, POS_LEVEL_INT);
    // Trigger the interrupts
    DrvIcbEnableIrq(irqSource);
    // Can enable the interrupt now
    swcLeonSetPIL(pil);

    //enable SPI block
    SET_REG_WORD(base + SPI_SSIENR_OFFSET, 1);

    return 0;
}

void DrvSpiMasterSendBuffer(spiHandler_t *handler, void* buffer, u32 size, u8 channelId)
{
    u32 base = getSpiBaseAddr(handler->device);

    handler->txBuffer = buffer;
    handler->txChannel = channelId;
    handler->txSize = size;
    handler->txState = WRITE_HEADER;
    handler->txFlags = TXN_MST_WRITE;

    SET_REG_WORD(base + SPI_TXFTRL_OFFSET, TX_EMPTY_LEVEL);

    txFifoEmptyHandler(handler);
    // enable interrupt only if the transfer is not already done
    if (handler->txSize > 0)
    {
        unmaskInterrupt(base, SPI_IMR_TXEIM);
        maskInterrupt(base, SPI_IMR_RXFIM);
    }
}

void DrvSpiMasterReceiveBuffer(spiHandler_t *handler, void* buffer, u32 size, u8 channelId)
{
    u32 base = getSpiBaseAddr(handler->device);
    u32 i;
    u8 headerSize;

    handler->rxBuffer = buffer;
    handler->rxChannel = channelId;
    handler->txChannel = channelId;
    handler->rxSize = size;
    handler->rxState = READ_DATA;
    handler->txState = WRITE_HEADER;
    handler->txFlags = TXN_MST_READ;


    //flush the rx fifo

    // wait for the header transmission to finish
    while (GET_REG_WORD_VAL(base + SPI_SR_OFFSET) & SPI_SR_BUSY);
    while (GET_REG_WORD_VAL(base + SPI_SR_OFFSET) & SPI_SR_RFNE)
    {
        GET_REG_WORD_VAL(base + SPI_DR_OFFSET);
    }

    //send the header to slave
    if (handler->wordSizeBytes == 1)
    {
        SET_REG_WORD(base + SPI_DR_OFFSET, handler->rxSize & 0xFF);
        SET_REG_WORD(base + SPI_DR_OFFSET, handler->rxSize >> 8);
        SET_REG_WORD(base + SPI_DR_OFFSET, TXN_COMPLETE | TXN_MST_READ);
        SET_REG_WORD(base + SPI_DR_OFFSET, handler->rxChannel);
    }
    else
    {
        SET_REG_WORD(base + SPI_DR_OFFSET, handler->rxSize);
        SET_REG_WORD(base + SPI_DR_OFFSET, (TXN_COMPLETE | TXN_MST_READ) | ((handler->rxChannel) << 8));
    }
    handler->rxState = READ_DATA;

    // wait for the header transmission to finish
    while (GET_REG_WORD_VAL(base + SPI_SR_OFFSET) & SPI_SR_BUSY);
    if (handler->wordSizeBytes == 1)
    {
        headerSize = HEADER_SIZE;
    }
    else {
        headerSize = HEADER_SIZE >> 1;
    }
    for (i = 0; i < headerSize; i++)
    {
        GET_REG_WORD_VAL(base + SPI_DR_OFFSET);
    }
    if ((handler->rxSize >> 1) < RX_FULL_LEVEL)
    {
        SET_REG_WORD(base + SPI_RXFTLR_OFFSET, ((handler->rxSize) >> 1) - 1);
    }
    else
    {
        SET_REG_WORD(base + SPI_RXFTLR_OFFSET, RX_FULL_LEVEL - 1);
    }
    SET_REG_WORD(base + SPI_TXFTRL_OFFSET, TX_EMPTY_LEVEL);

    handler->txState = WRITE_DUMMY;
    handler->txSize = size;
    txFifoEmptyHandler(handler);
    if (handler->txSize > 0)
    {
        // enable interrupt only if the transfer is not already done
        unmaskInterrupt(base, SPI_IMR_TXEIM);
    }
    unmaskInterrupt(base, SPI_IMR_RXFIM);
}

static void txFifoEmptyHandler(spiHandler_t* handler)
{
    u32 base;
    u32 i;

    base = getSpiBaseAddr(handler->device);

    if (handler->txState == WRITE_HEADER)
    {
        //send the header
        if (handler->txFlags & TXN_MST_WRITE)
        {
            if (handler->wordSizeBytes == 1)
            {
                SET_REG_WORD(base + SPI_DR_OFFSET, (handler->txSize) & 0xFF);
                SET_REG_WORD(base + SPI_DR_OFFSET, (handler->txSize) >> 8);
                SET_REG_WORD(base + SPI_DR_OFFSET, TXN_COMPLETE | TXN_MST_WRITE);
                SET_REG_WORD(base + SPI_DR_OFFSET, handler->txChannel);
            }
            else
            {
                SET_REG_WORD(base + SPI_DR_OFFSET, handler->txSize);
                SET_REG_WORD(base + SPI_DR_OFFSET, (TXN_COMPLETE | TXN_MST_WRITE) | ((handler->txChannel) << 8));
            }
            handler->txState = WRITE_DATA;
        }
    }
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
            // when the transmission is done mask the tx FIFO empty interrupt
            maskInterrupt(base, SPI_IMR_TXEIM);
            handler->txState = TX_IDLE;
            if (handler->writeDoneCallback != NULL)
            {
                handler->writeDoneCallback(handler->txSize, handler->txChannel, handler->txFlags, handler->txBuffer);
            }
        }
    }
    else if ((handler->txState == WRITE_DUMMY) && (handler->rxState == READ_DATA))
    {
        for (i = 0; (handler->txSize > 0) && (i < TX_FULL_LEVEL); i++)
        {
            SET_REG_WORD(base + SPI_DR_OFFSET, DUMMY);
            handler->txSize -= handler->wordSizeBytes;
        }

        if (handler->txSize == 0)
        {
            maskInterrupt(base, SPI_IMR_TXEIM);
            handler->txState = TX_IDLE;
        }
    }
}

static void rxFifoFullHandler(spiHandler_t* handler)
{
    u8 fifoSize;
    u32 i;
    u32 base;

    base = getSpiBaseAddr(handler->device);

    if (handler->rxState == READ_DATA)
    {
        fifoSize = GET_REG_WORD_VAL(base + SPI_RXFLR_OFFSET);
        for (i = 0; (i < fifoSize) && (handler->rxSize > 0); i++)
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
            //flush the rx FIFO
            while (GET_REG_WORD_VAL(base + SPI_SR_OFFSET) & SPI_SR_RFNE)
            {
                GET_REG_WORD_VAL(base + SPI_DR_OFFSET);
            }
            if (handler->readDoneCallback != NULL)
            {
                handler->readDoneCallback(handler->rxSize, handler->rxChannel, handler->rxFlags, handler->rxBuffer);
            }
            maskInterrupt(base, SPI_IMR_RXFIM);
            maskInterrupt(base, SPI_IMR_TXEIM);
        }
        else
        {
            if ((handler->rxSize >> 1) < RX_FULL_LEVEL)
            {
                SET_REG_WORD(base + SPI_RXFTLR_OFFSET, ((handler->rxSize) >> 1) - 1);
            }
            else
            {
                SET_REG_WORD(base + SPI_RXFTLR_OFFSET, RX_FULL_LEVEL - 1);
            }
        }
    }
}

static void spiMasterHandler(u32 source)
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

int DrvSpiTransferDone(spiHandler_t* handler)
{
    u32 base = getSpiBaseAddr(handler->device);
    return !(GET_REG_WORD_VAL(base + SPI_SR_OFFSET) & SPI_SR_BUSY);
}

void DrvSpiWaitTransferDone(spiHandler_t* handler)
{
    u32 base = getSpiBaseAddr(handler->device);

    while ((GET_REG_WORD_VAL(base + SPI_SR_OFFSET) & SPI_SR_BUSY));
}


///@}

