///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SPI Slave Functions.
///

// System Includes
// ----------------------------------------------------------------------------
#include <assert.h>
#include <stdio.h>

#include <rtems.h>
#include <bsp.h>
#include <bsp/irq-generic.h>
#include <rtems/status-checks.h>

#include <DrvSpiSlaveCP.h>
#include <OsDrvSpiSlaveCP.h>

#define EVENT_SPI_NOT_BUSY RTEMS_EVENT_0
#define TIMEOUT_SPI_NOT_BUSY 10

static void OsSpiSlaveCPIrqHandler(void * context);


rtems_status_code OsDrvSpiSlaveCPInitGlobally(spiHandler_t* handler,
                                              spiTxStartCallback_t* txStartCb,
                                              spiTxDoneCallback_t*  txDoneCb,
                                              spiRxStartCallback_t* rxStartCb,
                                              spiRxDoneCallback_t*  rxDoneCb,
                                              spiPeDoneCallback_t*  peOverCb)
{
    rtems_status_code sc;

    DrvSpiSlaveCPInitBlock(handler, spiConfig.bpw, spiConfig.useDma, spiConfig.device,
                           spiConfig.scpol, spiConfig.scpha, spiConfig.hostGpioIrq,
                           txStartCb, txDoneCb, rxStartCb, rxDoneCb, peOverCb);

    BSP_shared_interrupt_clear(handler->irqSource);

    sc = rtems_interrupt_handler_install(handler->irqSource,
                                         "SPISlave",
                                         RTEMS_INTERRUPT_UNIQUE,
                                         OsSpiSlaveCPIrqHandler,
                                         (void *)handler);
    RTEMS_CHECK_SC(sc, "install irq_status_reg handler\n");

    BSP_Set_interrupt_type_priority(handler->irqSource, POS_EDGE_INT, spiConfig.irqLevel);

    bsp_interrupt_vector_enable(handler->irqSource);

    return RTEMS_SUCCESSFUL;

}

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
                                      spiPeDoneCallback_t*  peOverCb)
{
    rtems_status_code sc;

    DrvSpiSlaveCPInitBlock(handler, wordSizeBytes, useDma, device, cpol, cpha, hostIrqGpio,
                           txStartCb, txDoneCb, rxStartCb, rxDoneCb, peOverCb);

    BSP_shared_interrupt_clear(handler->irqSource);

    sc = rtems_interrupt_handler_install(handler->irqSource,
                                         "SPISlave",
                                         RTEMS_INTERRUPT_UNIQUE,
                                         OsSpiSlaveCPIrqHandler,
                                         (void *)handler);
    RTEMS_CHECK_SC(sc, "install irq_status_reg handler\n");

    BSP_Set_interrupt_type_priority(handler->irqSource, POS_EDGE_INT, interruptLevel);

    bsp_interrupt_vector_enable(handler->irqSource);

    return RTEMS_SUCCESSFUL;

}

static void OsSpiSlaveCPIrqHandler(void * context)
{
    spiHandler_t *handler = (spiHandler_t*)context;

    DrvSpiSlaveCPIsrHandler(handler);
}
