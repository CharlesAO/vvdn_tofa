///
/// @file DrvSpiSlave.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvSpiSlave SpiSlave Driver
/// @{
/// @brief     SPI Slave API for loopback connections.
///


#ifndef _DRV_SPI_SLAVE_H_
#define _DRV_SPI_SLAVE_H_

// 1: Includes
// ----------------------------------------------------------------------------
#include "DrvSpiDefines.h"
#include "DrvSpiMaster.h"

#ifdef __cplusplus
extern "C" {
#endif


// 2:  Exported Global Data (generally better to avoid)
// ----------------------------------------------------------------------------

// 3:  Exported Functions (non-inline)
// ----------------------------------------------------------------------------

///Initialize the SPI slave device
///@param[out] SPI handler
///@param[in]  wordSizeBytes FIFO word size in bytes
///@param[in]  useDma specifies whether DMA is used for transfers or not
///@param[in]  device SPI device number
///@param[in]  cpol SPI polarity
///@param[in]  cpha SPI phase
///@param[in]  interruptLevel priority level for SPI interrupt
///@param[in]  writeCallback callback executed on a write request
///@param[in]  writeDoneCallback callback executed when a write is finished
///@param[in]  readCallback callback executed on a read request
///@param[in]  readDoneCallback callback executed when a read is finished
///@return non zero in case of error and 0 otherwise
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
);

/// Release the slave interface
///@param[in]  SPI handler
int DrvSpiClose(spiHandler_t *handler);

#ifdef __cplusplus
}
#endif

/// @}
#endif

///@}
