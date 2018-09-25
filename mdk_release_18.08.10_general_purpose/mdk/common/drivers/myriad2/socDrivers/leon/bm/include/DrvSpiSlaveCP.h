///
/// @file DrvSpiSlaveCP.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvSpiSlaveCP SpiSlaveCP Driver
/// @{
/// @brief     SPI Slave API.
///


#ifndef _DRV_SPI_SLAVE_CP_H_
#define _DRV_SPI_SLAVE_CP_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "DrvSpiSlaveDefinesCP.h"
#include "registersMyriad.h"

#ifdef __cplusplus
extern "C" {
#endif

// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

///Initialize the SPI slave device for bare metal usage
///@param[out] SPI handler
///@param[in]  wordSizeBytes FIFO word size in bytes
///@param[in]  useDma specifies whether DMA is used for transfers or not
///@param[in]  device SPI device number
///@param[in]  cpol SPI polarity
///@param[in]  cpha SPI phase
///@param[in]  interruptLevel priority level for SPI interrupt
///@param[in]  txStartCb callback executed on a write request
///@param[in]  txDoneCb callback executed when a write is finished
///@param[in]  rxStartCb callback executed on a read request
///@param[in]  rxDoneCb callback executed when a read is finished
///@return non zero in case of error and 0 otherwise
int DrvSpiSlaveCPInit(spiHandler_t* handler,
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

///Initialize the SPI slave device block and handler for general-purpose 
///@param[out] SPI handler
///@param[in]  wordSizeBytes FIFO word size in bytes
///@param[in]  useDma specifies whether DMA is used for transfers or not
///@param[in]  device SPI device number
///@param[in]  cpol SPI polarity
///@param[in]  cpha SPI phase
///@param[in]  interruptLevel priority level for SPI interrupt
///@param[in]  txStartCb callback executed on a write request
///@param[in]  txDoneCb callback executed when a write is finished
///@param[in]  rxStartCb callback executed on a read request
///@param[in]  rxDoneCb callback executed when a read is finished
///@return non zero in case of error and 0 otherwise
int DrvSpiSlaveCPInitBlock(spiHandler_t* handler,
                           wordSizeBytes_t wordSizeBytes,
                           dmaUsed_t useDma,
                           spiSlaveBlock_t device,
                           u32 cpol,
                           u32 cpha,
                           u32 hostIrqGpio,
                           spiTxStartCallback_t* txStartCb,
                           spiTxDoneCallback_t*  txDoneCb,
                           spiRxStartCallback_t* rxStartCb,
                           spiRxDoneCallback_t*  rxDoneCb,
                           spiPeDoneCallback_t*  peOverCb);

void DrvSpiSlaveCPIsrHandler(spiHandler_t *handler);

/// Release the slave interface
///@param[in]  SPI handler
int DrvSpiSlaveCPClose(spiHandler_t *handler);

void DrvSpiSlaveCPNotifyDataToSend(spiHandler_t *handler);

#ifdef __cplusplus
}
#endif

/// @}
#endif

///@}
