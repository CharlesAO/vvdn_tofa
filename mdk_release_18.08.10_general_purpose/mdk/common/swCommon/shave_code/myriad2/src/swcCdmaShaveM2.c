/*
 * Cdma.c
 *
 *  Created on: Mar 3, 2014
 */

#include "swcCdma.h"

#ifndef USE_CMX_DMA_NEW_DRIVER

#include <mv_types.h>
#include <svuCommonShave.h>
#include <DrvRegUtilsDefines.h>

static u8 usedAgentsNr = 3;
static u8 startFrom = 1;
u32 **tail[4];

#ifndef USE_CMX_DMA_GLOBAL_TAIL
#define USE_CMX_DMA_GLOBAL_TAIL 1
#endif
#ifndef USE_CMX_DMA_CANARY_TRANS
#define USE_CMX_DMA_CANARY_TRANS 0
#endif
#define DMA_MAGIC_STARTED 0xA5B6C7D8
#define DMA_MAGIC_FINISHED 0x12345678
#define DMA_MAGIC_COMPLETE 0x87654321

#define NO_CACHE_ABSOLUTE(ret, adr) {\
    u32 absoluteAdr = (u32) dmaSolveRelAddr((u32)adr, scGetShaveNumber());\
    if ((absoluteAdr >> 24) & 0x70){\
        ret = (void *)((u32)absoluteAdr | 0x08000000);\
    }else{\
        ret = (void *)((u32)absoluteAdr | 0x40000000);\
    }\
}
#if USE_CMX_DMA_CANARY_TRANS
static const  u32 canarySource[1] = {DMA_MAGIC_FINISHED}; // TODO this should live only until we implement per transaction canary
volatile u32 canaryDest[1] = {DMA_MAGIC_COMPLETE};
dmaTransactionList_t dmaCanaryTrans;
static u32* canaryDestNocachePtr;
static dmaTransactionList_t* canaryTransNocachePtr;
#endif
static void dmaSetup()
{
    CmxDmaInitTail(tail);
    //enable the DMA if it wasn't already enabled
    if(!(GET_REG_WORD_VAL(CDMA_CTRL_ADR) & DMA_ENABLE_BIT))
    {
        //Map each channel to a single Link Agent.
        SET_REG_WORD(CDMA_CHTOAGN_ADR, 0X3210);

        //Enable DMA
        SET_REG_WORD(CDMA_CTRL_ADR, DMA_ENABLE_BIT);
    }
}

//Default agent assignment policy
static inline u32 dmaAssignAgent()
{
    u32 assignedAgent;

    assignedAgent = startFrom + (scGetShaveNumber() % usedAgentsNr);

    return assignedAgent;
}

//TODO:reimplement dmaRequesterId in a struct with corresponding fields
dmaRequesterId dmaInitRequesterWithAgent(int priority, int agentToAssign)
{
    if (agentToAssign < 0 || agentToAssign > 3)
        return -1; // invalid id
    dmaRequesterId reqId = (agentToAssign << 6) | (scGetShaveNumber() << 2) | (priority & 0x3);
#if USE_CMX_DMA_CANARY_TRANS
    dmaCreateTransaction(reqId, &dmaCanaryTrans, (u8*)&canarySource[0],
                         (u8*)&canaryDest[0], sizeof(canarySource));
    NO_CACHE_ABSOLUTE(canaryDestNocachePtr, &canaryDest[0]);
    NO_CACHE_ABSOLUTE(canaryTransNocachePtr, &dmaCanaryTrans);
#endif
    dmaSetup();
    return reqId;
}

dmaRequesterId dmaInitRequester(int priority)
{
    u32 agentToAssign = dmaAssignAgent();
    return dmaInitRequesterWithAgent(priority, agentToAssign);
}

void dmaSetUsedAgents(u8 nrOfUsedAgents, u8 startingFrom)
{
    startFrom = startingFrom & 3;
    usedAgentsNr = (startFrom - 1) + nrOfUsedAgents <= 3 ? nrOfUsedAgents : 4 - startFrom;
}

static void dmaNewTransaction(u32 Type,
                              dmaRequesterId ReqId,
                              dmaTransactionList *NewTransaction,
                              u8* Src,
                              u8* Dst,
                              u32 ByteLength,
                              u32 SrcLineWidth,
                              u32 DstLineWidth,
                              s32 SrcStride,
                              s32 DstStride,
                              u32 num_planes,
                              s32 src_plane_stride,
                              s32 dst_plane_stride,
                              u8 BurstLength)
{
    if(NewTransaction)
    {
        SET_REG_WORD(&NewTransaction->linkAddress, NULL);
        GET_REG_WORD_VAL(&NewTransaction->linkAddress);
        NewTransaction->cfgLink.fullCfgRegister = Type                             |
                                                  DMA_PRIORITY(ReqId & 3)          |
                                                  DMA_BURST_LENGTH(0xF)            |
                                                  DMA_TRANSACTION_ID(ReqId >> 2)   |
                                                  DMA_INTERRUPT_TRIGGER(0)         |
                                                  DMA_ENABLE_INTERRUPTS            |
                                                  DMA_SKIP_NR(TASK_SKIP_DESCRIPTOR);
        if(BurstLength)
            NewTransaction->cfgLink.cfgBits.brstLength = BurstLength - 1;
        NewTransaction->src = (u8 *)dmaSolveRelAddr((u32)Src, scGetShaveNumber());
        NewTransaction->dst = (u8 *)dmaSolveRelAddr((u32)Dst, scGetShaveNumber());
        NewTransaction->length = ByteLength;
        NewTransaction->no_planes = num_planes;
        NewTransaction->src_width = SrcLineWidth;
        NewTransaction->src_stride = SrcStride;
        NewTransaction->dst_width = DstLineWidth;
        NewTransaction->dst_stride = DstStride;
        NewTransaction->src_plane_stride = src_plane_stride;
        NewTransaction->dst_plane_stride = dst_plane_stride;
        NewTransaction->agentOff = ReqId >> 6;//offset of assigned link agent
    }
}

dmaTransactionList* dmaCreateTransactionFullOptions(dmaRequesterId ReqId,
                                             dmaTransactionList *NewTransaction,
                                             u8* Src,
                                             u8* Dst,
                                             u32 ByteLength,
                                             u32 SrcLineWidth,
                                             u32 DstLineWidth,
                                             s32 SrcStride,
                                             s32 DstStride)
{
    dmaNewTransaction(DMA_2D_TRANSACTION, ReqId,
                      NewTransaction,
                      Src, Dst, ByteLength,
                      SrcLineWidth, DstLineWidth,
                      SrcStride, DstStride,
                      SWC_CMX_DMA_DEFAULT_NUM_PLANE,
                      SWC_CMX_DMA_DEFAULT_PLANE_STRIDE,
                      SWC_CMX_DMA_DEFAULT_PLANE_STRIDE,
                      0);

    return ((dmaTransactionList *)dmaSolveRelAddr((u32)NewTransaction, scGetShaveNumber()));
}

dmaTransactionList* dmaCreateTransaction(dmaRequesterId ReqId,
                                         dmaTransactionList *NewTransaction,
                                         u8* Src,
                                         u8* Dst,
                                         u32 ByteLength)
{
    dmaNewTransaction(DMA_1D_TRANSACTION, ReqId,
                      NewTransaction,
                      Src, Dst, ByteLength,
                      1, 1, 1, 1,
                      SWC_CMX_DMA_DEFAULT_NUM_PLANE,
                      SWC_CMX_DMA_DEFAULT_PLANE_STRIDE,
                      SWC_CMX_DMA_DEFAULT_PLANE_STRIDE,
                      0);

    return ((dmaTransactionList *)dmaSolveRelAddr((u32)NewTransaction, scGetShaveNumber()));
}

dmaTransactionList* dmaCreateTransactionSrcStride(dmaRequesterId ReqId,
                                                  dmaTransactionList *NewTransaction,
                                                  u8* Src,
                                                  u8* Dst,
                                                  u32 ByteLength,
                                                  u32 LineWidth,
                                                  s32 SrcStride)
{
    if (ByteLength < 16)
        return dmaCreateTransactionFullOptions(ReqId, NewTransaction, Src, Dst, ByteLength, LineWidth, ByteLength, SrcStride, ByteLength);
    else
        return dmaCreateTransactionFullOptions(ReqId, NewTransaction, Src, Dst, ByteLength, LineWidth, 16, SrcStride, 16);
}

dmaTransactionList* dmaCreateTransactionDstStride(dmaRequesterId ReqId,
                                                  dmaTransactionList *NewTransaction,
                                                  u8* Src,
                                                  u8* Dst,
                                                  u32 ByteLength,
                                                  u32 LineWidth,
                                                  s32 DstStride)
{
    if (ByteLength < 16)
        return dmaCreateTransactionFullOptions(ReqId, NewTransaction, Src, Dst, ByteLength, ByteLength, LineWidth, ByteLength, DstStride);
    else
        return dmaCreateTransactionFullOptions(ReqId, NewTransaction, Src, Dst, ByteLength, 16, LineWidth, 16, DstStride);

}

dmaTransactionList* dmaCreate3DTransaction(dmaRequesterId ReqId,
                                           dmaTransactionList *NewTransaction,
                                           u8* Src,
                                           u8* Dst,
                                           u32 ByteLength,
                                           u32 SrcLineWidth,
                                           u32 DstLineWidth,
                                           s32 SrcStride,
                                           s32 DstStride,
                                           u32 NumPlanes,
                                           s32 SrcPlaneStride,
                                           s32 DstPlaneStride) {
  if (NewTransaction && Src && Dst &&
    (NumPlanes >= MIN_NUM_PLANES) && (NumPlanes <= MAX_NUM_PLANES)) {
    if ((SrcLineWidth != 0) || (DstLineWidth != 0) ||
      (SrcStride != 0) || (DstStride != 0) ||
      (SrcPlaneStride != 0) || (DstPlaneStride != 0)) {
      // In 3D mode, the "NumPlanes" field holds the number of planes minus one.
      NumPlanes--;

      dmaNewTransaction(DMA_2D_TRANSACTION, ReqId, NewTransaction,
                        Src, Dst, ByteLength,
                        SrcLineWidth, DstLineWidth,
                        SrcStride, DstStride,
                        NumPlanes, SrcPlaneStride,
                        DstPlaneStride, 0);

      NewTransaction = ((dmaTransactionList *)dmaSolveRelAddr((u32)NewTransaction,
                                                                     scGetShaveNumber()));
    } else {
      NewTransaction = NULL; // Invalid parameter values.
    }
  } else {
    NewTransaction = NULL; // Invalid parameter values.
  }

  return NewTransaction;
}

void dmaLinkTasks(dmaTransactionList* listHead, u32 nmbTasks, ...)
{
    dmaTransactionList *listEnd, *val;
    va_list a_list;
    u32 i;

    va_start(a_list, nmbTasks);

    listEnd = listHead;
    for(i = 0; i < nmbTasks; i++)
    {
        val = va_arg(a_list, dmaTransactionList*);
        listEnd->linkAddress = val;
        listEnd = listEnd->linkAddress;
    }

    va_end(a_list);
}

#define dmaWaitUntilLinkAgentIsntFetchingDescriptors(linkAgent)\
{\
    int is_fetching, req_sm;\
    do\
    {\
        req_sm = 0x7 & (GET_REG_WORD_VAL(CDMA_CHANNEL_DBG_ADR + 4) >> (linkAgent * 4));\
        is_fetching = (req_sm != S_POP_REQ_ISSUE) && (req_sm != S_POP_WAIT_RSP);\
    } while (!is_fetching);\
}

static dmaTransactionList* findLast(dmaTransactionList *dptr)
{
    while(dptr->linkAddress) {dptr = dptr->linkAddress;}
    return dptr;
}


int dmaStartTask(dmaTransactionList* ListPtr, u32* saveTail)
{
    u8 assignedLA;
    int result = 0;

    //get the assigned Link Agent for the current task
    assignedLA = ListPtr->agentOff;
    u32 cdma_link_cfg_adr = CDMA_LINK_CFG0_ADR + 8 * assignedLA;
    u32 cdma_link_cfg_adr_hi_word = cdma_link_cfg_adr + 4;
    u32 channel_mask = 0xF;
    // as a resolution of Bug 26749, we need to stop all channels while adding a task
    scMutexRequest(DMA_MUTEX_COMMON);
    SET_REG_WORD(CDMA_SET_CHGATE_ADR, channel_mask);
    dmaWaitUntilLinkAgentIsntFetchingDescriptors(assignedLA);
    u32 isShadowValid = GET_REG_WORD_VAL(cdma_link_cfg_adr_hi_word) & 1;

    if(!isShadowValid)
    {
        SET_REG_DWORD(cdma_link_cfg_adr, ((u64)((u32)ListPtr)) | START_LCA_BIT); //enable the Link Agent
        result = 1;
    }
    else
    {
        //patch the end of the list in order to be linked to our list (ListPtr)
        dmaTransactionList* lastTask;
        if(!USE_CMX_DMA_GLOBAL_TAIL){
            dmaTransactionList* currentTask =
                    (dmaTransactionList*)GET_REG_WORD_VAL(cdma_link_cfg_adr);
            lastTask = findLast(currentTask);
        }else{
            lastTask = (dmaTransactionList*)*tail[assignedLA];
        }
        SET_REG_WORD(&lastTask->linkAddress, ListPtr);
        GET_REG_WORD_VAL(&lastTask->linkAddress); // read back to ensure that previous write transaction is finished
        result = 2;
    }
    if(USE_CMX_DMA_GLOBAL_TAIL){
        SET_REG_WORD(tail[assignedLA], saveTail);
        GET_REG_WORD_VAL(tail[assignedLA]); // read back to ensure that previous write transaction is finished
    }
    SET_REG_WORD(CDMA_CLR_CHGATE_ADR, channel_mask);
    scMutexRelease(DMA_MUTEX_COMMON);
    return result;
}

int dmaStartListTask(dmaTransactionList* ListPtr)
{

    u8 result = 0;
    dmaTransactionList* ptr;

    u32 *saveTail = NULL;
#if USE_CMX_DMA_CANARY_TRANS
    saveTail = (u32*)&dmaCanaryTrans;
    if (*canaryDestNocachePtr != DMA_MAGIC_COMPLETE)
        while(1); // TODO: what should we do here?
    SET_REG_WORD(canaryDestNocachePtr, DMA_MAGIC_STARTED);
    dmaCanaryTrans.cfgLink.cfgBits.interruptTrigger = ListPtr->cfgLink.cfgBits.id ;
    SET_REG_WORD(&dmaCanaryTrans.linkAddress, NULL);
    dmaCanaryTrans.cfgLink.cfgBits.disableInt = 0;
#endif
    ptr = ListPtr;
    while(ptr)
    {
        ptr->cfgLink.cfgBits.disableInt = 1;
        ptr->cfgLink.cfgBits.interruptTrigger = ListPtr->cfgLink.cfgBits.id ;
        if(ptr->linkAddress == NULL)
        {
#if USE_CMX_DMA_CANARY_TRANS
            SET_REG_WORD(&ptr->linkAddress, canaryTransNocachePtr);
            GET_REG_WORD_VAL(&ptr->linkAddress);
#else
            saveTail = (u32*)ptr;
            ptr->cfgLink.cfgBits.disableInt = 0;
#endif
            break;
        }
        ptr = ptr->linkAddress;
    }

    result = dmaStartTask(ListPtr, saveTail);

    return result;
}
void dmaWaitTask(dmaTransactionList* ListPtr)
{
    UNUSED(ListPtr);

#if USE_CMX_DMA_CANARY_TRANS
    while (GET_REG_WORD_VAL(canaryDestNocachePtr) != DMA_MAGIC_FINISHED)
        ;
    *canaryDestNocachePtr = DMA_MAGIC_COMPLETE;
#else
    while(!(GET_REG_WORD_VAL(CDMA_INT_STATUS_ADR) & (1 << ListPtr->cfgLink.cfgBits.interruptTrigger)));
    SET_REG_WORD(CDMA_INT_RESET_ADR, 1 << ListPtr->cfgLink.cfgBits.interruptTrigger);
#endif
}

int dmaIsTaskFinished(dmaTransactionList* ListPtr)
{
    UNUSED(ListPtr);
#if USE_CMX_DMA_CANARY_TRANS
    if (GET_REG_WORD_VAL(canaryDestNocachePtr) != DMA_MAGIC_FINISHED)
        return 0;
    else{
        *canaryDestNocachePtr = DMA_MAGIC_COMPLETE;
        return 1;
    }
#else
    if((GET_REG_WORD_VAL(CDMA_INT_STATUS_ADR) & (1 << ListPtr->cfgLink.cfgBits.interruptTrigger))){
        SET_REG_WORD(CDMA_INT_RESET_ADR, 1 << ListPtr->cfgLink.cfgBits.interruptTrigger);
        return 1;
    }else{
        return 0;
    }
#endif
}

u32 dmaSolveRelAddr(u32 inAddr, u32 shaveNumber)
{
    u32 window=0;
    u32 windowBase;
    u32 * windowRegPtr = (u32 *)(SHAVE_0_BASE_ADR + (SVU_SLICE_OFFSET * shaveNumber) + SLC_TOP_OFFSET_WIN_A);

    u32 resolved;
    switch (inAddr >> 24)
    {
        case 0x1C: window=0; break;
        case 0x1D: window=1; break;
        case 0x1E: window=2; break;
        case 0x1F: window=3; break;
        default  : return(inAddr);  break;  //absolute address, no translation is to be done
    }
    windowBase = windowRegPtr[window];
    resolved = ((inAddr & 0x00FFFFFF) + windowBase);

    return resolved;
}

dmaTransactionList* dmaCreateTransactionExt(u32 Type,
                                            dmaRequesterId ReqId,
                                            dmaTransactionList *NewTransaction,
                                            u8* Src,
                                            u8* Dst,
                                            u32 ByteLength,
                                            u32 SrcLineWidth,
                                            u32 DstLineWidth,
                                            s32 SrcStride,
                                            s32 DstStride,
                                            u8 BurstLength)
{
    if (BurstLength < DMA_MIN_BL || BurstLength > DMA_MAX_BL)
        return NULL;

    dmaNewTransaction(Type, ReqId,
                      NewTransaction,
                      Src, Dst, ByteLength,
                      SrcLineWidth, DstLineWidth,
                      SrcStride, DstStride,
                      SWC_CMX_DMA_DEFAULT_NUM_PLANE,
                      SWC_CMX_DMA_DEFAULT_PLANE_STRIDE,
                      SWC_CMX_DMA_DEFAULT_PLANE_STRIDE,
                      BurstLength);

    return ((dmaTransactionList *)dmaSolveRelAddr((u32)NewTransaction, scGetShaveNumber()));;
}

#endif // USE_CMX_DMA_NEW_DRIVER

