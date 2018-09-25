///
/// @file      sippPipelineValidate.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Examine a pipeline for validity & features pertinent to implementation
///            Platform(s) supported : MA2x5x
///

#include <sipp.h>
#include <sippInternal.h>



////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAnalysePipe                                                  //
//                                                                                //
//  DESCRIPTION: Analyses pipeline for patterns germane to the SIPP processing    //
//               framework                                                        //
//                                                                                //
//  INPUTS:      pPipe     - Pipeline for analysis                                //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       This should be a validate and analyse pipe function...           //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippAnalysePipe (pSippPipeline pPipe)
{
    #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
    sippAnalysePipe2x5x (pPipe);
    #endif

}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippValidatePipe                                                 //
//                                                                                //
//  DESCRIPTION: Checks validity of pipe and analyses it for patterns germane to  //
//               the SIPP processing framework                                    //
//                                                                                //
//  INPUTS:      pPipe      - Pipeline for validation and analysis                //
//                                                                                //
//  OUTPUTS:     None.                                                            //
//                                                                                //
//  RETURNS:     None.                                                            //
//                                                                                //
//  NOTES:       This should be a validate and analyse pipe function...           //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////


bool sippValidatePipe (pSippPipeline pPipe)
{
    u32  uIdx;
    bool bPipelineValid = TRUE;

    for (uIdx = 0; uIdx < SIPP_MAX_FILTER_ID; uIdx++)
    {
        // Is this sufficient
        if (pPipe->hwSippFltCnt[uIdx])
        {
            pPipe->uHWFilterUsageBitMask[(uIdx >> 0x5)] |= (0x1 << uIdx);
        }
    }

    if (pPipe->gi.numShaves == 0)
    {
        if (pPipe->nFiltersSvu)
        {
            bPipelineValid = FALSE;
            // set an insufficient sw resource flag
        }
    }

    if (bPipelineValid)
    {
        sippAnalysePipe (pPipe);

        pPipe->uHWFilterIBufUsageMask = pPipe->uHWFilterOBufUsageMask = 0;
        for (uIdx = 0; uIdx <  pPipe->nFilters; uIdx++)
        {
            SippFilter * ptrFilt = pPipe->filters[uIdx];
            u32          buffIdx;

            if (ptrFilt->unit <= SIPP_MAX_FILTER_ID)
            {
                for (buffIdx = 0; buffIdx < ptrFilt->numIBufs; buffIdx++)
                {
                    pPipe->uHWFilterIBufUsageMask |= gSipp.uInfo[ptrFilt->unit].hwGetIbufIds (ptrFilt, buffIdx);
                }
                for (buffIdx = 0; buffIdx < ptrFilt->numOBufs; buffIdx++)
                {
                    pPipe->uHWFilterOBufUsageMask |= gSipp.uInfo[ptrFilt->unit].hwGetObufIds (ptrFilt, buffIdx);
                }
            }
        }
    }

    return bPipelineValid;
}
