#ifndef __PORTAB_H__
#define __PORTAB_H__

#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#ifdef MYRIAD2

#ifdef CVMA2480
   #include "DrvCmxDma.h"
   #include "ShDrvCmxDma.h"
#else
   #include "swcCdma.h"
#endif
#endif

#if (defined MYRIAD2)

#ifdef CVMA2480
#define DMA_TRANSACTION_LIST ShDrvCmxDmaTransaction
#define DMA_CREATE_TRANSACTION(_TaskList, _TaskNr, _NewTaskList, _ReqId, _Src, _Dst, _ByteLength)                       \
{          \
    _TaskNr++;                                                                                                          \
    memcpy((u8*)(_Dst), (u8*)(_Src), (_ByteLength)); \
}
#define DMA_CREATE_TRANSACTION_SRC_STRIDE(_TaskList, _TaskNr, _NewTaskList, _ReqId, _Src, _Dst, _ByteLength, _LineWidth, _SrcStride)\
{                                                                                                                       \
    _TaskNr++;                                                                                                          \
    for(int i = 0; i < (_ByteLength)/(_LineWidth); i++) \
    {    \
        int outOffset = _LineWidth * i;\
        int inOffset = _SrcStride * i;\
        memcpy((u8*)(_Dst) + outOffset, (u8*)(_Src) + inOffset, _LineWidth);\
    }\
}
#define DMA_CREATE_TRANSACTION_DST_STRIDE(_TaskList, _TaskNr, _NewTaskList, _ReqId, _Src, _Dst, _ByteLength, _LineWidth, _DstStride)\
{                                                                                                                       \
    _TaskNr++;                                                                                                          \
    for(int i = 0; i < (_ByteLength)/(_LineWidth); i++) \
    {    \
        int outOffset = _DstStride * i;\
        int inOffset = _LineWidth * i;\
        memcpy((u8*)(_Dst) + outOffset, (u8*)(_Src) + inOffset, _LineWidth);\
    }\
}

#define DMA_LINK_TWO_TASKS(_TaskList, _TaskNr)  //dmaLinkTasks(_TaskList[_TaskNr - 1], 1, _TaskList[_TaskNr]);
//#define DMA_START_LIST_TASK(_Task, _TaskNr)     dmaStartListTask(_Task);
#define DMA_START_LIST_TASK(_Task, _TaskNr)     //ShDrvCmxDmaStartTransfer(&id1);
//#define DMA_WAIT_TASK(_Task)                    dmaWaitTask(_Task);
#define DMA_WAIT_TASK(_Task)                    //ShDrvCmxDmaWaitTransaction(&id1);
#else
#define DMA_TRANSACTION_LIST dmaTransactionList_t
#define DMA_CREATE_TRANSACTION(_TaskList, _TaskNr, _NewTaskList, _ReqId, _Src, _Dst, _ByteLength)                       \
{                                                                                                                       \
    _TaskNr++;                                                                                                          \
    _TaskList[_TaskNr] = dmaCreateTransaction(_ReqId, &_NewTaskList[_TaskNr], (u8*)(_Src), (u8*)(_Dst), (_ByteLength)); \
}
#define DMA_CREATE_TRANSACTION_SRC_STRIDE(_TaskList, _TaskNr, _NewTaskList, _ReqId, _Src, _Dst, _ByteLength, _LineWidth, _SrcStride)\
{                                                                                                                       \
    _TaskNr++;                                                                                                          \
    _TaskList[_TaskNr] = dmaCreateTransactionSrcStride(_ReqId, &_NewTaskList[_TaskNr], (u8*)(_Src), (u8*)(_Dst), (_ByteLength), (_LineWidth), (_SrcStride)); \
}
#define DMA_CREATE_TRANSACTION_DST_STRIDE(_TaskList, _TaskNr, _NewTaskList, _ReqId, _Src, _Dst, _ByteLength, _LineWidth, _DstStride)\
{                                                                                                                       \
    _TaskNr++;                                                                                                          \
    _TaskList[_TaskNr] = dmaCreateTransactionDstStride(_ReqId, &_NewTaskList[_TaskNr], (u8*)(_Src), (u8*)(_Dst), (_ByteLength), (_LineWidth), (_DstStride)); \
}

#define DMA_LINK_TWO_TASKS(_TaskList, _TaskNr)  dmaLinkTasks(_TaskList[_TaskNr - 1], 1, _TaskList[_TaskNr]);
#define DMA_START_LIST_TASK(_Task, _TaskNr)     dmaStartListTask(_Task);
#define DMA_WAIT_TASK(_Task)                    dmaWaitTask(_Task);
#endif
#else
#error "Platform was not selected!!!"
#endif

#endif /*__PORTAB_H__*/
