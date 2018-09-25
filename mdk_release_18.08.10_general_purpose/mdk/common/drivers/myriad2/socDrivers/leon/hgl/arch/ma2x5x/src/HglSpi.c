///
/// @file HglSpi.c
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup HglSpi
/// @{
/// @brief     HGL for SPI
///


// System Includes
// ----------------------------------------------------------------------------
#include "HglSpi.h"

#include <swcLeonUtils.h>

#include <mv_types.h>
#include <mvMacros.h>
#include "registersMyriad.h"
#include "DrvRegUtils.h"

#include <assert.h>
#include <string.h>

#ifdef DEBUG_HGL_SPI
#ifdef __RTEMS__
#include <rtems.h>
#include <rtems/bspIo.h>
#define HglSpiPrint(...) printk(__VA_ARGS__)
#else
#include <stdio.h>
#define HglSpiPrint(...) printf(__VA_ARGS__)
#endif
#else
#define HglSpiPrint(...)
#endif

/// returns the base address of a SPI device given the device number
///@param[in]  device SPI device number
static inline struct myr2SpiRegs* getSpiBaseAddr(int device)
{
    return ((device == 0) ? (struct myr2SpiRegs*)SPI1_BASE_ADR :
            (device == 1) ? (struct myr2SpiRegs*)SPI2_BASE_ADR :
            (device == 2) ? (struct myr2SpiRegs*)SPI3_BASE_ADR : NULL);
}

static inline int getCtrlR0Value(int bytesPerWord, char cpol, char cpha)
{
    u32 ctrlR0Val = 0;

    if(bytesPerWord < 1 || bytesPerWord > 2)
        return -1;

    ctrlR0Val |= SPI_CTRLR0_SRL_NORMAL;
    ctrlR0Val |= SPI_CTRLR0_SLV_OE_ENA;
    ctrlR0Val |= cpol ? SPI_CTRLR0_SCPOL_BIT : 0;
    ctrlR0Val |= cpha ? SPI_CTRLR0_SCPH_BIT : 0;
    ctrlR0Val |= SPI_CTRLR0_FRF_MOT_SPI;
    ctrlR0Val |= (bytesPerWord * 8 - 1) & SPI_CTRLR0_DFS_MASK;

    return ctrlR0Val;
}

static inline int configureSpiRole(spiHandler_t* handler, spiType_t st)
{
    volatile u32 regVal = 0;

    regVal = GET_REG_WORD_VAL(CPR_GEN_CTRL_ADR);
    switch(st)
    {
    case HGL_SPI_SLAVE:
        HglSpiPrint("conf as SPI SLAVE \n");
        regVal = regVal & ~(1 << (handler->device + 4));
        break;
    case HGL_SPI_MASTER:
        regVal = regVal |  (1 << (handler->device + 4));
        break;
    default:
        return -1;
    }
    SET_REG_WORD(CPR_GEN_CTRL_ADR, regVal);

    return 0;
}

static s32 setFifo(spiHandler_t *handler, volatile u32 * offset,
                   s32 targetDepth, s32 requestedByteLength, s32 rxtx)
{
    s32 fifoLevel;

    // set the tx FIFO level for the next transfer
    fifoLevel = requestedByteLength / handler->bytesPerWord;

    // make sure it's positive
    fifoLevel = MAX(fifoLevel, 0);

    if(rxtx == 1)
        fifoLevel = MAX(fifoLevel, targetDepth);
    else
        fifoLevel = MIN(fifoLevel, targetDepth);

    *offset = fifoLevel - 1;

    return fifoLevel;
}

static inline void setTxFifoThreshold(spiHandler_t *handler, s32 requestedByteLength)
{
    // TODO: targetDepth could be calculated based on sysclk and spi clk
    setFifo(handler, &handler->regs->TXFTRL,
            SPI_TX_FIFO_DEPTH_ISR, requestedByteLength, 1);
}

static inline void setRxFifoThreshold(spiHandler_t *handler, s32 requestedByteLength)
{
    // TODO: targetDepth could be calculated based on sysclk and spi clk
    setFifo(handler, &handler->regs->RXFTLR,
            SPI_RX_FIFO_DEPTH_ISR, requestedByteLength, 0);
}

void HGLSpiIrqOperation(spiHandler_t * handler, spiIrqOperation_t operation)
{
    switch (operation)
    {
        case SPI_TX_IRQ_ENABLE:
            handler->regs->IMR |= SPI_IMR_TXEIM;
            break;
        case SPI_TX_IRQ_DISABLE:
            handler->regs->IMR &= ~SPI_IMR_TXEIM;
            break;
        case SPI_RX_IRQ_ENABLE:
            handler->regs->IMR |= SPI_IMR_RXFIM;
            break;
        case SPI_RX_IRQ_DISABLE:
            handler->regs->IMR &= ~SPI_IMR_RXFIM;
            break;
        default:
            assert(0);
            break;
    }
}

void HGLSpiUpdateTrfMode(spiHandler_t* handler,
                        spiModeRxTx_t mode)
{
    // disable the SPI block to be able to configure it without hassle
    handler->regs->SSIENR = 0;

    handler->regs->CTRLR0 &= ~(SPI_CTRLR0_TMOD_TX_ONLY | SPI_CTRLR0_TMOD_RX_ONLY);
    handler->regs->CTRLR0 |= mode;

    // enable the SPI block at end of configuration
    handler->regs->SSIENR = 1;
}

int HGLSpiUpdateConfiguration(spiHandler_t* handler,
                              u32 bytesPerWord,
                              u32 cpol,
                              u32 cpha,
                              u32 baudRate)
{
    volatile u32 regVal = 0; UNUSED(regVal);

    // disable the SPI block to be able to configure it without hassle
    handler->regs->SSIENR = 0;

    regVal = handler->regs->ICR;
    regVal = handler->regs->TXOICR;
    regVal = handler->regs->RXOICR;
    regVal = handler->regs->RXUICR;

    handler->regs->CTRLR0 = getCtrlR0Value(bytesPerWord, cpol, cpha);

    handler->regs->BAUDR = baudRate;

    handler->regs->RX_SAMPLE_DLY = 4;

    // enable the overflow and underflow interrupts to signal us for lost words
    handler->regs->IMR = SPI_IMR_RXOIM | SPI_IMR_RXUIM | SPI_IMR_TXOIM;

    // enable the SPI block at end of configuration
    handler->regs->SSIENR = 1;

    return 0;
}

int HGLSpiInit(spiHandler_t* handler,
               spiType_t st,
               wordSizeBytes_t bytesPerWord,
               spiSlaveBlock_t device,
               u32 cpol,
               u32 cpha,
               u32 baudRate)
{
    int rc;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
    if(device < SPI1 || device >= SPI_DEVICE_NO)
        return -1;
#pragma GCC diagnostic pop

    handler->device = device;
    handler->regs = getSpiBaseAddr(device);
    if(handler->regs == NULL)
        return -1;

    handler->bytesPerWord = bytesPerWord;

    memset(&handler->rx, 0, sizeof(handler->rx));
    memset(&handler->tx, 0, sizeof(handler->tx));

    // TODO: This is dangerous but I leave it here as a workaround until HglCpr is created (created; what's next? HglCpr not foreseen to be used by other Hgl layers)
    rc = configureSpiRole(handler, st);
    if (rc)
        return rc;

    HGLSpiUpdateConfiguration(handler, bytesPerWord, cpol, cpha, baudRate);

    return 0;
}

int HGLSpiSetSlave(spiHandler_t* handler, u32 slave)
{
    if (slave >= 3)
        return -1;

    handler->regs->SER = 1 << slave;

    return 0;
}

int HglSpiCheckError(spiHandler_t *handler)
{
    volatile u32 regVal = 0; UNUSED(regVal);
    int rFlags = 0;

    if(handler->regs->ISR & SPI_ISR_TXOIS)
    {
        HglSpiPrint("TX ovef !!\n");
        regVal = handler->regs->TXOICR;
        rFlags |= SPI_TX_OVERFLOW;
    }
    if(handler->regs->ISR & SPI_ISR_RXOIS)
    {
        HglSpiPrint("RX ovef !!\n");
        regVal = handler->regs->RXOICR;
        rFlags |= SPI_RX_OVERFLOW;
    }
    if(handler->regs->ISR & SPI_ISR_RXUIS)
    {
        HglSpiPrint("RX undf !!\n");
        regVal = handler->regs->RXUICR;
        rFlags |= SPI_RX_UNDERFLOW;
    }
    return rFlags;
}

void HglSpiFillSpiTask(struct spiTask_t *st, void *origin, u32 size)
{
    st->crtPtr = st->originPtr = origin;
    st->remainingSize = st->totalSize = size;
}

void HglSpiRequestData(struct spiTask_t *st, void **data, u32 *size)
{
    *size = MIN(*size, st->remainingSize);
    st->remainingSize -= *size;

    if(st->originPtr == NULL)
    {
        *data = NULL;
    }
    else
    {
        *data = st->crtPtr;
        st->crtPtr += *size;
    }
}

void HglSpiRequestDataAligned(struct spiTask_t *st,
                              void **data, u32 *size, u32 align)
{
    u32 srcAlignment;

    srcAlignment = ALIGN_UP(st->crtPtr, align) - st->crtPtr;
    if(srcAlignment)
    {
        *size = MIN(*size, srcAlignment); // return just the misaligned part
    }

    *size = MIN(*size, st->remainingSize);

    if(*size > align)
    {
        *size = ALIGN_DOWN(*size, align);
    }

    st->remainingSize -= *size;

    if(st->originPtr == NULL)
    {
        *data = NULL;
    }
    else
    {
        *data = st->crtPtr;
        st->crtPtr += *size;
    }

}

void HglSpiClearSpiTask(struct spiTask_t *st)
{
    st->crtPtr = st->originPtr = NULL;
    st->remainingSize = st->totalSize = 0;
}

int HglSpiTxHandler(spiHandler_t * handler, u32 * remainingSize)
{
    volatile u32 regVal = 0; UNUSED(regVal);

    // make sure handling directly is required
    if ( ! (handler->regs->IMR & SPI_IMR_TXEIM) )
    {
        *remainingSize = handler->tx.remainingSize;
        return 0;
    }

    while( (handler->regs->TXFLR < SPI_TX_FIFO_DEPTH ) &&
           (handler->tx.remainingSize > 0))
    {
        u32 txWord = 0xFF;

        if (handler->tx.crtPtr)
        {
            txWord = *(u8*)handler->tx.crtPtr;
            handler->tx.crtPtr++;
        }

        handler->regs->DR[0] = txWord;

        handler->tx.remainingSize--;
    }

    // clear interrupts here
    regVal = handler->regs->ICR;

    setTxFifoThreshold(handler, handler->tx.remainingSize);
    *remainingSize = handler->tx.remainingSize;
    if(handler->tx.remainingSize <= 0)
    {
        HglSpiClearSpiTask(&handler->tx);

        HglSpiPrint("handler: 0x%p\n", handler);

        return 1;
    }

    return 0;
}

int HglSpiRxHandler(spiHandler_t * handler, u32 * remainingSize)
{
    volatile u32 regVal = 0; UNUSED(regVal);

    // make sure handling directly is required
    if ( ! (handler->regs->IMR & SPI_IMR_RXFIM) )
    {
        *remainingSize = handler->rx.remainingSize;
        return 0;
    }

    while( (handler->regs->RXFLR > 0 ) &&
           (handler->rx.remainingSize > 0))
    {
        u32 rxWord = handler->regs->DR[0];

        if(handler->rx.crtPtr)
        {
            *(u8*)handler->rx.crtPtr = rxWord;
            handler->rx.crtPtr++;
        }

        handler->rx.remainingSize--;
    }

    // clear interrupts here
    regVal = handler->regs->ICR;

    setRxFifoThreshold(handler, handler->rx.remainingSize);
    *remainingSize = handler->rx.remainingSize;
    if(handler->rx.remainingSize <= 0)
    {
        HglSpiClearSpiTask(&handler->rx);

        HglSpiPrint("handler: 0x%p\n", handler);

        return 1;
    }

    return 0;
}

int HglSpiGetBusy(spiHandler_t *handler)
{
    if(handler->regs->SR & 1)
        return 1;
    else
        return 0;
}

int HglSpiRx(spiHandler_t * handler, void *rxBuff, u32 size)
{
    HglSpiFillSpiTask(&handler->rx, (void *)rxBuff, size);

    setRxFifoThreshold(handler, size);

    return 0;
}

int HglSpiTx(spiHandler_t * handler, const void *txBuff, u32 size)
{
    HglSpiFillSpiTask(&handler->tx, (void *)txBuff, size);

    setTxFifoThreshold(handler, size);

    return 0;
}

int HglSpiRxTx(spiHandler_t * handler, void *rxBuff, const void *txBuff, u32 size)
{
    HglSpiTx(handler, txBuff, size);
    HglSpiRx(handler, rxBuff, size);

    HglSpiPrint(" rx: 0x%p tx: 0x%p  size: %d \n",
           rxBuff, txBuff, size);

    return 0;
}

void HglSpiDmaEnableRx(spiHandler_t *handler, u32 dmaThreshold)
{
    assert(dmaThreshold <= 256);
    //receive threshold for DMA
    handler->regs->DMARDLR = dmaThreshold - 1;
    // enable RX DMA
    handler->regs->DMACR |= SPI_DMACR_RDMAE;
}

void HglSpiDmaEnableTx(spiHandler_t *handler, u32 dmaThreshold)
{
    assert(dmaThreshold <= 256);
    //receive threshold for DMA
    handler->regs->DMATDLR = dmaThreshold;
    // enable TX DMA
    handler->regs->DMACR |= SPI_DMACR_TDMAE;
}

void HglSpiDmaDisableRx(spiHandler_t *handler)
{
    // disable RX DMA
    handler->regs->DMACR &= ~SPI_DMACR_RDMAE;
}

void HglSpiDmaDisableTx(spiHandler_t *handler)
{
    // disable TX DMA
    handler->regs->DMACR &= ~SPI_DMACR_TDMAE;
}

///@}
