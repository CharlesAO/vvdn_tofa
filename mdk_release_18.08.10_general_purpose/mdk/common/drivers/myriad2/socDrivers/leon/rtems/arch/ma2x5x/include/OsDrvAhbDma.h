///
/// @file OsDrvAhbDma.h
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved
///            For License Warranty see: common/license.txt
/// @defgroup OsDrvAhbDma OsAhbDma Driver
/// @{
/// @brief     AHB DMA functions API.
///
/// --------------------------------------------------------------------------------
///


#ifndef _AHB_DMA_H_
#define _AHB_DMA_H_

// System Includes
// ----------------------------------------------------------------------------

// Application Includes
// ----------------------------------------------------------------------------
#include <rtems.h>
#include "OsDrvAhbDmaDefines.h"
// Common for all OS drivers
#include <OsCommon.h>
// Bare-metal layer include
#include <DrvAhbDma.h>

#ifdef __cplusplus
extern "C" {
#endif

// Source Specific #defines and types  (typedef,enum,struct)
// ----------------------------------------------------------------------------

// Global function prototypes
// ----------------------------------------------------------------------------

/// AHB DMA driver API initialization.
/// @param[in] interruptLevel interrupt level for AHB DMA
///@return
///     OS_MYR_DRV_ERROR - initialization failed
///     OS_MYR_DRV_SUCCESS - initialization finished successfully
///     OS_MYR_DRV_TIMEOUT - could not access the resource in the given time
///
int OsDrvAhbDmaInit(u32 interruptLevel);

/// Initialize an AHB DMA handler on the given channel
///
/// It must be followed by a call to DrvAhbDmaChannelConfigHw/Sw to configure the channel
///@param [out] handler the AHB DMA handler
///@param [in]  channel the channel on which the transfer takes place
///@param [in]  interruptLevel level of priority for AHB DMA interrupt
///@param [in] transferDoneCallback this parameter is not used; it will be used in the
///      future to provide callbacks in thread context
///@return
///     OS_MYR_DRV_ERROR - the resource couldn't be accessed in the given amount of time
///     OS_MYR_DRV_SUCCESS - an error has occurred while initializing the API
///     OS_MYR_DRV_TIMEOUT - could not access the resource in the given time
///
int OsDrvAhbDmaOpenChannel(osDrvAhbDmaHandler_t* handler, u32 channel,
        ahbDmaTransferDoneCallback_t* transferDoneCallback, OS_MYRIAD_DRIVER_PROTECTION protection);

/// Configures a DMA channel
///@param [in] handler the AHB DMA handler previously created by a call to DrvAhbDmaInit
///@param [in] dstTrWidth destination transfer width
///@param [in] srcTrWidth source transfer width
///@param [in] dstInc indicates whether to increment or decrement the destination address on every destination transfer
///@param [in] srcInc indicates whether to increment or decrement the source address on every source transfer
///@param [in] dstBurstSize destination burst transaction length
///@param [in] srcBurstSize source burst transaction length
///@param [in] transferType transfer type and flow control
/// The following transfer types are supported
///  - memory to memory
///  - memory to peripheral
///  - peripheral to memory
///  - peripheral to peripheral
///@param [in] priority DMA channel priority
///
int OsDrvAhbDmaChannelConfig(osDrvAhbDmaHandler_t* handler,
                drvAhbDmaTrWidth_t dstTrWidth, drvAhbDmaTrWidth_t srcTrWidth,
                drvAhbDmaInc_t dstInc, drvAhbDmaInc_t srcInc,
                drvAhbDmaBurstSize_t dstBurstSize, drvAhbDmaBurstSize_t srcBurstSize,
                drvAhbDmaCtlTransferType_t transferType, u32 priority);

///Searches for a free channel and returns a handler that uses the found channel.
///@param [out] handler AHB DMA handler
///@param [in] transferDoneCallback this parameter is not used; it will be used in the
///      future to provide callbacks in thread context
///@param [in] protection the type of protection used for the new opened channel
///@return
///     OS_MYR_DRV_ERROR - the resource couldn't be accessed in the given amount of time
///     OS_MYR_DRV_SUCCESS - an error has occurred while initializing the API
///     OS_MYR_DRV_TIMEOUT - could not access the resource in the given time
///
int OsDrvAhbDmaOpenAnyChannel(osDrvAhbDmaHandler_t* handler, ahbDmaTransferDoneCallback_t* transferDoneCallback,
        OS_MYRIAD_DRIVER_PROTECTION protection);

///Checks if the specified channel is free
///@param [in] channel the channel number
///@param [out] free the status of the channel: 0 channel is not free, 1 channel is free
///@return
///     OS_MYR_DRV_ERROR - the resource couldn't be accessed in the given amount of time
///     OS_MYR_DRV_SUCCESS - an error has occurred while initializing the API
///     OS_MYR_DRV_TIMEOUT - could not access the resource in the given time
///
int OsDrvAhbDmaIsFreeChannel(u32 channel, s32* free);

///Performs a single block transfer
///@param [in] handler the AHB DMA handler
///@param [in] handler the AHB DMA handler
///@param [in] src address of the transfer source
///@param [out] dst address of the transfer destination
///@param [in] transferSize the size of the transfer in bytes
///@return
///     OS_MYR_DRV_ERROR - the resource couldn't be accessed in the given amount of time
///     OS_MYR_DRV_SUCCESS - an error has occurred while initializing the API
///     OS_MYR_DRV_TIMEOUT - could not access the resource in the given time
///
int OsDrvAhbDmaSingleBlockTransfer(osDrvAhbDmaHandler_t* handler, void* src, void* dst,
                                      u32 transferSize, u32 enAndWait);

///Performs a transfer using auto-reload on source and/or destination
///@param [in] handler the AHB DMA handler
///@param [in] handler the AHB DMA handler
///@param [in] src address of the transfer source
///@param [out] dst address of the transfer destination
///@param [in] transferSize the size of the transfer in bytes
///@param [in] blockSize the size of a single block transfer that happens between autoreloads
///@param [in] enAndWait The parameter can take one of two values:
///   DRV_AHB_DMA_START_NO_WAIT - the function returns immediately and the task continues it's execution
///   DRV_AHB_DMA_START_WAIT - the task gets blocked until the transfer finishes
///@return
///     OS_MYR_DRV_ERROR - the resource couldn't be accessed in the given amount of time
///     OS_MYR_DRV_SUCCESS - an error has occurred while initializing the API
///     OS_MYR_DRV_TIMEOUT - could not access the resource in the given time
///
int OsDrvAhbDmaAutoreloadTransfer(osDrvAhbDmaHandler_t* handler, void* src, void* dst, u32 transferSize,
                    u32 blockSize, ahbDmaAutoreload_t reload, u32 enAndWait);

///Select the handshaking interfaces for source and destination
///@param [in] handler the AHB DMA handler
///@param [in] dstHsSel the type of handshake interface for destination
///@param [in] srcHsSel the type of handshake interface for source
///@param [in] dstHwHs hardware handshake interface number for destination;
//    if the destination uses software handshake, this parameter is ignored
///@param [in] srcHwHs hardware handshake interface number for source;
///    if the source uses software handshake, this parameter is ignored
///@param [in] enAndWait The parameter can take one of two values:
///   DRV_AHB_DMA_START_NO_WAIT - the function returns immediately and the task continues it's execution
///   DRV_AHB_DMA_START_WAIT - the task gets blocked until the transfer finishes and the function return
///   only after that
///@return
///     OS_MYR_DRV_ERROR - the resource couldn't be accessed in the given amount of time
///     OS_MYR_DRV_SUCCESS - an error has occurred while initializing the API
///     OS_MYR_DRV_TIMEOUT - could not access the resource in the given time
///
int OsDrvAhbDmaSetHs(osDrvAhbDmaHandler_t* handler, ahbDmaHsSel_t dstHsSel,
                    ahbDmaHsSel_t srcHsSel, u32 dstHwHs, u32 srcHwHs);

///Destination scatter configuration
///@param [in] dstScatterCount specifies the number of contiguous destination transfers between successive scatter boundaries
///@param [in] dstScatterInterval destination scatter interval
///@return
///     OS_MYR_DRV_ERROR - the resource couldn't be accessed in the given amount of time
///     OS_MYR_DRV_SUCCESS - an error has occurred while initializing the API
///     OS_MYR_DRV_TIMEOUT - could not access the resource in the given time
///
int OsDrvAhbDmaDstScatterConfig(osDrvAhbDmaHandler_t* handler, u32 dstScatterCount, u32 dstScatterInterval);

///Disables destination scatter option for handler parameter
///@param [in] handler AHB dma handler for which the destination scatter should be disabled
///
int OsDrvAhbDmaDstScatterDisable(osDrvAhbDmaHandler_t* handler);

///Source gather configuration
///@param [in] srcGatherCount source contiguous transfer count between successive gather boundaries
///@param [in] srcGatherInterval source gather interval
///@return
///     OS_MYR_DRV_ERROR - the resource couldn't be accessed in the given amount of time
///     OS_MYR_DRV_SUCCESS - an error has occurred while initializing the API
///     OS_MYR_DRV_TIMEOUT - could not access the resource in the given time
///
int OsDrvAhbDmaSrcGatherConfig(osDrvAhbDmaHandler_t* handler, u32 srcGatherCount, u32 srcGatherInterval);

///Disables source gather option for handler parameter
///@param [in] handler AHB dma handler for which the source gather should be disabled
///
int OsDrvAhbDmaSrcGatherDisable(osDrvAhbDmaHandler_t* handler);

///Performs a memory to memory copy from source to destination
///@param [in] src the address of the transfer source
///@param [out] the address of the address of the transfer destination
///@param [in] size transfer size in bytes
///@param [in] irqSource
///     OS_MYR_DRV_ERROR - the resource couldn't be accessed in the given amount of time
///     OS_MYR_DRV_SUCCESS - an error has occurred while initializing the API
///
int OsDrvAhbDmaMemCopy(void* src, void* dst, s32 size);

/// Releases the resources allocated for the AHB DMA channel associated to the handler given as parameter
///@param [out] handler the AHB DMA handler
///@return
///     OS_MYR_DRV_ERROR - the resource couldn't be accessed in the given amount of time
///     OS_MYR_DRV_SUCCESS - an error has occurred while initializing the API
///     OS_MYR_DRV_TIMEOUT - could not access the resource in the given time
///
int OsDrvAhbDmaCloseChannel(osDrvAhbDmaHandler_t* handler);

/// Releases all the resources allocated for the AHB DMA
///@param [out] handler the AHB DMA handler
///@return
///     OS_MYR_DRV_ERROR - the resource couldn't be accessed in the given amount of time
///     OS_MYR_DRV_SUCCESS - an error has occurred while initializing the API
///     OS_MYR_DRV_TIMEOUT - could not access the resource in the given time
///
int OsDrvAhbDmaClose(void);

/// Waits for the transfer to finish for all the channels mentioned in the events parameter
///@param [in] channels one or more channels for which to wait to finish
///@param [in] timeout the number of clock ticks to wait for the channels to finish
///  If this parameter is 0 the function will block the thread until all the channels finish their
///  transactions. If the timeout is greater than 0 the thread will wait only for the given amount
///  of time and will return in the channelsActive variable the set of channnels that are still active.
///@param [out] channelsActive the set of channels that are still active after the timeout expires
///@return
///     OS_MYR_DRV_ERROR - the resource couldn't be accessed in the given amount of time
///     OS_MYR_DRV_SUCCESS - an error has occurred while initializing the API
///     OS_MYR_DRV_TIMEOUT - could not access the resource in the given time
/// This function MUST be called in the same thread that started the transaction
/// for which the function is waiting to finish.
int OsDrvAhbDmaWaitTransferFinish(u32 noOfChannels, osDrvAhbDmaHandler_t* channelList, u32 timeout, u32* channelsActive );

/// }@
#ifdef __cplusplus
}
#endif


#endif

///@}
