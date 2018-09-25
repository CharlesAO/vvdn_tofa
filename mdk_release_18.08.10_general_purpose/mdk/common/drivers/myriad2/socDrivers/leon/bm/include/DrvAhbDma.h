///
/// @file DrvAhbDma.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup DrvAhbDma AhbDma Driver
/// @{
/// @brief     AHB DMA Driver API
///
/// This module contains the helper functions necessary to control the 
/// AHB DMA block in the Myriad Soc
/// 


#ifndef DRV_AHB_DMA_H
#define DRV_AHB_DMA_H 

#include "DrvAhbDmaDefines.h"
#include "mv_types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern ahbDmaHandler_t* ahbDmaHandler[DRV_AHB_DMA_CHANNEL_NO];

/// Initializes AHB DMA driver API
///
/// This must be the first function called before using any other function form the driver
///
///@param [in] irqLevel Priority to be assigned to the interrupt
///@param [in] dynIrqSource dynamic interrupt source
///    The interrupt source must be set to IRQ_DYNAMIC_0 - IRQ_DYNAMIC_11 to redirect to LRT 
///    0 if standard AHB DMA IRQ source is to be used
///
///@return
///     AHB_DMA_DRV_SUCCESS the driver was successfully initialized
///     AHB_DMA_DRV_ALREADY_INITIALIZED the driver is was already initialized with DrvAhbDmaInit call
int DrvAhbDmaInit(u32 interruptLevel, u32 dynIrqSource);

/// Initializes AHB DMA driver API without configuring the interrupts
///
///@return
///     AHB_DMA_DRV_SUCCESS the driver was successfully initialized
///     AHB_DMA_DRV_ALREADY_INITIALIZED the driver is was already initialized with DrvAhbDmaInit call
int DrvAhbDmaOpen(void);

/// Configures AHB DMA interrupts
///
///@param [in] irqLevel Priority to be assigned to the interrupt
///@param [in] dynIrqSource dynamic interrupt source
///    The interrupt source must be set to IRQ_DYNAMIC_0 - IRQ_DYNAMIC_11 to redirect to LRT
///    0 if standard AHB DMA IRQ source is to be used
///
///@return
///     AHB_DMA_DRV_SUCCESS the driver was successfully initialized
///     AHB_DMA_DRV_ALREADY_INITIALIZED the driver is was already initialized with DrvAhbDmaInit call
int DrvAhbDmaConfigureIrq(u32 irqLevel,u32 dynIrqSource);

/// Processes AHB DMA interrupts
///
void DrvAhbDmaProcessIrq(void);

/// Creates an AHB DMA handler with the given parameters
///
///@param [out] handler the AHB DMA handler
///@param [in]  channel the channel on which the transfer takes place
///@param [in]  transferDoneCallback this callback will be executed at the end of the DMA transaction
///
///@return
///     AHB_DMA_DRV_SUCCESS the handler was successfully created
///     AHB_DMA_DRV_BUSY the handler could not be created because the channel is not free
///     AHB_DMA_DRV_NOT_INITIALIZED the driver is not initialized
int DrvAhbDmaOpenChannel(ahbDmaHandler_t* handler, u32 channel, ahbDmaTransferDoneCallback_t* transferDoneCallback);

/// Releases the handler and the channel associated with it
///
///@param [in]  handler the handler that needs to be released
///
///@return
///     AHB_DMA_DRV_SUCCESS the handler was successfully released
///     AHB_DMA_DRV_BUSY the channel didn't finish it's operation, so it can not be closed
///     AHB_DMA_DRV_NOT_INITIALIZED the driver is not initialized
int DrvAhbDmaCloseChannel(ahbDmaHandler_t* handler);

/// Configures a DMA channel
///@param [in] handler the AHB DMA handler previously created by a call to DrvAhbDmaInit
///@param [in] dstTrWidth destination transfer width
///@param [in] srcTrWidth source transfer width
///@param [in] dstInc indicates whether to increment or decrement the destination address on every destination transfer
///@param [in] srcInc indicates whether to increment or decrement the source address on every source transfer
///@param [in] dstBurstSize destination burst transaction length
///@param [in] srcBurstSize source burst transaction length
///@param [in] srcBurstSize source burst transaction length
///@param [in] transferType transfer type and flow control
/// The following transfer types are supported
///  - memory to memory
///  - memory to peripheral
///  - peripheral to memory
///  - peripheral to peripheral
///@param [in] priority DMA channel priority
///
///@return
///     AHB_DMA_DRV_SUCCESS the channel was successfully configured
///     AHB_DMA_DRV_BUSY the channel didn't finish it's operation, so it can not be configured
///     AHB_DMA_DRV_NOT_INITIALIZED the driver is not initialized
int DrvAhbDmaChannelConfig(ahbDmaHandler_t* handler,
                   drvAhbDmaTrWidth_t dstTrWidth, drvAhbDmaTrWidth_t srcTrWidth,
                   drvAhbDmaInc_t dstInc, drvAhbDmaInc_t srcInc,
                   drvAhbDmaBurstSize_t dstBurstSize, drvAhbDmaBurstSize_t srcBurstSize,
                   drvAhbDmaCtlTransferType_t transferType, u32 priority);

///Select the handshaking interfaces for source and destination
///@param [in] handler the AHB DMA handler
///@param [in] dstHsSel the type of handshake interface for destination
///@param [in] dstHwHs hardware handshake interface number for destination;
///    if the destination uses software handshake, this parameter is ignored
///@param [in] srcHwHs hardware handshake interface number for source;
///    if the source uses software handshake, this parameter is ignored
///
///@return
///     AHB_DMA_DRV_SUCCESS the handshake interface was successfully configured
///     AHB_DMA_DRV_BUSY the channel didn't finish it's operation, so it can not be configured
///     AHB_DMA_DRV_NOT_INITIALIZED the driver is not initialized
int DrvAhbDmaSetHs(ahbDmaHandler_t* handler, ahbDmaHsSel_t dstHsSel,
                    ahbDmaHsSel_t srcHsSel, u32 dstHwHs, u32 srcHwHs);

///Performs a single block transfer
///@param [in] handler the AHB DMA handler
///@param [in] handler the AHB DMA handler
///@param [in] src address of the transfer source
///@param [out] dst address of the transfer destination
///@param [in] transferSize the size of the transfer in bytes
///
///@return
///     AHB_DMA_DRV_SUCCESS the transfer finished successfully
///     AHB_DMA_DRV_BUSY the channel didn't finish it's operation, so a new transfer can not be started
///     AHB_DMA_DRV_NOT_INITIALIZED the driver is not initialized
int DrvAhbDmaSingleBlockTransfer(ahbDmaHandler_t* handler, void* src, void* dst, u32 transferSize);

///Performs a transfer using source and/or destination auto-reload
///@param [in] handler the AHB DMA handler
///@param [in] src address of the transfer source
///@param [out] dst address of the transfer destination
///@param [in] transferSize the size of the transfer in bytes
///@param [in] blockSize size of one block that will be transferred between reloads
///@param [in] reload specifies if auto-reload is used for source or destination
///
///@return
///     AHB_DMA_DRV_SUCCESS the transfer finished successfully
///     AHB_DMA_DRV_BUSY the channel didn't finish it's operation, so a new transfer can not be started
///     AHB_DMA_DRV_NOT_INITIALIZED the driver is not initialized
int DrvAhbDmaAutoreloadTransfer(ahbDmaHandler_t* handler, void* src, void* dst, u32 transferSize, u32 blockSize, ahbDmaAutoreload_t reload);

///Enables destination scatter for the given channel
///@param [in] dstScatterCount specifies the number of contiguous destination transfers between successive scatter boundaries
///@param [in] dstScatterInterval destination contiguous transfer count between successive scatter boundaries
///
///@return
///     AHB_DMA_DRV_SUCCESS the channel was successfully configured
///     AHB_DMA_DRV_BUSY the channel didn't finish it's operation, so it can not be configured
///     AHB_DMA_DRV_NOT_INITIALIZED the driver is not initialized
int DrvAhbDmaDstScatterConfig(ahbDmaHandler_t* handler, u32 dstScatterCount, u32 dstScatterInterval);

///Disables destination scatter option for handler parameter
///@param [in] handler AHB dma handler for which the destination scatter should be disabled
///
///@return
///     AHB_DMA_DRV_SUCCESS destination scatter was successfully disabled
///     AHB_DMA_DRV_BUSY the channel didn't finish it's operation, so it can not be configured
///     AHB_DMA_DRV_NOT_INITIALIZED the driver is not initialized
int DrvAhbDmaDstScatterDisable(ahbDmaHandler_t* handler);

///Performs a DMA transfer with source gather enabled
///@param [in] srcGatherCount source contiguous transfer count between successive gather boundaries
///@param [in] srcGatherInterval source gather interval
///
///@return
///     AHB_DMA_DRV_SUCCESS the channel was successfully configured
///     AHB_DMA_DRV_BUSY the channel didn't finish it's operation, so it can not be configured
///     AHB_DMA_DRV_NOT_INITIALIZED the driver is not initialized
int DrvAhbDmaSrcGatherConfig(ahbDmaHandler_t* handler, u32 srcGatherCount, u32 srcGatherInterval);

///Disables source gather option for handler parameter
///@param [in] handler AHB dma handler for which the source gather should be disabled
///
///@return
///     AHB_DMA_DRV_SUCCESS source gather was successfully disabled
///     AHB_DMA_DRV_BUSY the channel didn't finish it's operation, so it can not be configured
///     AHB_DMA_DRV_NOT_INITIALIZED the driver is not initialized
int DrvAhbDmaSrcGatherDisable(ahbDmaHandler_t* handler);

///Returns a free AHB DMA channel
///If the channel number is free than it is returned, otherwise another free channel is returned if available
///@param [out] channel the number of the first free channel that was found, -1 if no free channel was found
///
///@return
///  AHB_DMA_DRV_SUCCESS the operation finished successfully
///  AHB_DMA_DRV_NOT_INITIALIZED the driver API was not initialized
int DrvAhbDmaGetFreeChannel(s32* channel);

///Performs a memory to memory copy from source to destination using.
///@param [in] src the address of the transfer source
///@param [out] the address of the address of the transfer destination
///@param [in] size transfer size in bytes
///@param [in] irqSource
///@return returns 0 on success an -1 on failure
///
///@return
///     AHB_DMA_DRV_SUCCESS the transfer finished successfully
///     AHB_DMA_DRV_BUSY the channel didn't finish it's operation, so a new transfer can not be started
///     AHB_DMA_DRV_NOT_INITIALIZED the driver is not initialized
int DrvAhbDmaMemCopy(void* src, void* dst, s32 size);

///Closes all the AHB DMA channels
///
///@return
///  AHB_DMA_CHANNEL_SUCCESS the driver was closed successfully
///  AHB_DMA_CHANNEL_BUSY at least one of the channels could not be closed. The functions
///  stops at the first channel which can not be closed.
///
///@return
///     AHB_DMA_DRV_SUCCESS all the channels were successfully closed
///     AHB_DMA_DRV_BUSY at least one channel didn't finish it's execution
///     AHB_DMA_DRV_NOT_INITIALIZED the driver is not initialized
int DrvAhbDmaCloseAllChannels(void);

///Closes the AHB DMA driver
///
///@return
///  AHB_DMA_CHANNEL_SUCCESS the driver was closed successfully
///  AHB_DMA_CHANNEL_BUSY at least one of the channels could not be closed. The functions
///  stops at the first channel which can not be closed.
///
///@return
///     AHB_DMA_DRV_SUCCESS the driver was successfully closed
///     AHB_DMA_DRV_BUSY at least one channel didn't finish it's execution
///     AHB_DMA_DRV_NOT_INITIALIZED the driver is not initialized
int DrvAhbDmaClose(void);

#ifdef __cplusplus
}
#endif

#endif // DRV_AHB_DMA_H 

///@}
