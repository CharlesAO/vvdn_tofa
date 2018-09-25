///
/// @file DrvCmxDmaDefines.h
/// 
/// 
/// @ingroup DrvCmxDma
/// @{
///

#ifndef _DMA_DEFINES_H_
#define _DMA_DEFINES_H_

#include <mv_types.h>
#include <DrvCommon.h>
#include <CmxDma.h>

#define DMA_START_CTRL_REG 0x1

#define DMA_SET_RESERVED1  (1 << 16)
#define IS_DMA_FINISHED(x) ((x) & (1 << 16))

#ifndef WAITING_QUEUE_SIZE
#define WAITING_QUEUE_SIZE     30
#endif

#ifndef DEFAULT_CDMA_INT_LEVEL
#define DEFAULT_CDMA_INT_LEVEL 7
#endif

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

   //Additional members which don't belong to DESCRIPTOR definition as per CMXDMA HW spec,
   //but are used by Leon for various purposes.
    u32 agentOff;
    u32 userData0; //stuff data member so that sizeof(dmaTransactionList_t) % 8 == 0
}dmaTransactionList_t __attribute__ ((aligned (8)));  // dmaTransactionLists must be aligned to 64 bit boundaries (This is a requirement of the hardware DMA engine)


typedef dmaTransactionList_t dmaTransactionList;
typedef void (*dmaIrqHandler)(dmaTransactionList* ListPtr, void* userContext);

enum dma_interrupts
{
	DMA_INT0 = 0,
	DMA_INT1,
	DMA_INT2,
	DMA_INT3,
	DMA_INT4,
	DMA_INT5,
	DMA_INT6,
	DMA_INT7,
	DMA_INT8,
	DMA_INT9,
	DMA_INT10,
	DMA_INT11,
	DMA_INT12,
	DMA_INT13,
	DMA_INT14,
	DMA_INT15
};

typedef struct
{
    dmaTransactionList_t *ListPtr;
    dmaIrqHandler callbackFunc;
    void *userContext;
    u32 taskNum;
    u32 *saveTail;
}dmaQueueElement_t;

typedef enum
{
    DRV_CDMA_UNINITIALIZED = 0,
    DRV_CDMA_INITIALIZED
}dmaStatus_t;

typedef struct
{
    dmaStatus_t status;
}drvStat_t;

typedef enum
{
    CDMA_TASK_STARTED = MYR_DRV_CUSTOM_CODE_START_OFFSET,
    CDMA_TASK_LINKED,
    CDMA_TASK_QUEUED,
    CDMA_TASK_QUEUE_FULL,
    CDMA_TASK_FINISHED,
    CDMA_TASK_ERR
}dmaTaskStatus_t;

typedef enum
{
    CMX_DMA_DRV_SUCCESS = MYR_DRV_SUCCESS,
    CMX_DMA_DRV_ERROR = MYR_DRV_ERROR,
    CMX_DMA_DRV_NOT_INITIALIZED = MYR_DRV_NOT_INITIALIZED,
    CMX_DMA_DRV_ALREADY_INITIALIZED = MYR_DRV_ALREADY_INITIALIZED
}dmaDrvStatus_t;

#endif

///@}
