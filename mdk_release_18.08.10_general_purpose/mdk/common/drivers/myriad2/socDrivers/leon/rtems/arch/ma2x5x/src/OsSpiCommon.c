///
/// @file OsSpiCommon.c
/// 
/// 
/// @ingroup OsSpiCommon
/// @{
///

#include <OsSpiCommon.h>
#include <malloc.h>
#include <mvMacros.h>
#include <assert.h>

#define __CACHE_LINE_SIZE 64

#define SPI_DMA_CH_PRIO        (5)

#define SPI_DMA_HW_INT_TX_0    (10)
#define SPI_DMA_HW_INT_RX_0    (11)
#define SPI_DMA_HW_INT_TX_1    (12)
#define SPI_DMA_HW_INT_RX_1    (13)
#define SPI_DMA_HW_INT_TX_2    (14)
#define SPI_DMA_HW_INT_RX_2    (15)

#define RTEMS_EVENT_ERR  ( RTEMS_EVENT_RXOIS |  \
                           RTEMS_EVENT_RXUIS |  \
                           RTEMS_EVENT_TXOIS )

#define MAX_DMA_SIZE (4096 - __CACHE_LINE_SIZE) // make sure DMA size is aligned to 64 (cache line)

typedef enum {
    SPI_COMMON_DMA_RX,
    SPI_COMMON_DMA_TX
} dmaDirection_t;

static u8 __attribute__((aligned(16))) rxFake[16];
static u8 __attribute__((aligned(16))) txFake[16] =
{
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF
};

static struct osSpiHandler_t *spiBlockHandlers[SPI_DEVICE_NO];

// TODO: Save context for IRQ handler here
struct osSpiHandler_t * handlerToChannel[DRV_AHB_DMA_CHANNEL_NO];

static int OsSpiCommonOpenDMA(struct osSpiHandler_t * handler);

static inline void rxDone(struct osSpiHandler_t *handler);
static inline void txDone(struct osSpiHandler_t *handler);

static void OsSpiCommonHalISR(void * context)
{
    struct osSpiHandler_t *handler = (struct osSpiHandler_t*)context;
    int rc;
    u32 rxRemainingSize;
    u32 txRemainingSize;
    u32 level;

    rc = HglSpiCheckError(&handler->hglHndl);
    if(rc)
    {
        if(rc & SPI_RX_OVERFLOW)
        {
            handler->eventFlags |= RTEMS_EVENT_RXOIS;
            rtems_semaphore_release( handler->sid );
        }
        if(rc & SPI_RX_UNDERFLOW)
        {
            handler->eventFlags |= RTEMS_EVENT_RXUIS;
            rtems_semaphore_release( handler->sid );
        }
        if(rc & SPI_TX_OVERFLOW)
        {
            handler->eventFlags |= RTEMS_EVENT_TXOIS;
            rtems_semaphore_release( handler->sid );
        }
    }

    rc = HglSpiRxHandler(&handler->hglHndl, &rxRemainingSize);
    if (rc)
        rxDone(handler);
    if (rxRemainingSize <= 0)
    {
       rtems_interrupt_disable(level);
       HGLSpiIrqOperation(&handler->hglHndl, SPI_RX_IRQ_DISABLE);
       rtems_interrupt_enable(level);
    }

    rc = HglSpiTxHandler(&handler->hglHndl, &txRemainingSize);
    if (rc)
        txDone(handler);

    if (txRemainingSize <= 0)
    {
       rtems_interrupt_disable(level);
       HGLSpiIrqOperation(&handler->hglHndl, SPI_TX_IRQ_DISABLE);
       rtems_interrupt_enable(level);
    }

    BSP_shared_interrupt_clear(handler->irqVector);
}

static inline void calculateIncAndPtr(void *buff, void *fake,
                                      drvAhbDmaInc_t *inc, void **ptr)
{
    if(buff)
    {
        *inc = AHB_DMA_CTL_INC;
        *ptr = buff;
    }
    else
    {
        *inc = AHB_DMA_CTL_NO_INC;
        *ptr = fake;
    }
}

static int OsSpiCommonSetDma(struct osSpiHandler_t * handler,
                      dmaDirection_t d, void *buff, u32 size)
{
    osDrvAhbDmaHandler_t * dmaHndl;
    drvAhbDmaCtlTransferType_t tt;
    drvAhbDmaInc_t srcInc, dstInc;
    void *srcPtr, *dstPtr;
    ahbDmaHsSel_t srcHs, dstHs;
    u32 srcHwHs, dstHwHs;
    int sc;

    switch (d)
    {
    case SPI_COMMON_DMA_RX:
        dmaHndl = handler->rxDmaHandler;
        srcInc = AHB_DMA_CTL_NO_INC;
        srcPtr = (void*)&handler->hglHndl.regs->DR;

        calculateIncAndPtr(buff, rxFake, &dstInc, &dstPtr);

        dstHs = AHB_DMA_CFG_HS_SEL_SW;
        srcHs = AHB_DMA_CFG_HS_SEL_HW;

        dstHwHs = 0;
        srcHwHs = SPI_DMA_HW_INT_RX_0 + (2 * handler->hglHndl.device);

        tt = AHB_DMA_CTL_TT_FC_D_P2M;
        break;
    case SPI_COMMON_DMA_TX:
        dmaHndl = handler->txDmaHandler;

        calculateIncAndPtr(buff, txFake, &srcInc, &srcPtr);

        dstInc = AHB_DMA_CTL_NO_INC;
        dstPtr = (void*)&handler->hglHndl.regs->DR;

        dstHs = AHB_DMA_CFG_HS_SEL_HW;
        srcHs = AHB_DMA_CFG_HS_SEL_SW;

        dstHwHs = SPI_DMA_HW_INT_TX_0 + (2 * handler->hglHndl.device);
        srcHwHs = 0;

        tt = AHB_DMA_CTL_TT_FC_D_M2P;
        break;
    default:
        return -1;
    }

    sc = DrvAhbDmaChannelConfig(dmaHndl,
                                AHB_DMA_CTL_TR_WIDTH_8,
                                AHB_DMA_CTL_TR_WIDTH_8,
                                dstInc,
                                srcInc,
                                AHB_DMA_CTL_MSIZE_32,
                                AHB_DMA_CTL_MSIZE_32,
                                tt,
                                SPI_DMA_CH_PRIO);
    if(sc) return sc;

    sc = DrvAhbDmaSetHs(dmaHndl, dstHs, srcHs, dstHwHs, srcHwHs);
    if(sc) return sc;

    sc = DrvAhbDmaSingleBlockTransfer(dmaHndl, srcPtr, dstPtr, size);
    if(sc) return sc;

    return RTEMS_SUCCESSFUL;
}

static inline int OsSpiCommonSetRxDma(struct osSpiHandler_t * handler,
                                      void *rxBuff, u32 size)
{
    return OsSpiCommonSetDma(handler, SPI_COMMON_DMA_RX, rxBuff, size);
}

static inline int OsSpiCommonSetTxDma(struct osSpiHandler_t * handler,
                                      void *txBuff, u32 size)
{
    return OsSpiCommonSetDma(handler, SPI_COMMON_DMA_TX, txBuff, size);
}

int OsSpiCommonRunRx(struct osSpiHandler_t * handler)
{
    int sc;
    u32 level;
    u32 rxTrfSize = MAX_DMA_SIZE;
    void *rxTrfBuff;

    if(handler->useDma)
        HglSpiRequestDataAligned(&handler->rx, &rxTrfBuff, &rxTrfSize,
                                 __CACHE_LINE_SIZE);
    else
        HglSpiRequestData(&handler->rx, &rxTrfBuff, &rxTrfSize);

    if(rxTrfSize == 0)
    {
        HglSpiDmaDisableRx(&handler->hglHndl);
        return 0;
    }

    if(handler->useDma && rxTrfSize >= __CACHE_LINE_SIZE)
    {
        sc = OsSpiCommonSetRxDma(handler, rxTrfBuff, rxTrfSize);
        if(sc) return sc;
        HglSpiDmaEnableRx(&handler->hglHndl, 32);
    }
    else
    {
        HglSpiDmaDisableRx(&handler->hglHndl);

        HglSpiRx(&handler->hglHndl, rxTrfBuff, rxTrfSize);
        rtems_interrupt_disable(level);
        HGLSpiIrqOperation(&handler->hglHndl, SPI_RX_IRQ_ENABLE);
        rtems_interrupt_enable(level);
    }

    return rxTrfSize;
}

static int OsSpiCommonStartRx(struct osSpiHandler_t * handler,
                       void *rxBuff, u32 size)
{
    int sc;

    HglSpiFillSpiTask(&handler->rx, rxBuff, size);

    sc = OsSpiCommonRunRx(handler);
    if (sc == 0) return -1;

    return 0;
}

static int OsSpiCommonRunTx(struct osSpiHandler_t * handler)
{
    int sc;
    u32 level;
    u32 txTrfSize = MAX_DMA_SIZE;
    void *txTrfBuff;

    if(handler->useDma)
        HglSpiRequestDataAligned(&handler->tx, &txTrfBuff, &txTrfSize,
                                 __CACHE_LINE_SIZE);
    else
        HglSpiRequestData(&handler->tx, &txTrfBuff, &txTrfSize);

    if(txTrfSize == 0)
    {

        HglSpiDmaDisableTx(&handler->hglHndl);
        return 0;
    }

    if(handler->useDma && txTrfSize >= __CACHE_LINE_SIZE)
    {
        sc = OsSpiCommonSetTxDma(handler, txTrfBuff, txTrfSize);
        if(sc) return sc;
        HglSpiDmaEnableTx(&handler->hglHndl, 32);
    }
    else
    {
        HglSpiDmaDisableTx(&handler->hglHndl);

        HglSpiTx(&handler->hglHndl, txTrfBuff, txTrfSize);
        rtems_interrupt_disable(level);
        HGLSpiIrqOperation(&handler->hglHndl, SPI_TX_IRQ_ENABLE);
        rtems_interrupt_enable(level);
    }

    return txTrfSize;
}

static int OsSpiCommonStartTx(struct osSpiHandler_t * handler,
                  const void *txBuff, u32 size)
{
    int sc;

    HglSpiFillSpiTask(&handler->tx, (void*)txBuff, size);

    sc = OsSpiCommonRunTx(handler);
    if (sc == 0) return -1;

    return 0;
}

static int OsSpiCommonRxTx(struct osSpiHandler_t * handler,
                    void *rxBuff, const void *txBuff, u32 size)
{
    int sc;

    sc = OsSpiCommonStartRx(handler, rxBuff, size);
    if(sc) return sc;

    sc = OsSpiCommonStartTx(handler, txBuff, size);
    if(sc) return sc;

    return RTEMS_SUCCESSFUL;
}

int OsSpiCommonWaitSPIEvent(struct osSpiHandler_t * handler)
{
    rtems_status_code rc;
    u32 clearflags = 0;
    rtems_interrupt_level irqlevel = 0;

    rc = rtems_semaphore_obtain( handler->sid,
                                 RTEMS_DEFAULT_OPTIONS,
                                 handler->ioTimeoutTicks );
    if(rc)
        return -rc;

    // Check events received
    if (handler->eventFlags & RTEMS_EVENT_RXOIS)
        clearflags = RTEMS_EVENT_RXOIS;
    else if (handler->eventFlags & RTEMS_EVENT_RXUIS)
        clearflags = RTEMS_EVENT_RXUIS;
    else if (handler->eventFlags & RTEMS_EVENT_TXOIS)
        clearflags = RTEMS_EVENT_TXOIS;
    else if (handler->eventFlags & RTEMS_EVENT_TX_DONE)
        clearflags = RTEMS_EVENT_TX_DONE;
    else if (handler->eventFlags & RTEMS_EVENT_RX_DONE)
        clearflags = RTEMS_EVENT_RX_DONE;

    // Clear event processed in a secure way
    if (clearflags) {
        rtems_interrupt_disable(irqlevel);
        handler->eventFlags &= ~clearflags;
        rtems_interrupt_enable(irqlevel);
    }

    return clearflags;
}


int OsSpiCommonWaitForSPIToFinish(struct osSpiHandler_t * handler)
{
    u32 rxtxnotdone = 0;

    while(rxtxnotdone != (RTEMS_EVENT_RX_DONE | RTEMS_EVENT_TX_DONE))
    {
        int evt = OsSpiCommonWaitSPIEvent(handler);

        if(evt < 0)
        {
            return evt;
        }

        if(evt & RTEMS_EVENT_ERR)
        {
            return -evt;
        }

        rxtxnotdone |= evt;
    }

    return RTEMS_SUCCESSFUL;
}

static inline void rxDone(struct osSpiHandler_t *handler)
{
    s32 remaining;
    remaining = OsSpiCommonRunRx(handler);

    if(remaining == 0)
    {
        if(handler->useDma && handler->rx.originPtr != NULL)
        {
            void *from = handler->rx.originPtr;
            void *to = handler->rx.originPtr + handler->rx.totalSize;

            from = ALIGN_UP(from, __CACHE_LINE_SIZE);
            to = ALIGN_DOWN(to, __CACHE_LINE_SIZE);

            // Make sure with alignment that we don't invalidate other data
            _CPU_cache_invalidate_data_range(from, to - from);
        }

        handler->eventFlags |= RTEMS_EVENT_RX_DONE;
        rtems_semaphore_release( handler->sid );

        if(handler->osRxCb)
            handler->osRxCb(handler);
        // Only Clear Higher level SPI Task only when buffer has been fully used up
        if (handler->rx.remainingSize == 0)
            HglSpiClearSpiTask(&handler->rx);
    }
}

static inline void txDone(struct osSpiHandler_t *handler)
{
    s32 remaining;
    remaining = OsSpiCommonRunTx(handler);

    if(remaining == 0)
    {
        handler->eventFlags |= RTEMS_EVENT_TX_DONE;
        rtems_semaphore_release( handler->sid );

        if(handler->osTxCb)
            handler->osTxCb(handler);
        // Only Clear Higher level SPI Task only when buffer has been fully used up
        if (handler->tx.remainingSize == 0)
            HglSpiClearSpiTask(&handler->tx);
    }
}

static void ahbDmaTransferDoneCbRx(u32 channel, ahbDmaChannelStatus_t status)
{
    UNUSED(status);// hush the compiler warning.

    struct osSpiHandler_t * handler = handlerToChannel[channel];
    rxDone(handler);
}

static void ahbDmaTransferDoneCbTx(u32 channel, ahbDmaChannelStatus_t status)
{
    UNUSED(status);// hush the compiler warning.

    struct osSpiHandler_t * handler = handlerToChannel[channel];
    txDone(handler);
}

static int OsSpiCommonSetISR(struct osSpiHandler_t * handler,
                             spiType_t st,
                             u32 irqPriority)
{
    int sc;
    char name[25];

    switch (st)
    {
    case HGL_SPI_MASTER:
        strcpy(name, "SPI_MASTER_0");
        break;
    case HGL_SPI_SLAVE:
        strcpy(name, "SPI_SLAVE_0");
        break;
    default:
        return -1;
    }
    name[strlen(name) - 1] += handler->hglHndl.device;

    // very carefully clear pending interrupts before enabling them
    BSP_Clear_interrupt(handler->irqVector);

    // Set Priority and level
    BSP_Set_interrupt_type_priority(handler->irqVector,
                                    POS_LEVEL_INT,
                                    irqPriority);

    /* Install interrupt handler and disable this vector */
    sc = rtems_interrupt_handler_install(handler->irqVector,
                                         name,
                                         RTEMS_INTERRUPT_UNIQUE,
                                         OsSpiCommonHalISR,
                                         handler);
    if (sc)
        return sc;

    return RTEMS_SUCCESSFUL;
}


int OsSpiCommonInit(struct osSpiHandler_t * handler,
                    spiType_t st,
                    struct spiSlaveTransferSettings_t *settings,
                    struct spiGeneralSettings_t *general,
                    u32 baudrate)
{
    int sc;

    sc = HGLSpiInit((spiHandler_t *)handler,
                    st,
                    settings->bytesPerWord,
                    general->spiDevice,
                    settings->cpol,
                    settings->cpha,
                    baudrate);
    if (sc)
        return sc;

    sc = rtems_semaphore_create(rtems_build_name( 'S', 'S', 'P', 'I' ),
                                0,  /* created locked */
                                RTEMS_DEFAULT_ATTRIBUTES,
                                0,
                                &handler->sid);
    if(sc)
        return sc;

    if (general->irqLevel < 0 ||
        general->irqLevel >= 15)
        return RTEMS_INVALID_NUMBER;

    handler->irqVector = IRQ_SPI1 + general->spiDevice;

    sc = OsSpiCommonSetISR(handler, st, general->irqLevel);
    if (sc)
        return sc;

    handler->useDma = general->useDma;
    if(handler->useDma)
    {
        sc = OsDrvAhbDmaInit(general->irqLevel);
        if(sc != OS_MYR_DRV_ALREADY_INITIALIZED && sc != 0)
            return sc;

        sc = OsSpiCommonOpenDMA(handler);
        if (sc)
            return sc;
    }

    spiBlockHandlers[general->spiDevice] = handler;

    return RTEMS_SUCCESSFUL;
}

int OsSpiCommonOpenDMA(struct osSpiHandler_t * handler)
{
    int sc;

    handler->rxDmaHandler =
        (osDrvAhbDmaHandler_t*)malloc(sizeof(osDrvAhbDmaHandler_t));
    handler->txDmaHandler =
        (osDrvAhbDmaHandler_t*)malloc(sizeof(osDrvAhbDmaHandler_t));

    sc = OsDrvAhbDmaOpenAnyChannel(handler->rxDmaHandler,
                                   ahbDmaTransferDoneCbRx,
                                   OS_MYR_PROTECTION_SEM);
    if(sc)
        return sc;

    handlerToChannel[handler->rxDmaHandler->channel] = handler;

    sc = OsDrvAhbDmaOpenAnyChannel(handler->txDmaHandler,
                                   ahbDmaTransferDoneCbTx,
                                   OS_MYR_PROTECTION_SEM);
    if(sc)
        return sc;

    handlerToChannel[handler->txDmaHandler->channel] = handler;

    return 0;
}

int OsSpiCommonTrfRxTx(struct osSpiHandler_t * handler,
                       void *rxBuff, const void *txBuff, u32 size)
{
    int sc;

    sc = OsSpiCommonTrfRxTxNoWait(handler, rxBuff, txBuff, size);
    if (sc)
        return sc;

    sc = OsSpiCommonWaitForSPIToFinish(handler);
    if(sc)
        return sc;

    return 0;
}

int OsSpiCommonTrfRxTxNoWait(struct osSpiHandler_t * handler,
                             void *rxBuff, const void *txBuff, u32 size)
{
    int sc;
    u32 level;

    if(handler->useDma && size > 48)
    {
        if(handler->useDma && txBuff != NULL)
        {
            // Make sure with alignment that we don't flush other data
            const void *from = ALIGN_UP(txBuff, __CACHE_LINE_SIZE);
            const void *to = ALIGN_DOWN(txBuff + size, __CACHE_LINE_SIZE);

            // Make sure with we don't have stale data to be written back in cache
            // which the DMA needs to transfer
            _CPU_cache_flush_data_range(from, to - from);
        }
        if(handler->useDma && rxBuff != NULL)
        {
            // Make sure with alignment that we don't flush other data
            void *from = ALIGN_UP(rxBuff, __CACHE_LINE_SIZE);
            void *to = ALIGN_DOWN(rxBuff + size, __CACHE_LINE_SIZE);

            // Make sure with we don't have stale data to be written back in cache
            // that might be written back later, overwriting DMA'd data
            _CPU_cache_invalidate_data_range(from, to - from);
        }

        sc = OsSpiCommonRxTx(handler, rxBuff, (void *)txBuff, size);
    }
    else
    {
        sc = HglSpiRxTx(&handler->hglHndl, rxBuff, txBuff, size);
        rtems_interrupt_disable(level);
        HGLSpiIrqOperation(&handler->hglHndl, SPI_RX_IRQ_ENABLE);
        HGLSpiIrqOperation(&handler->hglHndl, SPI_TX_IRQ_ENABLE);
        rtems_interrupt_enable(level);
    }

    return sc;
}

struct osSpiHandler_t * OsSpiCommonGetHandler(spiSlaveBlock_t spiBlock)
{
    assert(spiBlock < SPI_DEVICE_NO);
    return spiBlockHandlers[spiBlock];
}

///@}
