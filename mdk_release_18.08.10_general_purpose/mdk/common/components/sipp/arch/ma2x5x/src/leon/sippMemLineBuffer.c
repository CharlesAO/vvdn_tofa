///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

////////////////////////////////////////////////////
// Header files
////////////////////////////////////////////////////

#include <sipp.h>
#include <sippInternal.h>

////////////////////////////////////////////////////
// Local prototypes
////////////////////////////////////////////////////

u32 sippMapRegionMapAddrToSliceZero (SippPipeline * pipe,
                                        u32         addrOffset);

////////////////////////////////////////////////////
// Globals
////////////////////////////////////////////////////
extern u8 *sippCmxBase;

////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMapRegionToCmx                                               //
//                                                                                //
//  DESCRIPTION: Taking an input memory region - map it to a CMX slice and add it //
//               to a list for the current pipe                                   //
//                                                                                //
//  INPUTS:      pipe - Pointer to relevant pipeline                              //
//               nextMemRegion - Pointer to memory region definition              //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippMapRegionToCmx (SippPipeline *  pipe,
                         SippMemRegion * nextMemRegion)
{
    pSippMemRegionListNode pListNode;
    pSippMemRegionListNode pPrev;
    u32                    cmxSliceNum;

    // Which CMX slice does this region lie in
    cmxSliceNum = nextMemRegion->regionOffset >> 0x11;   // assumes 128kB sized CMX slices
    pPrev       = pipe->pCmxMap->pCmxSliceRegionList[cmxSliceNum];

    // Before allocating anything - validate that the memory region does not straddle CMX slices
    if ((nextMemRegion->regionOffset + nextMemRegion->regionSize) > ((cmxSliceNum + 1) << 0x11))
    {
        // TBD
        // The region straddles two slices - what error should I return here?
    }

    // Allocate a list node from the CMX pool
    pListNode = (pSippMemRegionListNode)sippMemAlloc (&pipe->tHeapMCB,
                                                      vPoolPipeStructs,
                                                      sizeof(SippMemRegionListNode));

    if (pListNode)
    {
        // Populate the node
        pListNode->pNext         = (void *)0x0;
        pListNode->pNextChunkReg = (void *)0x0;
        pListNode->chainStart    = false;
        pListNode->chainLinked   = false;
        pListNode->regionAddr    = (u32)sippCmxBase + nextMemRegion->regionOffset;
        pListNode->slice0Addr    = sippMapRegionMapAddrToSliceZero (pipe,
                                                                    nextMemRegion->regionOffset);
        pListNode->regionSize    = nextMemRegion->regionSize;
        pListNode->regionUsed    = 0;
        pListNode->regionUsedPtr = &nextMemRegion->regionUsed;

        // Add to list for relevant CMX slice
        if (pPrev)
        {
            pSippMemRegionListNode pNext = (pSippMemRegionListNode)pipe->pCmxMap->pCmxSliceRegionList[cmxSliceNum]->pNext;
            // This is wrong surely - if no pNext exists?
            pSippMemRegionListNode pPrev = pNext;

            // Add to tail
            while (pNext)
            {
                pPrev = pNext;
                pNext = (pSippMemRegionListNode)pipe->pCmxMap->pCmxSliceRegionList[cmxSliceNum]->pNext;
            }

            pPrev->pNext = pListNode;

        }
        else
        {
            pipe->pCmxMap->pCmxSliceRegionList[cmxSliceNum] = pListNode;
        }
    }
    else
    {
        // What error do I raise here?
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGetNextMemRegion                                             //
//                                                                                //
//  DESCRIPTION: Get the next memory region from input list                       //
//                                                                                //
//  INPUTS:      pipe - Pointer to relevant pipeline                              //
//               memRegList - Pointer to null terminated memory region list       //
//                                                                                //
//  OUTPUTS:     ppNextMemRegion -                                                //
//                                                                                //
//  RETURNS:     TRUE if output region valid, FALSE if no further regions in list //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u8 sippGetNextMemRegion (SippPipeline *   pipe,
                         SippMemRegion *  memRegList,
                         SippMemRegion ** ppNextMemRegion)
{
    u8 retVal = true;

    if ((memRegList[pipe->numMemRegions].regionOffset == 0x0) &&
        (memRegList[pipe->numMemRegions].regionSize == 0x0) &&
        (memRegList[pipe->numMemRegions].regionUsed == 0x0))
    {
        retVal = false;
    }
    else
    {
        *ppNextMemRegion = &memRegList[pipe->numMemRegions++];
    }

    return retVal;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMapRegionMapAddrToSliceZero                                  //
//                                                                                //
//  DESCRIPTION: Get the next memory region from input list                       //
//                                                                                //
//  INPUTS:      pipe - Pointer to relevant pipeline                              //
//               memRegList - Pointer to null terminated memory region list       //
//                                                                                //
//  OUTPUTS:     ppNextMemRegion -                                                //
//                                                                                //
//  RETURNS:     TRUE if output region valid, FALSE if no further regions in list //
//                                                                                //
//  NOTES:       The HW filters share the slice config register which means they  //
//               all share a start and end slice. Consider a mixed pipeline of HW //
//               filters with say 2 shaves for the SW filters. The HW will assume //
//               that all of the output buffers addresses we write down are slice //
//               0 addresses. And also that all the filters in the pipe have      //
//               their actual locations in the slices specified in the slice      //
//               config register. Of-course this is exactly what we wish not to   //
//               be the case since the slice config start/end slices will reflect //
//               the shaves that are in use only. So we need to adjust the        //
//               addresses written down so that if for example a region started   //
//               in slice 7 but the pipline's start/end slice setup was 4 & 5 we  //
//               will write down an address modified by only 4 slice widths       //
//                                                                                //
//               When HW does its 'slice correction' on the address, it will end  //
//               up at the correct place                                          //
//                                                                                //
//               This is the same principle as that employed by                   //
//               sippComputeSwOutCt - This looks upon all the addresses as slice  //
//               0 addresses - and corrects them to give the slice addresses      //
//               within the active slice ranges - so this is doing for the SW     //
//               filters what HW will do itself for the HW filters                //
//                                                                                //
//               If  SIPP_SLICE0_ADDRESS is not defined then we do not do this    //
//               correction - instead we write down the actual address and        //
//               instead tell HW that the first slice in use is slice 0. This     //
//               avoids a MoviSim issue with corrected addresses lying outside    //
//               the CMX address map                                              //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippMapRegionMapAddrToSliceZero (SippPipeline * pipe,
                                     u32            addrOffset)
{
    UNUSED (pipe);

	u32 sliceZeroAddr;
    #ifdef SIPP_SLICE0_ADDRESS
    u32 startSlice = pipe->gi.sliceFirst;
    u32 sliceSize  = pipe->gi.sliceSize;

    sliceZeroAddr = (u32)sippCmxBase + addrOffset - startSlice * sliceSize;
    #else
    sliceZeroAddr = (u32)sippCmxBase + addrOffset;
    #endif

    return sliceZeroAddr;
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMemRegionAllocLineBuffer                                     //
//                                                                                //
//  DESCRIPTION: Attempt to allocate a line buffer within regions mapped to the   //
//               specified slice                                                  //
//                                                                                //
//  INPUTS:      pCmxMap - CMX Map for all the allocated memory regions           //
//               cmxSlice - CMX slice number in which line buffer should be       //
//                          allocated                                             //
//               ptrFilt - pointer to the filter whose line buffer is to be       //
//                         allocated                                              //
//                                                                                //
//  OUTPUTS:     TRUE if line buffer could be allocated in the specified slice    //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       none.                                                            //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u8 sippMemRegionAllocLineBuffer (pSippCmxBufferMap        pCmxMap,
                                 u32                      cmxSlice,
                                 SippFilter *             ptrFilt,
                                 u32                      oBufIdx,
                                 pSippMemRegionListNode * ppStartOfChainNode)
{
    pSippMemRegionListNode pMemNode     = pCmxMap->pCmxSliceRegionList[cmxSlice];
    u8                     done         = false;
    u32                    totalBits    = (ptrFilt->lineStride[oBufIdx] * ptrFilt->nLines[oBufIdx] * ptrFilt->nPlanes[oBufIdx]) * ptrFilt->bpp[oBufIdx];
                           totalBits   += 0x7;
                           totalBits   &= 0xFFFFFFF8;
    u32                    requiredSize = totalBits >> 3;

    while (done == false)
    {
        if (pMemNode)
        {
            if (pMemNode->chainStart)
            {
                u32 padBytes = (pMemNode->slice0Addr + pMemNode->regionUsed) & 0x7;

                padBytes = (padBytes) ? 0x8 - padBytes : padBytes;

                if ((pMemNode->regionSize - (pMemNode->regionUsed + padBytes)) > requiredSize)
                {
                    ptrFilt->outputBuffer[oBufIdx] = (u8 *)(pMemNode->slice0Addr + pMemNode->regionUsed + padBytes);
                    ptrFilt->oBufAlloc[oBufIdx]    = 0x1;
                    pMemNode->regionUsed          += (padBytes + requiredSize);
                    done                           = true;
                    *ppStartOfChainNode            = pMemNode;
                }
                else
                {
                    pMemNode = (pSippMemRegionListNode)pMemNode->pNext;
                }
            }
            else
            {
                pMemNode = (pSippMemRegionListNode)pMemNode->pNext;
            }
        }
        else
        {
            return false;
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMemAllocChainChunk                                           //
//                                                                                //
//  DESCRIPTION: Once the first chunk of a line buffer has been allocated, this   //
//               function allocates the remaining chunks in the memory region     //
//               'chain' previously identified by the framework                   //
//                                                                                //
//  INPUTS:      pStartOfChainNode - memory Region in which first chunk was       //
//                                   allocated                                    //
//               numChunks - the number fo chunks in use for the pipeline         //
//                                                                                //
//  OUTPUTS:     TRUE if line buffer could be allocated in the specified chain    //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       none.                                                            //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u8 sippMemAllocChainChunk (pSippMemRegionListNode pStartOfChainNode,
                           u32                    numChunks,
                           SippFilter *           ptrFilt,
                           u32                    oBufIdx)
{
    u8 retVal                              = true;
    pSippMemRegionListNode pChainNode      = (pSippMemRegionListNode)pStartOfChainNode->pNextChunkReg;
    u32                    chunksRemaining = numChunks - 1;
    u32                    totalBits       = (ptrFilt->lineStride[oBufIdx] * ptrFilt->nLines[oBufIdx] * ptrFilt->nPlanes[oBufIdx]) * ptrFilt->bpp[oBufIdx];
                           totalBits      += 0x7;
                           totalBits      &= 0xFFFFFFF8;
    u32                    requiredSize    = totalBits >> 3;

    while (chunksRemaining)
    {
        u32 padBytes = (pChainNode->slice0Addr + pChainNode->regionUsed) & 0x7;

        padBytes = (padBytes) ? 0x8 - padBytes : padBytes;

        if ((pChainNode->regionSize - (pChainNode->regionUsed + padBytes)) > requiredSize)
        {
            pChainNode->regionUsed += (padBytes + requiredSize);
            chunksRemaining--;

            if (chunksRemaining == 0)
            {
                break;
            }
            else
            {
                pChainNode = (pSippMemRegionListNode)pChainNode->pNextChunkReg;
            }
        }
        else
        {
            retVal = false;
            break;
        }

    }

    return retVal;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippConfirmChunkChain                                            //
//                                                                                //
//  DESCRIPTION: Called once a full chain of memory regions is established to     //
//               mark all members appropriately                                   //
//                                                                                //
//  INPUTS:      nodeChain - array of nodes correpsoniding to the chian in order  //
//               chainLength - Number of nodes in the chain                       //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       Called from sippMemLBMatchRegionsToChunks only                   //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippConfirmChunkChain (pSippMemRegionListNode * nodeChain,
                            u32                      chainLength)
{
    u32 chainIdx;

    if (chainLength)
    {
        nodeChain[0]->chainStart = true;
    }

    for (chainIdx = 0; chainIdx < chainLength; chainIdx++)
    {
        nodeChain[chainIdx]->chainLinked = true;
    }

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMemLBMatchRegionsToChunks                                    //
//                                                                                //
//  DESCRIPTION: If chunking is used, this function decides if the memory regions //
//               provided are suitable for use (are there sufficient regions      //
//               spaced at slice stride from each other?)                         //
//                                                                                //
//  INPUTS:      pCmxMap - cmx region map                                         //
//               chunkStride - This is the slice stride for the system - it is    //
//                             the distance apart in memory that the start of one //
//                             chunk should be from the start of the next chunk   //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     true on success (regions are suitable for a chunking system)     //
//                                                                                //
//  NOTES:       Called from sippAssignCmxMemRegion only                          //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u8 sippMemLBMatchRegionsToChunks (pSippCmxBufferMap pCmxMap,
                                  u32               chunkStride,
                                  u32               numChunks)
{
    u32 chainLinks = 0;
    u32 suitableChunkChains = 0;
    u32 idx;
    pSippMemRegionListNode confirmedNodes[CMX_NSLICES];

    // Look at all the regions provided and ascertain if they form part of a group
    // which may be used in a system employing chunking

    // The rule is that the next region must start exactly one stride away from the start address
    // of the current region
    for (idx = 0; idx < CMX_NSLICES; idx++)
    {
        pSippMemRegionListNode pNode = pCmxMap->pCmxSliceRegionList[idx];

        while (pNode)
        {
            // There is at least one region in this CMX slice
            // Check if it is already part of a chain -
            if (pNode->chainLinked == false)
            {
                u8                     chainComplete = false;
                pSippMemRegionListNode pChainNode    = pNode;

                confirmedNodes[chainLinks++] = pChainNode;

                // If the chain reaches a length of numChunks, confirm it as a usable
                // chain and go no further
                if (chainLinks == numChunks)
                {
                    // Set cmxSliceUsageBitMask
                    if (!(pCmxMap->cmxSliceUsageBitMask & (0x1 << idx)))
                    {
                        pCmxMap->numCmxSlicesAvail++;
                    }

                    pCmxMap->cmxSliceUsageBitMask |= (0x1 << idx);

                    sippConfirmChunkChain (confirmedNodes,
                                           chainLinks);
                    chainLinks    = 0;
                    chainComplete = true;
                    suitableChunkChains++;
                }

                while (chainComplete == false)
                {
                    u32                    nextRegCmxLocation, nextRegAddr;
                    u8                     foundNextChunkReg = false;
                    pSippMemRegionListNode pSearchNode;

                    // Which CMX slice will the next chunk start in?
                    nextRegAddr        = pChainNode->regionAddr + chunkStride;
                    nextRegCmxLocation = (nextRegAddr - (u32)sippCmxBase) >> 0x11;

                    pSearchNode = pCmxMap->pCmxSliceRegionList[nextRegCmxLocation];

                    while (pSearchNode)
                    {
                        if (pSearchNode->regionAddr == nextRegAddr)
                        {
                            pChainNode->pNextChunkReg    = pSearchNode;
                            foundNextChunkReg            = true;
                            pChainNode                   = pSearchNode;

                            confirmedNodes[chainLinks++] = pChainNode;

                            // If the chain reaches a length of numChunks, confirm it as a usable
                            // chain and go no further
                            if (chainLinks == numChunks)
                            {
                                // Set cmxSliceUsageBitMask
                                if (!(pCmxMap->cmxSliceUsageBitMask & (0x1 << idx)))
                                {
                                    pCmxMap->numCmxSlicesAvail++;
                                }

                                pCmxMap->cmxSliceUsageBitMask |= (0x1 << idx);

                                sippConfirmChunkChain (confirmedNodes,
                                                       chainLinks);
                                chainLinks    = 0;
                                chainComplete = true;
                                suitableChunkChains++;
                            }
                            break;
                        }
                        else
                        {
                            pSearchNode = (pSippMemRegionListNode)pSearchNode->pNext;
                        }
                    }

                    if (foundNextChunkReg == false)
                    {
                        if (chainComplete == false)
                        {
                            // The chain is not long enough but is now completed
                            chainLinks    = 0;
                            break;
                        }
                    }
                }
            }

            pNode = (pSippMemRegionListNode)pNode->pNext;

        }
    }

    return (suitableChunkChains) ? true : false;
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMemLBRemoveNode                                              //
//                                                                                //
//  DESCRIPTION: Remove a region node form teh linked list for specified slice    //                           //
//                                                                                //
//  INPUTS:      pCmxMap - cmx region map                                         //
//               sliceIdx - slice index                                           //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     TRUE if found and rmoved.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

bool sippMemLBRemoveNode (pSippCmxBufferMap      pCmxMap,
                          u32                    sliceIdx,
                          pSippMemRegionListNode pNodeRemove)
{
    pSippMemRegionListNode pNode     = pCmxMap->pCmxSliceRegionList[sliceIdx];
    pSippMemRegionListNode pPrevNode = (pSippMemRegionListNode)NULL;
    bool                   retVal    = FALSE;

    while (pNode)
    {
        if (pNode == pNodeRemove)
        {
            if (pPrevNode)
            {
                pPrevNode->pNext = pNode->pNext;
            }
            else
            {
                pCmxMap->pCmxSliceRegionList[sliceIdx] = pNode->pNext;
            }
            retVal = TRUE;
            break;
        }
        else
        {
            pNode = pNode->pNext;
        }
    }

    return retVal;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMemLBConsolidateRegions                                      //
//                                                                                //
//  DESCRIPTION: Each entry in the linked list of regions provided cannot         //
//               straddle a slice boundary. However when opipe runtime in use     //
//               they are most useful as contiguous regions unaware of slices     //
//               This function establishes them as such                           //
//                                                                                //
//  INPUTS:      pCmxMap - cmx region map                                         //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Called from sippAssignCmxMemRegion only                          //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippMemLBConsolidateRegions (pSippCmxBufferMap pCmxMap)
{
    pSippMemRegionListNode pTrialNode, pNode;
    u32                    idx, trialIdx, regionEnd;
    bool                   continueSearch;

    for (idx = 0; idx < CMX_NSLICES; idx++)
    {
        pNode = pCmxMap->pCmxSliceRegionList[idx];

        trialIdx = idx;

        // Any node in any slice could be contiguous with any node in the next slice
        // if it is the combined node could in turm be contiguous with some subsequent
        // other node in the next slice or in some subsequent slice if there are no further nodes
        // Each node's path must be followed through to exhaustion, the contiguous nodes merged into
        // it and removed as individual entities themselves and then the process continued until
        // we are left with one or more non contiguous regions of memory
        while (pNode)
        {
            regionEnd      = pNode->regionSize + pNode->regionAddr;
            continueSearch = FALSE;

            if ((regionEnd & 0x1FFFF) == 0)
            {
                if (idx == (CMX_NSLICES - 0x1))
                {
                    break;
                }
                trialIdx++;
            }

            pTrialNode = pCmxMap->pCmxSliceRegionList[trialIdx];

            while (pTrialNode)
            {
                if (pTrialNode != pNode)
                {
                    if (pTrialNode->regionAddr == regionEnd)
                    {
                        pNode->regionSize += pTrialNode->regionSize;

                        // Remove trial node
                        sippMemLBRemoveNode (pCmxMap,
                                             trialIdx,
                                             pTrialNode);

                        // Merge this loop so try again...
                        continueSearch = TRUE;
                    }
                }

                pTrialNode = pTrialNode->pNext;
            }

            // Node chain exhasted, move on
            if (continueSearch == FALSE)
            {
                pNode = pNode->pNext;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAssignCmxMemRegion                                           //
//                                                                                //
//  DESCRIPTION: Run through the memory region list allocated, mapping each to    //
//               CMX slices and associate all this info with the relevant pipe    //
//                                                                                //
//  INPUTS:      pipe - pipeline to which regions are to be assigned              //
//               memRegList - NULL terminated memory region list                  //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     0 on success (only a failed sippMemAlloc call can prevent this)  //
//                                                                                //
//  NOTES:       Called from main API (sippAllocCmxMemRegion) only                //
//               If chunking is employed in this system i.e. more than one shave  //
//               is in use then each allocated line buffer must be split up among //
//               a number of regions (number equal to number chunks) and so the   //
//               regions are dealt with in terms of chains of regions. In this    //
//               way a non-chunking system is simply a chain of regions having a  //
//               chain length of 1                                                //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

s32 sippAssignCmxMemRegion (SippPipeline *  pipe,
                            SippMemRegion * memRegList)
{
    SippMemRegion * nextMemRegion;

    pipe->pCmxMap = (pSippCmxBufferMap)sippMemAlloc (&pipe->tHeapMCB,
                                                     vPoolPipeStructs,
                                                     sizeof(SippCmxBufferMap));

    if (pipe->pCmxMap)
    {

        sippPalMemset ((void *)pipe->pCmxMap,
                       0x0,
                       sizeof(SippCmxBufferMap));

        while (sippGetNextMemRegion (pipe,
                                     memRegList,
                                     &nextMemRegion))
        {
            pipe->pCmxMap->totalMem += nextMemRegion->regionSize;

            sippMapRegionToCmx (pipe,
                                nextMemRegion);
        }
    }
    else
    {
        return -1;
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAllocCmxLineBuffers                                          //
//                                                                                //
//  DESCRIPTION: Allocate the output line buffers for all suitable filters in the //
//               input pipeline                                                   //
//                                                                                //
//  INPUTS:      pipe - input pipeline                                            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     0 on success, -1 on failure                                      //
//                                                                                //
//  NOTES:       Called by sippBuildLnBuffs                                       //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

s32 sippAllocCmxLineBuffers (SippPipeline * pipe)
{
    SippFilter *      ptrFilt;
    u32               idx, oBufIdx;
    s32               retVal    = 0x0;  /* Default to success */
    pSippCmxBufferMap pCmxMap   = pipe->pCmxMap;
    u8                done      = false;
    u32               numChunks = pipe->gi.sliceLast - pipe->gi.sliceFirst + 1;
    u32               candidateFilterList[SIPP_MAX_BUFFERS_PER_PIPELINE];
    u32               candidateFilterBufIdx[SIPP_MAX_BUFFERS_PER_PIPELINE];
    cmxRegUsage       availCmxRegions[CMX_NSLICES];
    cmxRegUsage *     sortedCmxRegions[CMX_NSLICES];
    u32               cmxRegionUsedCount[CMX_NSLICES];
    u32               cmxRegionsIdx = 0, numCmxRegions;
    u32               candidateListIndex = 0, numCandidates, totalCMXBuffers;

    // Identify HW filters with only HW consumers
    //
    // Note: In future maybe SW filters with only HW consumers would also be suitable candidates??
    // It would not be too bad to go 'off-piste' for the 1 output pixel per operation would it?
    // This would mean that sippComputePaddingOffsets would have to change a little though? Or would it?

    for (idx = 0, numCandidates = 0, totalCMXBuffers = 0; idx < pipe->nFilters; idx++)
    {
        // Ref to current filter
        ptrFilt = pipe->filters[idx];

        for (oBufIdx = 0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
        {
            if (ptrFilt->nLines[oBufIdx])
            {
                totalCMXBuffers++;

                if (ptrFilt->unit <= SIPP_MAX_ID)
                {
                    if (ptrFilt->oBufs[oBufIdx]->numSWConsumers == 0)
                    {
                        candidateFilterBufIdx[numCandidates] = oBufIdx;
                        candidateFilterList[numCandidates++] = idx;
                    }
                }
            }
        }
    }

    for (idx = 0, numCmxRegions = 0; idx < CMX_NSLICES; idx++)
    {
        // This means that at least one chain of regions starts in this cmx slice
        if ((pCmxMap->cmxSliceUsageBitMask >> idx) & 0x1)
        {
            sortedCmxRegions[numCmxRegions]             = &availCmxRegions[numCmxRegions];
            availCmxRegions[numCmxRegions].cmxRegionIdx = idx;
            availCmxRegions[numCmxRegions].fullCmxSlice = false;
            availCmxRegions[numCmxRegions++].usedCount  = 0;
        }
    }

    // Now add the full CMX slices - actually i should only add the chains
    // so if there are 2 shaves but sw filters - only add the first slice
    // if for 2x5x there were 2 slices but no SW filters - assume we would not chunk and so add both
    if ((pCmxMap->cmxSliceUsageBitMask >> pipe->gi.sliceFirst) & 0x1)
    {
        u8 sortRequired = false;

        for (idx = 0; idx < numCmxRegions; idx++)
        {
            if (availCmxRegions[idx].cmxRegionIdx == pipe->gi.sliceFirst)
            {
                availCmxRegions[numCmxRegions].usedCount  = totalCMXBuffers - numCandidates;
                sortRequired                              = (totalCMXBuffers - numCandidates) ? true : false;
                break;
            }
        }

        if (sortRequired)
        {
            // The entry currently at index needs to go to the end of the list with all other entries being moved up
            if (idx < numCmxRegions)
            {
                cmxRegUsage * temp =  sortedCmxRegions[idx];

                for (; idx < (numCmxRegions - 1); idx++)
                {
                    sortedCmxRegions[idx] = sortedCmxRegions[idx+1];
                }
                sortedCmxRegions[idx] = temp;
            }
        }
    }
    else
    {
        sortedCmxRegions[numCmxRegions]             = &availCmxRegions[numCmxRegions];
        availCmxRegions[numCmxRegions].cmxRegionIdx = pipe->gi.sliceFirst;
        availCmxRegions[numCmxRegions].fullCmxSlice = true;
        availCmxRegions[numCmxRegions++].usedCount  = totalCMXBuffers - numCandidates;
    }

    // The sorted list contains the full CMX slices at its tail as clearly they are the only
    // regions which can be effectively assigned filter output buffers by this stage - all the
    // filters which are not candidates in this process will have their output line buffers
    // assigned there

    if (numCandidates == 0)
    {
        done = true;
    }

    // Attempt to assign the candidate filter line buffers
    while ( done == false )
    {
        SippFilter *           ptrFilt         = pipe->filters[candidateFilterList[candidateListIndex]];
        u32                    candidateBufIdx = candidateFilterBufIdx[candidateListIndex];
//        psSchLineBuffer        ptrBuf          = ptrFilt->oBufs[candidateBufIdx];
        u8                     success         = false;
        pSippMemRegionListNode pStartOfChainNode;

        if (sortedCmxRegions[cmxRegionsIdx]->fullCmxSlice)
        {
            u32 planeSize = ptrFilt->lineStride[candidateBufIdx] * ptrFilt->nLines[candidateBufIdx];

            u32                         totalBits  = (ptrFilt->nPlanes[candidateBufIdx] * planeSize) * ptrFilt->bpp[candidateBufIdx];
                                        totalBits += 0x7;
                                        totalBits &= 0xFFFFFFF8;
            ptrFilt->outputBuffer[candidateBufIdx] = (u8 *)sippMemAlloc (&pipe->tHeapMCB,
                                                                         vPoolFilterLineBuf,
                                                                         totalBits >> 3);
            ptrFilt->oBufAlloc[candidateBufIdx]    = 0x1;
            success                                = true;
        }
        else
        {
            if (sippMemRegionAllocLineBuffer (pCmxMap,
                                              sortedCmxRegions[cmxRegionsIdx]->cmxRegionIdx,
                                              ptrFilt,
                                              candidateBufIdx,
                                              &pStartOfChainNode))
            {
                success = true;
            }
        }

        if (success == false)
        {
            cmxRegionsIdx++;
            if (cmxRegionsIdx == numCmxRegions)
            {
                // Unable to find a home for the current filter's output buffer
                // This means the full CMX slice failed in this case also - so we are a bit screwed!!
                retVal = -1;
                done = true;
            }
        }
        else
        {
            // Technically we should look at chunking now -
            // if it is in place for this pipeline need to find other spaces at exact multiples
            // of stride away
            // Of-course this only needs to be done for regions allocated via the sippAllocCmxMemRegion
            // API, a line buffer in a region allocated in one of the full CMX slices assigned to the
            // pipeline will inherently be covered in terms of chunking needs in the other slices which
            // effectively set the parameters for chunking (number of full CMX slices == number of chunks)
            if (sortedCmxRegions[cmxRegionsIdx]->fullCmxSlice == false)
            {
                if (numChunks > 0x1)
                {
                    // Need to ensure the remaining links in the chain have sufficient
                    sippMemAllocChainChunk (pStartOfChainNode,
                                            numChunks,
                                            ptrFilt,
                                            candidateBufIdx);

                    // TODO - if this fails need to undo the initial call to sippMemRegionAllocLineBuffer
                }
            }


            candidateListIndex++;

            sortedCmxRegions[cmxRegionsIdx]->usedCount++;

            if (candidateListIndex == numCandidates)
            {
                done = true;
            }
            else
            {
                for (idx = 0; idx < numCmxRegions; idx++)
                {
                    sortedCmxRegions[idx]   = &availCmxRegions[idx];
                    cmxRegionUsedCount[idx] = availCmxRegions[idx].usedCount;
                }

                // Sort the list based on the number of times they have been used
                sippListSort ((s32 *)sortedCmxRegions,
                              (s32 *)cmxRegionUsedCount,
                              numCmxRegions,
                              false);

                cmxRegionsIdx = 0;
            }
        }
    }

    // Update all the used pointers in the original SippMemRegion list (for analysis by the client)
    for (idx = 0; idx < CMX_NSLICES; idx++)
    {
        if ((pCmxMap->cmxSliceUsageBitMask >> idx) & 0x1)
        {
            pSippMemRegionListNode pMemNode = pCmxMap->pCmxSliceRegionList[idx];

            while (pMemNode)
            {
                *pMemNode->regionUsedPtr = pMemNode->regionUsed;
                pMemNode                 = (pSippMemRegionListNode)pMemNode->pNext;
            }
        }
    }

    return retVal;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAllocCmxLineBuffersOPipe                                     //
//                                                                                //
//  DESCRIPTION: Allocate the output line buffers for all suitable filters in the //
//               input pipeline                                                   //
//                                                                                //
//  INPUTS:      pipe - input pipeline                                            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     0 on success, -1 on failure                                      //
//                                                                                //
//  NOTES:       Called by sippBuildLnBuffsOPipe                                  //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

s32 sippAllocCmxLineBuffersOPipe (SippPipeline * pipe)
{
    u32                 i, o, idx, totalBits;
    SippFilter *        fptr;
    u32                 filtOBufsSize[SIPP_MAX_FILTERS_PER_PIPELINE];
    u32                 numOBufsAlloc = 0;
    u32                 numRegions, regionIdx;
    pmemRegDescriptor   pRegList;
    sippOpipeBufInfo    oBufInfo[CMX_NSLICES];
    sippOpipeBufInfo *  sortedOBufInfo[CMX_NSLICES];
    bool                doAlloc;

    for (i = 0; i < pipe->nFilters; i++)
    {
        fptr = pipe->filters[i];

        // Filters requiring shifted planes are not considered for allocation here
        if ((fptr->flags & SIPP_SHIFT_PLANES) == 0)
        {
            for (o = 0; o < fptr->numOBufs; o++)
            {
                if (fptr->nLines[o] > 0) // If it has an output buffer:
                {
                    if (fptr->oBufAlloc[o] == 0)   // Wasn't already allocated in an alternative CMX region
                    {
                        // Compute memory size of output buffer that needs allocated
                        u32 planeSize = fptr->lineStride[o] * fptr->nLines[o];
                        totalBits     = (fptr->nPlanes[o] * planeSize) * fptr->bpp[o];
                        totalBits    += 0x7;
                        totalBits    &= 0xFFFFFFF8;

                        // Add all filters that have output buffers in a list
                        sortedOBufInfo[numOBufsAlloc]   = &oBufInfo[numOBufsAlloc];
                        oBufInfo[numOBufsAlloc].ptrFilt = fptr;
                        oBufInfo[numOBufsAlloc].oBufIdx = o;
                        filtOBufsSize[numOBufsAlloc]    = totalBits >> 3;
                        numOBufsAlloc++;
                    }
                }
            }
        }
    }

    // Sort the list of filters that have output buffers based on the memory size
    sippListSort ((s32 *)sortedOBufInfo,
                  (s32 *)filtOBufsSize,
                  numOBufsAlloc,
                  true);

    // Make a convenient list of all the memory regions
    // Frst, how many are we left with...
    for (idx = 0, numRegions = 0; idx < CMX_NSLICES; idx++)
    {
        pSippMemRegionListNode pNode = pipe->pCmxMap->pCmxSliceRegionList[idx];

        while (pNode)
        {
            numRegions++;
            pNode = pNode->pNext;
        }
    }

    // Allocate memory for convenient descriptor for each
    pRegList = (pmemRegDescriptor)sippMemAlloc (&pipe->tHeapMCB,
                                                (SippVirtualPool)(vPoolScheduleTemp),
                                                sizeof(memRegDescriptor) * numRegions);

    // Populate these descriptors
    for (idx = 0, regionIdx = 0; idx < CMX_NSLICES; idx++)
    {
        pSippMemRegionListNode pNode = pipe->pCmxMap->pCmxSliceRegionList[idx];

        while (pNode)
        {
            pRegList[regionIdx].freeBase      = pNode->regionAddr;
            pRegList[regionIdx].sizeRemaining = pNode->regionSize;

            regionIdx++;
            pNode = pNode->pNext;
        }
    }

    // Now attempt to allocate all candidates...
    for (i = 0; i < numOBufsAlloc; i++)
    {
        fptr = sortedOBufInfo[i]->ptrFilt;
        o    = sortedOBufInfo[i]->oBufIdx;

        #ifdef SIPP_PC
        if (pipe->dbgLevel > 0)
        {
            sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "sippAllocCmxLineBuffersOPipe - Filter %u Buffer %u Size %u\n",
                                                  fptr->unit,
                                                  o,
                                                  filtOBufsSize[i]);
        }
        #endif

        regionIdx = 0;
        doAlloc   = TRUE;

        while (pRegList[regionIdx].sizeRemaining < filtOBufsSize[i])
        {
            if (regionIdx < (numRegions - 0x1))
            {
                regionIdx++;
            }
            else
            {
                doAlloc = FALSE;
                break;
            }
        }

        if (doAlloc == FALSE)
        {
            #ifdef RT_DBG
            sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "sippAllocCmxLineBuffersOPipe : fptr[%d]->outputBuffer[%d] = %x (Lines = %d)\n", fptr->unit, o, fptr->outputBuffer[o], fptr->nLines[o]);
            #endif
        }
        else
        {
            fptr->outputBuffer[o]              = (u8 *)pRegList[regionIdx].freeBase;
            fptr->oBufAlloc[o]                 = 0x1;
            pRegList[regionIdx].freeBase      += filtOBufsSize[i];
            pRegList[regionIdx].sizeRemaining -= filtOBufsSize[i];
        }
    }

    // DEBUG: At the end of all allocs, display the status
    sippMemStatus ();

    return 0x0;
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAllocCmxMemRegion                                            //
//                                                                                //
//  DESCRIPTION: Run through the memory region list allocated, mapping each to    //
//               CMX slices and associate all this info with the relevant pipe    //
//                                                                                //
//  INPUTS:      pipe - pipeline to which regions are to be assigned              //
//               memRegList - NULL terminated memory region list                  //
//                                                                                //
//  OUTPUTS:                                                                      //
//                                                                                //
//  RETURNS:                                                                      //
//                                                                                //
//  NOTES:       This function is a main SIPP API                                 //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

// For myriad X consider changing this to allocMemregion - the CMX bit should be lost sio it can be in DDR
s32 sippAllocCmxMemRegion (SippPipeline *  pipe,
                           SippMemRegion * memRegList)
{
    s32 retVal;

    if (0 == sippAssignCmxMemRegion (pipe,
                                     memRegList))
    {
        pipe->useCmxRegMap = 1;

        // Call to finalise the pipe
        sippFinalizePipeline (pipe);

        // Then a check on the pipeline code?
        retVal = pipe->cmxMapResult;
    }
    else
    {
        retVal = -3;
    }

    return retVal;
}
