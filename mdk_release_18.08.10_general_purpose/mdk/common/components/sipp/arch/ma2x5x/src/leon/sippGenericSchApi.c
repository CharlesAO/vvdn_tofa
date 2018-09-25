///
/// @file      sippSchedApi.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP framework API
///            Platform(s) supported : MA2x5x
///

////////////////////////////////////////////////////
// Header files
////////////////////////////////////////////////////

#include <sipp.h>
#include <sippInternal.h>

////////////////////////////////////////////////////
// Global variables
////////////////////////////////////////////////////

////////////////////////////////////////////////////
// Static variables
////////////////////////////////////////////////////

////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////

//###########################################################################
// MipiTX obeys some timings for first line, so for first line, it won't
// actually send the line in that iteration.
// So patch the start here to actually trigger the start 1 line earlier !

#if defined(SIPP_PC) || defined(MYRIAD2)

void sippGenericPatchMipiTxStart (pSippPipeline pl)
{
    u32 i, u;

    for (u = SIPP_MIPI_TX0_ID; u <= SIPP_MIPI_TX1_ID; u++)
    {
        if (pl->hwSippFltCnt[u]) // If current MIPI-TX unit is used...
        {
            // Search for first iteration where it runs
            for (i = 0; i < pl->nIter; i++)
            {
                if (pl->schedInfo[i].sippHwWaitMask & (1 << u))
                {
                    // Clear the start from first iteration
                    pl->schedInfo[i].sippHwWaitMask &= ~(1 << u);
                    // And move it one prior iteration
                    // MIPI TX can't be called in firs iteration, so i > 1
                    pl->schedInfo[i-1].sippHwWaitMask |= (1 << u);
                    break;
                }
            }
        }
    }
}

#endif

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericSearchBuffer                                          //
//                                                                                //
//  DESCRIPTION: Search the input buffer for a specified line                     //
//                                                                                //
//  INPUTS:      psLineBuf - Input buffer to search                               //
//               lineRequired - Line to search for                                //
//               linePos - set to -1 for full search, or non-negative value to    //
//                         search only that location                              //
//               fullSearch - Force a full search, irrespective of linePos        //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     returns the location of lineRequired if it is found,             //
//               -1 otherwise.                                                    //
//                                                                                //
//  NOTES:       if linePos is set, searches that specific location for           //
//               lineRequired, if linepos is -1, returns the location of line     //
//               required in the whole buffer (if found) - if the line required   //
//               is at more than one location due to bottomLineReplication, then  //
//               the earliest entry location is returned                          //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

s32 sippGenericSearchBuffer (psSchLineBuffer psLineBuf,
                             u32             lineRequired,
                             s32             linePos,
                             u32             fullSearch)
{
    s32 retVal;

    if ((linePos == -1) ||
        (fullSearch))
    {
        if ((s32)lineRequired <= psLineBuf->mostRecentLine)
        {
            retVal = psLineBuf->mostRecentLine - lineRequired + psLineBuf->bottomLineReplication;
        }
        else
        {
            retVal = -1;
        }
    }
    else
    {
        s32 lineCorrection = linePos - (s32)psLineBuf->bottomLineReplication;

        if (lineCorrection < 0) lineCorrection = 0;

    	// Search a specific line location in the buffer
        if (psLineBuf->mostRecentLine - lineCorrection ==
            (s32)lineRequired)
        {
            retVal = linePos;
        }
        else
        {
            retVal = -1;
        }
    }

    return retVal;

}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericRecordParentKS                                        //
//                                                                                //
//  DESCRIPTION: Record the kernel start position for each input buffer of the    //
//               specified SE                                                     //
//                                                                                //
//  INPUTS:      pSchEnt - pointer to scheduling entity                           //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Called once the SE can run - that is the KS should be            //
//               capable of being found                                           //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippGenericRecordParentKS (SippSchEnt * pSchEnt,
                                u32          iBufIdx)
{
    // Search in buffer for needed data
    pSchEnt->parentKS[iBufIdx] = sippGenericSearchBuffer (pSchEnt->iBufs[iBufIdx],
                                                          pSchEnt->maxLineRequired[iBufIdx],
                                                          pSchEnt->parentKS[iBufIdx],
                                                          pSchEnt->iBufCtx[iBufIdx]->runFullSearch);

    if (pSchEnt->parentKS[iBufIdx] == -1)
    {
        sippError ((u32 *)NULL, E_DATA_NOT_FOUND); // this should never happen: filter is said
                                                   // to have data, yet it's not found
    }

    if (pSchEnt->parentKS[iBufIdx] > pSchEnt->parentKSMin[iBufIdx])
    {
        pSchEnt->parentKSMin[iBufIdx] = pSchEnt->parentKS[iBufIdx];
    }

    pSchEnt->recordParentKS[iBufIdx]         = 0;
    pSchEnt->iBufCtx[iBufIdx]->runFullSearch = 0;
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericCheckSERunParents                                     //
//                                                                                //
//  DESCRIPTION: Check the parent run conditions are met for a scheduling entity  //
//                                                                                //
//  INPUTS:      pSchEnt - pointer to SE                                          //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     RunStatus                                                        //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

RunStatus sippGenericCheckSERunParents (SippSchEnt * pSchEnt)
{
    u32 iBufIdx;
    u8  noParentKSCheck[SIPP_SE_MAX_IBUFS] = {0};

    // v2 code says if if current filter finished the data, the PARENT restriction
    // disappears, so can say "can run"
    // maxOutputH calculation takes care of maximum latency of the SE also
    if (pSchEnt->numLineRuns >= (s32)pSchEnt->maxOutputH - 1)
    {
        pSchEnt->dbgJustRoll = 1;
        return RS_CAN_RUN;
    }
    else
    {
        pSchEnt->dbgJustRoll = 0;
    }

    if (pSchEnt->numIBufs)
    {
        for (iBufIdx = 0; iBufIdx < pSchEnt->numIBufs; ++iBufIdx)
        {
            if (pSchEnt->iBufCtx[iBufIdx]->funcAsk (pSchEnt,
                                                    iBufIdx,
                                                    pSchEnt->numLineRuns + 1,
                                                    0x1))
            {
                if (sippGenericSearchBuffer (pSchEnt->iBufs[iBufIdx],
                                             pSchEnt->maxLineRequired[iBufIdx],
                                             pSchEnt->parentKS[iBufIdx],
                                             pSchEnt->iBufCtx[iBufIdx]->runFullSearch) == -1)
                {
                    return RS_CANNOT;
                }
                else if (pSchEnt->runPadCheck[iBufIdx])
                {
                    // For certain non-regular filters, knowing the line at the KS
                    // position in the buffer is not enough.
                    // Sometimes during padding at picture vertical edges more detail
                    // on the amount of padding which has been done is required.
                    // eg a Resizer filter running a 5x5 kernel may say it needs a
                    // certain line to be at the KS point, but also needs to know how
                    // many lines of the kernel have resulted from vertical padding
                    // to know when to run - required ver kernel for an input height=53
                    // may be lines[53, 53, 52, 51, 50]
                    // So checking that 53 is at the KS position is insufficient to distinguish
                    // this from [53, 52, 51, 50, 49]. In these cases check the value at the
                    // other edge of the kernel is the minimum value expected by the kernel also
                    if (sippGenericSearchBuffer (pSchEnt->iBufs[iBufIdx],
                                                 pSchEnt->minLineRequired[iBufIdx],
                                                 pSchEnt->parentKS[iBufIdx] + (pSchEnt->iBufCtx[iBufIdx]->numLinesUsed - 1),
                                                 pSchEnt->iBufCtx[iBufIdx]->runFullSearch) == -1)
                    {
                        return RS_CANNOT;
                    }
                }
            }
            else
            {
                noParentKSCheck[iBufIdx] = 0x1;
            }
        }

        // Buffer specific parentKS checks now
        for (iBufIdx = 0; iBufIdx < pSchEnt->numIBufs; ++iBufIdx)
        {
            if (pSchEnt->recordParentKS[iBufIdx])
            {
                if (noParentKSCheck[iBufIdx] == 0)
                {
                    sippGenericRecordParentKS (pSchEnt,
                                               iBufIdx);
                }
            }
        }

        return RS_CAN_RUN;

    }
    else
    {
        return RS_CAN_RUN;
    }
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericGetBufferEntry                                        //
//                                                                                //
//  DESCRIPTION: return the entry at a specified buffer location                  //
//                                                                                //
//  INPUTS:      psLineBuf - the line buffer                                      //
//               linePos   - location within buffer of entry to return            //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     Entry @ specified location                                       //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

s32 sippGenericGetBufferEntry (psSchLineBuffer psLineBuf,
                               s32             linePos)
{
    s32 retVal;

    retVal = psLineBuf->mostRecentLine - ((linePos < (s32)psLineBuf->bottomLineReplication) ? 0 : linePos - psLineBuf->bottomLineReplication);

    if (retVal < 0) retVal = ( psLineBuf->mostRecentLine == -1 ) ? -1 : 0;

    return retVal;

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericRunSE                                                 //
//                                                                                //
//  DESCRIPTION: Run a scheduling entity for one iteration, populating output     //
//               buffers                                                          //
//                                                                                //
//  INPUTS:      pSchEnt - pointer to SE                                          //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

static void sippGenericRunSE (SippSchEnt * pSchEnt)
{
    u32 bufIdx;

    for (bufIdx = 0; bufIdx < pSchEnt->numOBufs; bufIdx++)
    {
        psSchLineBuffer psLineBuf = pSchEnt->oBufs[bufIdx];

        if (psLineBuf->internalFillLevel == psLineBuf->latency)
        {
            if (psLineBuf->mostRecentLine + 1 < (s32)psLineBuf->filtOutHeight)
            {
                psLineBuf->mostRecentLine++;
            }
            else
            {
                psLineBuf->bottomLineReplication++;
            }
        }
        else
        {
            psLineBuf->internalFillLevel++;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericRewindSE                                              //
//                                                                                //
//  DESCRIPTION: Run a scheduling entity by one iteration, updating output        //
//               buffers                                                          //
//                                                                                //
//  INPUTS:      pSchEnt - pointer to SE                                          //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

static void sippGenericRewindSE (SippSchEnt *pSchEnt)
{
    u32 bufIdx;

    for (bufIdx = 0; bufIdx < pSchEnt->numOBufs; bufIdx++)
    {
        psSchLineBuffer psLineBuf = pSchEnt->oBufs[bufIdx];

        if (psLineBuf->bottomLineReplication)
        {
            psLineBuf->bottomLineReplication--;
        }
        else
        {
            if (psLineBuf->internalFillLevel == psLineBuf->latency)
            {
                if (psLineBuf->mostRecentLine == -1)
                {
                    psLineBuf->internalFillLevel--;
                }
                else
                {
                    psLineBuf->mostRecentLine--;
                }
            }
            else
            {
                psLineBuf->internalFillLevel--;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericMinimizeLineBuffs                                     //
//                                                                                //
//  DESCRIPTION: Minimise the size of all line buffers in the system              //
//                                                                                //
//  INPUTS:      pipeLine -                                                       //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       All sim starts with "BUFF_HUGE_SZ" assumption                    //
//               After everything is done, we can adjust the size of line buffers //
//               by looking at min-KS value among all the consumers of each       //
//               buffer                                                           //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

static void sippGenericMinimizeLineBuffs (pSippPipeline pipeLine)
{
    SippSchEnt * pSE;
    SippSchEnt * ObufConsList[SIPP_SE_MAX_OBUFS][SIPP_FILTER_MAX_CONSUMERS];
    u32          ObufConsListIbufIdx[SIPP_SE_MAX_OBUFS][SIPP_FILTER_MAX_CONSUMERS];
    u32          consBufIdx[SIPP_SE_MAX_OBUFS];
    u32          minKS[SIPP_SE_MAX_OBUFS];
    u32          i, consIdx, oBufIdx;
    u32          numSE = pipeLine->numSE;

    // Reset consBufIdx[SIPP_SE_MAX_OBUFS];
    for (i = 0; i < SIPP_SE_MAX_OBUFS; i++)
    {
    	consBufIdx[i] = 0;
    }

    // At the minute we have a list of consumers in each SE but that does not directly state
    // which output buffer they are consumers of - to keep a hold of that would require
    // a lot of extra space and we can't really justify that as we can take care of it in code
    // once here and let that do...
    for (i = 0; i < numSE; i++)
    {
        pSE = pipeLine->pSE[i];

        if (pSE->numOBufs)
        {
            // Sort the consumers into a list of consumers per oBuf
            if (pSE->numOBufs > 1)
            {
                for (consIdx = 0; consIdx < pSE->numConsumers; consIdx++)
                {
                    // Find the consumer iBuf in the oBuf list of the current SE
                    SippSchEnt *    pConSE   = pSE->cons[consIdx];
                    psSchLineBuffer consOBuf = pConSE->iBufs[pSE->consIbufIdx[consIdx]];

                    for (oBufIdx = 0; oBufIdx < pSE->numOBufs; oBufIdx++)
                    {
                        if (pSE->oBufs[oBufIdx] == consOBuf)
                        {
                            u8 consMinKS = pConSE->parentKSMin[pSE->consIbufIdx[consIdx]];

                            // Mark the minKS among all the consumers of this oBuf
                            if (consBufIdx[oBufIdx] == 0)
                            {
                                minKS[oBufIdx] = consMinKS;
                            }
                            else
                            {
                                if (consMinKS < minKS[oBufIdx])
                                {
                                    minKS[oBufIdx] = consMinKS;
                                }
                            }

                            // Add the consumer to the list of consumers for this oBuf
                            ObufConsListIbufIdx[oBufIdx][consBufIdx[oBufIdx]] = pSE->consIbufIdx[consIdx];
                            ObufConsList[oBufIdx][consBufIdx[oBufIdx]++]      = pConSE;

                            break;
                        }
                    }
                    if (oBufIdx == pSE->numOBufs)
                    {
                        sippError (pipeLine->errorStatus, E_DATA_NOT_FOUND);
                    }
                }

                // Compute optimal buffer size for each oBuf by subtracting the smallest
                // KS ever recorded
                for (oBufIdx = 0; oBufIdx < pSE->numOBufs; oBufIdx++)
                {
                    pSE->oBufs[oBufIdx]->numLines = pSE->oBufs[oBufIdx]->numLines - minKS[oBufIdx];

                    // Adjust all the consumer KS in line with the minKS adjustment
                    for (consIdx = 0; consIdx < consBufIdx[oBufIdx]; consIdx++)
                    {
                        ObufConsList[oBufIdx][consIdx]->parentKS[ObufConsListIbufIdx[oBufIdx][consIdx]] -= minKS[oBufIdx];
                    }
                }
            }
            else
            {
                oBufIdx = 0;

                // Note - this case of one output buffer would work equally well were it to be processed by the
                // code for multi-output buffers above but giving it a special optimised case because it should
                // be by far the most common situation!!
                for (consIdx = 0; consIdx < pSE->numConsumers; consIdx++)
                {
                    SippSchEnt * pConSE    = pSE->cons[consIdx];
                    u8           consMinKS = pConSE->parentKSMin[pSE->consIbufIdx[consIdx]];

                    if (consIdx == 0)
                    {
                        minKS[0] = consMinKS;
                    }
                    else
                    {
                        if (consMinKS < minKS[oBufIdx])
                        {
                            minKS[oBufIdx] = consMinKS;
                        }
                    }
                }

                pSE->oBufs[0]->numLines = pSE->oBufs[0]->numLines - minKS[0];

                for (consIdx = 0; consIdx < pSE->numConsumers; consIdx++)
                {
                    // Find the consumer iBuf in the oBuf list of the current SE
                    SippSchEnt * pConSE = pSE->cons[consIdx];
                    pConSE->parentKS[pSE->consIbufIdx[consIdx]] -= minKS[0];
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericProcessScheduleInfo                                   //
//                                                                                //
//  DESCRIPTION: Process info gathered during the schedule creation process which //
//               is needed by the remainder of the pipeline                       //
//                                                                                //
//  INPUTS:      pipeLine - Pipeline whose per frame operation is to scheduled    //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

static void sippGenericProcessScheduleInfo (pSippPipeline pipeLine)
{
    // Convert KS from its scheduler sense to that expected by the remainder of the framework
    // In the scheduling sense parentKS is the distance from the newest line in the buffer of the most recent line needed by the kernel
    // in the framework sense parentKS is the distance below the oldest line held in the output buffer that the kernel starts for the consumer filter
    u32 seIdx, iBufIdx;
    SippSchEnt * pSE;

    for (seIdx = 0; seIdx < pipeLine->numSE; seIdx++)
    {
        pSE = pipeLine->pSE[seIdx];

        for (iBufIdx = 0; iBufIdx <  pSE->numIBufs; iBufIdx++)
        {
            u32 iBufLinesUsed      = pSE->iBufCtx[iBufIdx]->numLinesUsed;
            u32 scheduleKS         = pSE->parentKS[iBufIdx] + iBufLinesUsed;
            pSE->parentKS[iBufIdx] = pSE->iBufs[iBufIdx]->numLines - scheduleKS;

            // parentKSMin also needs converted here
            scheduleKS                = pSE->parentKSMin[iBufIdx] + iBufLinesUsed;
            pSE->parentKSMin[iBufIdx] = pSE->iBufs[iBufIdx]->numLines - scheduleKS;
        }
    }

    /* Now call sippGenericMinimizeLineBuffs to change the line buffer sizes from BUFF_HUGE_SZ to the  */
    /* minimum that the schedule creation algo believes we can get away with and modify the            */
    /* KS accordingly */
    sippGenericMinimizeLineBuffs (pipeLine);

    // Copy the parentKS data back into the original filter structure
    for (seIdx = 0; seIdx < pipeLine->numSE; seIdx++)
    {
        pSE = pipeLine->pSE[seIdx];

        for (iBufIdx = 0; iBufIdx <  pSE->numIBufs; iBufIdx++)
        {
            *(pSE->ptrFiltKS[iBufIdx]) = pSE->parentKS[iBufIdx];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericRunAddSchedChecks                                     //
//                                                                                //
//  DESCRIPTION: Scheduling entities can ask for additional checks to be made     //
//               to deal with corner case situations.                             //
//                                                                                //
//  INPUTS:      pipeLine - Pipeline                                              //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       This check is made after parent checks are established by        //
//               generic scheduler                                                //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

static void sippGenericRunAddSchedChecks (pSippPipeline pipeLine)
{
    SippSchEnt ** ppSchEnts        = pipeLine->pSE;
    u32           runAddSchedCheck = pipeLine->runAddSchedCheck;

    while (runAddSchedCheck)
    {
        u32 nextSEId = sippPalFindHighestBit (runAddSchedCheck);

        ppSchEnts[nextSEId]->pfRunAdditionalCheck (pipeLine,
                                                   ppSchEnts[nextSEId],
                                                   ppSchEnts[nextSEId]->pRunAdditionalParam);

        runAddSchedCheck &= (~(0x1 << nextSEId));
    }
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericRunScheduleGeneration                                 //
//                                                                                //
//  DESCRIPTION: Create a runtime schedule for the pipeline                       //
//                                                                                //
//  INPUTS:      pipeLine - Pipeline whose per frame operation is to scheduled    //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     0x0 if success, 1 if processing error detected                   //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

static u32 sippGenericRunScheduleGeneration (pSippPipeline pipeLine)
{
    SippSchEnt ** ppSchEnts = pipeLine->pSE;
    u32           nSchEnts  = pipeLine->numSE;
    u32           iteration = 0;
    u32           schEntIdx, consumerIdx;
    u8            somethingRuns, allKnown = 0;
    s32           last_ln_p, last_ln_c, frst_ln_p, frst_ln_c;

    do
    {
        somethingRuns = 0;

        for (schEntIdx = 0; schEntIdx < nSchEnts; schEntIdx++)
        {
            ppSchEnts[schEntIdx]->canRunC = RS_DONT_KNOW;
        }

        //=====================================================================
        // Decide what scheduling entities can run in current iteration
        //=====================================================================

        //=====================================================================
        // P-condition: look in all parents buffers and see if data is available
        //=====================================================================
        for (schEntIdx = 0; schEntIdx < nSchEnts; schEntIdx++)
        {
            ppSchEnts[schEntIdx]->canRunP  = sippGenericCheckSERunParents (ppSchEnts[schEntIdx]);

            if (ppSchEnts[schEntIdx]->canRunP == RS_CANNOT)
            {
                ppSchEnts[schEntIdx]->canRunC =  RS_CANNOT;
            }
        }

        if (pipeLine->runAddSchedCheck)
        {
            sippGenericRunAddSchedChecks (pipeLine);
        }

        //=====================================================================
        // C-condition: look through all consumers and see if anybody needs
        //              data that's not already in the buffer (in next iteration)
        //=====================================================================
        do
        {
            allKnown = 1;

            for (schEntIdx = 0; schEntIdx < nSchEnts; schEntIdx++)
            {
                SippSchEnt * pSchEnt = ppSchEnts[schEntIdx];

                if (pSchEnt->canRunP == RS_CAN_RUN)
                {
                    if (pSchEnt->numConsumers == 0)
                    {   //if filter has no consumers, then C-condition becomes true
                        pSchEnt->canRunC = RS_CAN_RUN;
                    }
                    else
                    {
                        pSchEnt->canRunC = RS_CAN_RUN; //assume we can run...

                        //Trial buffer advancement
                        pSchEnt->numLineRuns++;
                        sippGenericRunSE (pSchEnt);

                        for (consumerIdx = 0x0; consumerIdx < pSchEnt->numConsumers; consumerIdx++)
                        {
                            SippSchEnt * pConsSchEnt = pSchEnt->cons[consumerIdx];
                            u32          consIbufIdx = pSchEnt->consIbufIdx[consumerIdx];

                            if ((pConsSchEnt->numLineRuns + 1 < (int)pConsSchEnt->maxOutputH) &&
                                ((int)pConsSchEnt->parentKS[consIbufIdx] != -1))
                            {
                                if (pConsSchEnt->canRunC == RS_DONT_KNOW)
                                {
                                    pSchEnt->canRunC = RS_DONT_KNOW;
                                    allKnown = 0;
                                    break;
                                }
                                else
                                {
                                    u8 checkOverwrite;
                                    // Find consumer expectations for it's run in NEXT ITERATION

                                    // A consumer filter will only have set canRunC == RS_CAN_RUN
                                    // if it has been called by the consumer checking algo for the current iteration
                                    // If consumer checking has been called for the consumer filter in turn that means
                                    // it was able to run based on its parents. So essentially at this point in code
                                    // a check of pConsSchEnt->canRunC == RS_CAN_RUN is a check on the consumer
                                    // filter to see if it WILL run this iteration!
                                    if (pConsSchEnt->canRunC == RS_CAN_RUN)
                                    {
                                        // The consumer filter WILL run this iteration
                                        checkOverwrite = pConsSchEnt->iBufCtx[consIbufIdx]->funcAsk (pConsSchEnt,
                                                                                                     consIbufIdx,
                                                                                                     pConsSchEnt->numLineRuns + 2,
                                                                                                     0x1);

                                    }
                                    else
                                    {
                                        // This consumer filter will NOT run this iteration
                                        // Check ptrConsFilt->numLineRuns + 1
                                        checkOverwrite = pConsSchEnt->iBufCtx[consIbufIdx]->funcAsk (pConsSchEnt,
                                                                                                     consIbufIdx,
                                                                                                     pConsSchEnt->numLineRuns + 1,
                                                                                                     0x1);
                                    }

                                    if (checkOverwrite)
                                    {
                                        last_ln_c = (s32)pConsSchEnt->maxLineRequired[consIbufIdx];
                                        frst_ln_c = (s32)pConsSchEnt->minLineRequired[consIbufIdx];

                                        if (frst_ln_c < 0) frst_ln_c = 0;

                                        // Call search buffer again - KS will have been set

                                        last_ln_p = (s32)sippGenericGetBufferEntry (pConsSchEnt->iBufs[consIbufIdx],
                                                                                    pConsSchEnt->parentKS[consIbufIdx]);
                                        frst_ln_p = (s32)sippGenericGetBufferEntry (pConsSchEnt->iBufs[consIbufIdx],
                                                                                    pConsSchEnt->parentKS[consIbufIdx] + (pConsSchEnt->iBufCtx[consIbufIdx]->numLinesUsed - 1));

                                        //If consumers OVERWRITE occurs:
                                        if ((last_ln_p > last_ln_c) ||
                                            (frst_ln_p > frst_ln_c))
                                        {
    // Removing this for now as a scheduling entity may be much more than just one filter
    // so could do a check that it is just a resizing filter but lets see what happens if we don't
    //                                //##########################################################
    //                                // Buffering on output: if resizer overwrites one consumer,
    //                                // then let it run, but tell ALL consumers they need to update
    //                                // in next iteration: we don't clear the KS = -1 as we'd alter
    //                                // current iteration, but we set a flag, (ks_clr) that gets
    //                                // interpreted at the beginning of next iteration
    //                                if ( //(ptrFilt->funcSvuRun == RUN_bilin_gen) &&
    //                                    (pSchEnt->iBufCtx[consIbufIdx]->runFullSearch) &&    output buffer height & flags are not held anywhere yet
    //                                    (pSchEnt->numLineRuns < (int)ptrFilt->outputH ))
    //                                {//
    //                                    // KMC - This does not actually use cc!!
    //                                    // So surely this would be broken if we had more than one consumer?? Ask AL//
    //                                    u32 cc;
    //                                    //ptrFilt->canRunC = RS_CAN_RUN; //this is the default assumption
    //                                    for (cc = 0; cc < ptrFilt->numConsumers; cc++ )
    //                                    {
    //                                        u32 xx = getParentIndex (ptrConsFilt, ptrFilt);
    //                                    }
    //                                    break;
    //                                    }
    //                                    else
    //                                    {
                                                //if cannot run due to a consumer
                                                //that's good enough: break !
                                                pSchEnt->canRunC = RS_CANNOT;
                                                break;
    //                                    }

                                        } //else, leave RS_CAN_RUN decision
                                    }
                                }
                            }
                        }

                        // Rewind filter
                        pSchEnt->numLineRuns--;
                        sippGenericRewindSE (pSchEnt);
                    }
                }
            }
        }
        while (allKnown == 0);

        //===================================================================
        // If we reach here, and some filters stat is "DON'T KNOW", we failed !
        //===================================================================
        #ifdef RT_DBG

        for (schEntIdx = 0; schEntIdx < nSchEnts; schEntIdx++)
        {
            if ((ppSchEnts[schEntIdx]->canRunP == RS_DONT_KNOW) ||
                (ppSchEnts[schEntIdx]->canRunC == RS_DONT_KNOW)  )
            {
                sippError (pipeLine->errorStatus, E_RUN_DON_T_KNOW);
                return 0x1;
            }
        }
        #endif

        //==============================================================
        // Refine a bit rolling decision (bottom replication purposes)
        //==============================================================
        for (schEntIdx = 0; schEntIdx < nSchEnts; schEntIdx++)
        {
            SippSchEnt * pSchEnt = ppSchEnts[schEntIdx];

            // If a filter is finished,
            if (pSchEnt->numLineRuns >= (int)pSchEnt->maxOutputH - 1)
            {
                s32 allConsumersDone = 1;

                // and all its consumers are finished, then don't run anymore
                for (consumerIdx = 0x0; consumerIdx < pSchEnt->numConsumers; consumerIdx++)
                {
                    if (pSchEnt->cons[consumerIdx]->numLineRuns < (int)pSchEnt->cons[consumerIdx]->maxOutputH - 1)
                    {
                        allConsumersDone = 0;
                        break;
                    }
                }

                if (allConsumersDone)
                {
                    pSchEnt->canRunC = RS_CANNOT;
                }
            }
        }

        //=====================================================================
        // Buffer rolling
        //=====================================================================
        for (schEntIdx = 0; schEntIdx < nSchEnts; schEntIdx++)
        {
            SippSchEnt * pSchEnt = ppSchEnts[schEntIdx];

            if ((pSchEnt->canRunP == RS_CAN_RUN) &&
                (pSchEnt->canRunC == RS_CAN_RUN))
            {
                pSchEnt->numLineRuns++;
                sippGenericRunSE (pSchEnt);
            }
        }

        //=====================================================================
        // Print runnable tasks:
        //=====================================================================
        if (pipeLine->dbgLevel > 0)
        {
            sippGenericDbgPrintRunnable (ppSchEnts,
                                         nSchEnts,
                                         iteration);
        }

        sippGenericSchedWr (pipeLine,
                            iteration);

        //=====================================================================
        // See if anything actually ran this iteration
        //=====================================================================
        somethingRuns = 0;

        for (schEntIdx = 0; schEntIdx < nSchEnts; schEntIdx++)
        {
            SippSchEnt * pSchEnt = ppSchEnts[schEntIdx];

            if ((pSchEnt->canRunP == RS_CAN_RUN) &&
                (pSchEnt->canRunC == RS_CAN_RUN))
            {
                if (!pSchEnt->dbgJustRoll)
                {
                    somethingRuns = 1;
                    break;
                }
            }
        }

        iteration++;

        if (iteration > pipeLine->schedInfoEntries)
        {
            #if defined(SIPP_PC)
            printf("__ERROR: schedule mem too small !\n");
            exit(2);
            #endif
            sippError(pipeLine->errorStatus, E_CANNOT_FINISH_FILTER);
            return 0x1;
        }

        #if defined(SIPP_PC)
        if (pipeLine->dbgLevel > 0)
        {
            printf("\n");
        }
        #else
        VCS_PRINT_INT(0x00001111);
        #endif

   } while (somethingRuns);


   //==================================================
   //If filters didn't reach their run numbers it's failure!
   for (schEntIdx = 0; schEntIdx < nSchEnts; schEntIdx++)
   {
        SippSchEnt * pSchEnt = ppSchEnts[schEntIdx];
        if (pSchEnt->numLineRuns < (s32)pSchEnt->maxOutputH - 1)
        {
            #if defined(SIPP_PC)
            printf("ERROR : filter %d didn't finish! \n", schEntIdx);
            #else
            VCS_PRINT_INT(0xDEADDEAD);
            #endif

            sippError(pipeLine->errorStatus, E_CANNOT_FINISH_FILTER);
            return 0x1;
        }
    }

    
    #if defined(SIPP_PC)
    printf("\n\n Frame-schedule finished :) \n\n");
    if (pipeLine->dbgLevel > 1)
    {
        sippGenericDbgShowBufferReq (ppSchEnts, nSchEnts);
    }
    #else
    VCS_PRINT_INT(0x6666);
    #endif

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericSchedPipeInit                                         //
//                                                                                //
//  DESCRIPTION: Update filter variables for this scheduler                       //
//                                                                                //
//  INPUTS:      pipeLine - Pipeline whose per frame operation is to be scheduled //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippGenericSchedPipeInit (pSippPipeline pipeLine)
{
    u32          nFilters  = pipeLine->nFilters;
    u32          filterIdx;
    SippFilter * pFilter;

    pipeLine->nIter = 0;

    for (filterIdx = 0; filterIdx < nFilters; filterIdx++)
    {
        pFilter = pipeLine->filters[filterIdx];

        pFilter->linesThisIter[0] = pFilter->linesThisIter[1] = 1;
    }
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericAllocRuntimeSched                                     //
//                                                                                //
//  DESCRIPTION: Allocate storage space for schedule and associated items         //
//                                                                                //
//  INPUTS:      pipeLine - Pipeline whose per frame operation is to scheduled    //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippGenericAllocRuntimeSched (pSippPipeline pipeLine)
{
    u32 max_H, i;

    ///////////////////////////////////////////////////////////////
    // Allocate storage in SIPP-shared pool for scheduler
    // For this we need to know the largest height in the pipeline, We use this info
    // to guide the allocs. TBD: add checks here !
    max_H = 0;
    for (i = 0; i < pipeLine->numSE; i++)
    {
        if (pipeLine->pSE[i]->maxOutputH > max_H)
        {
            max_H = pipeLine->pSE[i]->maxOutputH;
        }
    }

    if (pipeLine->schedInfoEntries == 0)
    {
        if (max_H > 0x40)
        {
            pipeLine->schedInfoEntries = max_H + (max_H >> 1);
        }
        else
        {
            pipeLine->schedInfoEntries = (max_H << 1);

            if (pipeLine->nFilters > 3)
            {
                pipeLine->schedInfoEntries = (max_H << 2);
            }

        }
    }

    pipeLine->schedInfo = (SchedInfo *) sippMemAlloc (&pipeLine->tHeapMCB,
                                                      vPoolSchedule,
                                                      pipeLine->schedInfoEntries * sizeof(SchedInfo));

    if (pipeLine->schedInfo == (SchedInfo *)NULL)
    {
        sippPalTrace (SIPP_CORE_TL_SEVERE, "sippGenericAllocRuntimeSched : Failed to allocate runtime schedule\n");
        sippError (pipeLine->errorStatus, E_OUT_OF_MEM);
        return 0x1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericSchCreateSEFromFilter                                 //
//                                                                                //
//  DESCRIPTION: Create a scheduling entity from filter details                   //
//                                                                                //
//  INPUTS:      pFilter - Filter to create entity from                           //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       Only called for filters which are not part of an SE              //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippGenericSchCreateSEFromFilter (SippFilter *pFilter)
{
    u32          idx, maxOutputH;
    SippSchEnt * pSE   = pFilter->sch;

    // Set up the input buffer related items
    pSE->numIBufs     = pFilter->numIBufs;

    for (idx = 0; idx < pSE->numIBufs; idx++)
    {
        pSE->recordParentKS[idx] = 1;
        pSE->iBufs[idx]          = pFilter->iBufs[idx];
        pSE->iBufCtx[idx]        = pFilter->iBufCtx[idx];
        pSE->runPadCheck[idx]    = 0;
        pSE->parentKS[idx]       = -1;
        pSE->parentKSMin[idx]    = -1;
        pSE->ptrFiltKS[idx]      = &pFilter->parentsKS[idx];
    }

    // Set up the consumer list
    pSE->numConsumers = pFilter->nCons;

    for (idx = 0; idx < pSE->numConsumers; idx++)
    {
        pSE->cons[idx]        = pFilter->cons[idx]->sch;
        pSE->consIbufIdx[idx] = pFilter->consIbufIdx[idx];
    }

    // Set up the output buffer related items
    pSE->numOBufs     = pFilter->numOBufs;

    for (idx = 0, maxOutputH = 0; idx < pSE->numOBufs; idx++)
    {
        pSE->oBufs[idx]               = pFilter->oBufs[idx];
        pSE->oBufs[idx]->latency      = pFilter->OBufProdLatency[idx];
        pFilter->oBufs[idx]->latency  = pFilter->OBufProdLatency[idx];

        if (idx == 0)
        {
            maxOutputH = pFilter->oBufs[idx]->filtOutHeight + pFilter->oBufs[idx]->latency;
        }
        else if (pFilter->oBufs[idx]->filtOutHeight + pFilter->oBufs[idx]->latency > maxOutputH)
        {
            maxOutputH = pFilter->oBufs[idx]->filtOutHeight + pFilter->oBufs[idx]->latency;
        }
    }

    pSE->maxOutputH  = maxOutputH;
    pSE->numLineRuns = -1;
    pSE->canRunP     = RS_DONT_KNOW;
    pSE->canRunC     = RS_DONT_KNOW;

    if ((pFilter->unit == SIPP_UPFIRDN0_ID) ||
        (pFilter->unit == SIPP_UPFIRDN1_ID) ||
        (pFilter->unit == SIPP_UPFIRDN2_ID))
    {
        pSE->canConsume = RS_DONT_KNOW;
    }
    else
    {
        pSE->canConsume = RS_CANNOT;
    }

    if (pFilter->unit == SIPP_CC_ID)
    {
        // Find the chroma parent
        if (pFilter->iBufCtx[0x1]->funcAsk == askHwColorCombChroma)
        {
            pSE->pfRunAdditionalCheck = sippGenericColorCombChromaCheck;
            pSE->pRunAdditionalParam  = (void *)pFilter->parents[0x1]->sch;

            pFilter->pPipe->runAddSchedCheck |= (0x1 << pSE->pipeSEId);
        }
    }
    else if (pFilter->unit == SIPP_DMA_ID)
    {
        if (pFilter->nCons)
        {
            // If non of the consumers have the croping flag set then run allow a DMA
            // check to run to make sure we do not store a needlessly large
            u32 consIdx;
            u32 runDMACheck = 1;

            for (consIdx = 0; consIdx < pFilter->nCons; consIdx++)
            {
                if (pFilter->cons[consIdx]->flags & SIPP_CROP)
                {
                    runDMACheck = 0;
                }
            }

            if (runDMACheck)
            {
                pSE->pfRunAdditionalCheck         = sippGenericDMACheck;
                pSE->pRunAdditionalParam          = (void *)0x0;
                pFilter->pPipe->runAddSchedCheck |= (0x1 << pSE->pipeSEId);
            }
            else
            {
                pSE->pfRunAdditionalCheck = 0x0;
            }
        }
        else
        {
            pSE->pfRunAdditionalCheck = 0x0;
        }
    }
    else
    {
        pSE->pfRunAdditionalCheck = 0x0;
    }

}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericSchCreateSEFromOSE                                    //
//                                                                                //
//  DESCRIPTION: Create a scheduling entity from OSE details                      //
//                                                                                //
//  INPUTS:      pOSE - OSE to create entity from                                 //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippGenericSchCreateSEFromOSE (pSippOse pOSE)
{
    u32          idx, maxOutputH;
    SippSchEnt * pSE = pOSE->sch;

    // Set up the input buffer related items
    pSE->numIBufs    = pOSE->numIBufs;

    for (idx = 0; idx < pSE->numIBufs; idx++)
    {
        pSE->recordParentKS[idx] = 1;
        pSE->iBufs[idx]          = pOSE->iBufs[idx];
        pSE->iBufCtx[idx]        = pOSE->iBufCtx[idx];
        pSE->parentKS[idx]       = -1;
        pSE->parentKSMin[idx]    = -1;
        pSE->ptrFiltKS[idx]      = pOSE->ptrFiltKS[idx];
    }

    // Set up the consumer list
    pSE->numConsumers = pOSE->numConsumers;

    for (idx = 0; idx < pSE->numConsumers; idx++)
    {
        pSE->cons[idx]        = *(pOSE->consSE[idx]);
        pSE->consIbufIdx[idx] = pOSE->consIbufIdx[idx];
    }

    // Set up the output buffer related items
    pSE->numOBufs = pOSE->numOBufs;

    for (idx = 0, maxOutputH = 0; idx < pSE->numOBufs; idx++)
    {
        pSE->oBufs[idx] = pOSE->oBufs[idx];

        if (idx == 0)
        {
            maxOutputH = pOSE->oBufs[idx]->filtOutHeight + pOSE->oBufs[idx]->latency;
        }
        else if (pOSE->oBufs[idx]->filtOutHeight + pOSE->oBufs[idx]->latency > maxOutputH)
        {
            maxOutputH = pOSE->oBufs[idx]->filtOutHeight + pOSE->oBufs[idx]->latency;
        }
    }

    pSE->maxOutputH  = maxOutputH;
    pSE->numLineRuns = -1;
    pSE->canRunP     = RS_DONT_KNOW;
    pSE->canRunC     = RS_DONT_KNOW;

    if (pOSE->flags & SIPP_RESIZE)
    {
        pSE->canConsume = RS_DONT_KNOW;
    }
    else
    {
        pSE->canConsume = RS_CANNOT;
    }

    // This assumes a Color Comb filter can only be at the start of an OSE which
    // as of March 2016 is correct!
    if (pOSE->filts[0x0]->unit == SIPP_CC_ID)
    {
        // Find the chroma parent
        if (pOSE->filts[0x0]->iBufCtx[0x1]->funcAsk == askHwColorCombChroma)
        {
            pSE->pfRunAdditionalCheck = sippGenericColorCombChromaCheck;
            pSE->pRunAdditionalParam  = (void *)pOSE->filts[0x0]->parents[0x1]->sch;

            pOSE->filts[0x0]->pPipe->runAddSchedCheck |= (0x1 << pSE->pipeSEId);
        }
    }
    else
    {
        pSE->pfRunAdditionalCheck = 0x0;
    }

}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericSchedAllocTempStorage                                 //
//                                                                                //
//  DESCRIPTION: Allocate temporary storage elements for use in schedule creation //
//                                                                                //
//  INPUTS:      pipeLine - Pipeline whose per frame operation is to scheduled    //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippGenericSchedAllocTempStorage (pSippPipeline pipeLine)
{
    u32          nFilters  = pipeLine->nFilters;
    u32          filterIdx;
    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    u32          oseIdx;
    SippOse *    pOSE;
    #endif
    SippFilter * pFilter;

    // By this point there should be a bunch of filters and in the case of 2x5x onwards
    // potentially oPipe Scheduling Entities OSEs in the pipeline.
    // These groups form the basic scheduling entities
    // Create storage for the structures used in the management of these scheduling entities
    // through the scheduling process
    for (filterIdx = 0; filterIdx < nFilters; filterIdx++)
    {
        pFilter = pipeLine->filters[filterIdx];

        #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
        if (pFilter->memOSE == 0)
        {
        #endif

        pFilter->sch = (SippSchEnt *)sippMemAlloc (&pipeLine->tHeapMCB,
                                                   vPoolScheduleTemp,
                                                   sizeof(SippSchEnt));

        if (pFilter->sch == (SippSchEnt *)NULL)
        {
            sippError (pFilter->errorStatus, E_OUT_OF_MEM );
            sippError (pipeLine->errorStatus, E_OUT_OF_MEM );
            return 0x1;
        }

        sippPalMemset (pFilter->sch, 0, sizeof(SippSchEnt));

        pFilter->sch->pipeSEId           = pipeLine->numSE;
        pipeLine->pSE[pipeLine->numSE++] = pFilter->sch;

        #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
        }
        #endif
    }

    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    for (oseIdx = 0; oseIdx < pipeLine->numOSE; oseIdx++)
    {
        pOSE = pipeLine->pOSE[oseIdx];

        pOSE->sch = (SippSchEnt *)sippMemAlloc (&pipeLine->tHeapMCB,
                                                vPoolScheduleTemp,
                                                sizeof(SippSchEnt));

        if (pOSE->sch == (SippSchEnt *)NULL)
        {
            sippError (pipeLine->errorStatus, E_OUT_OF_MEM );
            return 0x1;
        }

        sippPalMemset (pOSE->sch, 0, sizeof(SippSchEnt));

        pOSE->sch->pipeSEId              = pipeLine->numSE;
        pipeLine->pSE[pipeLine->numSE++] = pOSE->sch;
    }

    #endif

    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    // For those filters which became part of an OSE, still set their SE to be the SE
    // allocated to the OSE which the filter became part of (OSE consumer lists are pointer
    // to pointer lists via the filter's SE structs)
    for (filterIdx = 0; filterIdx < nFilters; filterIdx++)
    {
        pFilter = pipeLine->filters[filterIdx];

        if (pFilter->memOSE)
        {
            pFilter->sch = pFilter->pOSE->sch;
        }
    }

    #endif

    return 0x0;
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericSchedInitTempStorage                                  //
//                                                                                //
//  DESCRIPTION: Initialise temporary storage elements for use in schedule        //
//               creation                                                         //
//                                                                                //
//  INPUTS:      pipeLine - Pipeline whose per frame operation is to scheduled    //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       We cannot guarantee the order in which the SEs are created so    //
//               this initilalisation happens after all SEs are allocated so that //
//               the list of consumer SEs for each SE actually points to          //
//               something!                                                       //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippGenericSchedInitTempStorage (pSippPipeline pipeLine)
{
    u32          nFilters  = pipeLine->nFilters;
    u32          filterIdx;
    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    u32          oseIdx;
    SippOse *    pOSE;
    #endif
    SippFilter * pFilter;

    for (filterIdx = 0; filterIdx < nFilters; filterIdx++)
    {
        pFilter = pipeLine->filters[filterIdx];

        #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
        if (pFilter->memOSE == 0)
        {
        #endif

        sippGenericSchCreateSEFromFilter (pFilter);

        #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
        }
        #endif
    }

    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    for (oseIdx = 0; oseIdx < pipeLine->numOSE; oseIdx++)
    {
        pOSE = pipeLine->pOSE[oseIdx];

        sippGenericSchCreateSEFromOSE (pOSE);
    }
    #endif

    return 0;
}


/////////////////////////////////////////////////////////////////////////////
// Scheduler interface API

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericSchedule                                              //
//                                                                                //
//  DESCRIPTION: Main API function for framework to cause creation of a schedule  //
//               suitable for a runtime to follow to process one frame on the     //
//               specified pipeline                                               //
//                                                                                //
//  INPUTS:      pipeLine - Pipeline whose per frame operation is to scheduled    //
//               allocMem - Set TRUE to allocate space for the runtime schedule   //
//                          Otherwise assume this space is already available      //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     0x0 if successful, 0x1 if processing error                       //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippGenericSchedule (pSippPipeline pipeLine,
                         bool          allocMem,
                         bool          reschedPipe)
{
    UNUSED (reschedPipe);

    // Init the pipeline variables
    sippGenericSchedPipeInit (pipeLine);

    // Allocate temp storage elements
    if (sippGenericSchedAllocTempStorage (pipeLine))
    {
        return 0x1;
    }

    // Initialise temp storage elements
    if (sippGenericSchedInitTempStorage (pipeLine))
    {
         return 0x1;
    }

    if (allocMem)
    {
        // Allocate mem for runtime Scheduler info in CMX mempool
        if (sippGenericAllocRuntimeSched (pipeLine))
        {
            return 0x1;
        }
    }

    //Figure out buffering requirements and the schedule
    if (sippGenericRunScheduleGeneration (pipeLine) == 0)
    {
        // Process the info from schedule creation necessary to define the pipeline
        sippGenericProcessScheduleInfo (pipeLine);

        #if defined(SIPP_PC) || defined(MYRIAD2)
        // TODO - Ask AL about this...
        sippGenericPatchMipiTxStart (pipeLine);  //Patch mipi-TX start
        #endif

        #ifdef SIPP_RUNTIME_PROFILING
        if (pipeLine->pRTStats)
        {
            pipeLine->pRTStats->iterTime = (u64 *)sippMemAlloc (&pipeLine->tHeapMCB,
                                                                vPoolGeneral,
                                                                sizeof(u64) * SIPP_RT_PER_ITER_STATS_SIZE * pipeLine->nIter);

            if (pipeLine->pRTStats->iterTime == (u64 *)NULL)
            {
                sippPalTrace (SIPP_CORE_TL_SEVERE, "sippGenericSchedule : Failed to allocate runtime iter stats area\n");
                sippError (pipeLine->errorStatus, E_OUT_OF_MEM);
                return 0x1;
            }
        }
        #endif

        return 0x0;
    }

    return 0x1;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericScheduleSetBufConsModels                              //
//                                                                                //
//  DESCRIPTION: Main API function for framework to set the input buffer          //
//               consumption models for all filter buffers                        //
//                                                                                //
//  INPUTS:      pipeLine - Pipeline                                              //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippGenericScheduleSetBufConsModels (pSippPipeline pipeLine)
{
    u32          nFilters  = pipeLine->nFilters;
    u32          filterIdx, parentIdx;
    SippFilter * pFilter;

    for (filterIdx = 0; filterIdx < nFilters; filterIdx++)
    {
        pFilter = pipeLine->filters[filterIdx];

        for (parentIdx = 0; parentIdx < pFilter->nParents; parentIdx++)
        {
            if (pFilter->flags & SIPP_RESIZE)
            {
                #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
                if ((pFilter->unit == SIPP_UPFIRDN0_ID) ||
                    (pFilter->unit == SIPP_UPFIRDN1_ID) ||
                    (pFilter->unit == SIPP_UPFIRDN2_ID))
                {
                    pFilter->iBufCtx[parentIdx]->funcAsk = askPolyFirResizer;
                }
                else if (pFilter->unit == SIPP_CGEN_ID)
                {
                    pFilter->iBufCtx[parentIdx]->funcAsk = askChromaGenDownsizer;
                }
                else
                #endif
                pFilter->iBufCtx[parentIdx]->funcAsk = askResizer;
            }
            else if (pFilter->flags & SIPP_CROP)
            {
                pFilter->iBufCtx[parentIdx]->funcAsk = askCrop;
            }
            else
            {
                pFilter->iBufCtx[parentIdx]->funcAsk = askRegular;
            }

            // Custom scheduling functions:
            switch (pFilter->unit)
            {
                case SIPP_CC_ID     :   {
                                            if (pFilter->iBufCtx[parentIdx]->numLinesUsed > 0x1)
                                            {
                                                // Assume this is the chroma input of the colour combination filter
                                                // based on the vertical kernel size
                                                pFilter->iBufCtx[parentIdx]->funcAsk = askHwColorCombChroma;
                                            }

                                        } break;
                case SIPP_MED_ID    :   {
                                            if (parentIdx == 1)
                                            {
                                                // If we need to sub-sample the luma we need a different buffer
                                                // consumption model
                                                MedParam * medCfg = (MedParam *)pFilter->params;

                                                if ((medCfg->cfg >> 28) & 0x1)
                                                {
                                                    // We will assume this is the connection of a luma buffer used for
                                                    // alpha blending
                                                    pFilter->iBufCtx[parentIdx]->funcAsk = askHwMedLumaLatency;
                                                }
                                            }

                                        } break;
            }
        }
    }
}

// End of file
