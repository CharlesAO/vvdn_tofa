///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

/////////////////////////////////////////////////////////////////////////////////
//  Header files
/////////////////////////////////////////////////////////////////////////////////

#include <sipp.h>
#include <sippInternal.h>

/////////////////////////////////////////////////////////////////////////////////
//  Local Macros
/////////////////////////////////////////////////////////////////////////////////
#define SIPP_MEMPOOL_CMX 0
#define SIPP_MEMPOOL_DDR 1
#define SIPP_MEMPOOL_NUM_GENERAL_POOLS 2

#define SIPP_MEMPOOL_TOTAL_POOLS (SIPP_MEMPOOL_NUM_GENERAL_POOLS + (2 * SIPP_MAX_SUPPORTED_PIPELINES))

/////////////////////////////////////////////////////////////////////////////////
//  Storage Allocation Globals
/////////////////////////////////////////////////////////////////////////////////

extern u32 gSippSliceSz;
u8 * sippCmxBase; // On PC, create storage for 16 slices
                  // On Myriad, just point to CMX_

// The main memory pool for dynamic allocation that holds pipeline, filter definitions
// This structure is visible for Leon and Shaves
u8 sippMemPoolCMX[SIPP_CMX_POOL_SZ] ALIGNED(32) CMX_DATA =
{
    1,0,0,0
};

u8 sippMemPoolDDR[SIPP_DDR_POOL_SZ] ALIGNED(32) DDR_DATA;

// Allocate control structs for the 2 general pools and the per pipeline pools (lineBuf and Schedule)
tSippPhysicalPool sippPools[SIPP_MEMPOOL_NUM_GENERAL_POOLS] =
{
    { "GLOBAL_CMX", sippMemPoolCMX,
                    sippMemPoolCMX,
                    sippMemPoolCMX + sizeof(sippMemPoolCMX),
                    0},

    { "GLOBAL_DDR", sippMemPoolDDR,
                    sippMemPoolDDR,
                    sippMemPoolDDR + sizeof(sippMemPoolDDR),
                    0},
};

u32           sippPoolsUsage[SIPP_MEMPOOL_TOTAL_POOLS] = {0};
tSippVPhysMap gsSippMemMap[SIPP_MAX_SUPPORTED_PIPELINES];

///////////////////////////////////////////////////////////////////////
// Code

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippInitPhysicalPoolGlobal                                       //
//                                                                                //
//  DESCRIPTION: Establish the global physical pools within the system - i.e.     //
//               those pools communally accessed by all pipelines                 //
//                                                                                //
//  INPUTS:      None as parameters - all setup of these pools is doen via build  //
//               time defines                                                     //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       on PC: attempt to allocate buffer on same addr as on myriad2     //
//               then clear the pool (this is done just once, no matter how many  //
//               pipelines we create                                              //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippInitPhysicalPoolGlobal ()
{
    //Clear SIPP mempool:
    #if defined(__sparc)
    // On Sparc, just point to CMX base
    sippCmxBase = (u8*)MXI_CMX_BASE_ADR;

    #elif defined(SIPP_PC)
    // To help debug on PC side, try to allocate the CMX in same address-space as on Myriad2

    #if defined(_WIN32)
    sippCmxBase = (u8*)VirtualAlloc ((LPVOID)CMX_BASE_ADR, 16 * gSippSliceSz, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    if (sippCmxBase == NULL)
    {
        // Virtual alloc failed, try standard mem alloc
        sippCmxBase = (u8*)malloc (16 * gSippSliceSz);

        if (sippCmxBase == NULL)
        {
            sippError ((u32 *)NULL, E_PC_CMX_MEM_ALLOC_ERR);
        }
    }

    #else
    sippCmxBase = (u8*)malloc (16 * gSippSliceSz);

    if (sippCmxBase == NULL)
    {
        sippError ((u32 *)NULL, E_PC_CMX_MEM_ALLOC_ERR);
    }
    else
    {
        // Run a memset - this is useful to initialise all line buffers to their value in
        // myriad builds so that filters such as Fast9M2 which do not write to all entries in the
        // line may be compared against the same reference data
    	sippPalMemset ((void *)sippCmxBase, 0xCD, 16 * gSippSliceSz);
    }
    #endif
    #endif

    // Reset start pointer
    sippPools[SIPP_MEMPOOL_CMX].pos = sippPools[SIPP_MEMPOOL_CMX].start;
    sippPools[SIPP_MEMPOOL_DDR].pos = sippPools[SIPP_MEMPOOL_DDR].start;

    // SIPP framework will manage the DDR and CMX pools as conventional heaps so establish that control here
    sippPools[SIPP_MEMPOOL_CMX].pHeap = sippHeapCreate ((u8 *)sippPools[SIPP_MEMPOOL_CMX].start,
                                                        (u32)(sippPools[SIPP_MEMPOOL_CMX].end - sippPools[SIPP_MEMPOOL_CMX].start));

    #if defined DDR_SIPP_DATA
    sippPools[SIPP_MEMPOOL_DDR].pHeap = sippHeapCreate ((u8 *)sippPools[SIPP_MEMPOOL_DDR].start,
                                                        (u32)(sippPools[SIPP_MEMPOOL_DDR].end - sippPools[SIPP_MEMPOOL_DDR].start));
    #endif

    if ((sippPools[SIPP_MEMPOOL_CMX].pHeap == (tsSippHeap *)NULL)
    #if defined DDR_SIPP_DATA
        || (sippPools[SIPP_MEMPOOL_DDR].pHeap == (tsSippHeap *)NULL)
    #endif
       )
    {
        sippError ((u32 *)NULL, E_HEAP_CREATION_FAIL);
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippInitPhysicalPoolPipe                                         //
//                                                                                //
//  DESCRIPTION: Establish the physical pools within the system                   //
//                                                                                //
//  INPUTS:                                                                       //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippInitPhysicalPoolPipe (ptSippMCB ptMCB)
{
    ptSippPhysicalPool pPhysPoolDesc;

    // Allocate a memory mapping to this pipe
    ptMCB->pVirtPhysMap = &gsSippMemMap[ptMCB->pipeIdx];

    // Allocate physcial pool descriptors for Line Buffer and Scheduler pool
    pPhysPoolDesc = (ptSippPhysicalPool)sippMemAlloc (ptMCB,
                                                      vPoolPipeStructs,
                                                      sizeof(tSippPhysicalPool) * 2);

    if (pPhysPoolDesc == (ptSippPhysicalPool)NULL)
    {
        sippError ((u32 *)NULL, E_OUT_OF_MEM);
        return 0x0;
    }

    ptMCB->pVirtPhysMap->physPoolMap[vPoolFilterLineBuf] = pPhysPoolDesc++;
    ptMCB->pVirtPhysMap->physPoolMap[vPoolScheduleTemp]  = pPhysPoolDesc;

    return 0x1;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippInitLnMemPool                                                //
//                                                                                //
//  DESCRIPTION: Establish line-buffer pool for a pipeline                        //
//                                                                                //
//  INPUTS:      ptMCB - Pointer to a (pipeline's) memory control block           //
//               start - Pointer to the line buffer base                          //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       After parsing the svuElf, we know where we can place the         //
//               line-buffer (i.e. after code, static-data and STACK)             //
///                                                                               //
////////////////////////////////////////////////////////////////////////////////////

void sippInitLnMemPool (ptSippMCB ptMCB,
                        u8 *      start)
{
    ptSippPhysicalPool pSippPool = ptMCB->pVirtPhysMap->physPoolMap[vPoolFilterLineBuf];

    pSippPool->start = start;
    pSippPool->pos   = start;
    pSippPool->end   = sippCmxBase + gSippSliceSz;
    pSippPool->pHeap = (tsSippHeap *)NULL;           // This pool will not be managed as a heap

    #ifdef RT_DBG
    sippPalTrace (SIPP_CORE_TL_INFO, "sippInitLnMemPool : Start = %08x, End = %08x, Size = %08x\n", (u32)start, (u32)pSippPool->end, (u32)pSippPool->end - (u32)start);
    #endif

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippInitLnMemPoolSlices                                          //
//                                                                                //
//  DESCRIPTION: Establish line-buffer pool per slice for a pipeline              //
//                                                                                //
//  INPUTS:      ptMCB - Pointer to a (pipeline's) memory control block           //
//               firstSlice - First CMX slice allocated to the pipe               //
//               lastSlice - Last CMX slice allocated to the pipe                 //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       Hitherto the SIPP framework has ran using chunked line buffers   //
//               by default. That is all line buffers are split over all slices.  //
//               The addition of the oPipe runtime (and preparation for ma2x8x)   //
//               requires SIPP allow for line buffers to be fully allocated       //
//               within individual slices
//                                                                                //
//               In this case we need to allocate physical pool descriptors       //
//               for each slice so that a virtual slice pool can target them      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32  sippInitLnMemPoolSlices (ptSippMCB ptMCB,
                              u32       firstSlice,
                              u32       lastSlice)
{
    ptSippPhysicalPool pPhysPoolDesc;
    u32                numSlices, physPoolIdx;

    // Allocate physical pool descriptors for each slice
    pPhysPoolDesc = (ptSippPhysicalPool)sippMemAlloc (ptMCB,
                                                      vPoolPipeStructs,
                                                      sizeof(tSippPhysicalPool) * (lastSlice - firstSlice + 1));

    if (pPhysPoolDesc == (ptSippPhysicalPool)NULL)
    {
        sippError ((u32 *)NULL, E_OUT_OF_MEM);
        return 0x1;
    }

    for (numSlices = (lastSlice - firstSlice + 1), physPoolIdx = 0; physPoolIdx < numSlices; physPoolIdx++, pPhysPoolDesc++)
    {
        pPhysPoolDesc->start = (u8 *)sippCmxBase + ((firstSlice + physPoolIdx) * gSippSliceSz);
        pPhysPoolDesc->pos   = (u8 *)pPhysPoolDesc->start;
        pPhysPoolDesc->end   = (u8 *)pPhysPoolDesc->start + gSippSliceSz;
        pPhysPoolDesc->pHeap = (tsSippHeap *)NULL;           // This pool will not be managed as a heap

        // Always use virtual pools 0 -> num slices - 1
        ptMCB->pVirtPhysMap->physPoolMap[(u32)vPoolFilterLineBuf0 + physPoolIdx] = pPhysPoolDesc;
        ptMCB->numVPools++;
    }

    return 0x0;

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippInitSchedPool                                                //
//                                                                                //
//  DESCRIPTION: Initialize Schedule pool for a pipeline                          //
//                                                                                //
//  INPUTS:      None                                                             //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippInitSchedPool (ptSippMCB ptMCB,
                        u32       sliceFirst,
                        u32       sliceLast)
{
    ptSippPhysicalPool pSippPool = ptMCB->pVirtPhysMap->physPoolMap[vPoolScheduleTemp];

    // Compute the start, size of "virtual" pool mempool_sched
    #if defined(__sparc) && defined(MYRIAD2)
    pSippPool->start  = (u8*)CMX_BASE_ADR;
    #elif defined(SIPP_PC)
    pSippPool->start  = sippCmxBase;
    #endif

    pSippPool->start += sliceFirst * gSippSliceSz;                                      // Point to first slice
    pSippPool->pos    = pSippPool->start;                                               // Cur_pos = start
    pSippPool->end    = pSippPool->start + (sliceLast - sliceFirst + 1) * gSippSliceSz; // Finish = start + N * SLC_SZ
    pSippPool->pHeap  = (tsSippHeap *)NULL;                                             // This pool will not be managed as a heap

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMemInitVirtPhysMaps                                          //
//                                                                                //
//  DESCRIPTION: Initial the virtual to physical pool mapping struct for each     //
//               pipeline - all other virtual pools must be individually          //
//               initialised                                                      //
//                                                                                //
//  INPUTS:      None as parameters - though setup of these maps is done via      //
//               build time defines                                               //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippMemInitVirtPhysMaps ()
{
    u32 mapIdx;

    for (mapIdx = 0; mapIdx < SIPP_MAX_SUPPORTED_PIPELINES; mapIdx++)
    {
        pSippVPhysMap pMap = &gsSippMemMap[mapIdx];

        pMap->physPoolMap[vPoolCMXDMADesc]    = &sippPools[SIPP_MEMPOOL_CMX];
        pMap->physPoolMap[vPoolGeneral]       = &sippPools[SIPP_MEMPOOL_DDR];

        #ifdef DDR_SIPP_DATA
        pMap->physPoolMap[vPoolPipeStructs]   = &sippPools[SIPP_MEMPOOL_DDR];
        pMap->physPoolMap[vPoolSchedule]      = &sippPools[SIPP_MEMPOOL_DDR];
        #else
        pMap->physPoolMap[vPoolPipeStructs]   = &sippPools[SIPP_MEMPOOL_CMX];
        pMap->physPoolMap[vPoolSchedule]      = &sippPools[SIPP_MEMPOOL_CMX];
        #endif


//        pMap->physPoolMap[vPoolFilterLineBuf] = sippPools[SIPP_MEMPOOL_NUM_GENERAL_POOLS + (mapIdx << 0x1)];
//        pMap->physPoolMap[vPoolScheduleTemp]  = SIPP_MEMPOOL_NUM_GENERAL_POOLS + (mapIdx << 0x1) + 0x1;

    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippChooseMemPool                                                //
//                                                                                //
//  DESCRIPTION: Allow the user to change the virtual to physical pool mapping    //
//                                                                                //
//  INPUTS:      pSippMCB - pointer to client's MCB                               //
//               vPool - virtual pool to allocate from                            //
//               phyPoolIdx - physical pool id                                    //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippChooseMemPool (ptSippMCB       pSippMCB,
                        SippVirtualPool vPool,
                        u32             physPoolIdx)
{
    u32 physPool;
    UNUSED (physPool); //hush the compiler warning

    if (pSippMCB)
    {
        // Get the mapping of virtual to physical pools
        pSippVPhysMap pMap = &gsSippMemMap[pSippMCB->pipeIdx];

        if (physPoolIdx == 0)
        {
            pMap->physPoolMap[vPool] = &sippPools[SIPP_MEMPOOL_CMX];
        }
        else if (physPoolIdx == 1)
        {
            pMap->physPoolMap[vPool] = &sippPools[SIPP_MEMPOOL_DDR];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMemAlloc                                                     //
//                                                                                //
//  DESCRIPTION: Allocates a region of the required size from the specified pool  //
//                                                                                //
//  INPUTS:      pSippMCB - pointer to client's MCB                               //
//               vPool - virtual pool to allocate from                            //
//               bytes - number of bytes requested                                //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     Pointer to allocated area                                        //
//                                                                                //
//  NOTES:       Always returns an 8-byte aligned memory chunk within required    //
//               pool                                                             //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void * sippMemAlloc (ptSippMCB       pSippMCB,
                     SippVirtualPool vPool,
                     s32             bytes)
{
    u32                 physPoolIdx;
    ptSippPhysicalPool  ptPhysPool;

    if (pSippMCB)
    {
        // Get the mapping of virtual to physical pools
        ptPhysPool = pSippMCB->pVirtPhysMap->physPoolMap[vPool];
    }
    else
    {
        #ifdef DDR_SIPP_DATA
        physPoolIdx = SIPP_MEMPOOL_DDR;
        #else
        physPoolIdx = SIPP_MEMPOOL_CMX;
        #endif

        ptPhysPool = &sippPools[physPoolIdx];
    }

    // Is this pool managed as a heap?
    if (ptPhysPool->pHeap)
    {
        void * pPrev    = (pSippMCB) ? pSippMCB->pVPoolListEnd[vPool] : NULL;
        // Make sure we return the next 8 byte aligned address within current pool
        u32    numBytes = ((bytes + 7) & ~7);
        void * pPtr     = sippAlloc (ptPhysPool->pHeap,
                                     pPrev,
                                     numBytes);

        if (pPtr)
        {
   //         sippPoolsUsage[physPool] += (numBytes + SIPP_HCB_SIZE);

            if (pSippMCB)
            {
                if (pSippMCB->pVPoolListStart[vPool] == (void *)NULL)
                {
                    pSippMCB->pVPoolListStart[vPool] = pPtr;
                }
                pSippMCB->pVPoolListEnd[vPool] = pPtr;
            }
        }
        else
        {
            #ifndef SIPP_NO_PRINTF
           u32 freeSpace = sippMemCheck (pSippMCB,
                                         vPool);
            #endif

           sippPalTrace (SIPP_CORE_TL_ERROR, "sippMemAlloc : Allocation failure - requested = %d (available = %d in Pool %d)\n",
                                             bytes, (u32)freeSpace, vPool);
        }

        return pPtr;
    }
    else
    {
        u8 *pos = ptPhysPool->pos;
        u8 *end = ptPhysPool->end;
        u8 *new_pos;

        // Make sure we return the next 8 byte aligned address within current pool
        pos = (u8 *)((7 + (u32)pos) & ~7);

        // Advance new ptr. position for next alloc (i.e. skip over what's allocated now)
        new_pos = pos + bytes;
        new_pos = (u8 *)((7 + (u32)new_pos) & ~7);

        if (new_pos > end)
        {
           sippPalTrace (SIPP_CORE_TL_ERROR, "sippMemAlloc : Allocation failure - requested = %ld (available = %lu in Pool %lu)\n",
                                              bytes, (u32)end - (u32)pos, vPool);
           return (void *)NULL;
        }
        else
        {
            // Save current ptr for next allocation
            ptPhysPool->pos = new_pos;
            VCS_PRINT_INT(pos);
        }

   //     sippPoolsUsage[physPool] += bytes;

        return pos;
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMemFreeList                                                  //
//                                                                                //
//  DESCRIPTION: Free all linked list items from a designated virtual pool        //
//                                                                                //
//  INPUTS:      pSippMCB - pointer to client's MCB                               //
//               vPool - virtual pool to free from                                //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       Only works for virtual pools mapped to physical pools managed as //
//               heaps                                                            //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippMemFreeList (ptSippMCB       pSippMCB,
                      SippVirtualPool vPool)
{
    ptSippPhysicalPool ptPhysPool;

    // Get the mapping of virtual to physical pools
    ptPhysPool = pSippMCB->pVirtPhysMap->physPoolMap[vPool];

    // Is this pool managed as a heap?
    if (ptPhysPool->pHeap)
    {
        sippFreeList (ptPhysPool->pHeap,
                      pSippMCB->pVPoolListStart[vPool]);
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMemFree                                                      //
//                                                                                //
//  DESCRIPTION: Free an item from a designated virtual pool - if that pool is    //
//               managed as a heap - otherwise the function does nothing          //
//                                                                                //
//  INPUTS:      pSippMCB - pointer to client's MCB                               //
//               vPool - virtual pool to free from                                //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       Should only be used on items not allocated in a linked list      //
//               for which a call to sippMemFreeList will be made. This is a      //                                                                                //
//               singly linked list so removal of one item and repairing the link //
//               is not possible - therefore the list is no longer suitable for   //
//               a call via sippMemFreeList                                       //
////////////////////////////////////////////////////////////////////////////////////

void sippMemFree (ptSippMCB       pSippMCB,
                  SippVirtualPool vPool,
                  void *          pPtr)
{
    u32                physPoolIdx;
    ptSippPhysicalPool ptPhysPool;

    if (pSippMCB)
    {
        // Get the mapping of virtual to physical pools
        ptPhysPool = pSippMCB->pVirtPhysMap->physPoolMap[vPool];
    }
    else
    {
        #ifdef DDR_SIPP_DATA
        physPoolIdx = SIPP_MEMPOOL_DDR;
        #else
        physPoolIdx = SIPP_MEMPOOL_CMX;
        #endif

        ptPhysPool = &sippPools[physPoolIdx];
    }

    // Is this pool managed as a heap?
    if (ptPhysPool->pHeap)
    {
        sippFreeList (ptPhysPool->pHeap,
                      pPtr);
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMemFindMaxLnMemPoolFree                                      //
//                                                                                //
//  DESCRIPTION: Search the line buffer virtual pools for the maximum chunk of    //
//               contigous memory. Naturally this could traverse multiple pools   //
//                                                                                //
//  INPUTS:      pSippMCB - pointer to client's MCB                               //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     Virtual pool in which max contiguous free area starts            //                                                            //
//                                                                                //
//  NOTES:       We have one virtual pool per CMX slice allocated for line        //
//               buffers                                                          //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

SippVirtualPool sippMemFindMaxLnMemPoolFree (ptSippMCB pSippMCB)
{
    ptSippPhysicalPool ptPhysPool;
    u32                freeSpace    = 0;
    u32                physPoolIdx  = 0;
    bool               bComplete    = FALSE;
    bool               bWrapDone    = FALSE;
    u32                freeRun      = 0;
    u32                maxRun       = 0;
    u32                startPool    = 0;
    u32                maxStartPool = 0;

    while (bComplete == FALSE)
    {
        ptPhysPool = pSippMCB->pVirtPhysMap->physPoolMap[(u32)vPoolFilterLineBuf0 + physPoolIdx];

        if (ptPhysPool)
        {
            if ((u32)ptPhysPool->pos >= (u32)ptPhysPool->end)
            {
                freeSpace = 0;
            }
            else
            {
                freeSpace = (u32)ptPhysPool->end - (u32)ptPhysPool->pos;
            }

            if (freeSpace == gSippSliceSz)
            {
                freeRun += freeSpace;
            }
            else
            {
                if (bWrapDone)
                {
                    bComplete = TRUE;
                }
                else
                {
                    freeRun   = freeSpace;
                    startPool = physPoolIdx;
                }
            }

            if (freeRun > maxRun)
            {
                maxRun       = freeRun;
                maxStartPool = startPool;
            }

            // Move to new virtual pool
            physPoolIdx++;

            // Ensure we don't go beyond virtual pool boundaries if the maximum slice is allocated to
            // this pipe
            if (((u32)vPoolFilterLineBuf0 + physPoolIdx) == vPoolLast)
            {
                if (maxRun == (gSippSliceSz * physPoolIdx))
                {
                    // No space is as yet used in any slice so can safely complete here...
                    bComplete = TRUE;
                }
                else
                {
                    // Now we should continue from the start again until we find some occupied memory
                    bWrapDone   = TRUE;
                    physPoolIdx = 0x0;
                }
            }
        }
        else
        {
            if (maxRun == (gSippSliceSz * physPoolIdx))
            {
                // No space is as yet used in any slice so can safely complete here...
                bComplete = TRUE;
            }
            else
            {
                // Now we should continue from the start again until we find some occupied memory
                bWrapDone   = TRUE;
                physPoolIdx = 0x0;
            }
        }
    }

    return (SippVirtualPool)((u32)vPoolFilterLineBuf0 + maxStartPool);
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMemCheck                                                     //
//                                                                                //
//  DESCRIPTION: Check the memory from a designated virtual pool                  //
//                                                                                //
//  INPUTS:      pSippMCB - pointer to client's MCB                               //
//               vPool - virtual pool to check                                    //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:                                                                        //
////////////////////////////////////////////////////////////////////////////////////

u32 sippMemCheck (ptSippMCB       pSippMCB,
                  SippVirtualPool vPool)
{
    u32 freeSpace = 0;

    u32                physPoolIdx;
    ptSippPhysicalPool ptPhysPool;

    if (pSippMCB)
    {
        // Get the mapping of virtual to physical pools
        ptPhysPool = pSippMCB->pVirtPhysMap->physPoolMap[vPool];
    }
    else
    {
        #ifdef DDR_SIPP_DATA
        physPoolIdx = SIPP_MEMPOOL_DDR;
        #else
        physPoolIdx = SIPP_MEMPOOL_CMX;
        #endif

        ptPhysPool = &sippPools[physPoolIdx];
    }

    // Is this pool managed as a heap?
    if (ptPhysPool->pHeap)
    {
        freeSpace = sippHeapCheck (ptPhysPool->pHeap);
    }
    else
    {
        u8 * pos = ptPhysPool->pos;
        u8 * end = ptPhysPool->end;

        if (pos >= end)
        {
             sippPalTrace (SIPP_CORE_TL_INFO, "No free space available!");
        }
        else
        {
            freeSpace = end - pos;
        }
    }

    return freeSpace;
}

//#################################################################################
// Useful for test mode only: to map the circular line buffers in DDR
//#################################################################################
void sippSetCircularLnBuffBase (u8 *base)
{
    UNUSED (base); //hush the compiler warning
    // Update memory pool info:
//    sippPools[mempool_lnbuff].start = base;
//    sippPools[mempool_lnbuff].pos   = base;
//    sippPools[mempool_lnbuff].end   = base + gSippSliceSz;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMemStatus                                                    //
//                                                                                //
//  DESCRIPTION: Report usage stats for all pools                                 //
//                                                                                //
//  INPUTS:      None                                                             //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippMemStatus ()
{
    #ifdef SIPP_PC
    u32 i, freeSpace;

    sippPalTrace (SIPP_CORE_TL_INFO, "\n\n");
    sippPalTrace (SIPP_CORE_TL_INFO, "Mem-pools status:\n");

    for (i = 0; i < SIPP_MEMPOOL_NUM_GENERAL_POOLS; i++)
    {
        if (sippPools[i].pHeap)
        {
            freeSpace = sippHeapCheck (sippPools[i].pHeap);
        }
        else
        {
            freeSpace = sippPools[i].pos - sippPools[i].start;
        }

        sippPalTrace (SIPP_CORE_TL_WARNING, "   Mem-POOL: __%s__ : size = %8d Bytes, occupied = %8d (%05.2f%%)\n",
                                             sippPools[i].name,
                                             sippPools[i].end - sippPools[i].start, //Size [bytes]
                                             sippPools[i].end - sippPools[i].start - freeSpace,
                                             100.0f*(sippPools[i].end - sippPools[i].start - freeSpace)/(sippPools[i].end - sippPools[i].start)); //Size [%]
    }
    printf("\n\n");

    #elif defined(MYRIAD2)
    #ifndef SIPP_NO_PRINTF
    u32 i, freeSpace;

    for (i = 0; i < SIPP_MEMPOOL_NUM_GENERAL_POOLS; i++)
    {
        if (sippPools[i].pHeap)
        {
            freeSpace = sippHeapCheck (sippPools[i].pHeap);
        }
        else
        {
            freeSpace = sippPools[i].pos - sippPools[i].start;
        }

        sippPalTrace (SIPP_CORE_TL_WARNING, "Mempool %d : Size %08x Occupied : %08x\n", i, (sippPools[i].end - sippPools[i].start), sippPools[i].end - sippPools[i].start - freeSpace);
    }
    #endif
    #endif
}
