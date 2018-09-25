
#include <stdio.h>
#include <swcCdma.h>
#include <svuCommonShave.h>

void *handle;
dmaHandle shvHandle_old;

dmaRequesterId dmaInitRequester(int priority)
{
    shvHandle_old.svu_nr = priority;
    shvHandle_old.lastTask = 0;

    handle = &shvHandle_old;
    return 1;
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
    return 0;
}

dmaTransactionList* dmaCreateTransaction(dmaRequesterId ReqId,
                                         dmaTransactionList *NewTransaction,
                                         u8* Src,
                                         u8* Dst,
                                         u32 ByteLength)
{
    dmaHandle *myHandle = (dmaHandle*)handle;

    if(myHandle->lastTask < MYRIAD1_MAX_NUMBER_OF_TASKS)
    {
		scDmaSetup((swcDmaTask_t)myHandle->lastTask, Src, Dst, ByteLength);
        myHandle->lastTask++;
        return 0;
    }
    return 0;

}

dmaTransactionList* dmaCreateTransactionSrcStride(dmaRequesterId ReqId,
                                                  dmaTransactionList *NewTransaction,
                                                  u8* Src,
                                                  u8* Dst,
                                                  u32 ByteLength,
                                                  u32 LineWidth,
                                                  s32 SrcStride)
{
    dmaHandle *myHandle = (dmaHandle*)handle;

    if(myHandle->lastTask < MYRIAD1_MAX_NUMBER_OF_TASKS)
    {


        if(ByteLength>0)
        {
            scDmaSetupStrideSrc((swcDmaTask_t)myHandle->lastTask,
                                             Src,
                                             Dst,
                                             ByteLength,
                                             LineWidth,
                                             SrcStride);


            myHandle->lastTask++;

        }
    }
    return 0;
}

dmaTransactionList* dmaCreateTransactionDstStride(dmaRequesterId ReqId,
                                                  dmaTransactionList *NewTransaction,
                                                  u8* Src,
                                                  u8* Dst,
                                                  u32 ByteLength,
                                                  u32 LineWidth,
                                                  s32 DstStride)
{
    dmaHandle *myHandle = (dmaHandle*)handle;

    if(myHandle->lastTask < MYRIAD1_MAX_NUMBER_OF_TASKS)
    {

        if(ByteLength>0)
        {


           scDmaSetupStrideDst((swcDmaTask_t)myHandle->lastTask,
                                                        Src,
                                                        Dst,
                                                        ByteLength,
                                                        LineWidth,
                                                        DstStride);

            myHandle->lastTask++;

        }
    }
    return 0;
}

void dmaLinkTasks(dmaTransactionList* listHead, u32 nmbTasks, ...)
{

}

int dmaStartTask(dmaTransactionList* ListPtr, dmaIrqHandler function, void *userContext)
{
    dmaHandle *myHandle = (dmaHandle*)handle;

    scDmaStart((swcDmaTask_t)myHandle->lastTask-1);
    return 0;
}


int dmaStartListTask(dmaTransactionList* ListPtr)
{

    int result = dmaStartTask(handle, NULL, NULL);

    return result;
}

int dmaStartTaskAsync(dmaTransactionList* ListPtr, dmaIrqHandler function, void *userContext)
{


    return 0;
}

void dmaWaitTask(dmaTransactionList* ListPtr)
{
    dmaHandle *myHandle = (dmaHandle*)handle;

    scDmaWaitFinished();
    myHandle->lastTask=0;
}

int dmaIsTaskFinished(dmaTransactionList* ListPtr)
{
   dmaHandle *myHandle = (dmaHandle*)handle;
    if (myHandle->lastTask == 0)
        return 1;
    else
        return 0;
}
