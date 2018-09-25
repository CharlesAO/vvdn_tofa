///
/// @file      sippOPipeSchedulingEntity.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Establish the OPipe Scheduling Entities within a pipeline
///            Platform(s) supported : ma2x5x
///

#include <sipp.h>
#include <sippInternal.h>

////////////////////////////////////////////////////
// Local macros
////////////////////////////////////////////////////

#define SIPP_OPIPE_FILTER_LIST ((1 << SIPP_SIGMA_ID) |\
                                (1 << SIPP_LSC_ID) |\
                                (1 << SIPP_RAW_ID) |\
                                (1 << SIPP_DBYR_ID) |\
                                (1 << SIPP_DOGL_ID) |\
                                (1 << SIPP_LUMA_ID) |\
                                (1 << SIPP_SHARPEN_ID) |\
                                (1 << SIPP_CGEN_ID) |\
                                (1 << SIPP_MED_ID) |\
                                (1 << SIPP_CHROMA_ID) |\
                                (1 << SIPP_CC_ID) |\
                                (1 << SIPP_LUT_ID) |\
                                (1 << SIPP_EDGE_OP_ID) |\
                                (1 << SIPP_CONV_ID) |\
                                (1 << SIPP_HARRIS_ID) |\
                                (1 << SIPP_UPFIRDN0_ID) |\
                                (1 << SIPP_UPFIRDN1_ID) |\
                                (1 << SIPP_UPFIRDN2_ID) |\
                                (1 << SIPP_MIPI_TX0_ID) |\
                                (1 << SIPP_MIPI_TX1_ID) |\
                                (1 << SIPP_MIPI_RX0_ID) |\
                                (1 << SIPP_MIPI_RX1_ID) |\
                                (1 << SIPP_MIPI_RX2_ID) |\
                                (1 << SIPP_MIPI_RX3_ID))

#define SIPP_OCTOPUS_MASK      ((1 << SIPP_SIGMA_ID) |\
                                (1 << SIPP_LSC_ID) |\
                                (1 << SIPP_RAW_ID) |\
                                (1 << SIPP_DBYR_ID) |\
                                (1 << SIPP_DOGL_ID) |\
                                (1 << SIPP_LUMA_ID) |\
                                (1 << SIPP_SHARPEN_ID) |\
                                (1 << SIPP_CGEN_ID) |\
                                (1 << SIPP_MED_ID) |\
                                (1 << SIPP_CHROMA_ID) |\
                                (1 << SIPP_CC_ID) |\
                                (1 << SIPP_LUT_ID))

////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOSEProcessFilterIBufs                                        //
//                                                                                //
//  DESCRIPTION: Process a filters input buffer details, adding them fully to the //
//               OSE                                                              //
//                                                                                //
//  INPUTS:      pOSE   - The oPipe Scheduling Entity to which the filter is      //
//                        being added                                             //
//               ptrFilt - the filter                                             //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     0x0 if success - 1 if processing failure                         //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippOSEProcessFilterIBufs (SippOse    * pOSE,
                               SippFilter * ptrFilt)
{
    u32  maxOSELatencyParBuf = 0;
    u32  iBufIdx             = pOSE->numIBufs;
    u32  parentIdx, oBufIdx;
    u32  subSampleScale      = 0;
    #ifdef SIPP_SCRT_ENABLE_OPIPE
    bool oPipeRuntime        = (ptrFilt->pPipe->pfnSippRuntime == sippOPipeRuntime) ? TRUE : FALSE;
    #else
    bool oPipeRuntime        = FALSE;
    #endif

    // Does it have any input buffers that come from outside the OSE?
    // This is the nitty gritty
    // We have the following info for the filter
    // 1) We know what its parents are
    // 2) We have sippCheckOPipeConnectionXXX to check if a parent is an oPipe parent
    //      (we need only have established that 1 parent is an oPipe connectionat this stage)
    // 3) We know the input and output latencies of this filter and that of the parents
    //    if we expanded sippSetBufLatenciesXXXX to check the parent type and set the latencies accordingly
    //    then by this stage we should be confident to just do a one to one map between a parent filter
    //    index and the input buffer index - i.e. parent[X] provides iBufConsLatency[x]

    // So we should attempt to check the parent connections and if they are NOT oPipe we have some new input
    // buffers for this OSE
    for (parentIdx = 0; parentIdx < ptrFilt->nParents; parentIdx++)
    {
        u32          oBufIdx, consumerIdx;
        SippFilter * ptrParFilt   = ptrFilt->parents[parentIdx];
        u32          oPipeFiltCfg = gSipp.uInfo[ptrFilt->unit].hwCheckOpipeCons (ptrFilt, ptrParFilt, &oBufIdx);
        u32          foundLink;
        u32          foundIdx[SIPP_SE_MAX_IBUFS] = {0};

        if (oPipeFiltCfg == 0)
        {
            // This parent is not compliant with an oPipe connection to the current filter
            // From this we can assume that the parent will never be a member of this
            // OSE so the parent connection to this filter within the OSE now becomes
            // a non oPipe connection and so the OSE has another valid input buffer
            // Fill in the data needed by the scheduling algorithm

            // Modify HW filters with LLB which are inputs to the OSE to ensure they are fully stacked
            // before starting
            if ((0x1 << ptrFilt->unit) & SIPP_FILTER_LLB_MASK)
            {
                ptrFilt->iBufCtx[parentIdx]->numLinesUsed += ptrFilt->linesPerIter;
            }

            pOSE->iBufs[iBufIdx]     = ptrFilt->iBufs[parentIdx];
            pOSE->iBufCtx[iBufIdx]   = ptrFilt->iBufCtx[parentIdx];
            pOSE->ptrFiltKS[iBufIdx] = &ptrFilt->parentsKS[parentIdx];

            // Need to update the consIbufIdx[] of the parent

            // Is this parent itself already part of an OSE?
            if (ptrParFilt->memOSE)
            {
                // Find the address of this filters SE ptr in the parent OSE's list of
                // consumer SEs
                SippOse * pParentOSE = ptrParFilt->pOSE;

                #ifdef RT_DBG
                if (oPipeRuntime == FALSE)
                {
                    if (pOSE == pParentOSE)
                    {
                        // The parent filter is saying it is part of the same OSE
                        // yet we think no oPipe connection may be made between the two filters!!
                        sippError (ptrFilt->errorStatus, E_OSE_CREATION_ERROR);
                        return 0x1;
                    }
                }
                #endif

                // To cater for a situation where a filter is linked multiple times to another filter
                // count the number of times the link is found
                for (consumerIdx = 0, foundLink = 0; consumerIdx < pParentOSE->numConsumers; consumerIdx++)
                {
                    if (pParentOSE->consSE[consumerIdx] == (&(ptrFilt->sch)))
                    {
                        foundIdx[foundLink++] = consumerIdx;
                    }
                }

                if (foundLink)
                {
                    // The standard situation is a filter is linked once to another filter
                    if (foundLink == 1)
                    {
                        pParentOSE->consIbufIdx[foundIdx[0]] = iBufIdx;
                    }
                    else
                    {
                        u32 seekIdx, linkCount;

                        // If a filter has been linked multiple times to another filter -
                        // count how often up to this point in the parent search for the current filter
                        for (seekIdx = 0, linkCount = 0; seekIdx < parentIdx + 1; seekIdx++)
                        {
                            if (ptrParFilt == ptrFilt->parents[parentIdx]) linkCount++;
                        }

                        // Use the current tally to index the found location list created above
                        // Use that in turn to access pParentOSE->consIbufIdx
                        pParentOSE->consIbufIdx[foundIdx[linkCount-1]] = iBufIdx;
                    }
                }
            }
            else
            {
                // Find this filter in the consumer list of the parent
                // To cater for a situation where a filter is linked multiple times to another filter
                // count the number of times the link is found
                for (consumerIdx = 0, foundLink = 0; consumerIdx < ptrParFilt->nCons; consumerIdx++)
                {
                    if (ptrParFilt->cons[consumerIdx] == ptrFilt)
                    {
                        foundIdx[foundLink++] = consumerIdx;
                    }
                }

                if (foundLink)
                {
                    // The standard situation is a filter is linked once to another filter
                    if (foundLink == 1)
                    {
                        if (oPipeRuntime == FALSE) ptrParFilt->consIbufIdx[foundIdx[0]] = iBufIdx;
                    }
                    else
                    {
                        u32 seekIdx, linkCount;

                        // If a filter has been linked multiple times to another filter -
                        // count how often up to this point in the parent search for the current filter
                        for (seekIdx = 0, linkCount = 0; seekIdx < parentIdx + 1; seekIdx++)
                        {
                            if (ptrParFilt == ptrFilt->parents[parentIdx]) linkCount++;
                        }

                        // Use the current tally to index the found location list created above
                        // Use that in turn to access ptrParFilt->consIbufIdx
                        if (oPipeRuntime == FALSE) ptrParFilt->consIbufIdx[foundIdx[linkCount-1]] = iBufIdx;
                    }
                }
            }

            #if RT_DBG
            // Check for an error in the search above
            if (foundLink == 0)
            {
                sippError (ptrFilt->errorStatus, E_DATA_NOT_FOUND);
                return 0x1;
            }
            #endif

            iBufIdx++;

        }
        else
        {
            u32 nlinesUsedParent;

            // The parent filter is an oPipe filter and the link betwen that parent and the current filter
            // is allowable as part of an OSE
            // Since we have ordered based on standard oPipe flow the parent should already have been
            // added to the current OSE
            pOSE->OSEFiltCfg |= oPipeFiltCfg;

            // Set an invalid parentKS
            ptrFilt->parentsKS[parentIdx] = SIPP_INVALID_PARENT_KS;

            // In order to maximise throughput, all constituent parts of the OSE should have their inputs stacked though
            // That is to say if the final filter in a chain has a kernel size of 5, we should not look for
            // output until there are 3 lines at its input. Likewise for the penultimate filter and so on.
            // This means that when eventually space is created in the output buffer, filtering from the final filter
            // will commence immediately, filtering in the penultimate filter a very brief time later etc
            // This stacking can only occur when a filter has an LLB (when it doesn't this is no real issue as it is in
            // effect absorbed into the datapath of the previous filter)

            if (oPipeRuntime)
            {
                nlinesUsedParent = ((0x1 << ptrFilt->unit) & SIPP_FILTER_FULL_LLB_MASK) ? (ptrFilt->iBufCtx[parentIdx]->numLinesUsed >> 1) + 0x1 :
                                                                                          0x0;

                // We need to consider if this filter is operating at a subsampled rate with respect to ANY other filter in the OSE
                // This is only applicable for chroma pipes using the oPipe runtime which enables OSE connections of these units to
                // other filters so we should leverage that information in this check to make it as simple as possible
                if (ptrFilt->linesPerIter == 0)
                {
                    nlinesUsedParent <<= 0x1;
                    subSampleScale     = 0x1;
                }
            }
            else
            {
                nlinesUsedParent = ((0x1 << ptrFilt->unit) & SIPP_FILTER_LLB_MASK) ? (ptrFilt->iBufCtx[parentIdx]->numLinesUsed >> 1) + 0x1 :
                                                                                     0x0;
            }

            if ((ptrParFilt->oseLatency[oBufIdx] + nlinesUsedParent) > maxOSELatencyParBuf)
            {
                maxOSELatencyParBuf = ptrParFilt->oseLatency[oBufIdx] + nlinesUsedParent;
            }
        }
    }

    // Any additional input buffers now need to have their consumption latency updated
    while (iBufIdx != pOSE->numIBufs)
    {
        pOSE->iBufCtx[pOSE->numIBufs++]->consumptionLatency += maxOSELatencyParBuf;

        // This also means they need to have their funcAsk function updated if they have gone from a 0 consumption latency to a non-zero one
    }

    // Set the oseLatency for this filter's output buffers which is the number of iterations
    // of the OSE needed to see the first line appear in this filter's output buffer
    // This is needed for further consumers' calls to sippOSEProcessFilterIBufs as
    // well as by the schedule creation algorithms so keeping the result in two places for
    // convenience
    for (oBufIdx = 0; oBufIdx < ptrFilt->numOBufs; oBufIdx++)
    {
        ptrFilt->oseLatency[oBufIdx]     = maxOSELatencyParBuf + (ptrFilt->OBufProdLatency[oBufIdx] << subSampleScale);
        ptrFilt->oBufs[oBufIdx]->latency = ptrFilt->oseLatency[oBufIdx];
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOSEProcessFilterOBufs                                        //
//                                                                                //
//  DESCRIPTION: Process a filters output buffer details, adding them fully to    //
//               the OSE                                                          //
//                                                                                //
//  INPUTS:      pOSE   - The oPipe Scheduling Entity to which the filter is      //
//                        being added                                             //
//               ptrFilt - the filter                                             //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippOSEProcessFilterOBufs (SippOse    * pOSE,
                                SippFilter * ptrFilt)
{
    u32 consumerIdx;

    // Does it have any output buffers that go outside the OSE?
    // This is the nitty gritty
    // We have the following info for the filter
    // 1) We know what its consumers are
    // 2) we have sippCheckOPipeConnectionXXX to check if the parent is an oPipe parent
    //      (surely we have already established thatat this stage)
    // 3) we know the input and output latencies of this filter and that of the parents
    //    if we expanded sippSetBufLatenciesXXXX to check the parent type and set teh latencies accordingly
    //    then by this stage we should be confident to just do a one to one map between a parent filter
    //    index and the input buffer index - i.e. parent[X] provides iBufConsLatency[x]

    // This is not so straightforward here as there may be multiple consumers per output buffer
    for (consumerIdx = 0; consumerIdx < ptrFilt->nCons; consumerIdx++)
    {
        u32          oBufIdx      = 0;
        SippFilter * ptrConsFilt  = ptrFilt->cons[consumerIdx];
        u32          oPipeFiltCfg = 0;

        if ((1 << ptrConsFilt->unit) & SIPP_OPIPE_FILTER_LIST)
        {
            oPipeFiltCfg = gSipp.uInfo[ptrConsFilt->unit].hwCheckOpipeCons (ptrConsFilt, ptrFilt, &oBufIdx);
        }

        if (oPipeFiltCfg == 0)
        {
            psSchLineBuffer consOBuf;

            // This consumer is not compliant with an oPipe connection to the current filter so this becomes
            // a non oPipe connection and so another valid consumer

            // The consumer filter should not yet have a scheduling entity created
            // so use a ptr to pointer so we can acess its value when we need it by which time it
            // will be populated
            // Of-course it may already be or end up as a member of another OSE...
            // Either way, note the consumer filter and the appropriate SE will be substituted
            // when the SE is created for this OSE

            pOSE->consSE[pOSE->numConsumers]      = &ptrConsFilt->sch;
            pOSE->consIbufIdx[pOSE->numConsumers] = ptrFilt->consIbufIdx[consumerIdx];
            pOSE->numConsumers++;

            // Whichever oBUF this consumer pointed to in the filter oBUF list must be added to the OSE oBUF
            // list - unless it is already in the list!
            consOBuf = ptrConsFilt->iBufs[ptrFilt->consIbufIdx[consumerIdx]];

            for (oBufIdx = 0; oBufIdx < pOSE->numOBufs; oBufIdx++)
            {
                if (pOSE->oBufs[oBufIdx] == consOBuf)
                {
                    ptrConsFilt->iBufs[ptrFilt->consIbufIdx[consumerIdx]] = pOSE->oBufs[oBufIdx];
                    break;
                }
            }

            if (oBufIdx == pOSE->numOBufs)
            {
                // We did not find this output buffer in the current list so add it now
                pOSE->oBufs[pOSE->numOBufs]                           = consOBuf;
                ptrConsFilt->iBufs[ptrFilt->consIbufIdx[consumerIdx]] = pOSE->oBufs[pOSE->numOBufs];
                pOSE->numOBufs++;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOSEProcessFilter                                             //
//                                                                                //
//  DESCRIPTION: Process a filters details adding them fully to the OSE           //
//                                                                                //
//  INPUTS:      None                                                             //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     0x0 if success - 1 if processing failure                         //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippOSEProcessFilter (SippOse * pOSE,
                          u8        filterIdx)
{
    SippFilter * ptrFilt = pOSE->filts[filterIdx];
    u32          retVal  = 0;

    if (filterIdx == 0)
    {
        pOSE->linesPerIter = ptrFilt->linesPerIter;
    }
    else
    {
        ptrFilt->linesPerIter = pOSE->linesPerIter;
    }

    retVal  = sippOSEProcessFilterIBufs (pOSE,
                                         ptrFilt);

    if (retVal == 0)
    {
        sippOSEProcessFilterOBufs (pOSE,
                                   ptrFilt);
    }

    return retVal;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOSECreate                                                    //
//                                                                                //
//  DESCRIPTION: Return an address to an allocated OSE                            //
//                                                                                //
//  INPUTS:      None                                                             //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     Address of allocated OSE                                         //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

SippOse * sippOSECreate (SippPipeline * pPipe)
{
    SippOse * pOSE;

    pOSE = (SippOse *)sippMemAlloc (&pPipe->tHeapMCB,
                                    vPoolPipeStructs,
                                    sizeof(SippOse));

    if (pOSE)
    {
        pOSE->numFilts   = 0;
        pOSE->OSEFiltCfg = 0;
        pOSE->numIBufs   = 0;
        pOSE->numOBufs   = 0;
        pOSE->flags      = 0;
    }
    else
    {
        sippError ((u32 *)NULL, E_OUT_OF_MEM);
    }

    return pOSE;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOSEAddFilter                                                 //
//                                                                                //
//  DESCRIPTION: Add a sipp filter to an allocated OSE                            //
//                                                                                //
//  INPUTS:      pOSE The OSE                                                     //
//               ptrFilt - pointer to the sipp filter to add                      //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippOSEAddFilter (SippOse *    pOSE,
                       SippFilter * ptrFilt)
{
    if (pOSE->numFilts == 0)
    {
        pOSE->linesPerIter = ptrFilt->linesPerIter;
    }

    if (pOSE->numFilts <  SIPP_MAX_FILTS_OSE)
    {
        pOSE->filts[pOSE->numFilts++] = ptrFilt;
        pOSE->flags                  |= ptrFilt->flags;
//        pOSE->OSEFiltCfg             |= (1 << ptrFilt->unit);
        ptrFilt->memOSE               = 1;
        ptrFilt->pOSE                 = pOSE;
    }
    else
    {
        sippError(ptrFilt->errorStatus, E_OSE_CREATION_ERROR);
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOSEComplete                                                  //
//                                                                                //
//  DESCRIPTION: Called once all filters have been added to an OSE to set up the  //
//               data required by the scheduling algorithm                        //
//                                                                                //
//  INPUTS:      pOSE The OSE                                                     //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippOSEComplete (SippOse * pOSE)
{
    u32 filtUnitIds[SIPP_MAX_FILTS_OSE];
    u32 filtIdx;

    // Reorganise the filters into unit ID order which should be a standard flow
    for (filtIdx = 0; filtIdx < pOSE->numFilts; filtIdx++)
    {
        filtUnitIds[filtIdx]   = pOSE->filts[filtIdx]->unit;
    }

    sippListSort ((s32 *)pOSE->filts,
                  (s32 *)filtUnitIds,
                  pOSE->numFilts,
                  false);

    for (filtIdx = 0; filtIdx < pOSE->numFilts; filtIdx++)
    {
        sippOSEProcessFilter (pOSE,
                              filtIdx);
    }

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippFindConnectionsListRecursive                                 //
//                                                                                //
//  DESCRIPTION: Check for the input filter's connections in the given list       //
//                                                                                //
//  INPUTS:      ptrFilt  - Pointer to filter                                     //
//               filterList - list of filters                                     //
//               numList - number of filters in filterList                        //
//               parents - Set to 1 to check parent connections - otherwise check //
//                         consumers                                              //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       The Colour combination filter cannot be part of an OSE if it is  //
//               not the first filter in the chain so the recursive nature halts  //
//               when one is encountered before processing it if parents = 0 and  //
//               after processing it if parents =  1                              //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippFindConnectionsListRecursive (SippFilter *  ptrFilt,
                                      SippFilter ** filterList,
                                      u32           numList,
                                      u8            parents,
                                      u32           connectedFilts)
{
    u32 connectionIdx, listIdx, inList;

    SippFilter ** connectionList = (parents) ? ptrFilt->parents  : ptrFilt->cons;
    u32           maxIdx         = (parents) ? ptrFilt->nParents : ptrFilt->nCons;

    for (connectionIdx = 0; connectionIdx < maxIdx; connectionIdx++)
    {
        SippFilter * ptrConnectedFilt = connectionList[connectionIdx];
        u32          oBufIdx;

        if ((0x1 << ptrConnectedFilt->unit) & SIPP_OPIPE_FILTER_LIST)
        {
        	u32 oPipeConnect = (parents) ? gSipp.uInfo[ptrFilt->unit].hwCheckOpipeCons(ptrFilt, ptrConnectedFilt, &oBufIdx) :
                                           gSipp.uInfo[ptrConnectedFilt->unit].hwCheckOpipeCons(ptrConnectedFilt, ptrFilt, &oBufIdx);

            if (oPipeConnect)
            {
                for (listIdx = 0, inList = 0; listIdx < numList; listIdx++)
                {
                    if (ptrConnectedFilt == filterList[listIdx])
                    {
                        if (!(connectedFilts & (1 << listIdx)))
                        {
                            inList          = 1;
                            connectedFilts |= (1 << listIdx);
                            break;
                        }
                    }
                }

                if (inList)
                {
                    connectedFilts |= sippFindConnectionsListRecursive (ptrConnectedFilt,
                                                                        filterList,
                                                                        numList,
                                                                        parents,
                                                                        connectedFilts);


                    connectedFilts |= sippFindConnectionsListRecursive (ptrConnectedFilt,
                                                                        filterList,
                                                                        numList,
                                                                        (parents)? 0x0: 0x1,
                                                                        connectedFilts);

                }
            }
        }
    }

    return connectedFilts;
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippRemoveEntriesList                                            //
//                                                                                //
//  DESCRIPTION: Check tfor the input filter's parents in the given list          //
//                                                                                //
//  INPUTS:      Filters  - Bitmask of list enties to remove                      //
//               filterList - list of filters                                     //
//               numList - number of filters in filterList                        //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippRemoveEntriesList ( u32           entryMask,
                            SippFilter ** filterList,
                            u32           numEntries)
{
    s32 idx0, idx1, numRemoved;

    for (idx0 = (numEntries -1), numRemoved = 0; idx0 >= 0; idx0--)
    {
        if (entryMask & (1 << idx0))
        {
            numRemoved++;

            for (idx1 = idx0; idx1 < (s32)(numEntries - numRemoved); idx1++)
            {
                filterList[idx1] = filterList[idx1+1];
            }
        }
    }

    return numEntries - numRemoved;
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippIdentifyOPipeSchedulingEntity                                //
//                                                                                //
//  DESCRIPTION: Identify all groups of filters capable of becoming an oPipe      //
//               Scheduling Entity (OSE) and create an OSE for them               //
//                                                                                //
//  INPUTS:      pPipe     - Pipeline for analysis                                //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

s32 sippIdentifyOPipeSchedulingEntity (pSippPipeline pPipe)
{
    SippFilter * ptrFilt;
    u32          filtIdx;
    u32          filtUnitIds[SIPP_MAX_FILTERS_PER_PIPELINE];
    SippFilter * oPipeFilters[SIPP_MAX_FILTERS_PER_PIPELINE];
    #ifdef SIPP_TRACK_OCTOPUS
    SippFilter * nonoPipeFilters[SIPP_MAX_FILTERS_PER_PIPELINE];
    u8           nonoPipeFilterEntryIdx;
    #endif
    u8           oPipeFilterEntryIdx;
    u32          oPipeFullCfg = 0;
    u8           OctopusCfg = 0;

    pPipe->numOSE = 0;

    if (!(pPipe->flags & PLF_DISABLE_OPIPE_CONS))
    {
        // Find oPipe filters
        #ifdef SIPP_TRACK_OCTOPUS
        nonoPipeFilterEntryIdx = 0
        #endif
        for (filtIdx = 0, oPipeFilterEntryIdx = 0; filtIdx < pPipe->nFilters; filtIdx++)
        {
            // Should have a better list with only the HW filters in it to save time
            ptrFilt = pPipe->filters[filtIdx];

            if (ptrFilt->unit <= SIPP_MAX_FILTER_ID)
            {
                if ((0x1 << ptrFilt->unit) &
                     SIPP_OPIPE_FILTER_LIST)
                {
                    oPipeFilters[oPipeFilterEntryIdx++] = ptrFilt;
                    oPipeFullCfg                       |= (0x1 << ptrFilt->unit);
                }
            }
            #ifdef SIPP_TRACK_OCTOPUS
            // In order identify the octopus - also track non oPIpe filters
            else
            {
                nonoPipeFilters[nonoPipeFilterEntryIdx++] = ptrFilt;
            }
            #endif
        }

        // Octopus checks
        if (((oPipeFullCfg & SIPP_OCTOPUS_MASK) ^ SIPP_OCTOPUS_MASK) == 0)
        {
            // All octopus filters are in place - now check non oPipe filters

            // If still ok - validate it is the octopus by running thru all the connections

            // Finish me

            if (OctopusCfg == 1)
            {
                pPipe->numOSE = SIPP_OPIPE_ID;
            }
        }

        if (OctopusCfg == 0)
        {
            oPipeFullCfg = 0;

            // Reorganise the filters into unit ID order which should be a standard flow
            for (filtIdx = 0; filtIdx < oPipeFilterEntryIdx; filtIdx++)
            {
                filtUnitIds[filtIdx]   = oPipeFilters[filtIdx]->unit;
            }

            // Sort the list into ascending order of unit id so chains of oPipe parts
            // are faster to identify
            sippListSort ((s32 *)oPipeFilters,
                          (s32 *)filtUnitIds,
                          oPipeFilterEntryIdx,
                          false);

            // Now look at the oPipe filter connections to search for other oPipe filters
            if (oPipeFilterEntryIdx)
            {
                for (filtIdx = 0; filtIdx < oPipeFilterEntryIdx; filtIdx++)
                {
                    SippFilter * ptrParFilt;
                    u32          parentIdx;
                    u32          oPipeFiltCfg;
                    u32          oBufIdx;

                    ptrFilt = oPipeFilters[filtIdx];

                    for (parentIdx = 0; parentIdx < ptrFilt->nParents; parentIdx++)
                    {
                        ptrParFilt   = ptrFilt->parents[parentIdx];
                        oPipeFiltCfg = gSipp.uInfo[ptrFilt->unit].hwCheckOpipeCons(ptrFilt, ptrParFilt, &oBufIdx);

                        if (oPipeFiltCfg)
                        {
                            oPipeFullCfg |= oPipeFiltCfg;
                        }
                    }
                }
            }

            /* Group oPipe filters into oPipe Scheduling entities */
            if (oPipeFullCfg)
            {
                u8  complete      = false;
                u32 newOSEFilts   = 0;
                u32 entriesInList = oPipeFilterEntryIdx;

                filtIdx = 0;

                while (complete == false)
                {
                    // If a filter is added to an OSE - it must be removed from the filter list
                    ptrFilt = oPipeFilters[filtIdx];

                    newOSEFilts = sippFindConnectionsListRecursive (ptrFilt,
                                                                    oPipeFilters,
                                                                    entriesInList,
                                                                    0x0,
                                                                    0x0);

                    if (newOSEFilts)
                    {
                        u32 idx;

                        // If connections were found in the list, add the filter from
                        // which the search began to the list
                        newOSEFilts |= (1 << filtIdx);

                        // Add these filters to an OSE
                        if (pPipe->numOSE < SIPP_MAX_OSE_PER_PIPE)
                        {
                            pPipe->pOSE[pPipe->numOSE] = sippOSECreate (pPipe);

                            if (pPipe->pOSE[pPipe->numOSE])
                            {
                                for (idx = 0; idx < SIPP_MAX_FILTERS_PER_PIPELINE; idx++)
                                {
                                    if (newOSEFilts & (1 << idx))
                                    {
                                        sippOSEAddFilter (pPipe->pOSE[pPipe->numOSE],
                                                          oPipeFilters[idx]);
                                    }
                                }

                                sippOSEComplete (pPipe->pOSE[pPipe->numOSE]);
                                pPipe->numOSE++;
                            }
                            else
                            {
                                sippError(pPipe->errorStatus, E_OSE_CREATION_ERROR);
                            }

                            // Remove the OSE group from the original list
                            entriesInList = sippRemoveEntriesList (newOSEFilts,
                                                                   oPipeFilters,
                                                                   entriesInList);

                            if ((entriesInList == 0) ||
                                (filtIdx >= (entriesInList - 1)))
                            {
                                complete = true;
                            }
                        }
                        else
                        {
                            sippError(pPipe->errorStatus, E_OSE_CREATION_ERROR);
                        }
                    }
                    else
                    {
                        if (filtIdx == (entriesInList - 1))
                        {
                            complete = true;
                        }
                        else
                        {
                            filtIdx++;
                        }
                    }
                }
            }
            else
            {
                pPipe->numOSE = 0;
            }

            pPipe->oPipeCfg = oPipeFullCfg;
        }
    }

    return pPipe->numOSE;
}


////////////////////////////////////////////////////////////////
// Filter specific functions
//

// Sigma denoise filter
u32 sippCheckOPipeConnectionSigma (SippFilter * ptrFilt,
                                   SippFilter * parentFilt,
                                   u32        * parOBufIdx)
{
    UNUSED (ptrFilt); //hush the compiler warning

    if (parentFilt->unit == SIPP_MIPI_RX0_ID)
    {
        *parOBufIdx = 0;
        return (1 << SIPP_MIPI_RX0_ID);
    }
    else if (parentFilt->unit == SIPP_MIPI_RX1_ID)
    {
        *parOBufIdx = 0;
        return (1 << SIPP_MIPI_RX1_ID);
    }
    else if (parentFilt->unit == SIPP_MIPI_RX2_ID)
    {
        *parOBufIdx = 0;
        return (1 << SIPP_MIPI_RX2_ID);
    }
    else if (parentFilt->unit == SIPP_MIPI_RX3_ID)
    {
        *parOBufIdx = 0;
        return (1 << SIPP_MIPI_RX3_ID);
    }

    return 0;
}

// LSC filter
u32 sippCheckOPipeConnectionLsc (SippFilter * ptrFilt,
                                 SippFilter * parentFilt,
                                 u32        * parOBufIdx)
{
    UNUSED (ptrFilt); //hush the compiler warning

    if (parentFilt->unit == SIPP_SIGMA_ID)
    {
        *parOBufIdx = 0;
        return (1 << SIPP_SIGMA_ID);
    }

    return 0;
}

// RAW filter
u32 sippCheckOPipeConnectionRaw (SippFilter * ptrFilt,
                                 SippFilter * parentFilt,
                                 u32        * parOBufIdx)
{
    UNUSED (ptrFilt); //hush the compiler warning

    if (parentFilt->unit == SIPP_LSC_ID)
    {
        *parOBufIdx = 0;
        return (1 << SIPP_LSC_ID);
    }

    return 0;
}

// Debayer filter
u32 sippCheckOPipeConnectionDbyr (SippFilter * ptrFilt,
                                  SippFilter * parentFilt,
                                  u32        * parOBufIdx)
{
    UNUSED (ptrFilt); //hush the compiler warning

    if (parentFilt->unit == SIPP_RAW_ID)
    {
        *parOBufIdx = 0;
        return (1 << SIPP_RAW_ID);
    }

    return 0;
}

// DoGLTM filter
u32 sippCheckOPipeConnectionDoGLTM (SippFilter * ptrFilt,
                                    SippFilter * parentFilt,
                                    u32        * parOBufIdx)
{
    #ifdef SIPP_SCRT_ENABLE_OPIPE
    if (ptrFilt->pPipe->pfnSippRuntime == sippOPipeRuntime)
    {
        if (parentFilt->unit == SIPP_DBYR_ID)
        {
            #ifdef BUGZILLA_22593
            *parOBufIdx = 1;
            parentFilt->oBufs[*parOBufIdx]->allocReq  = 0x1;
            parentFilt->oBufs[*parOBufIdx]->manageReq = 0x1;
            return 0;
            #else
            *parOBufIdx = 1;
            return (1 << SIPP_DBYR_LUMA_ID);
            #endif
        }
    }
    #else
    UNUSED(ptrFilt);
    UNUSED(parentFilt);
    UNUSED(parOBufIdx);
    #endif

    return 0;
}

// Luma Denoise filter
u32 sippCheckOPipeConnectionLuma (SippFilter * ptrFilt,
                                  SippFilter * parentFilt,
                                  u32        * parOBufIdx)
{
    UNUSED (ptrFilt); //hush the compiler warning

    if (parentFilt->unit == SIPP_DOGL_ID)
    {
        *parOBufIdx = 0;
        return (1 << SIPP_DOGL_ID);
    }

    return 0;
}

// Sharpen filter
u32 sippCheckOPipeConnectionSharpen (SippFilter * ptrFilt,
                                     SippFilter * parentFilt,
                                     u32        * parOBufIdx)
{
    UNUSED (ptrFilt); //hush the compiler warning

    if (parentFilt->unit == SIPP_LUMA_ID)
    {
        *parOBufIdx = 0;
        return (1 << SIPP_LUMA_ID);
    }

    return 0;
}

// Gen Chroma filter
u32 sippCheckOPipeConnectionGenChroma (SippFilter * ptrFilt,
                                       SippFilter * parentFilt,
                                       u32        * parOBufIdx)
{
    #ifdef SIPP_SCRT_ENABLE_OPIPE
    if (ptrFilt->pPipe->pfnSippRuntime == sippOPipeRuntime)
    {
        if (parentFilt->unit == SIPP_DBYR_ID)
        {
            GenChrParam * pParam = (GenChrParam*)ptrFilt->params;
            *parOBufIdx = 0;

            if (((pParam->cfg >> CGEN_CFG_LLB_DSMODE_SHIFT) & CGEN_CFG_LLB_DSMODE_MASK) == 0x1)
            {
                ptrFilt->linesPerIter = 0;
            }

            return (1 << SIPP_DBYR_ID);
        }
    }
    #else
    UNUSED(ptrFilt);
    UNUSED(parentFilt);
    UNUSED(parOBufIdx);
    #endif

    return 0;
}

// Median filter
u32 sippCheckOPipeConnectionMedian (SippFilter * ptrFilt,
                                    SippFilter * parentFilt,
                                    u32        * parOBufIdx)
{
    #ifndef SIPP_SCRT_ENABLE_OPIPE
    UNUSED(ptrFilt);
    #endif

    if (parentFilt->unit == SIPP_CGEN_ID)
    {
        *parOBufIdx = 0;

        #ifdef SIPP_SCRT_ENABLE_OPIPE
        if (ptrFilt->pPipe->pfnSippRuntime == sippOPipeRuntime)
        {
            ptrFilt->linesPerIter = parentFilt->linesPerIter;
        }
        #endif
        return (1 << SIPP_CGEN_ID);
    }
    else
    {
        #ifdef SIPP_SCRT_ENABLE_OPIPE
        if (ptrFilt->pPipe->pfnSippRuntime == sippOPipeRuntime)
        {

            if (parentFilt->unit == SIPP_DBYR_ID)
            {
                #ifdef BUGZILLA_22593
                *parOBufIdx = 1;
                parentFilt->oBufs[*parOBufIdx]->allocReq  = 0x1;
                parentFilt->oBufs[*parOBufIdx]->manageReq = 0x1;
                return 0;
                #else
                *parOBufIdx = 1;
                return (1 << SIPP_DBYR_LUMA_ID);
                #endif
            }
        }
        #endif
    }

    // Note we do not allow an oPipe connection to the Debayer for alpha blending at the moment
    // outside of the oPipe runtime
    return 0;
}

// Chroma Denoise filter
u32 sippCheckOPipeConnectionChrDns (SippFilter * ptrFilt,
                                    SippFilter * parentFilt,
                                    u32        * parOBufIdx)
{
    UNUSED (ptrFilt); //hush the compiler warning

    if (parentFilt->unit == SIPP_MED_ID)
    {
        *parOBufIdx = 0;
        #ifdef SIPP_SCRT_ENABLE_OPIPE
        if (ptrFilt->pPipe->pfnSippRuntime == sippOPipeRuntime)
        {
            ptrFilt->linesPerIter = parentFilt->linesPerIter;
        }
        #endif
        return (1 << SIPP_MED_ID);
    }

    return 0;
}

// Colour Combination filter
u32 sippCheckOPipeConnectionColourComb (SippFilter * ptrFilt,
                                        SippFilter * parentFilt,
                                        u32        * parOBufIdx)
{
    #ifdef SIPP_SCRT_ENABLE_OPIPE
    if (ptrFilt->pPipe->pfnSippRuntime == sippOPipeRuntime)
    {
        if (parentFilt->unit == SIPP_SHARPEN_ID)
        {
            *parOBufIdx = 0;
            parentFilt->oBufs[*parOBufIdx]->allocReq  = 0x1;
            parentFilt->oBufs[*parOBufIdx]->manageReq = 0x0;
            return (1 << SIPP_SHARPEN_ID);
        }
        if (parentFilt->unit == SIPP_CHROMA_ID)
        {
            *parOBufIdx              = 0;
            parentFilt->linesPerIter = 0;
            return (1 << SIPP_CHROMA_ID);
        }
    }
    #else
    UNUSED(ptrFilt);
    UNUSED(parentFilt);
    UNUSED(parOBufIdx);
    #endif

    // When not using the oPipe runtime, the colour combination block is not allowed to be
    // part of an OSE unless it is the first filter in the flow - since it combines the input from two
    // pipelines which must be balanced, we cannot ensure this would work unless have CMX
    // buffers on both pipes at the input to this filter - so we may as well do this by
    // forcing a termination of any previous OSE(s) at this point
    // Of-course the Octopus config is balanced by simply having a CMX buffer between the sharpen filter
    // and the colour comb filter - i.e. on the luma path only. However the octopus has a fixed
    // configuration and fixed latencies of both paths so the balancing condition by adding this
    // latency to the luma path remains constant. The point of the OSE is that it is formed
    // from only part of the oPipe so the paths have the potential to vary their latency from the
    // octopus configuration (by a little or a lot!)

    return 0;
}

// Gamma Lut filter
u32 sippCheckOPipeConnectionLut (SippFilter * ptrFilt,
                                 SippFilter * parentFilt,
                                 u32        * parOBufIdx)
{
    if (parentFilt->unit == SIPP_CC_ID)
    {
        // Do not enable streaming connection unless LUT has been setup in channel mode
        LutParam  * pParam = (LutParam *)ptrFilt->params;

        if ((pParam->cfg >> LUT_CFG_CHANNEL_MODE_SHIFT) & LUT_CFG_CHANNEL_MODE_MASK)
        {
            *parOBufIdx = 0;
            return (1 << SIPP_CC_ID);
        }
    }

    return 0;
}

// Polyphase Scalar filter
u32 sippCheckOPipeConnectionPoly (SippFilter * ptrFilt,
                                  SippFilter * parentFilt,
                                  u32        * parOBufIdx)
{
    #ifdef SIPP_SCRT_ENABLE_OPIPE
    if (ptrFilt->pPipe->pfnSippRuntime == sippOPipeRuntime)
    {

        if (parentFilt->unit == SIPP_LUT_ID)
        {
            *parOBufIdx = 0;
            parentFilt->oBufs[*parOBufIdx]->allocReq  = 0x1;
            parentFilt->oBufs[*parOBufIdx]->manageReq = 0x0;
            return ((1 << SIPP_LUT_ID) | (1 << ptrFilt->unit));
        }
    }
    #else
    UNUSED(ptrFilt);
    UNUSED(parentFilt);
    UNUSED(parOBufIdx);
    #endif

    return 0;
}

u32 sippCheckOPipeConnectionDefault (SippFilter * ptrFilt,
                                     SippFilter * parentFilt,
                                     u32        * parOBufIdx)
{
    UNUSED (ptrFilt);    //hush the compiler warning
    UNUSED (parentFilt); //hush the compiler warning
    UNUSED (parOBufIdx); //hush the compiler warning

    return 0;
}
