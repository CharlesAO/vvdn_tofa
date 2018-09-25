///
/// @file      sippMLPISchedApi.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Multiple Line Per Iteration (MLPI) Scheduler API
///            Platform(s) supported : MA2x5x
///

#ifdef SIPP_SCRT_ENABLE_MLPI

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

void sippMLPIPatchMipiTxStart (pSippPipeline pl)
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
//  FUNCTION:    sippMLPISearchBuffer                                             //
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

s32 sippMLPISearchBuffer (psSchLineBuffer psLineBuf,
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
//  FUNCTION:    sippMLPIRecordParentKS                                           //
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

void sippMLPIRecordParentKS (SippSchEnt * pSchEnt,
                             u32          iBufIdx)
{
    // Search in buffer for needed data
    pSchEnt->parentKS[iBufIdx] = sippMLPISearchBuffer (pSchEnt->iBufs[iBufIdx],
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

    pSchEnt->recordParentKS[iBufIdx]= 0;
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPICheckSERunParents                                        //
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

RunStatus sippMLPICheckSERunParents (SippSchEnt * pSchEnt)
{
    u32 iBufIdx;
    u32 linesPerIter                       = pSchEnt->linesPerIter;
    u8  noParentKSCheck[SIPP_SE_MAX_IBUFS] = {0};
    s32 localKS;

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
                                                    linesPerIter))
            {
                if ((localKS = sippMLPISearchBuffer (pSchEnt->iBufs[iBufIdx],
                                                     pSchEnt->maxLineRequired[iBufIdx],
                                                     pSchEnt->parentKS[iBufIdx],
                                                     pSchEnt->iBufCtx[iBufIdx]->runFullSearch)) == -1)
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
                    if (sippMLPISearchBuffer (pSchEnt->iBufs[iBufIdx],
                                              pSchEnt->minLineRequired[iBufIdx],
                                              pSchEnt->parentKS[iBufIdx] + (pSchEnt->iBufCtx[iBufIdx]->numLinesUsed - 1),
                                              pSchEnt->iBufCtx[iBufIdx]->runFullSearch) == -1)
                    {
                        return RS_CANNOT;
                    }
                }

                if (pSchEnt->KSIterList[iBufIdx])
                {
                    s32 iterNumber = pSchEnt->numLineRuns + linesPerIter;

                    localKS += (pSchEnt->iBufCtx[iBufIdx]->numLinesUsed - 1);

                    // This should be adjusted since passing pSchEnt->iBufCtx[iBufIdx]->runFullSearch to
                    // sippMLPISearchBuffer will mean that we will find the first location of pSchEnt->maxLineRequired[iBufIdx]
                    // and base localKS on that - what we need is localKS to be based on the padded or pixel replicated
                    // location if that is the actual bottom of the kernel
                    if (pSchEnt->iBufCtx[iBufIdx]->runFullSearch)
                    {
                        localKS -= pSchEnt->iBufCtx[iBufIdx]->iPadLines;
                    }

                    // If we get here and this filter needs individual logging of kernel starts for all lines
                    // we should look at the KS positions of the intermediate lines now
                    // To do this in the most efficient way - funcAsk will tell us the offset of the KS of each
                    // iteration from the KS of the last iteration
                    // Local KS is the KS for the LAST iteration
                    pSchEnt->KSIterList[iBufIdx][iterNumber] = (u16)localKS;

                    for (iterNumber = linesPerIter - 2; iterNumber >= 0; iterNumber--)
                    {
                        pSchEnt->KSIterList[iBufIdx][pSchEnt->numLineRuns + iterNumber + 1] = (u16)(localKS + pSchEnt->currKSOffset[iBufIdx][iterNumber]);
                    }

                    // We log parentKSMin in order to help size the buffer afterwards
                    // For the non-uniform resizer filters, there can be non-regular intervals betwen the
                    // centre lines for the kernels in creating consecutive outputs - so the normal process
                    // of taking the location of the maximum line required in the buffer and working backwards
                    // from there must be done now while we have knowledge of both the KS and the gaps between them
                    if (pSchEnt->KSIterList[iBufIdx][pSchEnt->numLineRuns + 1] > pSchEnt->parentKSMin[iBufIdx])
                    {
                        pSchEnt->parentKSMin[iBufIdx] = pSchEnt->KSIterList[iBufIdx][pSchEnt->numLineRuns + 1];
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
                    sippMLPIRecordParentKS (pSchEnt,
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
//  FUNCTION:    sippMLPIGetBufferEntry                                           //
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

s32 sippMLPIGetBufferEntry (psSchLineBuffer psLineBuf,
                            s32             linePos)
{
    s32 retVal;

    retVal = psLineBuf->mostRecentLine - ((linePos < (s32)psLineBuf->bottomLineReplication) ? 0 : linePos - psLineBuf->bottomLineReplication);

    if (retVal < 0) retVal = ( psLineBuf->mostRecentLine == -1 ) ? -1 : 0;

    return retVal;

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIRunSE                                                    //
//                                                                                //
//  DESCRIPTION: Run a scheduling entity for a selected number of iterations,     //
//               populating output buffers                                        //
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

static void sippMLPIRunSE (SippSchEnt * pSchEnt)
{
    u32 bufIdx;
    u32 linesPerIter = pSchEnt->linesPerIter;

    for (bufIdx = 0; bufIdx < pSchEnt->numOBufs; bufIdx++)
    {
        u32             localLines = linesPerIter;
        psSchLineBuffer psLineBuf  = pSchEnt->oBufs[bufIdx];

        if (psLineBuf->internalFillLevel == psLineBuf->latency)
        {
            if ((psLineBuf->mostRecentLine + localLines) < psLineBuf->filtOutHeight)
            {
                psLineBuf->mostRecentLine += localLines;
            }
            else
            {
                if ((psLineBuf->filtOutHeight - 0x1) == (u32)psLineBuf->mostRecentLine)
                {
                    psLineBuf->bottomLineReplication += localLines;
                }
                else
                {
                    localLines                       -= ((psLineBuf->filtOutHeight - 0x1) - psLineBuf->mostRecentLine);
                    psLineBuf->mostRecentLine         = psLineBuf->filtOutHeight - 0x1;
                    psLineBuf->bottomLineReplication  = localLines;
                }
            }
        }
        else
        {
            if (psLineBuf->internalFillLevel + localLines < psLineBuf->latency)
            {
                psLineBuf->internalFillLevel += localLines;
            }
            else
            {
                localLines                  -= (psLineBuf->latency - psLineBuf->internalFillLevel);
                psLineBuf->internalFillLevel = psLineBuf->latency;

                if ((s32)(psLineBuf->mostRecentLine + localLines) < (s32)psLineBuf->filtOutHeight)
                {
                    psLineBuf->mostRecentLine += localLines;
                }
                else
                {
                    if ((psLineBuf->filtOutHeight - 0x1) == (u32)psLineBuf->mostRecentLine)
                    {
                        psLineBuf->bottomLineReplication += localLines;
                    }
                    else
                    {
                        localLines                       -= ((psLineBuf->filtOutHeight - 0x1) - psLineBuf->mostRecentLine);
                        psLineBuf->mostRecentLine         = psLineBuf->filtOutHeight - 0x1;
                        psLineBuf->bottomLineReplication  = localLines;
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIRewindSE                                                 //
//                                                                                //
//  DESCRIPTION: Run a scheduling entity by  a selected number of iterations,     //
//               updating output buffers                                          //
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

static void sippMLPIRewindSE (SippSchEnt * pSchEnt)
{
    u32 bufIdx;
    u32 linesPerIter = pSchEnt->linesPerIter;

    for (bufIdx = 0; bufIdx < pSchEnt->numOBufs; bufIdx++)
    {
        u32             localLines = linesPerIter;
        psSchLineBuffer psLineBuf  = pSchEnt->oBufs[bufIdx];

        if (psLineBuf->bottomLineReplication)
        {
            if (psLineBuf->bottomLineReplication < linesPerIter)
            {
                localLines                      -= psLineBuf->bottomLineReplication;
                psLineBuf->bottomLineReplication = 0;

                if (localLines < (psLineBuf->filtOutHeight - 0x1))
                {
                    psLineBuf->mostRecentLine -= localLines;
                }
                else
                {
                    localLines                   -= (psLineBuf->filtOutHeight - 0x1);
                    psLineBuf->mostRecentLine     = -1;
                    psLineBuf->internalFillLevel -= localLines;
                }
            }
            else
            {
                psLineBuf->bottomLineReplication -= linesPerIter;
            }
        }
        else
        {
            if (psLineBuf->internalFillLevel == psLineBuf->latency)
            {
                if (localLines < (psLineBuf->filtOutHeight - 0x1))
                {
                    if (psLineBuf->mostRecentLine == -1)
                    {
                    	psLineBuf->internalFillLevel -= localLines;
                    }
                    else
                    {
                        psLineBuf->mostRecentLine -= localLines;
                    }
                }
                else
                {
                    localLines                   -= (psLineBuf->filtOutHeight - 0x1);
                    psLineBuf->mostRecentLine     = -1;
                    psLineBuf->internalFillLevel -= localLines;
                }
            }
            else
            {
                psLineBuf->internalFillLevel -= linesPerIter;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIMinimizeLineBuffs                                        //
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

static void sippMLPIMinimizeLineBuffs (pSippPipeline pipeLine)
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
                    // Now we round minKS[oBufIdx] to make sure it is an integer multiple of
                    // numLinesPerIteration less than pSE->oBufs[0]->numLines
                    minKS[oBufIdx] /= pSE->linesPerIter;
                    minKS[oBufIdx] *= pSE->linesPerIter;

                    pSE->oBufs[oBufIdx]->numLines = pSE->oBufs[oBufIdx]->numLines - minKS[oBufIdx];

                    // Adjust all the consumer KS in line with the minKS adjustment
                    for (consIdx = 0; consIdx < consBufIdx[oBufIdx]; consIdx++)
                    {
                        SippSchEnt * pConsSE     = ObufConsList[oBufIdx][consIdx];
                        u32          consIbufIdx = ObufConsListIbufIdx[oBufIdx][consIdx];

                        pConsSE->parentKS[consIbufIdx] -= minKS[oBufIdx];

                        // If the consumer is a resizer filter in multi line per iteration mode - its KSIterList also needs updated
                        if (pConsSE->KSIterList[consIbufIdx])
                        {
                            // What adjustment steps are required below...
                            // If Val is a
                            // 1) b = a + numLinesUsed         // Move line to top of kernel - already done in checkSERunParents
                            // 2) c = BUFF_HUGE_SZ - b         // Switch to the entry pos of KS in a HUGE buffer filled from the top
                            // 3) d = c - minKS[oBufIdx];      // Switch to entry position in a minimally sized buffer
                            //
                            // Since 1) adjustment is done in checkSERunParents, KSIterList contains a list of b values
                            // so d = BUFF_HUGE_SZ - b - minKS[oBufIdx]

                            u32 runIdx;
                            s32 ksIterAdjust = (BUFF_HUGE_SZ - 0x1) - minKS[oBufIdx];

                            for (runIdx = 0; runIdx < (u32)pConsSE->numLineRuns; runIdx++)
                            {
                                pConsSE->KSIterList[consIbufIdx][runIdx] = ksIterAdjust - pConsSE->KSIterList[consIbufIdx][runIdx];
                            }
                        }
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
                        minKS[oBufIdx] = consMinKS;
                    }
                    else
                    {
                        if (consMinKS < minKS[oBufIdx])
                        {
                            minKS[oBufIdx] = consMinKS;
                        }
                    }
                }

                // Now we round minKS[oBufIdx] to make sure it is an integer multiple of
                // numLinesPerIteration less than pSE->oBufs[0]->numLines
                minKS[oBufIdx] /= pSE->linesPerIter;
                minKS[oBufIdx] *= pSE->linesPerIter;

                pSE->oBufs[0]->numLines = pSE->oBufs[0]->numLines - minKS[0];

                for (consIdx = 0; consIdx < pSE->numConsumers; consIdx++)
                {
                    // Find the consumer iBuf in the oBuf list of the current SE
                    SippSchEnt * pConSE      = pSE->cons[consIdx];
                    u32          consIbufIdx = pSE->consIbufIdx[consIdx];
                    pConSE->parentKS[consIbufIdx] -= minKS[oBufIdx];

                    // If the consumer is a resizer filter in multi line per iteration mode - its KSIterList also needs updated
                    if (pConSE->KSIterList[consIbufIdx])
                    {
                        // What adjustment steps are required below...
                        // If Val is a
                        // 1) b = a + numLinesUsed         // Move line to top of kernel - already done in checkSERunParents
                        // 2) c = BUFF_HUGE_SZ - b         // Switch to the entry pos of KS in a HUGE buffer filled from the top
                        // 3) d = c - minKS[oBufIdx];      // Switch to entry position in a minimally sized buffer
                        //
                        // Since 1) adjustment is done in checkSERunParents, KSIterList contains a list of b values
                        // so d = BUFF_HUGE_SZ - b - minKS[oBufIdx]

                        u32 runIdx;
                        s32 ksIterAdjust = (BUFF_HUGE_SZ - 0x1) - minKS[oBufIdx];

                        for (runIdx = 0; runIdx < (u32)pConSE->numLineRuns; runIdx++)
                        {
                            pConSE->KSIterList[consIbufIdx][runIdx] = ksIterAdjust - pConSE->KSIterList[consIbufIdx][runIdx];
                        }
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIProcessScheduleInfo                                      //
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

static void sippMLPIProcessScheduleInfo (pSippPipeline pipeLine)
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
            u32 iBufLinesUsed      = pSE->iBufCtx[iBufIdx]->numLinesUsed + (pSE->linesPerIter - 0x1);  // This is not correct for resizers when linesPerIter > 1 but in that case
                                                                                                       // we do not use it anyway.
            u32 scheduleKS         = pSE->parentKS[iBufIdx] + iBufLinesUsed;
            pSE->parentKS[iBufIdx] = pSE->iBufs[iBufIdx]->numLines - scheduleKS;

            // parentKSMin also needs converted here - it currently points to the most recent or max line required
            // Convert it to the oldest or minimum line required
            if (pSE->KSIterList[iBufIdx])
            {
                // For resizers, parentKSMin is already adjusted to point to the min line required of the first iteration
                // so does not need a lines per iter adjustment here
                scheduleKS                = pSE->parentKSMin[iBufIdx] + 1;
                pSE->parentKSMin[iBufIdx] = pSE->iBufs[iBufIdx]->numLines - scheduleKS;
            }
            else
            {
                // For non resizers, parentKSMin points to the max line required of the last iteration
                scheduleKS                = pSE->parentKSMin[iBufIdx] + iBufLinesUsed;
                pSE->parentKSMin[iBufIdx] = pSE->iBufs[iBufIdx]->numLines - scheduleKS;
            }
        }
    }

    // Now call sippMLPIMinimizeLineBuffs to change the line buffer sizes from BUFF_HUGE_SZ to the
    // minimum that the schedule creation algo believes we can get away with and modify the
    // KS accordingly
    sippMLPIMinimizeLineBuffs (pipeLine);

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
//  FUNCTION:    sippMLPIRunAddSchedChecks                                        //
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
//               MLPI scheduler                                                   //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

static void sippMLPIRunAddSchedChecks (pSippPipeline pipeLine)
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
//  FUNCTION:    sippMLPIRunScheduleGeneration                                    //
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

static u32 sippMLPIRunScheduleGeneration (pSippPipeline pipeLine)
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
            ppSchEnts[schEntIdx]->canRunP  = sippMLPICheckSERunParents (ppSchEnts[schEntIdx]);

            if (ppSchEnts[schEntIdx]->canRunP == RS_CANNOT)
            {
                ppSchEnts[schEntIdx]->canRunC =  RS_CANNOT;
            }
        }

        if (pipeLine->runAddSchedCheck)
        {
            sippMLPIRunAddSchedChecks (pipeLine);
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
                    {
                        // If SE has no consumers, then C-condition becomes true
                        pSchEnt->canRunC = RS_CAN_RUN;
                    }
                    else
                    {
                        pSchEnt->canRunC = RS_CAN_RUN; // Assume we can run...

                        // Trial buffer advancement
                        pSchEnt->numLineRuns += pSchEnt->linesPerIter;
                        sippMLPIRunSE (pSchEnt);

                        for (consumerIdx = 0x0; consumerIdx < pSchEnt->numConsumers; consumerIdx++)
                        {
                            SippSchEnt * pConsSchEnt = pSchEnt->cons[consumerIdx];
                            u32          consIbufIdx = pSchEnt->consIbufIdx[consumerIdx];

                            if (((pConsSchEnt->numLineRuns + 1) < (s32)pConsSchEnt->maxOutputH ) &&
                                ((s32)pConsSchEnt->parentKS[consIbufIdx] != -1))
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
                                                                                                     pConsSchEnt->numLineRuns + 1 + pConsSchEnt->linesPerIter,
                                                                                                     pConsSchEnt->linesPerIter);

                                    }
                                    else
                                    {
                                        // This consumer filter will NOT run this iteration
                                        // Check ptrConsFilt->numLineRuns + 1
                                        checkOverwrite = pConsSchEnt->iBufCtx[consIbufIdx]->funcAsk (pConsSchEnt,
                                                                                                     consIbufIdx,
                                                                                                     pConsSchEnt->numLineRuns + 1,
                                                                                                     pConsSchEnt->linesPerIter);
                                    }

                                    if (checkOverwrite)
                                    {
                                        last_ln_c = (s32)pConsSchEnt->maxLineRequired[consIbufIdx];
                                        frst_ln_c = (s32)pConsSchEnt->minLineRequired[consIbufIdx];

                                        if (frst_ln_c < 0) frst_ln_c = 0;

                                        // Call search buffer again - KS will have been set
                                        // if its a resize - KS is now effectively random - also which one do you use?

                                        // For filters with a fixed KS, this check is clear - we want to know if the data that filter
                                        // needs on a subsequent iteration will still have a possibility of appearing in the subsequent
                                        // iteration or any iteration subsequent to that in the position it needs it to be (dictated by the KS)
                                        // If we check the KS position and realise that after a run of the current filter the data occupying that
                                        // position-defined window is from a later context than that required by the consumer filter - in other words
                                        // is from a greater y location of the parent filter output image - then it is known that the data the
                                        // consumer filter needs to run its checked iteration will never appear in the location it wishes - as the
                                        // parent filter is not going to go back up the image again!!!

                                        // When the consumer filter is a resize filters in multi-line situations, we need to have a moving KS, that is KS
                                        // is not fixed - So what exactly does our check become?
                                        // Remember the parent buffer has not been sized yet, that must happen after scheduling so this scheduling process
                                        // can inform how big the buffer needs to be. Further we know the parent check for the consumer filter must have
                                        // worked in order for execution to get this far.
                                        // Thinking back on what this algo should achieve - this scheduling is a tradeoff between speed and memory
                                        // Adding more memory means we can run more often
                                        // We know the data is in there as the parent buffer check has passed
                                        // Our check becomes that the KS for the resize filter must be < X where X
                                        // is the number of lines per iteration of the PRODUCER filter
                                        // If KS < X, that c.f. with the data being at the correct location for operation in the regular filter check
                                        // If KS >= X that means we are going to add a full X lines on to the output buffer size if we decide to let this
                                        // proceed

                                        if (pConsSchEnt->iBufCtx[consIbufIdx]->runFullSearch)
                                        {
                                            s32 localKS = sippMLPISearchBuffer (pConsSchEnt->iBufs[consIbufIdx],
                                                                                pConsSchEnt->maxLineRequired[consIbufIdx],
                                                                                0x0,
                                                                                0x1);

                                            if ((localKS - (s32)pConsSchEnt->iBufs[consIbufIdx]->bottomLineReplication) > (s32)(pSchEnt->linesPerIter - 1))
                                            {
                                                pSchEnt->canRunC = RS_CANNOT;
                                            }
                                        }
                                        else
                                        {
                                            last_ln_p = (s32)sippMLPIGetBufferEntry (pConsSchEnt->iBufs[consIbufIdx],
                                                                                     pConsSchEnt->parentKS[consIbufIdx]);

                                            // It has to have the KS offset for the last line relative to the first included in this
                                            // This KS offset is basically the (linesPerIter - 1) for most filters
                                            // However it may be different for other filters - eg colorCombChroma which is an effective upsampler of
                                            // chroma data
                                            frst_ln_p = (s32)sippMLPIGetBufferEntry (pConsSchEnt->iBufs[consIbufIdx],
                                                                                     pConsSchEnt->parentKS[consIbufIdx] + (pConsSchEnt->iBufCtx[consIbufIdx]->numLinesUsed - 1) + pConsSchEnt->iBufCtx[consIbufIdx]->KSDelta);

                                            // If consumers OVERWRITE occurs:
                                            if ((last_ln_p > last_ln_c) ||
                                                (frst_ln_p > frst_ln_c))
                                            {
                                                pSchEnt->canRunC = RS_CANNOT;
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        // Rewind filter
                        pSchEnt->numLineRuns -= pSchEnt->linesPerIter;
                        sippMLPIRewindSE (pSchEnt);
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
                pSchEnt->numLineRuns += pSchEnt->linesPerIter;
                sippMLPIRunSE (pSchEnt);
            }
        }

        //=====================================================================
        // Print runnable tasks:
        //=====================================================================
        if (pipeLine->dbgLevel > 0)
        {
            sippMLPIDbgPrintRunnable (ppSchEnts,
                                      nSchEnts,
                                      iteration);
        }

        sippMLPISchedWr (pipeLine,
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
        sippMLPIDbgShowBufferReq (ppSchEnts, nSchEnts);
    }
    #else
    VCS_PRINT_INT(0x6666);
    #endif

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPISchedPipeAnalyse                                         //
//                                                                                //
//  DESCRIPTION: Analyse filter connections to ascertain scheduling entities and  //
//               to work out what shortcutrs could be made if any in schedule     //
//               creation                                                         //
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

void sippMLPISchedPipeAnalyse (pSippPipeline pipeLine)
{
    UNUSED (pipeLine); //hush the compiler warning

    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    // Just placing this here as a reminder that most of what this function does will be valid only in MA2x5x
    // silicon (and later)
    #endif
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPIAllocRuntimeSched                                        //
//                                                                                //
//  DESCRIPTION: Allocate storage elements for schedule and associated data       //
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

u32 sippMLPIAllocRuntimeSched (pSippPipeline pipeLine)
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
        if (max_H > 0x100)
        {
            if (pipeLine->linesPerIter < 8)
            {
                pipeLine->schedInfoEntries  = max_H + (max_H >> 1);
                pipeLine->schedInfoEntries /= pipeLine->linesPerIter;
            }
            else
            {
                pipeLine->schedInfoEntries  = (max_H << 1);
                pipeLine->schedInfoEntries /= pipeLine->linesPerIter;
            }
        }
        else if (max_H <= 0x10)
        {
            pipeLine->schedInfoEntries  = (max_H << 2);
        }
        else
        {
            if (pipeLine->linesPerIter < 8)
            {
                pipeLine->schedInfoEntries  = (max_H << 1);
                pipeLine->schedInfoEntries /= pipeLine->linesPerIter;
            }
            else
            {
                pipeLine->schedInfoEntries  = (max_H << 2);
                pipeLine->schedInfoEntries /= pipeLine->linesPerIter;
            }
        }
    }

    pipeLine->schedInfo = (SchedInfo *) sippMemAlloc (&pipeLine->tHeapMCB,
                                                      vPoolSchedule,
                                                      pipeLine->schedInfoEntries * sizeof(SchedInfo));

    if (pipeLine->schedInfo == (SchedInfo *)NULL)
    {
        sippPalTrace (SIPP_CORE_TL_SEVERE, "sippMLPIAllocRuntimeSched : Failed to allocate runtime schedule\n");
        sippError (pipeLine->errorStatus, E_OUT_OF_MEM);
        return 0x1;
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPISchCreateSEFromFilter                                    //
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

u32 sippMLPISchCreateSEFromFilter (SippFilter * pFilter)
{
    u32          idx, maxOutputH;
    SippSchEnt * pSE         = pFilter->sch;
    u8           irregularKS = ((pFilter->linesPerIter > 1) &&
                                (pFilter->flags & SIPP_RESIZE) &&
                                (pFilter->iBufCtx[0]->funcAsk == mlpiAskResizer)) ? 0x1 : 0x0;

    // Set up the input buffer related items
    pSE->numIBufs     = pFilter->numIBufs;
    pSE->linesPerIter = pFilter->linesPerIter;

    for (idx = 0; idx < pSE->numIBufs; idx++)
    {
        pSE->recordParentKS[idx] = 1;
        pSE->iBufs[idx]          = pFilter->iBufs[idx];
        pSE->iBufCtx[idx]        = pFilter->iBufCtx[idx];
        pSE->runPadCheck[idx]    = 0;
        pSE->parentKS[idx]       = -1;
        pSE->parentKSMin[idx]    = -1;
        pSE->ptrFiltKS[idx]      = &pFilter->parentsKS[idx];

        if (irregularKS)
        {
            pSE->currKSOffset[idx] = (s16 *)sippMemAlloc (&pFilter->pPipe->tHeapMCB,
                                                          vPoolSchedule,
                                                          sizeof(s16) * pSE->linesPerIter);

            if (pSE->currKSOffset[idx] == (s16 *)NULL)
            {
                sippError (pFilter->errorStatus, E_OUT_OF_MEM );
                return 0x1;
            }
        }
        else
        {
            pSE->currKSOffset[idx] = (s16 *)NULL;
        }
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

    // Now that the maxOutputH is known we can work out how many KS locations
    // we need to store
    if (irregularKS)
    {
        u32 numIters;
        u32 numRuns;
        u32 linesPerIterShift = 0;
        u32 localLinesPerIter = pSE->linesPerIter;

        while (localLinesPerIter > 0x1)
        {
            localLinesPerIter >>= 0x1;
            linesPerIterShift++;
        }

        numIters = (maxOutputH >> linesPerIterShift) + ((maxOutputH & ((1 << linesPerIterShift) - 0x1)) ? 0x1 : 0x0);
        numRuns  = (numIters  << linesPerIterShift)  + pSE->linesPerIter; // Add pSE->linesPerIter as we may go one
                                                                          // chunk of pSE->linesPerIter lines beyond
                                                                          // the max height duirng schedule creation

        for (idx = 0; idx < pSE->numIBufs; idx++)
        {
            // Assign this in the filter structure - slightly breaks the modular runtime feel??
            pFilter->KSIterList[idx] = (u16 *)sippMemAlloc (&pFilter->pPipe->tHeapMCB,
                                                            vPoolSchedule,
                                                            sizeof(u16) * numRuns);

            if (pFilter->KSIterList[idx] == (u16 *)NULL)
            {
                sippError (pFilter->errorStatus, E_OUT_OF_MEM );
                return 0x1;
            }
            else
            {
                pSE->KSIterList[idx] = pFilter->KSIterList[idx];
            }
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

    if (pFilter->unit == SIPP_DMA_ID)
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
                pSE->pfRunAdditionalCheck         = sippMLPIDMACheck;
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

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPISchCreateSEFromOSE                                       //
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

void sippMLPISchCreateSEFromOSE (pSippOse pOSE)
{
    u32          idx, maxOutputH;
    SippSchEnt * pSE = pOSE->sch;

    // Set up the input buffer related items
    pSE->numIBufs     = pOSE->numIBufs;
    pSE->linesPerIter = pOSE->linesPerIter;

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
    pSE->numOBufs     = pOSE->numOBufs;

    for (idx = 0, maxOutputH = 0; idx < pSE->numOBufs; idx++)
    {
        pSE->oBufs[idx]          = pOSE->oBufs[idx];

        if(idx == 0)
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

}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPISchedAllocTempStorage                                    //
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

u32 sippMLPISchedAllocTempStorage (pSippPipeline pipeLine)
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
//  FUNCTION:    sippMLPISchedInitTempStorage                                     //
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

u32 sippMLPISchedInitTempStorage (pSippPipeline pipeLine)
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

        if (sippMLPISchCreateSEFromFilter (pFilter))
        {
            return 1;
        }

        #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
        }
        #endif
    }

    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    for (oseIdx = 0; oseIdx < pipeLine->numOSE; oseIdx++)
    {
        pOSE = pipeLine->pOSE[oseIdx];

        sippMLPISchCreateSEFromOSE (pOSE);

    }
    #endif

    return 0;
}


/////////////////////////////////////////////////////////////////////////////
// Scheduler interface API

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippMLPISchedule                                                 //
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
//  RETURNS:     0x0 if successful, 0x1 if processing error                       //                                                            //
//                                                                                //
//  NOTES:       None.                                                            //
//                                                                                //
//  CONTEXT:     This function may be called from any context                     //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippMLPISchedule (pSippPipeline pipeLine,
                      bool          allocMem,
                      bool          reschedPipe)
{
    UNUSED (reschedPipe);

    pipeLine->nIter = 0;

    // Analyse the pipe and split it into schedulable units
    sippMLPISchedPipeAnalyse (pipeLine);

    // Allocate temp storage elements
    if (sippMLPISchedAllocTempStorage (pipeLine))
    {
        return 0x1;
    }

    // Initialise temp storage elements
    if (sippMLPISchedInitTempStorage (pipeLine))
    {
         return 0x1;
    }

    if (allocMem)
    {
         // Allocate mem for runtime Scheduler info in CMX mempool
        if (sippMLPIAllocRuntimeSched (pipeLine))
        {
            return 0x1;
        }

        #ifdef SIPP_PC
        // Print lines per iter for each SE
        //if (pipeLine->dbgLevel)
        {
            SippSchEnt ** ppSchEnts = pipeLine->pSE;
            u32           nSchEnts  = pipeLine->numSE;
            u32           schEntIdx;

            for (schEntIdx = 0; schEntIdx < nSchEnts; schEntIdx++)
            {
                SippSchEnt * pSchEnt = ppSchEnts[schEntIdx];
                printf ("SE %3u LPI %3u\n", schEntIdx, pSchEnt->linesPerIter);
            }
        }
        #endif

    }

    // Figure out buffering requirements and the schedule
    if (sippMLPIRunScheduleGeneration (pipeLine) == 0)
    {
        // Process the info from schedule creation necessary to define the pipeline
        sippMLPIProcessScheduleInfo (pipeLine);

        #if defined(SIPP_PC) || defined(MYRIAD2)
        // TODO - Ask AL about this...
        sippMLPIPatchMipiTxStart (pipeLine);  //Patch mipi-TX start
        #endif

        #ifdef SIPP_RUNTIME_PROFILING
        if (pipeLine->pRTStats)
        {
            pipeLine->pRTStats->iterTime = (u64 *)sippMemAlloc (&pipeLine->tHeapMCB,
                                                                vPoolGeneral,
                                                                sizeof(u64) * SIPP_RT_PER_ITER_STATS_SIZE * pipeLine->nIter);

            if (pipeLine->pRTStats->iterTime == (u64 *)NULL)
            {
                sippPalTrace (SIPP_CORE_TL_SEVERE, "sippMLPISchedule : Failed to allocate runtime iter stats area\n");
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
//  FUNCTION:    sippMLPIScheduleSetBufConsModels                                 //
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

void sippMLPIScheduleSetBufConsModels (pSippPipeline pipeLine)
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
                    pFilter->iBufCtx[parentIdx]->funcAsk = mlpiAskPolyFirResizer;
                }
                else if (pFilter->unit == SIPP_CGEN_ID)
                {
                    pFilter->iBufCtx[parentIdx]->funcAsk = mlpiAskChromaGenDownsizer;
                }
                else
                #endif
                pFilter->iBufCtx[parentIdx]->funcAsk = mlpiAskResizer;
            }
            else if (pFilter->flags & SIPP_CROP)
            {
                pFilter->iBufCtx[parentIdx]->funcAsk = mlpiAskCrop;
            }
            else
            {
                pFilter->iBufCtx[parentIdx]->funcAsk = mlpiAskRegular;
            }

            // Custom scheduling functions:
            switch (pFilter->unit)
            {
                case SIPP_CC_ID     :   {
                                            if (pFilter->iBufCtx[parentIdx]->numLinesUsed > 0x1)
                                            {
                                                // Assume this is the chroma input of the colour combination filter
                                                // based on the vertical kernel size
                                                pFilter->iBufCtx[parentIdx]->funcAsk = mlpiAskHwColorCombChroma;
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
                                                    pFilter->iBufCtx[parentIdx]->funcAsk = mlpiAskHwMedLumaLatency;
                                                }
                                            }

                                        } break;
            }
        }
    }
}

#endif // SIPP_SCRT_ENABLE_MLPI

// End of file
