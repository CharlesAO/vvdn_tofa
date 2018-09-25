///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @defgroup DrvSPISlave SPI Slave API
/// @{
/// @brief     SPI Slave API.
///

#ifndef _OS_DRV_SPI_SLAVE_H_
#define _OS_DRV_SPI_SLAVE_H_

#include <DrvSpiSlaveCP.h>

#include <bsp.h>
#include <bsp/irq-generic.h>
#include <rtems/status-checks.h>

#ifdef __cplusplus
extern "C" {
#endif


    typedef struct{
        spiSlaveBlock_t device;
        u32 scpol;
        u32 scpha;
        u32 bpw;
        dmaUsed_t useDma;
        u32 hostGpioIrq;
        u32 irqLevel;
    }spiSlaveCommunicationConfiguration_t;

#define DRVSPI_CONFIGURATION(dev, cpol, cpha, bytesPerWord, dmaEnabled, hostIrqGpio, interruptLevel) \
    spiSlaveCommunicationConfiguration_t spiConfig = {                  \
        .device = dev,                                                  \
        .scpol = cpol,                                                  \
        .scpha = cpha,                                                  \
        .bpw = bytesPerWord,                                            \
        .useDma = dmaEnabled,                                           \
        .hostGpioIrq = hostIrqGpio,                                     \
        .irqLevel = interruptLevel,                                     \
    }


    extern spiSlaveCommunicationConfiguration_t spiConfig;


    rtems_status_code OsDrvSpiSlaveCPInitGlobally(spiHandler_t* handler,
        spiTxStartCallback_t* txStartCb,
        spiTxDoneCallback_t*  txDoneCb,
        spiRxStartCallback_t* rxStartCb,
        spiRxDoneCallback_t*  rxDoneCb,
        spiPeDoneCallback_t*  peOverCb);

    rtems_status_code OsDrvSpiSlaveCPInit(spiHandler_t* handler,
        wordSizeBytes_t wordSizeBytes,
        dmaUsed_t useDma,
        spiSlaveBlock_t device,
        u32 cpol,
        u32 cpha,
        u32 interruptLevel,
        u32 hostIrqGpio,
        spiTxStartCallback_t* txStartCb,
        spiTxDoneCallback_t*  txDoneCb,
        spiRxStartCallback_t* rxStartCb,
        spiRxDoneCallback_t*  rxDoneCb,
        spiPeDoneCallback_t*  peOverCb);

    rtems_status_code OsDrvSpiSlaveCPSendPacket(spiHandler_t *handler, u8 channel, u8 flags, s32 size, void *buff);

#ifdef __cplusplus
}
#endif


#endif
