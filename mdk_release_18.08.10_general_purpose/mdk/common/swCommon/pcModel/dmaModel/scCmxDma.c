///
/// @file scCmxDma.c
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
/// @ingroup scCmxDma
/// @{
/// @brief     CMX DMA PC model
///

#include "scCmxDma.h"
#include <mv_types.h>
#include <svuCommonShave.h>
#include <string.h>

ScCmxDmaTransaction * DmaExecStartTasks(ScCmxDmaTransaction*);
ScCmxDmaTransaction * DmaExecWait(ScCmxDmaTransaction*);
u32 reqCount = 0;
int32_t ScCmxDmaInitialize(ScCmxDmaSetupStruct *config)
{
    //modulus of 3 from reqCount will be used to determine
    //to which Link Agent the tasks created with this ID will be assigned
    //this way we attain a round-robin distribution of link agents
    return 0;
}

static inline ScCmxDmaTransaction *scCmxDmaCreateTransaction(
        ScCmxDmaTransaction *transaction, ScCmxDmaTransactionHnd *handle,
        uint32_t type, uint8_t *src,
        uint8_t *dst, uint32_t length, uint32_t src_line_width,
        uint32_t dst_line_width, int32_t src_stride, int32_t dst_stride,
        uint32_t num_planes, int32_t src_plane_stride,
        int32_t dst_plane_stride, uint8_t burst_length)
{
    //handle->transaction = (ScCmxDmaTransaction*)malloc(sizeof(ScCmxDmaTransaction));

    handle->transaction.link_address = 0;
    handle->transaction.cfg_link.cfg_bits.type = type;
    handle->transaction.cfg_link.cfg_bits.burst_length = burst_length - 1;
    handle->transaction.src = src;
    handle->transaction.dst = dst;
    handle->transaction.length = length;
    handle->transaction.planes_no = num_planes;
    handle->transaction.src_width = src_line_width;
    handle->transaction.src_stride = src_stride;
    handle->transaction.dst_width = dst_line_width;
    handle->transaction.dst_stride = dst_stride;
    handle->transaction.src_plane_stride = src_plane_stride;
    handle->transaction.dst_plane_stride = dst_plane_stride;
    // just use the same skip id for all transactions
    handle->transaction.cfg_link.cfg_bits.skip_nr = 0;
    handle->transaction.cfg_link.cfg_bits.disable_int = 1;

    /*printf("transaction_ s %p -> %p : %d %d %d %d\n", handle->transaction->src, handle->transaction->dst,
            handle->transaction->src_width, handle->transaction->src_stride,
            handle->transaction->dst_width, handle->transaction->dst_stride  );*/

    handle->transaction.completed = 0;

    return &(handle->transaction);
}


int32_t ScCmxDmaCreateTransaction(ScCmxDmaTransactionHnd *handle,
        ScCmxDmaTransaction *transaction,
        uint8_t *src, uint8_t *dst, uint32_t size)
{
    ScCmxDmaTransaction* transaction_ = scCmxDmaCreateTransaction(transaction, handle,
                DRV_CMX_DMA_1D_TRANSACTION, src,
                dst, size, 1, 1, 1, 1,
                SC_CMX_DMA_DEFAULT_NUM_PLANE,
                SC_CMX_DMA_DEFAULT_PLANE_STRIDE,
                SC_CMX_DMA_DEFAULT_PLANE_STRIDE,
                SC_CMX_DMA_DEFAULT_BURST_SIZE);
}

int32_t ScCmxDmaLinkTransactions(ScCmxDmaTransactionHnd* listHead, u32 nmbTasks, ...)
{
    ScCmxDmaTransaction *listEnd, *val;
    va_list a_list;
    u32 i;

    va_start(a_list, nmbTasks);

    listEnd = listHead;
    for(i = 0; i < nmbTasks; i++)
    {
        val = va_arg(a_list, ScCmxDmaTransaction*);
        listEnd->link_address = val;
        listEnd = (ScCmxDmaTransaction*)listEnd->link_address;
    }

    va_end(a_list);
    return 0;
}

int32_t ScCmxDmaStartTransfer(ScCmxDmaTransactionHnd* handle)
{
    ScCmxDmaTransaction* ptr;
    ptr = &(handle->transaction);

    while(ptr)
        ptr = DmaExecStartTasks(ptr);

    return 0;
}



/*int dmaStartListTask(ScCmxDmaTransactionHnd* handle)
{
    int result = 0;

    result = dmaStartTask(handle->transaction, NULL, NULL);

    return result;
}*/



int32_t ScCmxDmaWaitTransaction(ScCmxDmaTransactionHnd* handle)
{
    ScCmxDmaTransaction* ptr;
    ptr = &(handle->transaction);
    while(ptr)
        ptr= DmaExecWait(ptr);

    return 0;
}

int dmaIsTaskFinished(ScCmxDmaTransactionHnd* handle)
{
    return handle->transaction.completed;
}

ScCmxDmaTransaction* DmaExecStartTasks(ScCmxDmaTransaction* transaction)
{
    u32 j;
    u8* src_adr;
    u8* dst_adr; //source and destination address to be manipulated in order the starting addresses to remain intact

    src_adr = (u8*)transaction->src;
    dst_adr = (u8*)transaction->dst;
    transaction->completed = 0;
    if(transaction->src_stride == 1 && transaction->dst_stride == 1)
        memset(transaction->dst, 0xFF, transaction->length);
    else
    {
        size_t src_line_remainder; //pixels left to be added to the source address in the respective line of the source block, being ,in fact, the remainder of the division between src_width and dst_width
        size_t dst_line_remainder; //pixels left to be written to destination after the source address jump to the next line of the source block
        //when src_width < dst_width the meanings are inversed

        if(transaction->src_stride > 1 && transaction->dst_stride > 1)
        {
            if(transaction->src_width<transaction->dst_width)
            {
                for(j=0; j<transaction->length/transaction->src_width; j++)
                {
                    //if we are at the start or the current destination address + source width is within the borders of destination width for the respective line line
                    while(dst_adr==((u8*)transaction->dst + transaction->dst_stride*j) || (dst_adr + transaction->src_width) <= ((u8*)transaction->dst + transaction->dst_stride*j + transaction->dst_width))
                    {
                        memset(dst_adr, 0xFF, transaction->src_width);
                        dst_adr+=transaction->src_width;
                        src_adr+=transaction->src_stride;
                    }
                    if((dst_adr + transaction->src_width) != ((u8*)transaction->dst + transaction->dst_stride*j + transaction->dst_width))
                    {
                        src_line_remainder = transaction->dst_width - (dst_adr-((u8*)transaction->dst + transaction->dst_stride*j));
                        dst_line_remainder = transaction->src_width - src_line_remainder;
                        memset(dst_adr, 0xFF, src_line_remainder);
                        dst_adr += src_line_remainder;
                        dst_adr += transaction->dst_stride - transaction->dst_width;
                        src_adr += src_line_remainder;
                        memset(dst_adr, 0xFF, dst_line_remainder);
                        dst_adr += dst_line_remainder;
                        src_adr -= src_line_remainder;
                        src_adr += transaction->src_stride;

                    }
                }
            }
            else
            {
                for(j=0; j<transaction->length/transaction->src_width; j++)
                {
                    while(src_adr==((u8*)transaction->src + transaction->src_stride*j) || (src_adr+transaction->dst_width)<=((u8*)transaction->src + transaction->src_stride*j + transaction->src_width))
                    {
                        memset(dst_adr, 0xFF, transaction->dst_width);
                        src_adr+=transaction->dst_width;
                        dst_adr+=transaction->dst_stride;
                    }
                    if((src_adr+transaction->dst_width) != ((u8*)transaction->src + transaction->src_stride*j + transaction->src_width))
                    {
                        src_line_remainder = transaction->src_width - (src_adr-((u8*)transaction->src + transaction->src_stride*j));
                        dst_line_remainder = transaction->dst_width - src_line_remainder;
                        memset(dst_adr, 0xFF, src_line_remainder);
                        src_adr += transaction->src_stride-transaction->src_width;

                        if (src_adr!=((u8*)transaction->src + transaction->src_stride*(j+1)))
                        {
                            src_adr+=src_line_remainder;
                            dst_adr+=src_line_remainder;
                            memset(dst_adr, 0xFF, dst_line_remainder);
                            src_adr += dst_line_remainder;
                            dst_adr-=src_line_remainder;
                            dst_adr+=transaction->dst_stride;
                        }
                        else
                            continue;
                    }
                }
            }
        }
        else
        {
            if(transaction->src_stride > 1 && transaction->dst_stride == 1)
            {
                memset(dst_adr, 0xFF, transaction->src_width);
            }
            if (transaction->src_stride == 1 && transaction->dst_stride > 1)
            {
                memset(dst_adr, 0xFF, transaction->dst_width);
            }
        }
    }
    transaction = (ScCmxDmaTransaction*)transaction->link_address;
    return transaction;
}


ScCmxDmaTransaction* DmaExecWait(ScCmxDmaTransaction* transaction)
{

    u32 j;
    if(transaction->src_stride==1 && transaction->dst_stride==1)
    {
        memcpy(transaction->dst, transaction->src, transaction->length);
        transaction->completed = 1;
    }
    else
    {
        u8* src_adr=(u8*)transaction->src;
        u8* dst_adr=(u8*)transaction->dst;
        size_t src_line_remainder;
        size_t dst_line_remainder;

        if(transaction->src_stride > 1 && transaction->dst_stride > 1)
        {
            if(transaction->src_width < transaction->dst_width)
            {
                for(j=0; j<transaction->length/transaction->src_width; j++)
                {
                    while(dst_adr==((u8*)transaction->dst + transaction->dst_stride*j) || (dst_adr + transaction->src_width) <= ((u8*)transaction->dst + transaction->dst_stride*j + transaction->dst_width))
                    {
                        memcpy(dst_adr, src_adr, transaction->src_width);
                        dst_adr+=transaction->src_width;
                        src_adr+=transaction->src_stride;
                    }
                    if((dst_adr + transaction->src_width) != ((u8*)transaction->dst + transaction->dst_stride*j + transaction->dst_width))
                    {
                        src_line_remainder = transaction->dst_width - (dst_adr-((u8*)transaction->dst + transaction->dst_stride*j));
                        dst_line_remainder = transaction->src_width - src_line_remainder;
                        memcpy(dst_adr, src_adr, src_line_remainder);
                        dst_adr += src_line_remainder;
                        dst_adr += transaction->dst_stride - transaction->dst_width;
                        src_adr += src_line_remainder;
                        memcpy(dst_adr, src_adr, dst_line_remainder);
                        dst_adr += dst_line_remainder;
                        src_adr -= src_line_remainder;
                        src_adr += transaction->src_stride;
                    }
                }
                transaction->completed = 1;
            }
            else
            {
                for( j=0; j<transaction->length/transaction->src_width; j++)
                {
                    while(src_adr==((u8*)transaction->src + transaction->src_stride*j) || (src_adr+transaction->dst_width) <= ((u8*)transaction->src + transaction->src_stride*j + transaction->src_width))
                    {
                        memcpy(dst_adr, src_adr, transaction->dst_width);
                        src_adr+=transaction->dst_width;
                        dst_adr+=transaction->dst_stride;
                    }
                    if((src_adr+transaction->dst_width) != ((u8*)transaction->src + transaction->src_stride*j + transaction->src_width))
                    {
                        src_line_remainder = transaction->src_width - (src_adr-((u8*)transaction->src + transaction->src_stride*j));
                        dst_line_remainder = transaction->dst_width - (transaction->src_width - (src_adr-((u8*)transaction->src + transaction->src_stride*j)));
                        memcpy(dst_adr, src_adr, dst_line_remainder);
                        src_adr += transaction->src_stride-transaction->src_width;
                        if (src_adr!=((u8*)transaction->src + transaction->src_stride*(j+1)))
                        {
                            src_adr+=src_line_remainder;
                            dst_adr+=src_line_remainder;
                            memcpy(dst_adr, src_adr, dst_line_remainder);
                            src_adr += dst_line_remainder;
                            dst_adr-=src_line_remainder;
                            dst_adr+=transaction->dst_stride;
                        }
                        else
                            continue;
                    }
                }
                transaction->completed = 1;
            }
        }
        else
        {
            if(transaction->src_stride > 1 && transaction->dst_stride == 1)
            {
                memcpy (dst_adr, src_adr, transaction->src_width);
                transaction->completed = 1;
            }
            if (transaction->src_stride == 1 && transaction->dst_stride > 1)
            {
                memcpy(dst_adr, src_adr, transaction->dst_width);
                transaction->completed = 1;
            }
        }
    }
    transaction = (ScCmxDmaTransaction*)transaction->link_address;
    return transaction;
}
