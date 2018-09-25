///
/// @file DrvSpiMaster.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvSpiMaster SpiMaster Driver
/// @{
/// @brief     SPI Master API.
///


#ifndef _DRV_SPI_MASTER_H_
#define _DRV_SPI_MASTER_H_

#include "DrvSpiDefines.h"
#include "registersMyriad.h"

#ifdef __cplusplus
extern "C" {
#endif

///Initialize the SPI master device
///@param[out] SPI handler
///@param[in]  wordSizeBytes FIFO word size in bytes
///@param[in]  useDma specifies whether DMA is used for transfers or not
///@param[in]  device SPI device number
///@param[in]  cpol SPI polarity
///@param[in]  cpha SPI phase
///@param[in]  cpha SPI phase
///@param[in]  clkDivider clock divider used to calculate the transfer speed
///@param[in]  writeCallback callback executed on a write request
///@param[in]  writeDoneCallback callback executed when a write is finished
///@param[in]  readCallback callback executed on a read request
///@param[in]  readDoneCallback callback executed when a read is finished
///@return non zero in case of error and 0 otherwise
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
);
///Initialize the SPI master device
///@param[in]  slaveNo number of the SPI device that needs to be enabled
///
void DrvSpiMasterEnableSlave(spiHandler_t* mstHandler, spiSlaveBlock_t slaveNo);
/// Sends a buffer to the slave
/// @param[in]  handler SPI handler
/// @param[in]  buffer the address of the buffer that needs to be sent
/// @param[in]  size length in bytes of the buffer that needs to be sent
/// @param[in]  channelId the channel on which the data is sent
///
void DrvSpiMasterSendBuffer(spiHandler_t *handler, void* buffer, u32 size, u8 channelId);
/// Sends a buffer to the slave
/// @param[in]  handler SPI handler
/// @param[in]  buffer the address where the received buffer will be stored
/// @param[in]  size length in bytes of the received data payload
/// @param[in]  channelId the channel on which the data is received
///
void DrvSpiMasterReceiveBuffer(spiHandler_t *handler, void* buffer, u32 size, u8 channelId);

/// Returns the status of the SPI transfer
///@param[in]  handler SPI handler
///@return 0 if the transfer is not finished and 1 otherwise
int DrvSpiTransferDone(spiHandler_t* handler);

/// waits for the transfer to finish an cleares the rx FIFO
///@param[in]  handler SPI handler
void DrvSpiWaitTransferDone(spiHandler_t* handler);

/// returns the base address of a SPI device given the device number
///@param[in]  device SPI device number
static inline u32 getSpiBaseAddr(int device)
{

    return ((device == 0) ? SPI1_BASE_ADR :
             (device == 1) ? SPI2_BASE_ADR :
             (device == 2) ? SPI3_BASE_ADR : 0);
}

#ifdef __cplusplus
}
#endif

/// @}
#endif /* _DRV_SPI_MASTER_H_ */

///@}
