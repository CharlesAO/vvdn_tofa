///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @defgroup swcCdmaApi CMXDMA API
/// @ingroup  swcCdmaApi
/// @{
/// @brief CMXDMA driver for Shave processors
///
/// This driver lets you perform fast data transfers using CMXDMA hardware
///

#ifndef _SWC_CDMA_H_
#define _SWC_CDMA_H_

#ifndef USE_CMX_DMA_NEW_DRIVER

#include "swcCdmaCommon.h"
#include <DrvRegUtilsDefines.h>
#include <stdarg.h>
#include <DrvIcbDefines.h>
#include <swcWhoAmI.h>

#ifdef __cplusplus
extern "C" {
#endif


/// @brief  Initialize a requester ID which will be used to properly initialize and distinguish single tasks or groups of tasks.
/// @param[in] priority         - The priority that will be assigned to all the tasks created using the returned ID
/// @param[in] agentToAssign    - The link agent to be used by CMXDMA while performing transfers initiated with generated requester ID.
/// @return a new requester ID
///
dmaRequesterId dmaInitRequesterWithAgent(int priority, int agentToAssign);

/// @brief Set up the number of link agents the driver will use in order to start new tasks.
///
/// If this function is not called, the default configuration will be to use all 4 link agents.
/// If an invalid configuration will be provided(e.g too many link agents to use),
/// the configuration will be rounded to the first appropriate.
/// @param[in] nrOfUsedAgents - How many agents to use
/// @param[in] startingFrom   - the first agent which will be used in the current configuration
/// @return void
///
void dmaSetUsedAgents(u8 nrOfUsedAgents, u8 startingFrom);

/// @brief Translate windowed address into real physical address.
/// Non-windowed address are passed through.
/// @param[in] inAddr      - Input virtual(windowed) Address
/// @param[in] shaveNumber - Shave to which the virtual address relates
/// @return Resolved address
///
u32 dmaSolveRelAddr(u32 inAddr, u32 shaveNumber);


/// @brief CMXDMA task structure initialization extension, allowing the user to set a custom burst length
/// @brief Please make sure the Src and Dst parameters are received with the proper restrictions if your application has particular ones.
/// @param[in] Type           Transaction type
/// @param[in] ReqId          A requester ID returned by function #dmaInitRequester used to set the task priority and the task ID
/// @param[in] NewTransaction Pointer to user-allocated space for a new task structure
/// @param[in] Src            Source address of data transfer
/// @param[in] Dst            Destination address of data transfer
/// @param[in] ByteLength     Size(in bytes) of the transfer
/// @param[in] SrcLineWidth   Source line width
/// @param[in] DstLineWidth   Destination line width
/// @param[in] SrcStride      Source stride
/// @param[in] DstStride      Destination stride
/// @param[in] BurstLength    Number of transactions in a burst (1 - 16)
/// @return Pointer to initialized CMXDMA structure
///
dmaTransactionList* dmaCreateTransactionExt(u32 Type, dmaRequesterId ReqId, dmaTransactionList *NewTransaction,
                                            u8* Src, u8* Dst, u32 ByteLength, u32 SrcLineWidth, u32 DstLineWidth,
                                            s32 SrcStride, s32 DstStride, u8 BurstLength);

/// @}
#ifdef __cplusplus
}
#endif

#endif // USE_CMX_DMA_NEW_DRIVER

#endif // _SWC_CDMA_H_

