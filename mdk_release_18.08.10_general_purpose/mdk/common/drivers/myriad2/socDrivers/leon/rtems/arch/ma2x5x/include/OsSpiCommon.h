///
/// @file OsSpiCommon.h
/// 
/// 
/// @defgroup OsSpiCommon OsSpiCommon Driver
/// @{
/// @brief SpiCommon API functions declarations.
///

#ifndef OSSPICOMMON_H__
#define OSSPICOMMON_H__

#include "HglSpi.h"
#include "OsDrvAhbDma.h"

#include <rtems.h>

#ifdef __cplusplus
extern "C" {
#endif

struct osSpiHandler_t;

typedef void (*SpiCommonRxCallback)(struct osSpiHandler_t *handler);
typedef void (*SpiCommonTxCallback)(struct osSpiHandler_t *handler);

struct spiSlaveTransferSettings_t
{
    int cpol:1;
    int cpha:1;
    int bytesPerWord;
};

struct spiGeneralSettings_t
{
    int spiDevice;
    int irqLevel;
    int useDma;
};

struct osSpiHandler_t {
    spiHandler_t hglHndl;
    rtems_vector_number irqVector;
    u32 useDma;

    osDrvAhbDmaHandler_t* txDmaHandler;
    osDrvAhbDmaHandler_t* rxDmaHandler;

    SpiCommonRxCallback osRxCb;
    SpiCommonTxCallback osTxCb;

    struct spiTask_t rx;
    struct spiTask_t tx;

    int ioTimeoutTicks;

    rtems_id tid;

    rtems_id sid;
    u32 eventFlags;
};

typedef enum {
    RTEMS_EVENT_RX_DONE = (1 << 0),
    RTEMS_EVENT_TX_DONE = (1 << 1),

    RTEMS_EVENT_RXOIS = (1 << 10),
    RTEMS_EVENT_RXUIS = (1 << 11),
    RTEMS_EVENT_TXOIS = (1 << 12),
} spiEvent_t;

int OsSpiCommonInit(struct osSpiHandler_t * handler,
                    spiType_t st,
                    struct spiSlaveTransferSettings_t *setings,
                    struct spiGeneralSettings_t *general,
                    u32 baudrate);

int OsSpiCommonTrfRxTx(struct osSpiHandler_t * handler,
                       void *rxBuff, const void *txBuff, u32 size);
int OsSpiCommonTrfRxTxNoWait(struct osSpiHandler_t * handler,
                             void *rxBuff, const void *txBuff, u32 size);

int OsSpiCommonWaitForSPIToFinish(struct osSpiHandler_t * handler);
int OsSpiCommonWaitSPIEvent(struct osSpiHandler_t * handler);

struct osSpiHandler_t * OsSpiCommonGetHandler(spiSlaveBlock_t spiBlock);

#ifdef __cplusplus
}
#endif

#endif

///@}
