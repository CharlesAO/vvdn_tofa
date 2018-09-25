///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippInternal.h>

////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    askRegular & askRegularLatency                                   //
//                                                                                //
//  DESCRIPTION: Generate required max and min lines required from the specified  //
//               input buffer of a particular scheduling entity to run the entity //
//               for iteration XX where XX is the input parameter iterationNum    //
//               askRegular represents a standard filter operating with a linear  //
//               vertical kernel, while askRegularLatency represents exactly the  //
//               same but also checking any consumption latencies                 //
//                                                                                //
//  INPUTS:      pSE - Scheduling entity                                          //
//               iBufId - input buffer ID within the scheduling entity            //
//               iterationNum - the run iteration of the SE to consider the lines //
//               needed for                                                       //
//                                                                                //
//  OUTPUTS:     Results left in pSE->maxLineRequired[iBufId] &                   //
//               pSE->minLineRequired[iBufId].                                    //
//                                                                                //
//  RETURNS:     0 if No lines are required, 1 otherwise                          //
//                                                                                //
//  NOTES:       The functions are split into two to optimise performance for the //
//               more common askRegular function                                  //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u8 askRegular (SippSchEnt * pSE,
               u32          iBufId,
               s32          iterationNum,
               u32          linesPerIter)
{
    u32                  in_H, topLobe;
    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];
    UNUSED(linesPerIter);

    if (ptrUsage->lastAskLineNo != (u32)iterationNum)
    {
        ptrUsage->lastAskLineNo        = iterationNum;

        in_H                           =  pSE->iBufs[iBufId]->filtOutHeight - 1;
        topLobe                        = (ptrUsage->numLinesUsed - 1) >> 1;
        pSE->maxLineRequired[iBufId]   = (iterationNum - ptrUsage->consumptionLatency ) + (ptrUsage->numLinesUsed - 1);
        pSE->maxLineRequired[iBufId]  -= topLobe;
        pSE->minLineRequired[iBufId]   = (pSE->maxLineRequired[iBufId] > (u32)(ptrUsage->numLinesUsed-1)) ?
                                          pSE->maxLineRequired[iBufId] - (ptrUsage->numLinesUsed-1) :
                                          0;

        if (pSE->maxLineRequired[iBufId] > in_H)
        {
            pSE->maxLineRequired[iBufId] = in_H;
        }
        if (pSE->minLineRequired[iBufId] > in_H)
        {
            pSE->minLineRequired[iBufId] = in_H;
        }
    }

    return 1;
}

u8 askRegularLatency (SippSchEnt * pSE,
                      u32          iBufId,
                      s32          iterationNum,
                      u32          linesPerIter)
{
    u32                  in_H, topLobe;
    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];
    UNUSED(linesPerIter);

    if (iterationNum < (s32)ptrUsage->consumptionLatency)
    {
        // In this situation - the SE does not need any data from the input buffer
        // currently in order that it may run the specified iteration
        return 0;
    }
    else
    {
        if (ptrUsage->lastAskLineNo != (u32)iterationNum)
        {
            ptrUsage->lastAskLineNo        = iterationNum;

            in_H                           =  pSE->iBufs[iBufId]->filtOutHeight - 1;
            topLobe                        = (ptrUsage->numLinesUsed - 1) >> 1;
            pSE->maxLineRequired[iBufId]   = (iterationNum - ptrUsage->consumptionLatency ) + (ptrUsage->numLinesUsed - 1);
            pSE->maxLineRequired[iBufId]  -= topLobe;
            pSE->minLineRequired[iBufId]   = (pSE->maxLineRequired[iBufId] > (u32)(ptrUsage->numLinesUsed-1)) ?
                                              pSE->maxLineRequired[iBufId] - (ptrUsage->numLinesUsed-1) :
                                              0;

            if (pSE->maxLineRequired[iBufId] > in_H)
            {
                pSE->maxLineRequired[iBufId] = in_H;
            }
            if (pSE->minLineRequired[iBufId] > in_H)
            {
                pSE->minLineRequired[iBufId] = in_H;
            }
        }
    }

    return 1;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    askCrop & askCropLatency                                         //
//                                                                                //
//  DESCRIPTION: Generate required max and min lines required from the specified  //
//               input buffer of a particular scheduling entity to run the entity //
//               for iteration XX where XX is the input parameter iterationNum    //
//               askCrop represents a cropping filter,askCroplatency represents   //
//               exactly the same but also checking any consumption latencies     //
//                                                                                //
//  INPUTS:      pSE - Scheduling entity                                          //
//               iBufId - input buffer ID within the scheduling entity            //
//               iterationNum - the run iteration of the SE to consider the lines //
//               needed for                                                       //
//                                                                                //
//  OUTPUTS:     Results left in pSE->maxLineRequired[iBufId] &                   //
//               pSE->minLineRequired[iBufId].                                    //
//                                                                                //
//  RETURNS:     0 if No lines are required, 1 otherwise                          //
//                                                                                //
//  NOTES:       The functions are split into two to optimise performance for the //
//               more common askCrop function                                     //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u8 askCrop (SippSchEnt * pSE,
            u32          iBufId,
            s32          iterationNum,
            u32          linesPerIter)
{
    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];
    UNUSED(linesPerIter);

    if (ptrUsage->lastAskLineNo  != (u32)iterationNum)
    {
        u32 in_H =  pSE->iBufs[iBufId]->filtOutHeight - 1;

        ptrUsage->lastAskLineNo   = iterationNum;

        //parent 0, first_line
        pSE->maxLineRequired[iBufId] = iterationNum + *((s32 *)ptrUsage->params);
        pSE->minLineRequired[iBufId] = pSE->maxLineRequired[iBufId];

        //TBD: use proper cast to CropParam... put this in a standard place

        if (pSE->maxLineRequired[iBufId] > in_H)
        {
            pSE->maxLineRequired[iBufId] = in_H;
        }
        if (pSE->minLineRequired[iBufId] > in_H)
        {
            pSE->minLineRequired[iBufId] = in_H;
        }
    }

    return 1;
}


u8 askCropLatency (SippSchEnt * pSE,
                   u32          iBufId,
                   s32          iterationNum,
                   u32          linesPerIter)
{
    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];
    UNUSED(linesPerIter);

    if (iterationNum < (s32)ptrUsage->consumptionLatency)
    {
        // In this situation - the SE does not need any data from this input buffer
        // currently in order that it may run the next iteration
        return 0;
    }
    else
    {
        if (ptrUsage->lastAskLineNo  != (u32)iterationNum)
        {
            u32 in_H =  pSE->iBufs[iBufId]->filtOutHeight - 1;

            ptrUsage->lastAskLineNo   = iterationNum;

            //parent 0, first_line
            pSE->maxLineRequired[iBufId] = iterationNum + *((s32 *)ptrUsage->params);
            pSE->minLineRequired[iBufId] = pSE->maxLineRequired[iBufId];

            //TBD: use proper cast to CropParam... put this in a standard place

            if (pSE->maxLineRequired[iBufId] > in_H)
            {
                pSE->maxLineRequired[iBufId] = in_H;
            }
            if (pSE->minLineRequired[iBufId] > in_H)
            {
                pSE->minLineRequired[iBufId] = in_H;
            }
        }
    }

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    askHwColorCombChroma                                             //
//                                                                                //
//  DESCRIPTION: Generate required max and min lines required from the specified  //
//               input buffer of a particular scheduling entity to run the entity //
//               for iteration XX where XX is the input parameter iterationNum    //
//               askHwColorCombChroma represents a colour combination filter      //
//               consuming from its chroma input buffer                           //
//                                                                                //
//  INPUTS:      pSE - Scheduling entity                                          //
//               iBufId - input buffer ID within the scheduling entity            //
//               iterationNum - the run iteration of the SE to consider the lines //
//               needed for                                                       //
//                                                                                //
//  OUTPUTS:     Results left in pSE->maxLineRequired[iBufId] &                   //
//               pSE->minLineRequired[iBufId].                                    //
//                                                                                //
//  RETURNS:     0 if No lines are required, 1 otherwise                          //
//                                                                                //
//  NOTES:       The color combination filter can only be the first filter in a   //
//               SE so it should never have a consumption latency on its input    //
//               buffers                                                          //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u8 askHwColorCombChroma (SippSchEnt * pSE,
                         u32          iBufId,
                         s32          iterationNum,
                         u32          linesPerIter)
{
    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];
    UNUSED(linesPerIter);

    if (ptrUsage->lastAskLineNo  != (u32)iterationNum)
    {
        u32 in_H            =  pSE->iBufs[iBufId]->filtOutHeight - 1;
        u32 chromaCentreVal = iterationNum >> 0x1;

        ptrUsage->lastAskLineNo   = iterationNum;

        //from Chroma parent (i.e. parent 1), we need 5 lines
        pSE->maxLineRequired[iBufId] = (chromaCentreVal) + 2;
        pSE->minLineRequired[iBufId] = (chromaCentreVal > 1) ? chromaCentreVal - 2 : 0;

        if (pSE->maxLineRequired[iBufId] > in_H)
        {
            pSE->maxLineRequired[iBufId] = in_H;
        }
        if (pSE->minLineRequired[iBufId] > in_H)
        {
            pSE->minLineRequired[iBufId] = in_H;
        }
    }

    return 1;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    askResizer & askResizerLatency                                   //
//                                                                                //
//  DESCRIPTION: Generate required max and min lines required from the specified  //
//               input buffer of a particular scheduling entity to run the entity //
//               for iteration XX where XX is the input parameter iterationNum    //
//               askResizer represents a scaling filter, askResizerLatency        //
//               represents exactly the same but also checking any consumption    //
//               latencies                                                        //
//                                                                                //
//  INPUTS:      pSE - Scheduling entity                                          //
//               iBufId - input buffer ID within the scheduling entity            //
//               iterationNum - the run iteration of the SE to consider the lines //
//               needed for                                                       //
//                                                                                //
//  OUTPUTS:     Results left in pSE->maxLineRequired[iBufId] &                   //
//               pSE->minLineRequired[iBufId].                                    //
//                                                                                //
//  RETURNS:     0 if No lines are required, 1 otherwise                          //
//                                                                                //
//  NOTES:       The functions are split into two to optimise performance for the //
//               more common askResizer function                                  //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u8 askResizer (SippSchEnt * pSE,
               u32          iBufId,
               s32          iterationNum,
               u32          linesPerIter)
{
    u32 topLobe;
    UNUSED(linesPerIter);

    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];

    if (ptrUsage->lastAskLineNo  != (u32)iterationNum)
    {
        s32   input_H   = pSE->iBufs[iBufId]->filtOutHeight;
        s32   output_H  = ptrUsage->outputHeight;
        float z_factor  = (float)output_H / input_H;

        s32   center_ln = (s32)sippGetCoord2 (iterationNum,
                                              z_factor,
                                              input_H /2.0f,  // Centered scale
                                              output_H/2.0f); // Centered scale

        ptrUsage->lastAskLineNo   = iterationNum;

        topLobe = (ptrUsage->numLinesUsed - 1) >> 1;

        pSE->maxLineRequired[iBufId] = center_ln + (ptrUsage->numLinesUsed - 1);
        pSE->maxLineRequired[iBufId] -= topLobe;
        pSE->minLineRequired[iBufId] = (center_ln > (s32)topLobe) ? center_ln - topLobe : 0;

        if (pSE->maxLineRequired[iBufId] > (u32)(input_H - 1))
        {
            pSE->runPadCheck[iBufId]     = 1;
            pSE->maxLineRequired[iBufId] = input_H - 1;
        }
        else
        {
            pSE->runPadCheck[iBufId]     = 0;
        }
        if (pSE->minLineRequired[iBufId] > (u32)(input_H - 1))
        {
            pSE->minLineRequired[iBufId] = input_H - 1;
        }
    }

    return 1;

}


u8 askPolyFirResizer (SippSchEnt * pSE,
                      u32          iBufId,
                      s32          iterationNum,
                      u32          linesPerIter)
{
    u32                  topLobe;
    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];
    UNUSED(linesPerIter);

    if (ptrUsage->lastAskLineNo  != (u32)iterationNum)
    {
        s32            input_H = pSE->iBufs[iBufId]->filtOutHeight;
        PolyFirParam * param   = (PolyFirParam *)ptrUsage->params;

        // Get access to the real numerator and denominator
        u32 vDenom = param->vertD;
        u32 vNum   = param->vertN;
        s32 center_ln;

        ptrUsage->lastAskLineNo = iterationNum;

        topLobe = (ptrUsage->numLinesUsed - 1) >> 1;

        center_ln = ((iterationNum * vDenom) / vNum);

        pSE->maxLineRequired[iBufId] = center_ln + (ptrUsage->numLinesUsed - 1);
        pSE->maxLineRequired[iBufId] -= topLobe;
        pSE->minLineRequired[iBufId] = (center_ln > (s32)topLobe) ? center_ln - topLobe : 0;

        if (pSE->maxLineRequired[iBufId] > (u32)(input_H - 1))
        {
            pSE->runPadCheck[iBufId]     = 1;
            pSE->maxLineRequired[iBufId] = input_H - 1;
        }
        else
        {
            pSE->runPadCheck[iBufId]     = 0;
        }

        if (pSE->minLineRequired[iBufId] > (u32)(input_H - 1))
        {
            pSE->minLineRequired[iBufId] = input_H - 1;
        }
    }

    return 1;

 }


/////////////////////////////////////////////////////
// Attempting to make this generic for possible re-use instead of being
// gen chroma specific but the fact that the kernel size is in effect
// an even number is currently making this prohibitive

u8 askChromaGenDownsizer (SippSchEnt * pSE,
                          u32          iBufId,
                          s32          iterationNum,
                          u32          linesPerIter)
{
    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];
    UNUSED(linesPerIter);

    if (ptrUsage->lastAskLineNo  != (u32)iterationNum)
    {
        s32 input_H     = pSE->iBufs[iBufId]->filtOutHeight;
        s32 output_H    = ptrUsage->outputHeight;
        u8  scaleFactor = 0;
        u32 centreLnStart;
        u32 lobeHeight;

        ptrUsage->lastAskLineNo = iterationNum;

        do
        {
            input_H >>= 1;
            scaleFactor++;

        } while (input_H != output_H);

        input_H       = pSE->iBufs[iBufId]->filtOutHeight;
        centreLnStart = (iterationNum << scaleFactor );
        lobeHeight    = ((ptrUsage->numLinesUsed >> 2) << scaleFactor);
        pSE->minLineRequired[iBufId] = (centreLnStart > lobeHeight) ? centreLnStart - lobeHeight : 0;
        pSE->maxLineRequired[iBufId] = centreLnStart + ((lobeHeight << 1) - 1);

        if (pSE->maxLineRequired[iBufId] > (u32)(input_H - 1))
        {
            pSE->runPadCheck[iBufId]     = 1;
            pSE->maxLineRequired[iBufId] = input_H - 1;
        }
        else
        {
            pSE->runPadCheck[iBufId]     = 0;
        }

        if (pSE->minLineRequired[iBufId] > (u32)(input_H - 1))
        {
            pSE->minLineRequired[iBufId] = input_H - 1;
        }
    }

    return 1;

 }

u8 askResizerLatency (SippSchEnt * pSE,
                      u32          iBufId,
                      s32          iterationNum,
                      u32          linesPerIter)
{
    u32 topLobe;
    UNUSED(linesPerIter);

    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];

    if (iterationNum < (s32)ptrUsage->consumptionLatency)
    {
        // In this situation - the SE does not need any data from this input buffer
        // currently in order that it may run the next iteration
        return 0;
    }
    else
    {
        if (ptrUsage->lastAskLineNo  != (u32)iterationNum)
        {

            s32   input_H   = pSE->iBufs[iBufId]->filtOutHeight;
            s32   output_H  = ptrUsage->outputHeight;
            float z_factor  = (float)output_H / input_H;

            s32   center_ln = (s32)sippGetCoord2 (iterationNum,
                                                  z_factor,
                                                  input_H /2.0f,  // Centered scale
                                                  output_H/2.0f); // Centered scale

            ptrUsage->lastAskLineNo   = iterationNum;

            topLobe = (ptrUsage->numLinesUsed - 1) >> 1;

            pSE->maxLineRequired[iBufId] = center_ln + (ptrUsage->numLinesUsed - 1);
            pSE->maxLineRequired[iBufId] -= topLobe;
            pSE->minLineRequired[iBufId] = (center_ln > (s32)topLobe) ? center_ln - topLobe : 0;

            if (pSE->maxLineRequired[iBufId] > (u32)(input_H - 1))
            {
                pSE->runPadCheck[iBufId]     = 1;
                pSE->maxLineRequired[iBufId] = input_H - 1;
            }
            else
            {
                pSE->runPadCheck[iBufId]     = 0;
            }
            if (pSE->minLineRequired[iBufId] > (u32)(input_H - 1))
            {
                pSE->minLineRequired[iBufId] = input_H - 1;
            }
        }
    }

    return 1;

}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    askHwMedLumaLatency                                              //
//                                                                                //
//  DESCRIPTION: Generate required max and min lines required from the specified  //
//               input buffer of a particular scheduling entity to run the entity //
//               for iteration XX where XX is the input parameter iterationNum    //
//               This function represents the median filter operating on a luma   //
//               input buffer used for alpha blending while also checking any     //
//               consumption latencies                                            //
//                                                                                //
//  INPUTS:      pSE - Scheduling entity                                          //
//               iBufId - input buffer ID within the scheduling entity            //
//               iterationNum - the run iteration of the SE to consider the lines //
//               needed for                                                       //
//                                                                                //
//  OUTPUTS:     Results left in pSE->maxLineRequired[iBufId] &                   //
//               pSE->minLineRequired[iBufId].                                    //
//                                                                                //
//  RETURNS:     0 if No lines are required, 1 otherwise                          //
//                                                                                //
//  NOTES:       The functions are split into two to optimise performance for the //
//               more common askHwMedLuma function                                //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u8 askHwMedLumaLatency (SippSchEnt * pSE,
                        u32          iBufId,
                        s32          iterationNum,
                        u32          linesPerIter)
{
    u32                  in_H;
    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];
    UNUSED(linesPerIter);

    if (iterationNum < (s32)ptrUsage->consumptionLatency)
    {
        // In this situation - the SE does not need any data from the input buffer
        // currently in order that it may run the specified iteration
        return 0;
    }
    else
    {
        if (ptrUsage->lastAskLineNo != (u32)iterationNum)
        {
            ptrUsage->lastAskLineNo        = iterationNum;

            in_H                           =  pSE->iBufs[iBufId]->filtOutHeight - 1;
            pSE->maxLineRequired[iBufId]   = ((iterationNum - ptrUsage->consumptionLatency ) << 0x1 ) + (ptrUsage->numLinesUsed - 1);
            pSE->minLineRequired[iBufId]   = ((iterationNum - ptrUsage->consumptionLatency ) << 0x1 );

            if (pSE->maxLineRequired[iBufId] > in_H)
            {
                pSE->maxLineRequired[iBufId] = in_H;
            }
            if (pSE->minLineRequired[iBufId] > in_H)
            {
                pSE->minLineRequired[iBufId] = in_H;
            }
        }
    }

    return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Additional scheduling checks
// These are bespoke checks for certain filters in certain configs

void sippGenericDMACheck (void * pPipe, void * pSE, void * pParam)
{
    pSippPipeline ptrPipe = (pSippPipeline)pPipe;
    SippSchEnt *  pDMASE  = (SippSchEnt *)pSE;
    u32           consumerIdx;
    u32           maxLinesUsed;

    UNUSED(pParam);

    // Does any consumer have a KS set? If so that becomes sufficient to control DMA
    for (consumerIdx = 0; consumerIdx < pDMASE->numConsumers; consumerIdx++)
    {
        SippSchEnt * pConsSchEnt = pDMASE->cons[consumerIdx];
        u32          consIbufIdx = pDMASE->consIbufIdx[consumerIdx];

        if ((int)pConsSchEnt->parentKS[consIbufIdx] != -1)
        {
            ptrPipe->runAddSchedCheck &= (~(0x1 << pDMASE->pipeSEId));

            return;
        }
    }

    // Until a consumer is capable of running - cap the number of lines in the DMA to
    // the largest value of numLinesused among its consumers
    for (consumerIdx = 0, maxLinesUsed = 0; consumerIdx < pDMASE->numConsumers; consumerIdx++)
    {
        SippSchEnt * pConsSchEnt = pDMASE->cons[consumerIdx];
        u32          consIbufIdx = pDMASE->consIbufIdx[consumerIdx];

        if (pConsSchEnt->iBufCtx[consIbufIdx]->numLinesUsed > maxLinesUsed)
        {
            maxLinesUsed = pConsSchEnt->iBufCtx[consIbufIdx]->numLinesUsed;
        }
    }

    // If the buffer size exceeds this cap, prevent DMA from running
    if (pDMASE->oBufs[0x0]->mostRecentLine > (int)maxLinesUsed)
    {
        pDMASE->canRunP = RS_CANNOT;
        pDMASE->canRunC = RS_CANNOT;
    }
}


void sippGenericColorCombChromaCheck (void * pPipe, void * pSE, void * pParam)
{
    pSippPipeline ptrPipe      = (pSippPipeline)pPipe;
    SippSchEnt *  pColorCombSE = (SippSchEnt *)pSE;
    SippSchEnt *  pParentSE    = (SippSchEnt *)pParam;

    // Always start the color comb filter on a cycle in which the chroma input will not run
    // This prevents sending any delay on the chroma pipe which if this pipe is linked back to
    // luma pipe can cause cyclic delays
    // This is not the case if the chroma is not sub-sampled, but we remove this check after
    // the first time it executes
    if (pColorCombSE->canRunP == RS_CAN_RUN)
    {
        if (pParentSE->canRunP == RS_CAN_RUN)
        {
            u32 idx;
            pColorCombSE->canRunP = RS_CANNOT;

            for (idx = 0; idx < pColorCombSE->numIBufs; idx++)
            {
                pColorCombSE->recordParentKS[idx] = 1;
                pColorCombSE->parentKS[idx]       = -1;
                pColorCombSE->parentKSMin[idx]    = -1;
            }

        }

        ptrPipe->runAddSchedCheck &= (~(0x1 << pColorCombSE->pipeSEId));

    }
}
