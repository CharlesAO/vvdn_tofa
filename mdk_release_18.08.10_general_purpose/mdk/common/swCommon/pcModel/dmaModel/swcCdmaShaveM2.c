/*
 * Cdma.c
 for PC
*/

#include "swcCdma.h"
#include <mv_types.h>
#include <svuCommonShave.h>
#include <string.h>

dmaTransactionList * DmaExecStartTasks(dmaTransactionList*);
dmaTransactionList * DmaExecWait(dmaTransactionList*);
u32 reqCount = 0;
dmaRequesterId dmaInitRequester(int priority)
{
    //modulus of 3 from reqCount will be used to determine
    //to which Link Agent the tasks created with this ID will be assigned
    //this way we attain a round-robin distribution of link agents
    return (reqCount++ << 2) | (priority & 0x3);
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
    if(NewTransaction)
    {
        NewTransaction->linkAddress = 0;
        NewTransaction->cfgLink.fullCfgRegister = DMA_2D_TRANSACTION                        |
                                                  DMA_PRIORITY(ReqId & 3)                   |
                                                  DMA_BURST_LENGTH(0xF)                     |
                                                  DMA_TRANSACTION_ID(ReqId >> 2)            |
                                                  DMA_INTERRUPT_TRIGGER(0)                  |
                                                  DMA_ENABLE_INTERRUPTS                     |
                                                  DMA_SKIP_NR(0);
        NewTransaction->src = Src;
        NewTransaction->dst = Dst;
        NewTransaction->length = ByteLength;
        NewTransaction->no_planes = 0;
        NewTransaction->src_width = SrcLineWidth;
        NewTransaction->src_stride = SrcStride;
        NewTransaction->dst_width = DstLineWidth;
        NewTransaction->dst_stride = DstStride;
        NewTransaction->completed = 0;
    }

    return NewTransaction;
}

dmaTransactionList* dmaCreateTransaction(dmaRequesterId ReqId,
                                         dmaTransactionList *NewTransaction,
                                         u8* Src,
                                         u8* Dst,
                                         u32 ByteLength)
{
    if(NewTransaction)
    {
        NewTransaction->linkAddress = 0;
        NewTransaction->cfgLink.fullCfgRegister = DMA_1D_TRANSACTION                        |
                                                  DMA_PRIORITY(ReqId & 3)                   |
                                                  DMA_BURST_LENGTH(0xF)                     |
                                                  DMA_TRANSACTION_ID(ReqId >> 2)            |
                                                  DMA_INTERRUPT_TRIGGER(0)                  |
                                                  DMA_ENABLE_INTERRUPTS                     |
                                                  DMA_SKIP_NR(0);
        NewTransaction->src = Src;
        NewTransaction->dst = Dst;
        NewTransaction->length = ByteLength;
        NewTransaction->no_planes = 0;
        NewTransaction->src_width = 1;
        NewTransaction->src_stride = 1;
        NewTransaction->dst_width = 1;
        NewTransaction->dst_stride = 1;
        NewTransaction->completed = 0;
    }

    return NewTransaction;
}

dmaTransactionList* dmaCreateTransactionSrcStride(dmaRequesterId ReqId,
                                                  dmaTransactionList *NewTransaction,
                                                  u8* Src,
                                                  u8* Dst,
                                                  u32 ByteLength,
                                                  u32 LineWidth,
                                                  s32 SrcStride)
{
    return dmaCreateTransactionFullOptions(ReqId, NewTransaction, Src, Dst, ByteLength, LineWidth, 1, SrcStride, 1);
}

dmaTransactionList* dmaCreateTransactionDstStride(dmaRequesterId ReqId,
                                                  dmaTransactionList *NewTransaction,
                                                  u8* Src,
                                                  u8* Dst,
                                                  u32 ByteLength,
                                                  u32 LineWidth,
                                                  s32 DstStride)
{
    return dmaCreateTransactionFullOptions(ReqId, NewTransaction, Src, Dst, ByteLength, 1, LineWidth, 1, DstStride);
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
        listEnd = (dmaTransactionList*)listEnd->linkAddress;
    }

    va_end(a_list);
}

int dmaStartTask(dmaTransactionList* ListPtr, dmaIrqHandler function, void *userContext)
{
    dmaTransactionList* ptr;
    ptr = ListPtr;

    while(ptr)
        ptr = DmaExecStartTasks(ptr);

    return 1;
}



int dmaStartListTask(dmaTransactionList* ListPtr)
{
    int result = 0;

    result = dmaStartTask(ListPtr, NULL, NULL);

    return result;
}



void dmaWaitTask(dmaTransactionList* ListPtr)
{
    dmaTransactionList* ptr;
    ptr = ListPtr;
    while(ptr)
        ptr= DmaExecWait(ptr);
}

int dmaIsTaskFinished(dmaTransactionList* ListPtr)
{
    return ListPtr->completed;
}


void dmaCopyBlocking(dmaRequesterId ReqId, dmaTransactionList *NewTransaction, u8* Src, u8* Dst, u32 ByteLength)
{
    dmaTransactionList *ref;

    ref = dmaCreateTransaction(ReqId, NewTransaction, Src, Dst, ByteLength);
    dmaLinkTasks(ref,0);
    dmaStartListTask(ref);
    dmaWaitTask(ref);
    dmaIsTaskFinished(ref);
}


dmaTransactionList* DmaExecStartTasks(dmaTransactionList* dmaTask)
{
    u32 j;
    u8* src_adr;
    u8* dst_adr; //source and destination address to be manipulated in order the starting addresses to remain intact

    src_adr = (u8*)dmaTask->src;
    dst_adr = (u8*)dmaTask->dst;
    dmaTask->completed = 0;
    if(dmaTask->src_stride == 1 && dmaTask->dst_stride == 1)
        memset(dmaTask->dst, 0xFF, dmaTask->length);
    else
    {
        size_t src_line_remainder; //pixels left to be added to the source address in the respective line of the source block, being ,in fact, the remainder of the division between src_width and dst_width
        size_t dst_line_remainder; //pixels left to be written to destination after the source address jump to the next line of the source block
        //when src_width < dst_width the meanings are inversed

        if(dmaTask->src_stride > 1 && dmaTask->dst_stride > 1)
        {
            if(dmaTask->src_width<dmaTask->dst_width)
            {
                for(j=0; j<dmaTask->length/dmaTask->src_width; j++)
                {
                    //if we are at the start or the current destination address + source width is within the borders of destination width for the respective line line
                    while(dst_adr==((u8*)dmaTask->dst + dmaTask->dst_stride*j) || (dst_adr + dmaTask->src_width) <= ((u8*)dmaTask->dst + dmaTask->dst_stride*j + dmaTask->dst_width))
                    {
                        memset(dst_adr, 0xFF, dmaTask->src_width);
                        dst_adr+=dmaTask->src_width;
                        src_adr+=dmaTask->src_stride;
                    }
                    if((dst_adr + dmaTask->src_width) != ((u8*)dmaTask->dst + dmaTask->dst_stride*j + dmaTask->dst_width))
                    {
                        src_line_remainder = dmaTask->dst_width - (dst_adr-((u8*)dmaTask->dst + dmaTask->dst_stride*j));
                        dst_line_remainder = dmaTask->src_width - src_line_remainder;
                        memset(dst_adr, 0xFF, src_line_remainder);
                        dst_adr += src_line_remainder;
                        dst_adr += dmaTask->dst_stride - dmaTask->dst_width;
                        src_adr += src_line_remainder;
                        memset(dst_adr, 0xFF, dst_line_remainder);
                        dst_adr += dst_line_remainder;
                        src_adr -= src_line_remainder;
                        src_adr += dmaTask->src_stride;

                    }
                }
            }
            else
            {
                for(j=0; j<dmaTask->length/dmaTask->src_width; j++)
                {
                    while(src_adr==((u8*)dmaTask->src + dmaTask->src_stride*j) || (src_adr+dmaTask->dst_width)<=((u8*)dmaTask->src + dmaTask->src_stride*j + dmaTask->src_width))
                    {
                        memset(dst_adr, 0xFF, dmaTask->dst_width);
                        src_adr+=dmaTask->dst_width;
                        dst_adr+=dmaTask->dst_stride;
                    }
                    if((src_adr+dmaTask->dst_width) != ((u8*)dmaTask->src + dmaTask->src_stride*j + dmaTask->src_width))
                    {
                        src_line_remainder = dmaTask->src_width - (src_adr-((u8*)dmaTask->src + dmaTask->src_stride*j));
                        dst_line_remainder = dmaTask->dst_width - src_line_remainder;
                        memset(dst_adr, 0xFF, src_line_remainder);
                        src_adr += dmaTask->src_stride-dmaTask->src_width;

                        if (src_adr!=((u8*)dmaTask->src + dmaTask->src_stride*(j+1)))
                        {
                            src_adr+=src_line_remainder;
                            dst_adr+=src_line_remainder;
                            memset(dst_adr, 0xFF, dst_line_remainder);
                            src_adr += dst_line_remainder;
                            dst_adr-=src_line_remainder;
                            dst_adr+=dmaTask->dst_stride;
                        }
                        else
                            continue;
                    }
                }
            }
        }
        else
        {
            if(dmaTask->src_stride > 1 && dmaTask->dst_stride == 1)
            {
                memset(dst_adr, 0xFF, dmaTask->src_width);
            }
            if (dmaTask->src_stride == 1 && dmaTask->dst_stride > 1)
            {
                memset(dst_adr, 0xFF, dmaTask->dst_width);
            }
        }
    }
    dmaTask = (dmaTransactionList*)dmaTask->linkAddress;
    return dmaTask;
}


dmaTransactionList* DmaExecWait(dmaTransactionList* dmaTask)
{

    u32 j;
    if(dmaTask->src_stride==1 && dmaTask->dst_stride==1)
    {
        memcpy(dmaTask->dst, dmaTask->src, dmaTask->length);
        dmaTask->completed = 1;
    }
    else
    {
        u8* src_adr=(u8*)dmaTask->src;
        u8* dst_adr=(u8*)dmaTask->dst;
        size_t src_line_remainder;
        size_t dst_line_remainder;

        if(dmaTask->src_stride > 1 && dmaTask->dst_stride > 1)
        {
            if(dmaTask->src_width < dmaTask->dst_width)
            {
                for(j=0; j<dmaTask->length/dmaTask->src_width; j++)
                {
                    while(dst_adr==((u8*)dmaTask->dst + dmaTask->dst_stride*j) || (dst_adr + dmaTask->src_width) <= ((u8*)dmaTask->dst + dmaTask->dst_stride*j + dmaTask->dst_width))
                    {
                        memcpy(dst_adr, src_adr, dmaTask->src_width);
                        dst_adr+=dmaTask->src_width;
                        src_adr+=dmaTask->src_stride;
                    }
                    if((dst_adr + dmaTask->src_width) != ((u8*)dmaTask->dst + dmaTask->dst_stride*j + dmaTask->dst_width))
                    {
                        src_line_remainder = dmaTask->dst_width - (dst_adr-((u8*)dmaTask->dst + dmaTask->dst_stride*j));
                        dst_line_remainder = dmaTask->src_width - src_line_remainder;
                        memcpy(dst_adr, src_adr, src_line_remainder);
                        dst_adr += src_line_remainder;
                        dst_adr += dmaTask->dst_stride - dmaTask->dst_width;
                        src_adr += src_line_remainder;
                        memcpy(dst_adr, src_adr, dst_line_remainder);
                        dst_adr += dst_line_remainder;
                        src_adr -= src_line_remainder;
                        src_adr += dmaTask->src_stride;
                    }
                }
                dmaTask->completed = 1;
            }
            else
            {
                for( j=0; j<dmaTask->length/dmaTask->src_width; j++)
                {
                    while(src_adr==((u8*)dmaTask->src + dmaTask->src_stride*j) || (src_adr+dmaTask->dst_width) <= ((u8*)dmaTask->src + dmaTask->src_stride*j + dmaTask->src_width))
                    {
                        memcpy(dst_adr, src_adr, dmaTask->dst_width);
                        src_adr+=dmaTask->dst_width;
                        dst_adr+=dmaTask->dst_stride;
                    }
                    if((src_adr+dmaTask->dst_width) != ((u8*)dmaTask->src + dmaTask->src_stride*j + dmaTask->src_width))
                    {
                        src_line_remainder = dmaTask->src_width - (src_adr-((u8*)dmaTask->src + dmaTask->src_stride*j));
                        dst_line_remainder = dmaTask->dst_width - (dmaTask->src_width - (src_adr-((u8*)dmaTask->src + dmaTask->src_stride*j)));
                        memcpy(dst_adr, src_adr, dst_line_remainder);
                        src_adr += dmaTask->src_stride-dmaTask->src_width;
                        if (src_adr!=((u8*)dmaTask->src + dmaTask->src_stride*(j+1)))
                        {
                            src_adr+=src_line_remainder;
                            dst_adr+=src_line_remainder;
                            memcpy(dst_adr, src_adr, dst_line_remainder);
                            src_adr += dst_line_remainder;
                            dst_adr-=src_line_remainder;
                            dst_adr+=dmaTask->dst_stride;
                        }
                        else
                            continue;
                    }
                }
                dmaTask->completed = 1;
            }
        }
        else
        {
            if(dmaTask->src_stride > 1 && dmaTask->dst_stride == 1)
            {
                memcpy (dst_adr, src_adr, dmaTask->src_width);
                dmaTask->completed = 1;
            }
            if (dmaTask->src_stride == 1 && dmaTask->dst_stride > 1)
            {
                memcpy(dst_adr, src_adr, dmaTask->dst_width);
                dmaTask->completed = 1;
            }
        }
    }
    dmaTask = (dmaTransactionList*)dmaTask->linkAddress;
    return dmaTask;
}
