///
/// @file DrvCmxDma.c
/// 
/// 
/// @date   Mar 3, 2014
/// @ingroup DrvCmxDma
/// @{
///

#ifndef USE_CMX_DMA_NEW_DRIVER

#include "DrvCmxDma.h"
#include "DrvCmxDmaArch.h"
#ifndef USE_CMX_DMA_GLOBAL_TAIL
#define USE_CMX_DMA_GLOBAL_TAIL 1
#endif
int usedAgentsNr = 2;//Number of used Link Agents(LA) by current processor
int startFrom = DMA_AGENT1;//The first LA to use
int intOffset = 13;//Bit position from INT_STATUS register used for task status

//Non-static because is also used in RTEMS driver.
u8 currentlyExecuting[NR_OF_LINK_AGENTS] = {0};//status variables used to prevent starting a task on specific LA before the previous one has finished.
static dmaQueueElement_t waitingQueue[NR_OF_LINK_AGENTS][WAITING_QUEUE_SIZE];//waiting queue for tasks that cannot be started at that moment
static u8 waitingQueueCounters[NR_OF_LINK_AGENTS] = {0};//counters for number of elements added in queue for each LA
static u8 waitingQueueReadBackCounters[NR_OF_LINK_AGENTS] = {0};//counters that increment when a task for specific LA has finished,

u32 dmaMutexes[4] = {DMA_MUTEX0,
                     DMA_MUTEX1,
                     DMA_MUTEX2,
                     DMA_MUTEX3};

u32 **tail[4];

drvStat_t cmxDmaStatus;

int DrvCmxDmaInitDefault()
{
    if(cmxDmaStatus.status == DRV_CDMA_UNINITIALIZED)
    {
        cmxDmaStatus.status = DRV_CDMA_INITIALIZED;

        if(swcWhoAmI() == PROCESS_LEON_OS)
            DrvCmxDmaSetup(intOffset, usedAgentsNr, startFrom);
        else
            DrvCmxDmaSetup(15, 1, DMA_AGENT3);//LRT will use in default configuration only link agent 3

        //Configure CMXDMA interrupt
        DrvIcbSetupIrq(IRQ_CMXDMA, DEFAULT_CDMA_INT_LEVEL, POS_EDGE_INT, DrvCmxDmaIrqHandlerFunc);

        return CMX_DMA_DRV_SUCCESS;
    }
    else return CMX_DMA_DRV_ALREADY_INITIALIZED;
}

int DrvCmxDmaInit(u32 intLevel, u32 interruptOffset, u32 nrOfAgents, u32 startingFrom)
{
    if(cmxDmaStatus.status == DRV_CDMA_UNINITIALIZED)
    {
        cmxDmaStatus.status = DRV_CDMA_INITIALIZED;

        DrvCmxDmaSetup(interruptOffset, nrOfAgents, startingFrom);

        //Configure CMXDMA interrupt
        DrvIcbSetupIrq(IRQ_CMXDMA, intLevel, POS_EDGE_INT, DrvCmxDmaIrqHandlerFunc);

        return CMX_DMA_DRV_SUCCESS;
    }
    else return CMX_DMA_DRV_ALREADY_INITIALIZED;
}

static u32 reqCount = 0;

dmaRequesterId DrvCmxDmaInitRequesterOnAgent(int priority, int agent)
{
    //Some checks to make sure that wrong agent will not be used
    //If agent is in range of allowed to use agents, then agent is offseted to 0.
    //startFrom value will be added back when tasks with this id will be started.
    if((agent >= startFrom) && (agent <= startFrom + usedAgentsNr - 1)) agent = agent - startFrom;
    else
    {
        //If agent to be used exceeds allowed range, bring its value to upper of lower value of interval.
        if(agent < startFrom) agent = 0;
        if(agent > startFrom + usedAgentsNr - 1) agent = usedAgentsNr - 1;
    }

    //The 'agent' parameter will be used to determine on what agent to start transfers
    return (reqCount++ << 4) | ((agent & 0x3) << 2) | (priority & 0x3);
}

dmaRequesterId DrvCmxDmaInitRequester(int priority)
{
    //modulus of 3 from reqCount will be used to determine
    //to which Link Agent the tasks created with this ID will be assigned
    //this way we attain a round-robin distribution of link agents
    //Set-up the number of link agents used an
    return (reqCount++ << 2) | (priority & 0x3);
}

//Agent assignment policy
static inline u32 dmaAssignAgent(dmaTransactionList *NewTransaction)
{
    return (NewTransaction->cfgLink.cfgBits.id % usedAgentsNr);
}

static void drvCmxDmaNewTransaction(u32 Type,
                                    dmaRequesterId ReqId,
                                    dmaTransactionList *NewTransaction,
                                    u8* Src,
                                    u8* Dst,
                                    u32 ByteLength,
                                    u32 SrcLineWidth,
                                    u32 DstLineWidth,
                                    s32 SrcStride,
                                    s32 DstStride,
                                    u32 NumPlanes,
                                    u32 SrcPlaneStride,
                                    u32 DstPlaneStride,
                                    u8 BurstLength)
{
    if(NewTransaction)
    {
        NewTransaction->linkAddress = 0;
        NewTransaction->cfgLink.fullCfgRegister = Type | DMA_CFG_SET(ReqId);
        if (BurstLength)
            NewTransaction->cfgLink.cfgBits.brstLength = BurstLength - 1;
        NewTransaction->src = Src;
        NewTransaction->dst = Dst;
        NewTransaction->length = ByteLength;
        NewTransaction->no_planes = 0;
        NewTransaction->src_width = SrcLineWidth;
        NewTransaction->src_stride = SrcStride;
        NewTransaction->dst_width = DstLineWidth;
        NewTransaction->dst_stride = DstStride;
        NewTransaction->no_planes = NumPlanes;
        NewTransaction->src_plane_stride = SrcPlaneStride;
        NewTransaction->dst_plane_stride = DstPlaneStride;
        NewTransaction->agentOff = dmaAssignAgent(NewTransaction);
    }
}

//All new created transactions need at least:
void DrvCmxDmaTransactionBriefInit(dmaRequesterId ReqId,
                                   u32 Type, //DMA_1D_TRANSACTION or DMA_2D_TRANSACTION
                                   dmaTransactionList *NewTransaction)
{
    NewTransaction->linkAddress             = 0;
    NewTransaction->cfgLink.fullCfgRegister = Type | DMA_CFG_SET(ReqId);
    NewTransaction->agentOff                = dmaAssignAgent(NewTransaction);
}

dmaTransactionList* DrvCmxDmaCreateTransactionFullOptions(dmaRequesterId ReqId,
                                             dmaTransactionList *NewTransaction,
                                             u8* Src,
                                             u8* Dst,
                                             u32 ByteLength,
                                             u32 SrcLineWidth,
                                             u32 DstLineWidth,
                                             s32 SrcStride,
                                             s32 DstStride)
{
    drvCmxDmaNewTransaction(DMA_2D_TRANSACTION, ReqId, NewTransaction,
                            Src, Dst, ByteLength,
                            SrcLineWidth, DstLineWidth,
                            SrcStride, DstStride,
                            CMX_DMA_DEFAULT_NUM_PLANE,
                            CMX_DMA_DEFAULT_PLANE_STRIDE,
                            CMX_DMA_DEFAULT_PLANE_STRIDE,
                            0);

    return NewTransaction;
}

dmaTransactionList* DrvCmxDmaCreateTransaction(dmaRequesterId ReqId,
                                         dmaTransactionList *NewTransaction,
                                         u8* Src,
                                         u8* Dst,
                                         u32 ByteLength)
{
    drvCmxDmaNewTransaction(DMA_1D_TRANSACTION, ReqId, NewTransaction,
                            Src, Dst, ByteLength, 1, 1, 1, 1,
                            CMX_DMA_DEFAULT_NUM_PLANE,
                            CMX_DMA_DEFAULT_PLANE_STRIDE,
                            CMX_DMA_DEFAULT_PLANE_STRIDE,
                            0);

    return NewTransaction;
}

dmaTransactionList* DrvCmxDmaCreateTransactionSrcStride(dmaRequesterId ReqId,
                                                  dmaTransactionList *NewTransaction,
                                                  u8* Src,
                                                  u8* Dst,
                                                  u32 ByteLength,
                                                  u32 LineWidth,
                                                  s32 SrcStride)
{
    if (ByteLength < 16)
        return DrvCmxDmaCreateTransactionFullOptions(ReqId, NewTransaction, Src, Dst, ByteLength, LineWidth, ByteLength, SrcStride, ByteLength);
    else
        return DrvCmxDmaCreateTransactionFullOptions(ReqId, NewTransaction, Src, Dst, ByteLength, LineWidth, 16, SrcStride, 16);
}

dmaTransactionList* DrvCmxDmaCreateTransactionDstStride(dmaRequesterId ReqId,
                                                  dmaTransactionList *NewTransaction,
                                                  u8* Src,
                                                  u8* Dst,
                                                  u32 ByteLength,
                                                  u32 LineWidth,
                                                  s32 DstStride)
{
    if(ByteLength < 16)
        return DrvCmxDmaCreateTransactionFullOptions(ReqId, NewTransaction, Src, Dst, ByteLength, ByteLength, LineWidth, ByteLength, DstStride);
    else
        return DrvCmxDmaCreateTransactionFullOptions(ReqId, NewTransaction, Src, Dst, ByteLength, 16, LineWidth, 16, DstStride);
}

dmaTransactionList* DrvCmxDmaCreate3DTransaction(dmaRequesterId ReqId,
                                                 dmaTransactionList *NewTransaction,
                                                 u8 *Src, u8 *Dst, u32 ByteLength,
                                                 u32 SrcWidth, u32 DstWidth,
                                                 u32 SrcStride, u32 DstStride,
                                                 u32 NumPlanes,
                                                 u32 SrcPlaneStride,
                                                 u32 DstPlaneStride) {
    if (NewTransaction && Src && Dst &&
        (NumPlanes >= MIN_NUM_PLANES) && (NumPlanes <= MAX_NUM_PLANES)) {
        if ((SrcWidth != 0) || (DstWidth != 0) ||
            (SrcStride != 0) || (DstStride != 0) ||
            (SrcPlaneStride != 0) || (DstPlaneStride != 0)) {
            // In 3D mode, the "NumPlanes" field holds the number of planes minus one.
            NumPlanes--;

            drvCmxDmaNewTransaction(DMA_2D_TRANSACTION, ReqId, NewTransaction,
                                    Src, Dst, ByteLength,
                                    SrcWidth, DstWidth,
                                    SrcStride, DstStride,
                                    NumPlanes, SrcPlaneStride,
                                    DstPlaneStride, 0);
        } else {
            NewTransaction = NULL; // Invalid parameter values.
        }
    } else {
        NewTransaction = NULL; // Invalid parameter values.
    }

    return NewTransaction;
}

int DrvCmxDmaLinkTasks(dmaTransactionList* listHead, u32 nmbTasks, ...)
{
    dmaTransactionList *listEnd, *val;
    va_list a_list;
    u32 i;

    if(cmxDmaStatus.status == DRV_CDMA_INITIALIZED)
    {
        va_start(a_list, nmbTasks);

        listEnd = listHead;
        for(i = 0; i < nmbTasks; i++)
        {
            val = va_arg(a_list, dmaTransactionList*);
            listEnd->linkAddress = val;
            listEnd = listEnd->linkAddress;
        }

        va_end(a_list);

        return CMX_DMA_DRV_SUCCESS;
    }
    else return CMX_DMA_DRV_NOT_INITIALIZED;
}

static inline void dmaWaitUntilLinkAgentIsntFetchingDescriptors(int linkAgent)
{
    int is_fetching, req_sm;
    do
    {
        req_sm = 0x7 & (GET_REG_WORD_VAL(CDMA_CHANNEL_DBG_ADR + 4) >> (linkAgent * 4));
        is_fetching = (req_sm != S_POP_REQ_ISSUE) && (req_sm != S_POP_WAIT_RSP);
    } while (!is_fetching);
}

static dmaTransactionList* findLast(dmaTransactionList *ptr)
{
    dmaTransactionList* next = (dmaTransactionList*)swcLeonReadNoCacheU32(&ptr->linkAddress);
    while(next) {
        ptr = next;
        next = (dmaTransactionList*)swcLeonReadNoCacheU32(&ptr->linkAddress);
    }
    return ptr;
}

static int DrvCmxDmaStartTask(dmaTransactionList* ListPtr, dmaIrqHandler function,
                              void *userContext, u32 *saveTail)
{
    UNUSED(function); // hush the compiler warning.
    UNUSED(userContext); // hush the compiler warning.

    u8 assignedLA;
    u32 result = CDMA_TASK_ERR;

    //get the assigned Link Agent for the current task
    //the Link Agent is determined by calculating modulus of 3 from the value of reqCount variable
    //used to generate the requesterID for this task
    assignedLA = startFrom + (ListPtr->agentOff);
    u32 cdma_link_cfg_adr = CDMA_LINK_CFG0_ADR + 8 * assignedLA;
    u32 cdma_link_cfg_adr_hi_word = cdma_link_cfg_adr + 4;
    u32 channel_mask = 0xF;

    // Since we're using mutexes in the following section of code, and it
    // could itself be interruptible by another call to the same functions
    // the following should be protected against interrupts happening. To
    // allow other interrupts to be serviced during this time, only CMXDMA
    // interrupt is disabled
    DrvIcbDisableIrq(IRQ_CMXDMA);
    // as a resolution of Bug 26749, we need to stop all channels while adding a task
    DRV_MUTEX_LOCK(DMA_MUTEX_COMMON);
    SET_REG_WORD(CDMA_SET_CHGATE_ADR, channel_mask);
    //disable the channel associated with our link agent
    dmaWaitUntilLinkAgentIsntFetchingDescriptors(assignedLA);
    u32 isShadowValid = GET_REG_WORD_VAL(cdma_link_cfg_adr_hi_word) & 1;

    if (!isShadowValid)
    {
        SET_REG_DWORD(cdma_link_cfg_adr, START_LCA_BIT | (u32)ListPtr);//enable the Link Agent
        result = CDMA_TASK_STARTED;
    }
    else
    {
        //patch the end of the list in order to be linked to our list (ListPtr)
        dmaTransactionList* lastTask;
        if (!USE_CMX_DMA_GLOBAL_TAIL){
            dmaTransactionList* currentTask =
                            (dmaTransactionList*)GET_REG_WORD_VAL(cdma_link_cfg_adr);
            lastTask = findLast(currentTask);
        }else{
            //tail points to an un-cached address space, so this should be fine
            lastTask = (dmaTransactionList*)*tail[assignedLA];
        }
        // the task is not necessarily in a cached space, so let's be sure that the write happens
        swcLeonWriteNoCache32(&lastTask->linkAddress, ListPtr);
        // read back the address to make sure that the value was written before being used
        GET_REG_WORD_VAL(lastTask);
        result = CDMA_TASK_LINKED;
    }
    if(USE_CMX_DMA_GLOBAL_TAIL){
        //save the pointer to the end of the newly added chain
        *tail[assignedLA] = saveTail;
        // read back the address to make sure that the value was written before being used
        GET_REG_WORD_VAL(tail[assignedLA]);
    }
    SET_REG_WORD(CDMA_CLR_CHGATE_ADR, channel_mask); //enable the channel associated with our link agent
    DRV_MUTEX_UNLOCK(DMA_MUTEX_COMMON);
    DrvIcbEnableIrq(IRQ_CMXDMA);

    return result;
}

void DrvCmxDmaIrqHandlerFunc(u32 source)
{
    UNUSED(source); // hush the compiler warning.

    int i;
    u32 intState;

    //read interrupt status bits
    intState = GET_REG_WORD_VAL(CDMA_INT_STATUS_ADR);

    for(i = 0; i < usedAgentsNr; i++)
    {
        u32 mask = 1 << (intOffset + i);//LA bit relative to first used agent
        u32 isInterruptActive = intState & mask;

        //check if the task that finished on currrent checked agent
        if(isInterruptActive)
        {
            u32 current = startFrom + i;
            dmaQueueElement_t *qElem;

            //reset the interrupt bit for our link agent
            SET_REG_WORD(CDMA_INT_RESET_ADR, mask);

            //mark the task as finished
            qElem = &waitingQueue[current][waitingQueueReadBackCounters[current]];
            qElem->ListPtr->cfgLink.fullCfgRegister |= DMA_SET_RESERVED1;

            dmaIrqHandler currentHandler = qElem->callbackFunc;
            //if currentHandler is NULL, synchronous tasks has finished.
            if(currentHandler)
            {
                //call the corresponding callback function
                currentHandler(qElem->ListPtr,
                               qElem->userContext);
            }

            //this will signal that the task finished its execution and the queue element can be rewritten.
            qElem->taskNum = 0;

            //increment the readback queue pointer
            //waitingQueueReadBackCounters[startFrom+i] = ++waitingQueueReadBackCounters[startFrom+i] % WAITING_QUEUE_SIZE;
            MODULO_INC(waitingQueueReadBackCounters[current], WAITING_QUEUE_SIZE);
            qElem = &waitingQueue[current][waitingQueueReadBackCounters[current]];

            //If there is another pending task on the current link agent(i), start it
            if(qElem->taskNum != 0)
            {
                DrvCmxDmaStartTask(qElem->ListPtr,
                                   qElem->callbackFunc,
                                   qElem->userContext,
                                   qElem->saveTail);
            }
            else currentlyExecuting[current] = 0;//Mark the current agent as not executing any task for us(Leon)

        }
    }
}

int DrvCmxDmaStartListTask(dmaTransactionList* ListPtr)
{
   return DrvCmxDmaStartTaskAsync(ListPtr, NULL, NULL);
}

int DrvCmxDmaStartTaskAsync(dmaTransactionList* ListPtr, dmaIrqHandler function, void *userContext)
{
    dmaTransactionList* ptr;
    u32 result = CDMA_TASK_ERR, taskCnt, assignedAgent;
    u32 *lastDescriptor = NULL;

    if(cmxDmaStatus.status == DRV_CDMA_INITIALIZED)
    {
        //Clear finish flag as we're just starting
        ListPtr->cfgLink.fullCfgRegister &= ~DMA_SET_RESERVED1;

        //Count the number of tasks from the current list and assign a common interrupt ID
        ptr = ListPtr; taskCnt = 0;
        while(ptr)
        {
            ptr->cfgLink.cfgBits.interruptTrigger = intOffset + (ListPtr->agentOff);
            if(ptr->linkAddress == NULL){
                ptr->cfgLink.cfgBits.disableInt = 0;
                lastDescriptor = (u32*)ptr;
            }
            else{
                ptr->cfgLink.cfgBits.disableInt = 1;
            }
            ptr = ptr->linkAddress;
            taskCnt++;
        }
        assignedAgent = startFrom + (ListPtr->agentOff);

        //Put our tasks in the waiting queue.
        //The queue is a circular buffer where its pointers will wrap around when the end will be reached.
        //If the new added elements in queue will overwrite the tasks that are not executed yet,
        //the new tasks won't be added in the queue.
        //Increase the WAITING_QUEUE_SIZE value in order to avoid queue overflow.
        if(waitingQueue[assignedAgent][waitingQueueCounters[assignedAgent]].taskNum == 0)
        {
            waitingQueue[assignedAgent][waitingQueueCounters[assignedAgent]].ListPtr = ListPtr;
            waitingQueue[assignedAgent][waitingQueueCounters[assignedAgent]].callbackFunc = function;
            waitingQueue[assignedAgent][waitingQueueCounters[assignedAgent]].userContext = userContext;
            waitingQueue[assignedAgent][waitingQueueCounters[assignedAgent]].saveTail = lastDescriptor;
            waitingQueue[assignedAgent][waitingQueueCounters[assignedAgent]].taskNum = taskCnt;

            //waitingQueueCounters[assignedAgent] = ++waitingQueueCounters[assignedAgent] % WAITING_QUEUE_SIZE;
             MODULO_INC(waitingQueueCounters[assignedAgent], WAITING_QUEUE_SIZE);

            //Check whether the previous task has finished. If it's not finished, then let our task in the queue
            //The interrupt handler will start it when current executed tasks have finished
            if(!currentlyExecuting[assignedAgent])
            {
                if (waitingQueueReadBackCounters[assignedAgent] != waitingQueueCounters[assignedAgent])
                {
                    currentlyExecuting[assignedAgent] = 1;

                    result = DrvCmxDmaStartTask(ListPtr, function, userContext, lastDescriptor);
                }
                else result = CDMA_TASK_QUEUED;
            }
            else result = CDMA_TASK_QUEUED;
        }
        else result = CDMA_TASK_QUEUE_FULL;
    }
    return result;
}

int DrvCmxDmaWaitTask(dmaTransactionList* ListPtr)
{
    if(cmxDmaStatus.status == DRV_CDMA_INITIALIZED)
    {
        u32 status = 0;
        while(!status)
        {
            status = GET_REG_WORD_VAL(&ListPtr->cfgLink.fullCfgRegister) &  DMA_SET_RESERVED1;
        }
        return CMX_DMA_DRV_SUCCESS;
    }
    return CMX_DMA_DRV_NOT_INITIALIZED;
}

int DrvCmxDmaIsTaskFinished(dmaTransactionList* ListPtr)
{
    if(cmxDmaStatus.status == DRV_CDMA_INITIALIZED)
    {
        //if the task is finished, then the interrupt handler function marked it as fixed
        if(IS_DMA_FINISHED(ListPtr->cfgLink.fullCfgRegister))
            return CDMA_TASK_FINISHED;
        return CDMA_TASK_STARTED;
    }
    return CMX_DMA_DRV_NOT_INITIALIZED;
}

dmaTransactionList* DrvCmxDmaCreateTransactionExt(u32 Type,
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
    if (BurstLength < DMA_MIN_BL && BurstLength > DMA_MAX_BL)
        return NULL;

    drvCmxDmaNewTransaction(Type, ReqId, NewTransaction,
                            Src, Dst, ByteLength,
                            SrcLineWidth, DstLineWidth,
                            SrcStride, DstStride,
                            CMX_DMA_DEFAULT_NUM_PLANE,
                            CMX_DMA_DEFAULT_PLANE_STRIDE,
                            CMX_DMA_DEFAULT_PLANE_STRIDE,
                            BurstLength);

    return NewTransaction;
}
#endif

/// @}

