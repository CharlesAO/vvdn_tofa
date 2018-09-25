///
/// @file      sippMLPISchedDebug.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Multiple Line Per Iteration (MLPI) Scheduler buffer
///            consumption model functions
///            Platform(s) supported : MA2x5x
///

#ifdef SIPP_SCRT_ENABLE_MLPI

////////////////////////////////////////////////////
// Header files
////////////////////////////////////////////////////

#include <sipp.h>
#include <sippInternal.h>

////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    mlpiAskRegular & mlpiAskRegularLatency                           //
//                                                                                //
//  DESCRIPTION: Generate required max and min lines required from the specified  //
//               input buffer of a particular scheduling entity to run the entity //
//               for iterations XX to YY where XX is the input parameter          //
//               lineRunNum and YY is lineRunNum + linesPerIter - 1               //
//               askRegular represents a standard filter operating with a linear  //
//               vertical kernel, while askRegularLatency represents exactly the  //
//               same but also checking any consumption latencies                 //
//                                                                                //
//  INPUTS:      pSE - Scheduling entity                                          //
//               iBufId - input buffer ID within the scheduling entity            //
//               lineRunNum - the first run iteration of the SE to consider the   //
//                            lines needed for                                    //
//               linesPerIter - the  number of lines to be produced by the SE     //
//                              on the current run under consideration            //
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

u8 mlpiAskRegular (SippSchEnt * pSE,
                   u32          iBufId,
                   s32          lineRunNum,
                   u32          linesPerIter)
{
    u32                  in_H, topLobe;
    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];

    if (ptrUsage->lastAskLineNo != (u32)lineRunNum)
    {
        u32 baseLine                   = (lineRunNum - ptrUsage->consumptionLatency );
        ptrUsage->lastAskLineNo        = lineRunNum;
        ptrUsage->iPadLines            = 0;

        in_H                           =  pSE->iBufs[iBufId]->filtOutHeight - 1;
        topLobe                        = (ptrUsage->numLinesUsed - 1) >> 1;
        pSE->maxLineRequired[iBufId]   = baseLine +  (ptrUsage->numLinesUsed - 1) + (linesPerIter - 0x1);
        pSE->maxLineRequired[iBufId]  -= topLobe;
        pSE->minLineRequired[iBufId]   = (baseLine > topLobe) ?
                                          baseLine - topLobe :
                                          0;

        if (pSE->maxLineRequired[iBufId] > in_H)
        {
            ptrUsage->iPadLines          = pSE->maxLineRequired[iBufId] - in_H;
            pSE->maxLineRequired[iBufId] = in_H;
        }
        if (pSE->minLineRequired[iBufId] > in_H)
        {
            pSE->minLineRequired[iBufId] = in_H;
        }
    }

    return 1;
}

u8 mlpiAskRegularLatency (SippSchEnt * pSE,
                          u32          iBufId,
                          s32          lineRunNum,
                          u32          linesPerIter)
{
    u32                  in_H, topLobe;
    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];

    if ((lineRunNum + (linesPerIter - 0x1)) < ptrUsage->consumptionLatency)
    {
        // In this situation - the SE does not need any data from the input buffer
        // currently in order that it may run the specified iterations
        return 0;
    }
    else
    {
        if (ptrUsage->lastAskLineNo != (u32)lineRunNum)
        {
            ptrUsage->lastAskLineNo        = lineRunNum;
            ptrUsage->iPadLines            = 0;

            in_H                           =  pSE->iBufs[iBufId]->filtOutHeight - 1;
            topLobe                        = (ptrUsage->numLinesUsed - 1) >> 1;
            pSE->maxLineRequired[iBufId]   = (lineRunNum - ptrUsage->consumptionLatency ) + (ptrUsage->numLinesUsed - 1) + (linesPerIter - 0x1);
            pSE->maxLineRequired[iBufId]  -= topLobe;
            pSE->minLineRequired[iBufId]   = (pSE->maxLineRequired[iBufId] > (u32)(ptrUsage->numLinesUsed-1)) ?
                                              pSE->maxLineRequired[iBufId] - (ptrUsage->numLinesUsed-1) :
                                              0;

            if (pSE->maxLineRequired[iBufId] > in_H)
            {
                ptrUsage->iPadLines          = pSE->maxLineRequired[iBufId] - in_H;
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
//  FUNCTION:    mlpiAskCrop & mlpiAskCropLatency                                 //
//                                                                                //
//  DESCRIPTION: Generate required max and min lines required from the specified  //
//               input buffer of a particular scheduling entity to run the entity //
//               for iterations XX to YY where XX is the input parameter          //
//               lineRunNum and YY is lineRunNum + linesPerIter - 1               //
//               askCrop represents a cropping filter,askCroplatency represents   //
//               exactly the same but also checking any consumption latencies     //
//                                                                                //
//  INPUTS:      pSE - Scheduling entity                                          //
//               iBufId - input buffer ID within the scheduling entity            //
//               lineRunNum - the first run iteration of the SE to consider the   //
//                            lines needed for                                    //
//               linesPerIter - the  number of lines to be produced by the SE     //
//                              on the current run under consideration            //
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

u8 mlpiAskCrop (SippSchEnt * pSE,
                u32          iBufId,
                s32          lineRunNum,
                u32          linesPerIter)
{
    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];

    if (ptrUsage->lastAskLineNo  != (u32)lineRunNum)
    {
        u32 in_H =  pSE->iBufs[iBufId]->filtOutHeight - 1;

        ptrUsage->lastAskLineNo   = lineRunNum;
        ptrUsage->iPadLines       = 0;

        //parent 0, first_line
        pSE->maxLineRequired[iBufId] = lineRunNum + *((s32*)ptrUsage->params) + (linesPerIter - 0x1);
        pSE->minLineRequired[iBufId] = lineRunNum + *((s32*)ptrUsage->params);

        //TBD: use proper cast to CropParam... put this in a standard place

        if (pSE->maxLineRequired[iBufId] > in_H)
        {
            ptrUsage->iPadLines          = pSE->maxLineRequired[iBufId] - in_H;
            pSE->maxLineRequired[iBufId] = in_H;
        }
        if (pSE->minLineRequired[iBufId] > in_H)
        {
            pSE->minLineRequired[iBufId] = in_H;
        }
    }

    return 1;
}


u8 mlpiAskCropLatency (SippSchEnt * pSE,
                       u32          iBufId,
                       s32          lineRunNum,
                       u32          linesPerIter)
{
    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];

    if ((lineRunNum + (linesPerIter - 0x1)) < ptrUsage->consumptionLatency)
    {
        // In this situation - the SE does not need any data from this input buffer
        // currently in order that it may run the next iterations
        return 0;
    }
    else
    {
        if (ptrUsage->lastAskLineNo  != (u32)lineRunNum)
        {
            u32 in_H =  pSE->iBufs[iBufId]->filtOutHeight - 1;

            ptrUsage->lastAskLineNo   = lineRunNum;

            //parent 0, first_line
            pSE->maxLineRequired[iBufId] = lineRunNum + *((s32*)ptrUsage->params) + (linesPerIter - 0x1);
            pSE->minLineRequired[iBufId] = lineRunNum + *((s32*)ptrUsage->params);

            //TBD: use proper cast to CropParam... put this in a standard place

            if (pSE->maxLineRequired[iBufId] > in_H)
            {
                ptrUsage->iPadLines          = pSE->maxLineRequired[iBufId] - in_H;
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
//  FUNCTION:    mlpiAskHwColorCombChroma                                         //
//                                                                                //
//  DESCRIPTION: Generate required max and min lines required from the specified  //
//               input buffer of a particular scheduling entity to run the entity //
//               for iterations XX to YY where XX is the input parameter          //
//               lineRunNum and YY is lineRunNum + linesPerIter - 1               //
//               askHwColorCombChroma represents a colour combination filter      //
//               consuming from its chroma input buffer                           //
//                                                                                //
//  INPUTS:      pSE - Scheduling entity                                          //
//               iBufId - input buffer ID within the scheduling entity            //
//               lineRunNum - the first run iteration of the SE to consider the   //
//                            lines needed for                                    //
//               linesPerIter - the  number of lines to be produced by the SE     //
//                              on the current run under consideration            //
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

u8 mlpiAskHwColorCombChroma (SippSchEnt * pSE,
                             u32          iBufId,
                             s32          lineRunNum,
                             u32          linesPerIter)
{
    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];

    if (ptrUsage->lastAskLineNo  != (u32)lineRunNum)
    {
        u32 in_H            =  pSE->iBufs[iBufId]->filtOutHeight - 1;
        u32 chromaCentreVal = lineRunNum >> 0x1;

        ptrUsage->lastAskLineNo   = lineRunNum;
        ptrUsage->iPadLines       = 0;

        // From Chroma parent (i.e. parent 1), we need 5 lines
        pSE->minLineRequired[iBufId] = (chromaCentreVal > 1) ? chromaCentreVal - 2 : 0;
        chromaCentreVal              = (lineRunNum + (linesPerIter - 0x1)) >> 0x1;
        pSE->maxLineRequired[iBufId] = (chromaCentreVal + 2);

        if (pSE->maxLineRequired[iBufId] > in_H)
        {
            ptrUsage->iPadLines          = pSE->maxLineRequired[iBufId] - in_H;
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
//  FUNCTION:    mlpiAskResizer                                                   //
//                                                                                //
//  DESCRIPTION: Generate required max and min lines required from the specified  //
//               input buffer of a particular scheduling entity to run the entity //
//               for iterations XX to YY where XX is the input parameter          //
//               lineRunNum and YY is lineRunNum + linesPerIter - 1               //
//               askResizer represents a scaling filter                           //
//                                                                                //
//  INPUTS:      pSE - Scheduling entity                                          //
//               iBufId - input buffer ID within the scheduling entity            //
//               lineRunNum - the first run iteration of the SE to consider the   //
//                            lines needed for                                    //
//               linesPerIter - the  number of lines to be produced by the SE     //
//                              on the current run under consideration            //
//                                                                                //
//  OUTPUTS:     Results left in pSE->maxLineRequired[iBufId] &                   //
//               pSE->minLineRequired[iBufId].                                    //
//                                                                                //
//  RETURNS:     0 if No lines are required, 1 otherwise                          //
//                                                                                //
//  NOTES:                                                                        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u8 mlpiAskResizer (SippSchEnt * pSE,
                   u32          iBufId,
                   s32          lineRunNum,
                   u32          linesPerIter)
{
    u32 topLobe;

    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];

    if (ptrUsage->lastAskLineNo  != (u32)lineRunNum)
    {
        s32   input_H   = pSE->iBufs[iBufId]->filtOutHeight;
        s32   output_H  = ptrUsage->outputHeight;
        float z_factor  = (float)output_H / input_H;

        s32   center_ln = (s32)sippGetCoord2 (lineRunNum,
                                              z_factor,
                                              input_H / 2.0f,
                                              output_H / 2.0f);

        ptrUsage->lastAskLineNo = lineRunNum;
        ptrUsage->iPadLines     = 0;

        topLobe = (ptrUsage->numLinesUsed - 1) >> 1;

        pSE->minLineRequired[iBufId] = (center_ln > (s32)topLobe) ? center_ln - topLobe : 0;

        // Gather the KS offsets for all iterations - note we use the offset as the distance from the KS
        // for the maximum iteration
        if (linesPerIter > 1)
        {
            u32 iterIdx;
            s32 oldCentreLine = center_ln;

            // Establish the centre line for the max iteration - no need to find the KS from this as the relative
            // distance between centre lines for each iteration will be the same as between kernel starts
            center_ln = (s16)sippGetCoord2 (lineRunNum + (linesPerIter - 1),
                                            z_factor,
                                            input_H / 2.0f,
                                            output_H / 2.0f);

            // Max iteration offset from itself is 0!
            pSE->currKSOffset[iBufId][(linesPerIter - 1)] = 0;

            for (iterIdx = (linesPerIter - 2); iterIdx; iterIdx--)
            {
                pSE->currKSOffset[iBufId][iterIdx]  = center_ln - (s32)sippGetCoord2 (lineRunNum + iterIdx,
                                                                                      z_factor,
                                                                                      input_H / 2.0f,  // centered scale
                                                                                      output_H / 2.0f);
            }

            pSE->currKSOffset[iBufId][iterIdx] = center_ln - oldCentreLine;
        }

        pSE->maxLineRequired[iBufId] = center_ln + (ptrUsage->numLinesUsed - 1);
        pSE->maxLineRequired[iBufId] -= topLobe;

        if (pSE->maxLineRequired[iBufId] > (u32)(input_H - 1))
        {
            pSE->runPadCheck[iBufId]     = 1;
            ptrUsage->iPadLines          = pSE->maxLineRequired[iBufId] - (u32)(input_H - 1);
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

        // Note we should probably have similar max line checks for the pSE->currKS entries but since
        // they may well be beyond the end of the pic, do we care?

    }

    return 1;

}


u8 mlpiAskPolyFirResizer (SippSchEnt * pSE,
                          u32          iBufId,
                          s32          lineRunNum,
                          u32          linesPerIter)
{
    u32 topLobe;

    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];

    if (ptrUsage->lastAskLineNo  != (u32)lineRunNum)
    {
        s32            input_H = pSE->iBufs[iBufId]->filtOutHeight;
        PolyFirParam * param   = (PolyFirParam *)ptrUsage->params;

        // Get access to the real numerator and denominator
        u32 vDenom = param->vertD;
        u32 vNum   = param->vertN;
        s32 center_ln;

        ptrUsage->lastAskLineNo = lineRunNum;
        ptrUsage->iPadLines     = 0;

        topLobe = (ptrUsage->numLinesUsed - 1) >> 1;

        center_ln = ((lineRunNum * vDenom) / vNum);

        pSE->minLineRequired[iBufId] = (center_ln > (s32)topLobe) ? center_ln - topLobe : 0;

        if (linesPerIter > 1)
        {
            center_ln = (((lineRunNum + (linesPerIter - 1)) * vDenom) / vNum);
        }

        pSE->maxLineRequired[iBufId] = center_ln + (ptrUsage->numLinesUsed - 1);
        pSE->maxLineRequired[iBufId] -= topLobe;

        if (pSE->maxLineRequired[iBufId] > (u32)(input_H - 1))
        {
            pSE->runPadCheck[iBufId]     = 1;
            ptrUsage->iPadLines          = pSE->maxLineRequired[iBufId] - (u32)(input_H - 1);
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

        // Note we should probably have similar max line checks for the pSE->currKS entries but since
        // they may well be beyond the end of the pic, do we care?

    }

    return 1;

 }


/////////////////////////////////////////////////////
// Attempting to make this generic for possible re-use instead of being
// gen chroma specific but the fact that the kernel size is in effect
// an even number is currently making this prohibitive

u8 mlpiAskChromaGenDownsizer (SippSchEnt * pSE,
                              u32          iBufId,
                              s32          lineRunNum,
                              u32          linesPerIter)
{
    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];

    if (ptrUsage->lastAskLineNo  != (u32)lineRunNum)
    {
        s32 input_H     = pSE->iBufs[iBufId]->filtOutHeight;
        s32 output_H    = ptrUsage->outputHeight;
        u8  scaleFactor = 0;
        u32 centreLnStart;
        u32 lobeHeight;

        ptrUsage->lastAskLineNo = lineRunNum;
        ptrUsage->iPadLines     = 0;

        do
        {
            input_H >>= 1;
            scaleFactor++;

        } while (input_H != output_H);

        input_H       = pSE->iBufs[iBufId]->filtOutHeight;
        centreLnStart = (lineRunNum << scaleFactor );
        lobeHeight    = ((ptrUsage->numLinesUsed >> 2) << scaleFactor);
        pSE->minLineRequired[iBufId] = (centreLnStart > lobeHeight) ? centreLnStart - lobeHeight : 0;
        pSE->maxLineRequired[iBufId] = centreLnStart + ((lobeHeight << 1) - 1) + ((linesPerIter - 1) << scaleFactor);

        if (pSE->maxLineRequired[iBufId] > (u32)(input_H - 1))
        {
            pSE->runPadCheck[iBufId]     = 1;
            ptrUsage->iPadLines          = pSE->maxLineRequired[iBufId] -  (u32)(input_H - 1);
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

        // Note we should probably have similar max line checks for the pSE->currKS entries but since
        // they may well be beyond the end of the pic, do we care?

    }

    return 1;

}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    mlpiAskHwMedLumaLatency                                          //
//                                                                                //
//  DESCRIPTION: Generate required max and min lines required from the specified  //
//               input buffer of a particular scheduling entity to run the entity //
//               for iterations XX to YY where XX is the input parameter          //
//               lineRunNum and YY is lineRunNum + linesPerIter - 1               //
//               This function represents the median filter operating on a luma   //
//               input buffer used for alpha blending while also checking any     //
//               consumption latencies                                            //
//                                                                                //
//  INPUTS:      pSE - Scheduling entity                                          //
//               iBufId - input buffer ID within the scheduling entity            //
//               lineRunNum - the first run iteration of the SE to consider the   //
//                            lines needed for                                    //
//               linesPerIter - the  number of lines to be produced by the SE     //
//                              on the current run under consideration            //
//                                                                                //
//  OUTPUTS:     Results left in pSE->maxLineRequired[iBufId] &                   //
//               pSE->minLineRequired[iBufId].                                    //
//                                                                                //
//  RETURNS:     0 if No lines are required, 1 otherwise                          //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u8 mlpiAskHwMedLumaLatency (SippSchEnt * pSE,
                            u32          iBufId,
                            s32          lineRunNum,
                            u32          linesPerIter)
{
    u32                  in_H;
    SchedIBufUsageInfo * ptrUsage = pSE->iBufCtx[iBufId];

    if ((u32)lineRunNum < ptrUsage->consumptionLatency)
    {
        // In this situation - the SE does not need any data from the input buffer
        // currently in order that it may run the specified iteration
        return 0;
    }
    else
    {
        if (ptrUsage->lastAskLineNo != (u32)lineRunNum)
        {
            ptrUsage->lastAskLineNo        = lineRunNum;
            ptrUsage->iPadLines            = 0;

            in_H                           =  pSE->iBufs[iBufId]->filtOutHeight - 1;
            pSE->maxLineRequired[iBufId]   = ((lineRunNum - ptrUsage->consumptionLatency ) << 0x1 ) + (ptrUsage->numLinesUsed - 0x1) + ((linesPerIter - 0x1) << 0x1);
            pSE->minLineRequired[iBufId]   = ((lineRunNum - ptrUsage->consumptionLatency ) << 0x1 );

            if (pSE->maxLineRequired[iBufId] > in_H)
            {
                ptrUsage->iPadLines          = pSE->maxLineRequired[iBufId] - in_H;
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

void sippMLPIDMACheck (void * pPipe, void * pSE, void * pParam)
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
    // the largest value of numLinesUsed among its consumers
    for (consumerIdx = 0, maxLinesUsed = 0; consumerIdx < pDMASE->numConsumers; consumerIdx++)
    {
        SippSchEnt * pConsSchEnt = pDMASE->cons[consumerIdx];
        u32          consIbufIdx = pDMASE->consIbufIdx[consumerIdx];

        if ((pConsSchEnt->iBufCtx[consIbufIdx]->numLinesUsed + pConsSchEnt->linesPerIter) > maxLinesUsed)
        {
            maxLinesUsed = pConsSchEnt->iBufCtx[consIbufIdx]->numLinesUsed + pConsSchEnt->linesPerIter;
        }
    }

    // If the buffer size exceeds this cap, prevent DMA from running
    if (pDMASE->oBufs[0x0]->mostRecentLine >= (int)maxLinesUsed)
    {
        pDMASE->canRunP = RS_CANNOT;
        pDMASE->canRunC = RS_CANNOT;
    }
}

#endif // SIPP_SCRT_ENABLE_MLPI
