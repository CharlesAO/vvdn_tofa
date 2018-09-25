/*
 * This is a component to manage memory for dynamic allocation
 *
 * @File      memManagerApi.c
 * @Author    Cristian Garjitzky
 * @Brief     Movidius Memory Manager
 * @copyright All code copyright Movidius Ltd 2017, all rights reserved
 *            For License Warranty see: common/license.txt
 */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "mv_types.h"
#include "mvMacros.h"

#if defined __sparc__

#ifdef MA2X8X
#include "OsDrvMutex.h"
#else
#include "DrvMutex.h"
#endif

#elif defined __shave__

#include "ShDrvMutex.h"

#endif

#include "memManagerApi.h"

#define MVLOG_UNIT_NAME MemManager
#include "mvLog.h"

//Ansi colors for memory draw
#define COLOR_FREE    "\x1b[32m"    /*green*/
#define COLOR_USED    "\x1b[31m"    /*red*/
#define COLOR_STATIC  "\x1b[35m"    /*magenta*/
#define COLOR_STACK   "\x1b[1;35m"  /*bright purple*/
#define COLOR_LARGE   "\x1b[1;32m"  /*bright green*/
#define COLOR_RESET   "\x1b[0m"


typedef enum { LEFT = 0, RIGHT = 1} shiftDirection;

static unsigned int historyFlag = 0;

////////////////////////////////////////////////////////////////////////////
//////////////////////////////////INTERNAL//////////////////////////////////
////////////////////////////////////////////////////////////////////////////
static void showHistory();
static void shiftArray(MemArea *area, unsigned int index, shiftDirection direction)
{
    int remIdx, incr, startPoint, endPoint;

    switch(direction)
    {
        case LEFT:
            startPoint = index;
            endPoint = area->nmbOfAllocs ;
            incr = +1;
            for (remIdx = startPoint; remIdx < endPoint+1; remIdx+=incr)
            {
                area->allocators[remIdx] =
                    area->allocators[remIdx+incr];
            } break;

        case RIGHT:
        default:
            startPoint = area->nmbOfAllocs+1;
            endPoint = index;
            incr = -1;
            for (remIdx = startPoint; remIdx > endPoint+1; remIdx+=incr)
            {
                area->allocators[remIdx] =
                    area->allocators[remIdx+incr];
            } break;
    }
}

static void freeSpaceMerge(MemArea *area, unsigned int index)
{
    /*Merge two adjacent free allocations into a single one
            (called after every allocation freeing)*/
    if(index >= area->nmbOfAllocs ||
        area->allocators[index].state != FREE)
        return;                         /*Sanity checks*/

    if (area->allocators[index-1].state == FREE)
    {
        area->allocators[index-1].size +=
            area->allocators[index].size;
        area->nmbOfAllocs -= 1;
        shiftArray(area, index, LEFT);
        freeSpaceMerge(area, index);    /*in case of 3 free-allocations merge scenario*/
    }
    else if (area->allocators[index+1].state == FREE)
    {
        area->allocators[index].size +=
            area->allocators[index+1].size;
        area->nmbOfAllocs -= 1;
        shiftArray(area, index+1, LEFT);
    }
}

static void addAllocator(MemArea *area, unsigned int allocIndex,
                           void* address, size_t size, allocState state)
{
    if(size > 0)
    {  // only add allocator if it's size is grater then 0
        area->allocators[allocIndex].address   = address;
        area->allocators[allocIndex].size      = size;
        area->allocators[allocIndex].state     = state;

        if (state == USED)
            area->usage += size;

        area->nmbOfAllocs++;
    }
}

static void removeAllocator(MemArea *area, unsigned int allocIndex)
{
    area->allocators[allocIndex].state = FREE;
    area->usage -= area->allocators[allocIndex].size;
    freeSpaceMerge(area, allocIndex);
}

static unsigned int isUsingDDR(void)
{
    return (__ddrSpaceEnd - __ddrSpaceStart);
}

static void showHistory()
{
    /*Explain Color meanings in memory draw*/
    if (historyFlag == 0){
        printf("\nAllocation Colors\n");
        printf(COLOR_RESET);
        //GREEN
        printf(COLOR_FREE"###");
        printf(COLOR_RESET " FREE\t");
        //RED
        printf(COLOR_USED"###");
        printf(COLOR_RESET " USED\t");
        //MAGENTA
        printf(COLOR_STATIC"###");
        printf(COLOR_RESET " STATIC\t");
        //Bright Magenta
        printf(COLOR_STACK"###");
        printf(COLOR_RESET " STACK\t");
        //Bright Green
        printf(COLOR_LARGE"###");
        printf(COLOR_RESET " LARGE\n\n");
    }
    historyFlag = 1;
}

#ifdef __sparc__
#ifdef MA2X8X
static void MemMgrMutexRequest(OsDrvMutexHandler * hndl, uint32_t id)
{
    // driver interface status code
    rtems_status_code sc;

    // opening a mutex will return a handle used for further operations
    sc = OsDrvMutexOpen(hndl, id);
    assert( sc == RTEMS_SUCCESSFUL   && "OsDrvMutexOpen() failed" );

    // obtain the mutex immediately or block until free
    sc = OsDrvMutexObtain(hndl);
    assert( sc == RTEMS_SUCCESSFUL   && "OsDrvMutexObtain() failed" );
}

static void MemMgrMutexRelease(OsDrvMutexHandler * hndl)
{
    // driver interface status code
    rtems_status_code sc;

    // make the mutex available for others to use
    sc = OsDrvMutexRelease(hndl);
    assert( sc == RTEMS_SUCCESSFUL   && "OsDrvMutexRelease() failed" );

    // release the resources associated with the mutex
    sc = OsDrvMutexClose(hndl);
    assert( sc == RTEMS_SUCCESSFUL   && "OsDrvMutexClose() failed" );
}
#endif // MA2X8X
#endif // __sparc__

////////////////////////////////////////////////////////////////////////////
/////////////////////////////////EXTERNAL///////////////////////////////////
////////////////////////////////////////////////////////////////////////////
void* MemMgrAlloc(size_t size, MemMgrAreas area, unsigned int MemAlignement)
{
    int leftoverSpace;
    void* AllocAddr     = NULL;
    MemArea *localArea  = &__MemManager.areas[area];
    Allocator *localAlloc;
    MemMgrErrorCode error = NO_ERROR;

#if defined __sparc__
#ifdef MA2X8X
    // mutex handler
    OsDrvMutexHandler hndl;
    MemMgrMutexRequest(&hndl, MEM_MGR_MUTEX);
#else
    DrvMutexLock(MEM_MGR_MUTEX);
#endif
#elif defined __shave__
    ShDrvMutexRequest(MEM_MGR_MUTEX);
#endif

    if (size <= 0)
    {
        error = MEMALLOC_INVALID_SIZE;
        mvLog(MVLOG_ERROR, "Error %d: MemArea-%d allocating size of (%d) is invalid!",
                error, area, size);
        goto exit;
    }

    /*Calculate size based on allocation-rate*/
    size = ALIGN_UP(size, localArea->allocRate);

    if (size > (localArea->size - localArea->usage))
    {
        error = MEMALLOC_SIZE_EXCEEDED;
        mvLog(MVLOG_ERROR, "Error %d: MemArea-%d size(%d) exceeded available space(%d)!",
                error, area, size,
                localArea->size - localArea->usage);
        goto exit;
    }

    for (unsigned int allocation = 0;
        allocation < localArea->nmbOfAllocs;
        allocation++)
    {
        localAlloc = &localArea->allocators[allocation];
        unsigned int AlignedAddress = (unsigned int) ALIGN_UP(localAlloc->address,MemAlignement);
        unsigned int AlignementCompSize = size + (AlignedAddress - (unsigned int)localAlloc->address);
        if (localAlloc->state == FREE && localAlloc->size >= AlignementCompSize)
        {
            leftoverSpace = localAlloc->size - AlignementCompSize;

            if (leftoverSpace > 0)
            {
                /*Shift allocators to the right*/
                shiftArray(localArea, allocation, RIGHT);

                /*Add new allocation with leftover space*/
                addAllocator(localArea,
                            allocation+1,
                            (void*)(AlignedAddress + AlignementCompSize),
                            leftoverSpace,
                            FREE);
            }
            addAllocator(localArea,
                        allocation,
                        (void*)AlignedAddress,
                        AlignementCompSize,
                        USED);

            localArea->nmbOfAllocs--;

            AllocAddr = (void*)AlignedAddress;
            mvLog(MVLOG_DEBUG, "Allocated %p size %d in area %d error %d\n",
                    AllocAddr, AlignementCompSize, area, error);
            goto exit;
        }
    }
    exit:
#if defined __sparc__
#ifdef MA2X8X
    MemMgrMutexRelease(&hndl);
#else
    DrvMutexUnlock(MEM_MGR_MUTEX);
#endif
#elif defined __shave__
    ShDrvMutexRelease(MEM_MGR_MUTEX);
#endif
    return AllocAddr;
}

MemMgrErrorCode MemMgrFree(void *AllocAddr)
{
    MemMgrErrorCode error = NO_ERROR;
    MemArea *localArea;
    Allocator *localAlloc;

#if defined __sparc__
#ifdef MA2X8X
    // mutex handler
    OsDrvMutexHandler hndl;
    MemMgrMutexRequest(&hndl, MEM_MGR_MUTEX);
#else
    DrvMutexLock(MEM_MGR_MUTEX);
#endif
#elif defined __shave__
    ShDrvMutexRequest(MEM_MGR_MUTEX);
#endif

    for(MemMgrAreas area = CMX_AREA0; area < MEM_TOTAL_COUNT; area++)
    {
        localArea  = &__MemManager.areas[area];
        for(unsigned int allocation = 0;
            allocation < localArea->nmbOfAllocs;
            allocation++)
        {
            localAlloc = &localArea->allocators[allocation];
            if(AllocAddr == localAlloc->address && localAlloc->state == USED)
            {
                mvLog(MVLOG_DEBUG,"Matched %p for deletion! Size of %d in Area %d",
                        AllocAddr,
                        localAlloc->size,
                        area);

                removeAllocator(localArea, allocation);
                goto exit;
            }
        }
    }
    /*Invalid address if not found*/
    error = MEM_INVALID_ADDRESS;
    mvLog(MVLOG_ERROR, "Error %d: Address %p is invalid ", error, AllocAddr);

    exit:
#if defined __sparc__
#ifdef MA2X8X
    MemMgrMutexRelease(&hndl);
#else
    DrvMutexUnlock(MEM_MGR_MUTEX);
#endif
#elif defined __shave__
    ShDrvMutexRelease(MEM_MGR_MUTEX);
#endif
    return error;
}

#ifdef __sparc__
MemMgrErrorCode MemMgrBufferExport(size_t bufSize, char *csvBuffer, size_t *csvLenght)
{
    const char* allocStates[] = {"FREE","USED","STATIC"};
    unsigned int nmbOfAreas;
    MemArea *localArea;
    Allocator *localAlloc;
    MemMgrErrorCode error = NO_ERROR;


#ifdef MA2X8X
    // mutex handler
    OsDrvMutexHandler hndl;
    MemMgrMutexRequest(&hndl, MEM_MGR_MUTEX);
#else
    DrvMutexLock(MEM_MGR_MUTEX);
#endif


    *csvLenght = 0;
    *csvLenght += snprintf(csvBuffer, bufSize, "%s", "MemArea,StartAddress,Size,State");

    nmbOfAreas = isUsingDDR() ? MEM_TOTAL_COUNT : CMX_TOTAL_COUNT;

    mvLog(MVLOG_DEBUG, "--EXPORT--");
    for(MemMgrAreas area = CMX_AREA0; area < nmbOfAreas; area++)
    {
        localArea  = &__MemManager.areas[area];
        mvLog(MVLOG_DEBUG, "MemArea: %d",
            area);
        mvLog(MVLOG_DEBUG, "Addr: %p",
            localArea->address);
        mvLog(MVLOG_DEBUG, "AlocRate: %d",
            localArea->allocRate);
        mvLog(MVLOG_DEBUG, "Nmb Allocs: %d",
            localArea->nmbOfAllocs);
        mvLog(MVLOG_DEBUG, "Size: %d",
            localArea->size);

        for(unsigned int allocation = 0;
            allocation < localArea->nmbOfAllocs;
            allocation++)
        {
            localAlloc = &localArea->allocators[allocation];
            mvLog(MVLOG_DEBUG, "\tAllocAddress:%p",
                localAlloc->address);
            mvLog(MVLOG_DEBUG, "\tAllocSize:%d",
                localAlloc->size);
            mvLog(MVLOG_DEBUG, "\t%s",
                allocStates[localAlloc->state]);

            *csvLenght += snprintf(csvBuffer + *csvLenght, bufSize,
                                "\n%d,%p,%d,%d",
                                area,
                                localAlloc->address,
                                localAlloc->size,
                                localAlloc->state);
        }
    }
    if (*csvLenght >= bufSize)
    {
        error = MEM_EXPORT_ALLOCMEM;
        mvLog(MVLOG_ERROR, "Error %d: Allocate more memory for export buffer !",
            error);
    }

#ifdef MA2X8X
    MemMgrMutexRelease(&hndl);
#else
    DrvMutexUnlock(MEM_MGR_MUTEX);
#endif

    return error;
}
#endif  // #ifdef __sparc__

void MemMgrExportDraw(int dispWidth)
{
    char string[128] = {0};
    MemArea *localArea;
    Allocator *localAlloc;
    unsigned int nmbOfBlocks = 0;
    unsigned int nmbOfAreas, sliceSize, GeneratedNmbOfBlocks, dispHeight;
    const char * plotColors[5] =
                    { COLOR_FREE, COLOR_USED, COLOR_STATIC, COLOR_STACK, COLOR_LARGE};
    // needs to be static in case used from MISA shave
    const char MEM_DRAW_STR = 35;  // Ansi char "#"

#if defined __sparc__
#ifdef MA2X8X
    // mutex handler
    OsDrvMutexHandler hndl;
    MemMgrMutexRequest(&hndl, MEM_MGR_MUTEX);
#else
    DrvMutexLock(MEM_MGR_MUTEX);
#endif
#elif defined __shave__
    ShDrvMutexRequest(MEM_MGR_MUTEX);
#endif

    showHistory();

    const unsigned int ddrMemoryAllocationSize = isUsingDDR();

    nmbOfAreas = ddrMemoryAllocationSize ? MEM_TOTAL_COUNT : CMX_TOTAL_COUNT;

    printf("\n");
    for(MemMgrAreas area = CMX_AREA0; area < nmbOfAreas; area++)
    {
        unsigned int totalAreaSpaceUsed = 0;
        localArea  = &__MemManager.areas[area];
        GeneratedNmbOfBlocks = 0;
        printf(COLOR_RESET "MemArea%d-%s[",(int)area, (area == DDR_AREA)?"DDR":"CMX");

        for(unsigned int allocation = 0;
            allocation < localArea->nmbOfAllocs;
            allocation++)
        {
            sliceSize = ((int)area == (int)DDR_AREA) ?
                        ddrMemoryAllocationSize :
                        CMX_SLICE_SIZE ;
            dispHeight = ((int)area == (int)DDR_AREA) ?
                        (ddrMemoryAllocationSize/(1024 * localArea->allocRate)):
                        2 *  (unsigned int)dispWidth;
            localAlloc = &localArea->allocators[allocation];

            nmbOfBlocks =(unsigned int)(((double)localAlloc->size/
                        (double)sliceSize) * (double)dispHeight + 0.5);
            nmbOfBlocks = nmbOfBlocks > 0 ? nmbOfBlocks : 1;
            totalAreaSpaceUsed += localAlloc->size;

            if ((GeneratedNmbOfBlocks + nmbOfBlocks) > (unsigned int)dispWidth)
            {
                // generate string buffer for used section
                memset(string, MEM_DRAW_STR, dispWidth - GeneratedNmbOfBlocks);
                string[dispWidth - GeneratedNmbOfBlocks + 1] = 0;
                printf("%s",plotColors[localAlloc->state]);
                printf("%s\n",string);
                printf(COLOR_RESET);
                // clear string buffer
                memset(string, 0, dispWidth - GeneratedNmbOfBlocks);
                nmbOfBlocks -= (dispWidth - GeneratedNmbOfBlocks);
                GeneratedNmbOfBlocks = 0;
            }

            // the total number of blocks would be grater then display width, but the current number of blocks is less, only one split
            for(int lines = 0; nmbOfBlocks >= (unsigned int)dispWidth; lines++)
            {
                // generate string buffer for used section
                memset(string, MEM_DRAW_STR, dispWidth);
                string[dispWidth + 1] = 0;
                printf("%s",plotColors[localAlloc->state]);
                printf("%s\n",string);
                printf(COLOR_RESET);
                // clear string buffer
                memset(string, 0, dispWidth);
                nmbOfBlocks -= dispWidth;
            }

            memset(string, MEM_DRAW_STR, nmbOfBlocks);
            string[nmbOfBlocks + 1] = 0;
            printf("%s",plotColors[localAlloc->state]);
            printf("%s",string);
            // clear string buffer
            memset(string, 0, nmbOfBlocks);
            printf(COLOR_RESET);
            GeneratedNmbOfBlocks += nmbOfBlocks;
        }
        printf("](%u/%u)\n", (unsigned int)localArea->usage, (unsigned int)localArea->size);
    }
#if defined __sparc__
#ifdef MA2X8X
    MemMgrMutexRelease(&hndl);
#else
    DrvMutexUnlock(MEM_MGR_MUTEX);
#endif
#elif defined __shave__
    ShDrvMutexRelease(MEM_MGR_MUTEX);
#endif
}
