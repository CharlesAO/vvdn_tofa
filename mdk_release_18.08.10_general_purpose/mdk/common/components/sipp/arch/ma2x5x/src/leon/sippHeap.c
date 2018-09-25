///
/// @file      sippHeap.c
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///


#include <sipp.h>
#include <sippInternal.h>

///////////////////////////////////////////////////////////////////////
// CFG macros - move to sippCfg.h eventually

#define  SIPP_CFG_MAX_NUM_HEAPS  0x2          // At the minute lets say one in CMX and one in DDR
#define  MCB_MAGIC               0x42434D4C   // Little-endian 'LMCB'
#define  SIPP_HEAP_DEFRAG_COUNT  0x5          // Count indicating how many frees cause the heap defrag algo to run

///////////////////////////////////////////////////////////////////////
// Globals

tsSippHeap sippHeaps[SIPP_CFG_MAX_NUM_HEAPS];

///////////////////////////////////////////////////////////////////////
// Code

///////////////////////////////////////////////////////////////////////
// Internal functions

void sippHeapInit (tsSippHeap * pSippHeap,
                   u8 *         start_address,
                   u32          size)
{
    pSippHeap->sippHeap_start = (u8 *) start_address;
    pSippHeap->sippHeap_end   = (u8 *) (start_address + size);
    pSippHeap->sippHeap_size  = size;
    pSippHeap->freeCounter    = 0;
    pSippHeap->freePtr        = (u8 *)0x0;

    sippPalMemset (pSippHeap->sippHeap_start,
                   0x00,
                   pSippHeap->sippHeap_size);

    // Setup the first MCB - covering the whole heap area
    // At the moment we will try to NOT use a MAGIC variable
    #ifdef RT_DBG
    *(u32 *)pSippHeap->sippHeap_start       = MCB_MAGIC;
    *(1 + (u32 *)pSippHeap->sippHeap_start) = (u32) (pSippHeap->sippHeap_end - pSippHeap->sippHeap_start);
    #else
    *(u32 *)pSippHeap->sippHeap_start = (u32) (pSippHeap->sippHeap_end - pSippHeap->sippHeap_start);
    #endif
}

SippHeapCB * sippHeapAlloc (tsSippHeap * pSippHeap, u32 size)
{
    if (!size)
        return 0;

    // Include MCB's header in size, and align to 8n
    size = (size + SIPP_HCB_SIZE + 7) & ~7;

    // Start scanning the MCB
    pSippHeapCB t = (pSippHeapCB) pSippHeap->sippHeap_start;
    while((u32) t < (u32) pSippHeap->sippHeap_end)
    {
        u32 bsize = t->used_size & ~0x80000000;
        sippPalTrace (SIPP_HEAP_TL_INFO, "Memory @ 0x%08lx Size = 0x%08lx %s\n", (u32)t, bsize, ( t->used_size >> 31 )? "Used ":"Free \n");

        #ifdef RT_DBG
        // If the current block is invalid, fail
        if (t->magic != MCB_MAGIC)
        {
            sippPalTrace (SIPP_HEAP_TL_INFO, "INVALID\n" );
            return 0;
        }
        #endif
        // If the current block is free
        if(!(t->used_size >> 31))
        {
            // If the current block is large enough
            if(bsize >= size)
            {
                // Create a new MCB with the remaining memory
                u32 next_size = t->used_size - size;
                sippPalTrace (SIPP_HEAP_TL_INFO, "Memory left %08lx\n", next_size);
                // If there is any memory left
                if(next_size)
                {
                    #ifdef RT_DBG
                    // Create a new free MCB
                    *(u32 *)(size + (u32) t)              = (u32) MCB_MAGIC;
                    *(u32 *)(1 + (u32*) (size + (u32) t)) = next_size;
                    #else
                    *(u32 *)(size + (u32) t)              = next_size;
                    #endif
                    sippPalTrace (SIPP_HEAP_TL_INFO, "; Next @0x%08lx\n", size +(u32)t);
                }
                else
                {
                    sippPalTrace (SIPP_HEAP_TL_INFO, "Size\n");
                }

                // Change the size of the current block
                t->used_size = 0x80000000 | size;
                // Zero the pNext entry
                t->pNext     = (void *)NULL;
                // Return a pointer to pSippHeapCB
                return t;
            }
        }
        else
        {
            sippPalTrace (SIPP_HEAP_TL_INFO, "Skipping\n");
        }
        t = (pSippHeapCB) (bsize + (u32) t);
    }

    sippPalTrace (SIPP_HEAP_TL_INFO, "\n");

    // If we got here, no block is large enough, so fail

    return (pSippHeapCB)NULL;
}

void sippHeapDefrag (tsSippHeap * pSippHeap)
{
    // Defragment the free memory
    pSippHeapCB t  = (pSippHeapCB) pSippHeap->sippHeap_start;
    pSippHeapCB t2 = (pSippHeapCB) ((t->used_size & ~0x80000000) + (u32) t);

    while((u32) t2 < (u32) pSippHeap->sippHeap_end)
    {
        // Abort if we went past ptr
        if((u32) t > (u32) pSippHeap->freePtr)
            return;
        // If the blocks at t and t2 are both free
        if(!(t->used_size & 0x80000000) && !(t2->used_size & 0x80000000))
        {
            sippPalTrace (SIPP_HEAP_TL_INFO, "Joining @0x%08X and @0x%08X\n", t, t2);
            // Join them
            t->used_size += t2->used_size;
            #ifdef RT_DBG
            t2->magic     = *((u32*) &"HEAP");
            #endif
            t2->used_size = *((u32*) &"HEAP");
        }
        else
        {
            // Go to the next
            t  = t2;
        }

        t2 = (pSippHeapCB) ((t->used_size & ~0x80000000) + (u32) t);
    }
}

///////////////////////////////////////////////////////////////////////
// API Calls

tsSippHeap * sippHeapCreate (u8 * sippHeap_start, u32 sippHeap_size)
{
    u32 idx;
    tsSippHeap * pSippHeap = (tsSippHeap *)NULL;

    // Find free heap descriptor
    for (idx = 0; idx < SIPP_CFG_MAX_NUM_HEAPS; idx++)
    {
        if (sippHeaps[idx].assigned == 0x0)
        {
            // Mark it as assigned
            sippHeaps[idx].assigned = 0x1;
            pSippHeap               = (tsSippHeap *)&sippHeaps[idx];
            break;
        }
    }

    if (pSippHeap)
    {
        // Initialise the heap space
        sippHeapInit (pSippHeap,
                      sippHeap_start,
                      sippHeap_size);
    }

    // Return a pointer to the heap descriptor
    return pSippHeap;
}


// Allocating a chunk of memory is split into two parts
// Basically we add a wrapper around sippAlloc called sippHeapAlloc to do some config which will allow all data to be freed more easily later on

// sippHeapAlloc - This configures the pNext aspect of the HCB
// --> sippAlloc - This allocates the memory


void * sippAlloc (tsSippHeap * pSippHeap,
                  void *       pPrev,
                  u32          size)
{
    SippHeapCB * pHCB;

    pHCB = sippHeapAlloc (pSippHeap,
                          size);

    if (pHCB)
    {
        if (pPrev)
        {
            SippHeapCB * pPrevHCB = (SippHeapCB *)((u32)pPrev - SIPP_HCB_SIZE);
            pPrevHCB->pNext       = pHCB;
        }

        return pHCB->data;
    }
    else
    {
        if (pSippHeap->freeCounter)
        {
            sippHeapDefrag (pSippHeap);

            pSippHeap->freeCounter = 0;

            pHCB = sippHeapAlloc (pSippHeap,
                                  size);

            if (pHCB)
            {
                if (pPrev)
                {
                    SippHeapCB * pPrevHCB = (SippHeapCB *)((u32)pPrev - SIPP_HCB_SIZE);
                    pPrevHCB->pNext       = pHCB;
                }

                return pHCB->data;
            }
        }
    }

    return NULL;
}

// Note we should NOT use this as an API - it is to be called from
// sippFreeList only - else we need to hold a doubly linked list!!
void sippFree (tsSippHeap * pSippHeap, void * pPtr)
{
    void * ptr;
    ptr = (void *) pPtr;
    // Check that the pointer is aligned properly
    if((u32) ptr & 7)
        return;
    // Check that the pointer is inside the heap
    pSippHeapCB t = (pSippHeapCB) ((u32) ptr - SIPP_HCB_SIZE);
    if ((u32) t < (u32) pSippHeap->sippHeap_start || (u32) t >=
                                                     (u32) pSippHeap->sippHeap_end)
        return;
    // Free the block
    t->used_size &= ~0x80000000;

    pSippHeap->freeCounter++;
    if ((u32)ptr > (u32)pSippHeap->freePtr)
    {
        pSippHeap->freePtr = ptr;
    }

    if (pSippHeap->freeCounter == SIPP_HEAP_DEFRAG_COUNT)
    {
        sippHeapDefrag (pSippHeap);
        pSippHeap->freeCounter = 0;
    }

}

void sippFreeList (tsSippHeap * pSippHeap, void * pStartPtr)
{
    SippHeapCB * pHCB     = (pStartPtr) ? (SippHeapCB *)((u32)pStartPtr - SIPP_HCB_SIZE) :
                                          (SippHeapCB *)0x0;
    SippHeapCB * pNextHCB = (pHCB) ? (SippHeapCB *)pHCB->pNext :
                                     (SippHeapCB *)0x0;

    while (pHCB)
    {
        sippFree (pSippHeap,
                  pHCB->data);

        pHCB     = pNextHCB;
        if (pNextHCB)
        {
            pNextHCB = (SippHeapCB *)pNextHCB->pNext;
        }
    }
}

u32 sippSizeList (tsSippHeap * pSippHeap, void * pStartPtr)
{
    UNUSED (pSippHeap); // Hush the compiler warning
    SippHeapCB * pHCB     = (SippHeapCB *)((u32)pStartPtr - SIPP_HCB_SIZE);
    SippHeapCB * pNextHCB = (SippHeapCB *)pHCB->pNext;
    u32          size     = 0;

    while (pHCB)
    {
        size += pHCB->used_size;
        pHCB  = pNextHCB;

        if (pNextHCB)
        {
            pNextHCB = (SippHeapCB *)pNextHCB->pNext;
        }
    }

    return size;
}

u32 sippHeapCheck(tsSippHeap * pSippHeap)
{
    pSippHeapCB t = (pSippHeapCB) pSippHeap->sippHeap_start;
    u32         freeSpace = 0;

    while((u32) t < (u32) pSippHeap->sippHeap_end)
    {
        u32 bsize = t->used_size & ~0x80000000;
        #ifdef RT_DBG
        // If the current block is invalid, fail
        if (t->magic != MCB_MAGIC)
        {
            sippPalTrace (SIPP_HEAP_TL_INFO, "Invalid\n");
            return 0;
        }
        #endif
        // If the current block is free
        if(!(t->used_size >> 31))
        {
            freeSpace += bsize;
            sippPalTrace (SIPP_HEAP_TL_INFO, "Freespace = %lu \n ", freeSpace);
        }
        else
        {
            sippPalTrace (SIPP_HEAP_TL_INFO, "Freespace = %lu \n ", freeSpace);
        }

        t = (pSippHeapCB) ( bsize + (u32) t);
    }

    return freeSpace;
}
