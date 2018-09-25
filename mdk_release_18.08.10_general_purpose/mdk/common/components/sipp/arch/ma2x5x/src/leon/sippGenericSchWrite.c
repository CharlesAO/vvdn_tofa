///
/// @file
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippInternal.h>

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGenericSchedWr                                               //
//                                                                                //
//  DESCRIPTION: Create a schedule entry for the active iteration                 //
//                                                                                //
//  INPUTS:      pl - Pipeline                                                    //
//               iteration - iteration number (wrt frame start)                   //
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

void sippGenericSchedWr (SippPipeline * pl,
                         u32            iteration)
{
    SippFilter *fptr;
    u32 i;
    u32 maskSipp;
    u32 maskShave;
    u32 maskDMA, cntDMA;
    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    u32 IbflIncMask;
    u32 waitMaskSipp;
    #endif

    //////////////////////////////////////////////////////
    // Clear mask
    pl->schedInfo[iteration].allMask = 0;

    for (i = 0; i < pl->nFilters; i++)
    {
        fptr = pl->filters[i];

        if ((fptr->sch->canRunP == RS_CAN_RUN) &&
            (fptr->sch->canRunC == RS_CAN_RUN))
        {
            pl->schedInfo[iteration].allMask |= (1<<i);

            #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
            if (fptr->sch->canConsume == RS_DONT_KNOW)
            {
                fptr->sch->canConsume =  RS_CAN_RUN;
                pl->schedConsumeMask |= (1<<i);
            }
            #endif
        }
    }

    //////////////////////////////////////////////////////
    // Build the SIPP Start mask

    maskSipp  = 0;

    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    IbflIncMask  = 0;
    waitMaskSipp = 0;

    #endif

    #if defined(SIPP_PC) || defined(MYRIAD2)
    for (i = 0; i < pl->nFilters; i++)
    {
        fptr = pl->filters[i];

        // Is this a HW filter
        if (fptr->unit <= SIPP_MAX_FILTER_ID)
        {
            // Is a filter is to RUN ?
            if ((pl->schedInfo[iteration].allMask & (1 << i))
                #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
            	|| (pl->schedConsumeMask & (1 << i))
                #endif
               )
            {
                #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)

                // Produce maskSipp to be used to decide which HW units to start on current iteration
                // Produce waitMaskSipp to be used to decide how to wait on HW completion (we will be waiting
                // on buffer increments)

                // For HW filters which are members of OSEs, fptr->sch points to that of the OSE.
                // An OSE or in fact any filter may need to be called multiple times before it
                // ever produces a line in its output buffer

                // This means the filter needs to be called many times more that fptr->outputH, maxOutputH takes latency into consideration
                if ((u32)fptr->sch->numLineRuns < fptr->sch->maxOutputH)
                {
                    u32 oBufIdx;

                    for (oBufIdx = 0x0; oBufIdx < fptr->numOBufs; oBufIdx++)
                    {
                        // Check if this is to be a real buffer (therefore not an oPipe one)
                        if (fptr->oBufs[oBufIdx]->allocReq)
                        {
                            // Add it to the start mask
                            maskSipp |= (1 << fptr->unit);

                            // Only wait if we are expecting output - NOT if we are starting to consume only
                            if (pl->schedInfo[iteration].allMask & (1 << i))
                            {
                                if (fptr->sch->numLineRuns >= fptr->oBufs[oBufIdx]->latency)
                                {
                                    waitMaskSipp |= fptr->oBufs[oBufIdx]->hwOutputBufId;

                                    // Is this output buffer the input buffer for another HW consumer?
                                    IbflIncMask  |= fptr->oBufs[oBufIdx]->hwInputBufId;
                                }
                            }
                        }
                    }
                }

                #endif
            }
        }
        else
        {
            if (pl->schedInfo[iteration].allMask & (1 << i))
            {
                // For non HW filters, still need to check if they are feeding HW filters
                // through any of their output buffers
                u32 oBufIdx;

                for (oBufIdx = 0x0; oBufIdx < fptr->numOBufs; oBufIdx++)
                {
                    if (fptr->oBufs[oBufIdx]->hwInputBufId)
                    {
                        if ((u32)fptr->sch->numLineRuns < fptr->outputH)
                        {
                            IbflIncMask  |= fptr->oBufs[oBufIdx]->hwInputBufId;
                        }
                    }
                }
            }
        }
    }

    #endif

    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    pl->schedInfo[iteration].sippHwWaitMask  = waitMaskSipp;
    pl->schedInfo[iteration].sippIbflIncMask = IbflIncMask;
    #endif

    //////////////////////////////////////////////////////
    // Shave masks : parse Shave task lists
    maskShave = 0;
    for (i = 0; i < pl->nFiltersSvu; i++)
    {
        fptr = pl->filtersSvu[i];

        if ((     fptr->sch->canRunP == RS_CAN_RUN) &&
            (     fptr->sch->canRunC == RS_CAN_RUN) &&
            ((u32)fptr->sch->numLineRuns < (fptr->outputH + fptr->linesPerIter - 0x1)))
        {
            maskShave |= (1<<i);
        }
    }

    pl->schedInfo[iteration].shaveMask = maskShave;

    //////////////////////////////////////////////////////
    // DMA mask
    maskDMA = 0;
    cntDMA  = 0;
    for (i = 0; i < pl->nFiltersDMA; i++)
    {
        fptr = pl->filtersDMA[i];

        if( (     fptr->sch->canRunP == RS_CAN_RUN) &&
            (     fptr->sch->canRunC == RS_CAN_RUN) &&
            ((u32)fptr->sch->numLineRuns < (fptr->outputH + fptr->linesPerIter - 0x1)))
        {
            maskDMA |= (1<<i);              // run bit
            cntDMA  += fptr->linesPerIter;  // inc skip count
        }
    }

    if (maskDMA)
    {
        // if no DMA, then might get 0-1 = FF, don't want that ...
        pl->schedInfo[iteration].dmaMask = maskDMA | ((cntDMA-1)<<24);
    }
    else
    {
        pl->schedInfo[iteration].dmaMask = 0x00000000;
    }

    pl->nIter = iteration;

}
