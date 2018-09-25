///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @defgroup swcCdmaDefines CMXDMA Defines
/// @ingroup  swcCdma 
/// @{
/// @brief  Common definitions and types needed by CMXDMA driver
///
/// This file contains all the definitions of constants, typedefs,
/// structures, enums and exported variables for CMXDMA driver for Shave and PC
///

#ifndef _SWC_CDMA_COMMON_DEFINES_H_
#define _SWC_CDMA_COMMON_DEFINES_H_
 

#include "CmxDma.h"

#ifndef __PC__
#define ALIGNED8 __attribute__ ((aligned (8)))
#else 
#define ALIGNED8
#endif

#define SVU_SLICE_OFFSET  0x10000

// Default source/destination plane stride and number of plane
// value used when calling the scCmxDmaCreateTransaction function
// for 1D or 2D DMA transaction.
#define SWC_CMX_DMA_DEFAULT_NUM_PLANE (0)
#define SWC_CMX_DMA_DEFAULT_PLANE_STRIDE (0)

// Valid range for the number of planes.
#define MIN_NUM_PLANES (1)
#define MAX_NUM_PLANES (256)

///Bit field for fine-grained configuration of CMXDMA transaction
typedef struct
        {
          u32 type             : 2;///< Transaction type(1D/2D)
          u32 priority         : 2;///< Transaction priority(0 - 3)
          u32 brstLength       : 4;///< Burst length
          u32 id               : 4;///< Transaction ID
          u32 interruptTrigger : 4;///< ID of interrupt to be generated when the task is executed
          u32 reserved1        : 4;///< Reserved
          u32 disableInt       : 1;///< Disable interrupts
          u32 reserved2        : 6;///< Reserved
          u32 skipNr           : 5;///< Skip descriptor
        }configBits;

///2D transaction type
typedef struct
{
    void *linkAddress;///< pointer to the next element in linked list
    union
    {
        configBits cfgBits;
        u32 fullCfgRegister;
    }cfgLink;
    void *src; ///< Pointer to the source of the data transfer
    void *dst; ///< Pointer to the destination
    u32 length;///< Transaction length
    u32 no_planes;///< Number of planes
    u32 src_width;///< Bytes of data required from one line of source
    u32 src_stride;///< Length in bytes from start of one line of data, to start of next line of data
    u32 dst_width;///< Bytes of data required from one line of destination
    u32 dst_stride;///< Length in bytes from start of one line of data, to start of next line of data
    u32 src_plane_stride;///< Source plane stride
    u32 dst_plane_stride;///< Destination plane stride
#ifdef __PC__
    u8 completed;
#endif
	   //Additional members which don't belong to DESCRIPTOR definition as per CMXDMA HW spec,
	   //but are used for various purposes.
	    u32 agentOff;
	    u32 userData0; //stuff data member so that sizeof(dmaTransactionList_t) % 8 == 0
}dmaTransactionList_t ALIGNED8;  // dmaTransactionLists must be aligned to 64 bit boundaries (This is a requirement of the hardware DMA engine)



#ifdef __cplusplus
extern "C" {
#endif

typedef dmaTransactionList_t dmaTransactionList;
typedef void (*dmaIrqHandler)(dmaTransactionList* ListPtr, void* userContext);
/// @}

#ifdef __cplusplus
}
#endif

#endif
