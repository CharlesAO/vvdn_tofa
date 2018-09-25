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
// Global function prototypes
////////////////////////////////////////////////////

#ifdef MYRIAD2
#ifndef SIPP_USE_C_PTR_GET_FUNCS
extern int SVU_SYM(__padGeneral)(void);
extern int SVU_SYM(__padOpt__Pl_1__Pad_8__Bpp_1)(void);
extern int SVU_SYM(__padOpt__Pl_1__Pad_8__Bpp_2)(void);
extern int SVU_SYM(__padOpt__Pl_1__Pad_16__Bpp_1)(void);
extern int SVU_SYM(__padOpt__Pl_1__Pad_16__Bpp_2)(void);
extern int SVU_SYM(__padOpt__Pl_3__Pad_8__Bpp_1)(void);
#endif
#endif

extern u32 gSippSliceSz;

////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippUsingPrecompSched                                            //
//                                                                                //
//  DESCRIPTION: Decide if a pre-compiled schedule is being used                  //
//                                                                                //
//  INPUTS:      pl - pipeline                                                    //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       If total number of lines is != 0, it means we're using a         //
//               precomputed schedule that inits all line buffers to proper       //
//               values BEFORE sippProcessFrame gets invoked                      //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

int sippUsingPrecompSched (SippPipeline *pl)
{
    u32 i, o, totLines = 0;

    // Count total number of lines
    totLines = 0;
    for (i = 0; i < pl->nFilters; i++)
    {
        for (o = 0; o < pl->filters[i]->numOBufs; o++)
        {
            totLines += pl->filters[i]->nLines[o];
        }
    }

    return totLines;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    adjustNodesRecursively                                           //
//                                                                                //
//  DESCRIPTION: Adjust the unassigned slice widths of parent / consumer filters  //
//               of the input filter based on the type of the input filter        //
//                                                                                //
//  INPUTS:      fptr - Filter of interest                                        //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void adjustNodesRecursively (SippFilter * fptr)
{
    u32 i;

    ///////////////////////////////////////////////////////////////////////////
    // 1) If filter is SW, then its parents must have required chunk !
    //    But don't need to enforce rules on parents if current filter is HW !
    if (fptr->unit == SIPP_SVU_ID)
    {
        for (i = 0; i < fptr->nParents; i++)
        {
            if (fptr->parents[i]->sliceWidth == 0) // Not yet computed
            {
                SippFilter *par = fptr->parents[i];
                par->sliceWidth = fptr->sliceWidth * par->outputW / fptr->outputW;

                // Check it's 8 Byte multiple
                if (par->sliceWidth & SIPP_LINE_BUFFER_ALIGNMENT)
                {
                    sippError (par->errorStatus, E_INVLD_SLICE_WIDTH);
                }

                // For SW, will enforce constraints on both parents and consumers
                // For HW, will only enforce constraints recursively on consumers
                adjustNodesRecursively (par);
            }
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    // 2) Adjust all SW consumers; we can't enforce HW restrictions at this point !
    //    as HW outputs can be different than input chunks.
    for (i = 0; i < fptr->nCons; i++)
    {
        if (fptr->cons[i]->sliceWidth == 0) //not yet computed
        {
            SippFilter *cons = fptr->cons[i];
            if (cons->unit == SIPP_SVU_ID)
            {
                cons->sliceWidth = fptr->sliceWidth * cons->outputW / fptr->outputW;

                if (cons->sliceWidth & SIPP_LINE_BUFFER_ALIGNMENT) //check it's 8 Byte multiple
                {
                    sippError(cons->errorStatus, E_INVLD_SLICE_WIDTH);
                }

                adjustNodesRecursively (cons);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippFiltersResetSliceWidths                                      //
//                                                                                //
//  DESCRIPTION: Rset all slice (chunk) widths for a pipe                         //
//                                                                                //
//  INPUTS:      pl - Pipeline of interest                                        //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippFiltersResetSliceWidths (SippPipeline *pl)
{
    SippFilter * fptr;
    u32          i;

    for (i = 0; i < pl->nFilters; i++)
    {
        fptr             = pl->filters[i];
        fptr->sliceWidth = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippComputeChunkWidthsSW                                         //
//                                                                                //
//  DESCRIPTION: Compute chunk widths for SW filters                              //
//                                                                                //
//  INPUTS:      pl - Pipeline of interest                                        //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       Algorithm ascertains the minimum width and sets initial chunk    //
//               size from that, scaling until all SW chunk widths are set        //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippComputeChunkWidthsSW (SippPipeline *pl)
{
    u32 nSlices = pl->gi.sliceLast - pl->gi.sliceFirst + 1;
    SippFilter *fptr;
    u32 foundMin, i, j;
    u32 minFullWidth;

    do
    {
        //  1) Find min_output_slice_width of a SW filter,
        //     or HW/SW filter that feeds a SW consummer
        //     (this filter can be SW or HW !)
        minFullWidth =      65536; //not found
        foundMin     = 0xFFFFFFFF; //not found

        for (i = 0; i < pl->nFilters; i++)
        {
            fptr = pl->filters[i];

            if ((fptr->sliceWidth ==           0) &&
                (fptr->outputW    < minFullWidth))
            {
                if (fptr->unit == SIPP_SVU_ID)
                {
                     foundMin     = i;
                     minFullWidth = fptr->outputW;
                }
                else
                {
                    // look for SW consumers:
                    for (j = 0; j < fptr->nCons; j++)
                    {
                        if(fptr->cons[j]->unit == SIPP_SVU_ID)
                        {
                            //If has SW consumers, then this is current MIN
                            foundMin     = i;
                            minFullWidth = fptr->outputW;
                        }
                    }
                }
            }
        }

        // 2) Compute slice width for that filter
        if (foundMin != 0xFFFFFFFF)
        {
            fptr = pl->filters[foundMin];
            // Compute slice width based on output width and number of slices
            fptr->sliceWidth = (fptr->outputW + nSlices - 1) / nSlices;
            fptr->sliceWidth = (fptr->sliceWidth + 7) & (~7);

            // Need to run a last slice check
            if ( nSlices > 1 )
            {
                if ((fptr->sliceWidth * (nSlices - 1)) >= fptr->outputW)
                {
                    // Reset the slice width of all filters to 0
                    sippFiltersResetSliceWidths (pl);
                    return 0x1;
                }
            }

            // 3) Adjust graph accordingly !
            adjustNodesRecursively (fptr);
        }

    // Find filter with minimal line width that doesn't have chunk width yet computed
    } while (foundMin != 0xFFFFFFFF); // while still found something not-allocated

    // Final sanity check on slice widths (see mdk/testApps/components/sipptests/testInterpolatePixelBilinear
    // for motivation)
    for (i = 0; i < pl->nFilters; i++)
    {
        fptr = pl->filters[i];

        // No need for it to be larger than the output width
        if (fptr->sliceWidth > fptr->outputW)
        {
            fptr->sliceWidth = (fptr->outputW + SIPP_LINE_BUFFER_ALIGNMENT) & (~SIPP_LINE_BUFFER_ALIGNMENT);
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippComputeChunkWidths                                           //
//                                                                                //
//  DESCRIPTION: Compute chunk widths for all filters                             //
//                                                                                //
//  INPUTS:      pl - Pipeline of interest                                        //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippComputeChunkWidths (SippPipeline *pl)
{
    u32          i;
    u32          nSlices;
    SippFilter * fptr;
    SippFilter * reTryList[SIPP_MAX_FILTERS_PER_PIPELINE];
    u32          reTryTotal = 0, reTryIdx;

    while (sippComputeChunkWidthsSW (pl))
    {
        pl->gi.sliceLast--;
        if (pl->gi.numShaves)
        {
            pl->gi.numShaves--;
        }
    }

    nSlices = pl->gi.sliceLast - pl->gi.sliceFirst + 1;

    for (i = 0; i < pl->nFilters; i++)
    {
        fptr = pl->filters[i];

        if (fptr->sliceWidth == 0)
        {
            // For DMA out, inherit parent slice-width
            if ((fptr->unit == SIPP_DMA_ID) && (fptr->nCons == 0))
            {
                if (fptr->parents[0]->sliceWidth == 0)
                {
                    reTryList[reTryTotal++] = fptr;
                }
                else
                {
                    fptr->sliceWidth = fptr->parents[0]->sliceWidth;
                }
            }
            else
            {
                fptr->sliceWidth = (fptr->outputW + nSlices - 1) / nSlices;
                fptr->sliceWidth = (fptr->sliceWidth + 7) & (~7);
            }
        }
    }

    // Some DMA out filters may not have a slice width assigned yet, as when the loop above was executed
    // their parent filter may have not had its slice width assigned when the DMA was processed if the
    // parent filter was created after the DMA Out filter in the app
    reTryIdx = reTryTotal;
    while (reTryTotal)
    {
        for (i = 0; i < reTryIdx; i++)
        {
            fptr = reTryList[i];

            if (fptr->sliceWidth == 0)
            {
                if (fptr->parents[0]->sliceWidth)
                {
                    fptr->sliceWidth = fptr->parents[0]->sliceWidth;
                    reTryTotal--;
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippComputeBufferProps                                           //
//                                                                                //
//  DESCRIPTION: Compute circular buffers properties                              //
//                                                                                //
//  INPUTS:      pl - Pipeline of interest                                        //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippComputeBufferProps (SippPipeline *pl)
{
    u32      i, j;
    SippFilter *fptr;

    // Compute filters slice widths while MAINTAINING horizontal aspect ratios!
    if (pl->filters[0]->sliceWidth == 0)
    {
        sippComputeChunkWidths(pl);
    }

    for (i = 0; i < pl->nFilters; i++)
    {
        u32 nSlices = pl->gi.sliceLast - pl->gi.sliceFirst + 1;
        fptr        = pl->filters[i];

        // Slice width was computed previously, now just compute last shave
        // width (used further in padding ....)
        fptr->sliceWidthLastSvu = fptr->outputW - (fptr->sliceWidth * (nSlices-1));

        // Compute plane stride (if not already assigned by user)
        for (j = 0; j < fptr->numOBufs; j++)
        {
            // Compute the slice line stride (consider LEFT + RIGHT horizontal padding)
            fptr->lineStride[j]  = (fptr->sliceWidth + fptr->hPadding[j] * 2);
            fptr->planeStride[j] = fptr->lineStride[j] * fptr->nLines[j];

            if(fptr->flags & SIPP_SHIFT_PLANES)
            {
                // Div by bpp, as will be multiplied by bpp later
                // Need to clean this up...
                // fptr->planeStride += (pl->gi.sliceSize >> fptr->bpp);
                // CHECK ME
                // Plane stride derived thus far is in terms of pixels so convert
                // slice size to the same
                fptr->planeStride[j] += ((pl->gi.sliceSize << 3) / fptr->bpp[j]);
            }
        }

        if (fptr->flags & SIPP_PROVIDE_CHUNK_POS)
        {
            UInt32 shaveNum, chunkNum;

            // Provide an area to report the chunk start locations
            fptr->ptrChunkPos = (SippPixelChunkPos *)sippMemAlloc (&pl->tHeapMCB,
                                                                   vPoolPipeStructs,
                                                                   sizeof(SippPixelChunkPos));

            if (fptr->ptrChunkPos == (SippPixelChunkPos *)NULL)
            {
                sippError (fptr->errorStatus, E_OUT_OF_MEM);
                fptr->flags &= ~SIPP_PROVIDE_CHUNK_POS;
            }
            else
            {

                for (shaveNum = pl->gi.sliceFirst, chunkNum = 0; shaveNum <= pl->gi.sliceLast; shaveNum++, chunkNum++)
                {
                    chunkNum                          = (shaveNum < fptr->firstOutSlc) ? (pl->gi.sliceLast - fptr->firstOutSlc + 1 + shaveNum - pl->gi.sliceFirst ) :
                                                                                         shaveNum - fptr->firstOutSlc;
                    fptr->ptrChunkPos->XPos[shaveNum] = chunkNum * fptr->sliceWidth;
                }
            }
        }
        else
        {
            fptr->ptrChunkPos = (SippPixelChunkPos *)NULL;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippInitBufferLnPointers                                         //
//                                                                                //
//  DESCRIPTION: Initialise the 5 sets of line pointers                           //
//                                                                                //
//  INPUTS:      pl - Pipeline of interest                                        //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       We need these lists for top/bottom replication, and buffer       //
//               rolling                                                          //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippInitBufferLnPointers (SippPipeline * pl)
{
    u32      i, j, o, totalBits;
    SippFilter *fptr;
    u8      **ppp;

    for (i = 0; i < pl->nFilters; i++)
    {
        fptr = pl->filters[i];

        for (o = 0; o < fptr->numOBufs; o++)
        {
            if (fptr->nLines[o] > 0)
            {
                #if defined(__sparc)
                if (fptr->flags & SIPP_REQ_SW_VIEW)
                #endif
                {
                    ppp = fptr->linePtrs1stBase[o];

                    ////////////////////////////////////////////////////////
                    // Top replication, i0
                    for (j = 0; j < fptr->nLines[o] - fptr->linesPerIter; j++)
                    {
                        totalBits  = (0/*idx*/ * fptr->lineStride[o] + fptr->hPadding[o]) * fptr->bpp[o];
                        totalBits += 0x7;
                        totalBits &= 0xFFFFFFF8;
                        ppp[j]     = fptr->outputBuffer[o] + (totalBits >> 3);
                    }
                    // i1, i2:
                    for (j = 0; j < fptr->nLines[o]; j++)
                    {
                        totalBits  = ((j * fptr->lineStride[o] + fptr->hPadding[o]) * fptr->bpp[o]);
                        totalBits += 0x7;
                        totalBits &= 0xFFFFFFF8;
                        ppp[j + (fptr->nLines[o] - fptr->linesPerIter)]                   = fptr->outputBuffer[o] + (totalBits >> 3);
                        ppp[j + (fptr->nLines[o] - fptr->linesPerIter) + fptr->nLines[o]] = ppp[j + (fptr->nLines[o] - fptr->linesPerIter)];
                    }

                    // i3:
                    for (j = 0; j < fptr->linesPerIter; j++)
                    {
                        ppp[3 * fptr->nLines[o] - fptr->linesPerIter + j] = ppp[2 * fptr->nLines[o] - fptr->linesPerIter + j];
                    }

                    fptr->linePtrs[o]        = (u8 *)&ppp[fptr->nLines[o] * 0];
                    fptr->linePtrs2ndBase[o] = (u8 *)&ppp[fptr->nLines[o] * 1];
                    fptr->linePtrs3rdBase[o] = (u8 *)&ppp[fptr->nLines[o] * 2];
                    fptr->linePtrs4thBase[o] = (u8 *)&ppp[fptr->nLines[o] * 3];
                    fptr->linePtrs5thBase[o] = (u8 *)&ppp[fptr->nLines[o] * 4];

                    ////////////////////////////////////////////////////////
                    // Bottom replication
                    {
                        u32 lastLn = (fptr->outputH - 1) % fptr->nLines[o];

                        // Where is the chunk of "linesPerIter" lines containing last line
                        // relative to fptr->linePtrs2ndBase[o]

                        // The first bottom replication area needs to represent the state of the buffer after
                        // the first padding lines are added (as we will switch to this as a view of the buffer
                        // in order to carry out the first padding section). So there are two sub-cases here.
                        // The first is that the last line occurs in the last entry of a numLinesIter chunk.
                        // In that case this section will have an entire 'padded' numLinesIters chunk at its tail.
                        // If the last line is somewhere else within the chunk then that chunk will itself
                        // become the last chunk of this section with all entries after the last line modified
                        // to be padded lines
                        u32 secondBaseLine  = fptr->linesPerIter - fptr->outLineDeficit;
                        u32 numChunkOffsets = 0;
                        u32 numChunks       = fptr->nLines[o] / fptr->linesPerIter;
                        u32 chunkSize       = fptr->linesPerIter * sizeof(u8 *);
                        u32 padLimit;

                        // Which chunk is the last line in, relative to that @ fptr->linePtrs2ndBase[o] - i.e. chunk 1
                        while (!((lastLn >= secondBaseLine) &&
                                 (lastLn < (secondBaseLine + fptr->linesPerIter))
                                )
                              )
                        {
                            secondBaseLine += fptr->linesPerIter;
//                            /if (secondBaseLine == fptr->nLines[o]) secondBaseLine = 0;   // surely should be if (secondBaseLine >= fptr->nLines[o]) secondBaseLine -= fptr->nLines[o];
                            if (secondBaseLine >= fptr->nLines[o]) secondBaseLine -= fptr->nLines[o];
                            numChunkOffsets++;
                        }

                        // Move forward one chunk - so that the chunk we have just found the last line in is now the last
                        // of the numChunks in the list
                        numChunkOffsets++;

                        if  (((lastLn - fptr->outLineDeficit) % fptr->linesPerIter) == (fptr->linesPerIter - 0x1))
                        {
                            // We want to pad one full chunk on the end - so move one further chunk on...
                            numChunkOffsets++;
                            // And set the pad point to the last entry of the penultimate chunk
                            padLimit     = fptr->nLines[o] - fptr->linesPerIter - 0x1;
                        }
                        else
                        {
//                            padLimit     = fptr->nLines[o] - fptr->linesPerIter + ((lastLn - fptr->outLineDeficit) % fptr->linesPerIter);
                            padLimit     = fptr->nLines[o] - fptr->linesPerIter + ((lastLn - (fptr->linesPerIter - fptr->outLineDeficit)) % fptr->linesPerIter);
                        }

                        numChunkOffsets = numChunkOffsets % numChunks;

                        // Can we establish then where fptr->linePtrs[o] will be when we get around to covering the last
                        // output - we need to copy nLines from there to bottom replication section and when we hit the last line and after
                        // we make sure they are all padded

                        u8 ** ptrLastIterWindow = (u8 **)((u32)fptr->linePtrs2ndBase[o] + (numChunkOffsets * chunkSize) - (fptr->outLineDeficit * sizeof(u8 *)));
                        for (j = 0; j < fptr->nLines[o]; j++)
                        {
                            if (j >= padLimit)
                            {
                                ppp[3 * fptr->nLines[o] + j] = ptrLastIterWindow[padLimit];
                            }
                            else
                            {
                                ppp[3 * fptr->nLines[o] + j] = ptrLastIterWindow[j];
                            }
                        }

                        // Populate Ptr5th base : all as [Last]
                        for (j = 0; j < fptr->nLines[o]; j++)
                        {
                            ppp[4 * fptr->nLines[o] + j] = ppp[3 * fptr->nLines[o] + padLimit];
                        }
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippIncrementOutBuffs                                            //
//                                                                                //
//  DESCRIPTION: For filters that have an associated output buffer in CMX,        //
//               update the buffer sizes to accommodate the ouput lines           //
//                                                                                //
//  INPUTS:      pPipe - Pipeline of interest                                     //
//                                                                                //
//  OUTPUTS:     Results left in                                                  //
//               pPipe->filters[Filter Idx]->nLines[Out Buffer Idx]               //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippIncrementOutBuffs (SippPipeline * pPipe)
{
    u32 i, o;

    for (i = 0; i < pPipe->nFilters; i++)
    {
        #ifdef SIPP_PC
        u32 j;
        if (pPipe->dbgLevel > 1)
        {
            for (o = 0; o < pPipe->filters[i]->numOBufs; o++)
            {
                printf("nLines[%2d] = %3d \n", i, pPipe->filters[i]->nLines[o]);
                for (j = 0; j < pPipe->filters[i]->nParents; j++)
                {
                    printf("    KS[%2d] = %d \n", j, pPipe->filters[i]->parentsKS[j]);
                }
            }
        }
        #endif

        for (o = 0; o < pPipe->filters[i]->numOBufs; o++)
        {
            // Add the output space for the filter
            pPipe->filters[i]->nLines[o] = (pPipe->filters[i]->nLines[o] == 0) ? 0 : (pPipe->filters[i]->nLines[o] + pPipe->filters[i]->linesPerIter);
        }

        #ifdef SIPP_PC
        if (pPipe->dbgLevel > 0)
        {
            for (o=0; o < pPipe->filters[i]->numOBufs; o++)
            {
                printf("Filt %i : Adjusted nLines[%2d] = %3d \n", i, o, pPipe->filters[i]->nLines[o]);
            }
        }
        #endif
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippBuildLnBuffs                                                 //
//                                                                                //
//  DESCRIPTION: Create and allocate the line buffers necessary in a pipeline     //
//                                                                                //
//  INPUTS:      pl - pipeline                                                    //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       This function is not used when oPipe runtime is selected         //
//               Main tasks are:                                                  //
//               update number of lines in filter's output buffer                 //
//               compute filter hPadding values                                   //
//               compute filter chunk-width/strides                               //
//               alloc the filter line buffers and init line pointers             //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippBuildLnBuffs (SippPipeline *pl)
{
    u32         i, o, totalBits;
    SippFilter *fptr;

    // Update the line numbers to accommodate 1 extra line (the current ouput)
    sippIncrementOutBuffs (pl);

    // Round hPadding to 8 multiple
    for (i = 0; i < pl->nFilters; i++)
    {
        fptr = pl->filters[i];

        for (o = 0; o < fptr->numOBufs; o++)
        {
            if ((fptr->hPadding[o]) &&
                (fptr->nLines[o]))
            {
                // THIS IS ONLY FOR SW FILTERS THAT DON'T USE PACKED10 AND PACKED12 FORMATS!
                // IF EVER WANTED TO USE THESE FORMATS, GET BACK TO THIS LINE!
                fptr->hPadding[o] =  ((63 + (fptr->hPadding[o] * fptr->bpp[o])) & ~63) / fptr->bpp[o];
                // At least 1 buffer in pipeline requires SW padding, thus flag this in the
                // pipeline data-struct; as shaves will need to run early to do this...
                pl->flags |= PLF_REQUIRES_SW_PADDING;

                // Add current buffer to the list of buffers to be padded by the SHAVEs
                // The same filter may appear multiple times without breaking anything
                // so long as there is a distinct hpadinfo??
                pl->padList[pl->nPadFilters].fptr   = fptr;
                pl->padList[pl->nPadFilters].oBufId = o;

                // Important :associate optimized padding functions for M2 builds"
                #ifdef MYRIAD2
                #ifndef SIPP_USE_C_PTR_GET_FUNCS
                {
                    pl->padList[pl->nPadFilters].svuPadFunc = (u32)SVU_SYM(__padGeneral);//default

                    // Associate an optimized padding function if possible:
                    if(fptr->nPlanes[o] == 1)
                    {
                        if     ((fptr->hPadding[o]== 8) && (fptr->bpp[o]==8))  pl->padList[pl->nPadFilters].svuPadFunc = (u32)SVU_SYM(__padOpt__Pl_1__Pad_8__Bpp_1);
                        else if((fptr->hPadding[o]== 4) && (fptr->bpp[o]==16)) pl->padList[pl->nPadFilters].svuPadFunc = (u32)SVU_SYM(__padOpt__Pl_1__Pad_8__Bpp_2);
                        else if((fptr->hPadding[o]==16) && (fptr->bpp[o]==8))  pl->padList[pl->nPadFilters].svuPadFunc = (u32)SVU_SYM(__padOpt__Pl_1__Pad_16__Bpp_1);
                        else if((fptr->hPadding[o]== 8) && (fptr->bpp[o]==16)) pl->padList[pl->nPadFilters].svuPadFunc = (u32)SVU_SYM(__padOpt__Pl_1__Pad_16__Bpp_2);
                    }
                    else if(fptr->nPlanes[o] == 3)
                    {
                        if ((fptr->hPadding[o]== 8) && (fptr->bpp[o]==8)) pl->padList[pl->nPadFilters].svuPadFunc = (u32)SVU_SYM(__padOpt__Pl_3__Pad_8__Bpp_1);
                    }
                }

                // Padding = 16, nPlanes = 1, optimize for 1,2,4 BPP

                #endif // !SIPP_USE_C_PTR_GET_FUNCS
                #endif // MYRIAD2

                // Increment the number of entries in the padding list
                // technically  this is the number of buffers to be padded now
                // as the same filter may appear at a number of locations in the list
                // - one for each of its potential output buffers
                pl->nPadFilters++;
            }
        }
    }

    // Compute buffers properties: chunk width, strides...
    sippComputeBufferProps (pl);

    // Alloc line storage for suitable filters within additionally supplied
    // i.e. outside of traditional mempool_lnbuff
    if (pl->useCmxRegMap)
    {
        pl->cmxMapResult = sippAllocCmxLineBuffers (pl);
    }

    // Alloc line-storage & associated pointers (5 sets) to all filters that have circular output buffers
    for (i = 0; i < pl->nFilters; i++)
    {
        fptr = pl->filters[i];

        for (o = 0; o < pl->filters[i]->numOBufs; o++)
        {
            if (fptr->nLines[o] > 0) // If it has an output buffer:
            {
                if (fptr->oBufAlloc[o] == 0) // Wasn't already allocated in an alternative CMX region
                {
                    /////////////////////////////////////////////
                    // Alloc the lines in SLICE pool
                    /////////////////////////////////////////////
                    // The pointer needs to be a SHAVE-RELATIVE ptr
                    u32 planeSize = fptr->lineStride[o] * fptr->nLines[o];

                    // Allocate lines for all planes; even if the data of next plane belongs to next chunk!
                    totalBits  = (fptr->nPlanes[o] * planeSize) * fptr->bpp[o];
                    totalBits += 0x7;
                    totalBits &= 0xFFFFFFF8;

                    #ifdef SIPP_PC
                    if (pl->dbgLevel > 0)
                    {
                        sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "sippBuildLnBuffs - Filter &u Buffer %u Size %u\n",
                                                            i,
                                                            o,
                                                            (totalBits >> 3));
                    }
                    #endif

                    fptr->outputBuffer[o] = (u8 *)sippMemAlloc (&pl->tHeapMCB,
                                                                vPoolFilterLineBuf,
                                                                totalBits >> 3);

                    if (fptr->outputBuffer[o] == (u8 *)NULL)
                    {
                        sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "sippBuildLnBuffs : Alloc failed for filter %d, buffer %d\n", i, o);
                        sippError (fptr->errorStatus, E_OUT_OF_MEM);
                        return 0x1;
                    }

                    fptr->outLineOffset[o] = (fptr->nLines[o] - (1 + fptr->outLineDeficit));

                    #ifndef SIPP_SLICE0_ADDRESS
                    fptr->outputBuffer[o] += (pl->gi.sliceFirst * pl->gi.sliceSize);
                    #endif
                    #ifdef RT_DBG
                    sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "sippMemAlloc : fptr[%d]->outputBuffer[%d] = %x (Lines = %d)\n", i, o, fptr->outputBuffer[o], fptr->nLines[o]);
                    #endif
               }

                #if defined(SIPP_PC)
                // Safety clamp to valid range
                if (fptr->firstOutSlc < pl->gi.sliceFirst)
                {
                    sippError (fptr->errorStatus , E_INVLD_FILT_FIRST_SLICE);
                }

                if (fptr->firstOutSlc > pl->gi.sliceLast)
                {
                    sippError (fptr->errorStatus, E_INVLD_FILT_LAST_SLICE);
                }
                #endif

                //On Myriad, just allocate & compute buffer linear view for filters that need one
                #if defined(__sparc)
                if(fptr->flags & SIPP_REQ_SW_VIEW)
                #endif
                {
                    /////////////////////////////////////////////
                    // Alloc the lines-pointers
                    /////////////////////////////////////////////
                    // We allocate here 5 sets of pointers:
                    //  - 3 sets to accomodate TopReplication and typical circular operation
                    //  - 2 sets to implement bottom replication
                    fptr->linePtrs1stBase[o] = (u8**)sippMemAlloc (&pl->tHeapMCB,
                                                                   vPoolSchedule,
                                                                   fptr->nLines[o] * 5 * sizeof(u8*));

                    if (fptr->linePtrs1stBase[o] == (u8 **)NULL)
                    {
                        sippError (fptr->errorStatus, E_OUT_OF_MEM);
                        return 0x1;
                    }
                }
            }
        }
    }

    // Init the line pointers previously allocated !
    sippInitBufferLnPointers (pl);

    // DEBUG: At the end of all allocs, display the status
    sippMemStatus ();

    return 0x0;
}

#ifdef SIPP_SCRT_ENABLE_OPIPE

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippOPipeSetBuffLineStrides                                      //
//                                                                                //
//  DESCRIPTION: Set line strides for all buffers (oPipe RT only)                 //
//                                                                                //
//  INPUTS:      pl - pipeline                                                    //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippOPipeSetBuffLineStrides (pSippPipeline pPipe)
{
    u32          i, o;
    SippFilter * fptr;

    for (i = 0; i < pPipe->nFilters; i++)
    {
        fptr = pPipe->filters[i];

        for (o = 0; o < fptr->numOBufs; o++)
        {
            // Compute the line stride
            fptr->lineStride[o]  = (fptr->outputW + SIPP_LINE_BUFFER_ALIGNMENT) & (~SIPP_LINE_BUFFER_ALIGNMENT);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippCheckSetPSLnBuffsOPipe                                       //
//                                                                                //
//  DESCRIPTION: Allows for plane stride setting when the client sets buffer      //
//               pointers outside framework (eg at the app)                       //
//               Also runs an initial check needed by the opipe runtime to        //
//
//                                                                                //
//  INPUTS:      pl - pipeline                                                    //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       Eventually run some compliance checks here                       //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippSetPSLnBuffsOPipe (SippPipeline * pl)
{
    u32                i, o;
    SippFilter *       fptr;

    // Any filter buffer with a dma client cannot have a
    // number of lines be greater than SIPP_NUM_DESCS_PER_CDMA * (1 << irqRate)
    // this rule needs to be applied to conventional allocation also
    for (i = 0; i < pl->nFilters; i++)
    {
        fptr = pl->filters[i];

        for (o = 0; o < fptr->numOBufs; o++)
        {
            if (fptr->nLines[o] > 0) // If it has an output buffer:
            {
                if (fptr->oBufAlloc[o] == 0)   // Wasn't already allocated in an alternative CMX region
                {
                    // Compute the plane stride - line stride already calculated by sippOPipeSetBuffLineStrides
                    fptr->planeStride[o] = fptr->lineStride[o] * fptr->nLines[o];
                }
            }
        }
    }

    return 0x0;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippBuildLnBuffsOPipe                                            //
//                                                                                //
//  DESCRIPTION: Alloc the filter line buffers for oPipe SchRT                    //
//                                                                                //
//  INPUTS:      pl - pipeline                                                    //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippBuildLnBuffsOPipe (SippPipeline * pl)
{
    u32                 i, o, idx, totalBits;
    SippFilter *        fptr;
    u32                 sliceIdx  = 0;
    u32                 numSlices = pl->gi.sliceLast - pl->gi.sliceFirst + 1;
    u32                 filtOBufsSize[SIPP_MAX_FILTERS_PER_PIPELINE];
    u32                 numOBufsAlloc = 0;

    sippOpipeBufInfo    oBufInfo[CMX_NSLICES];
    sippOpipeBufInfo *  sortedOBufInfo[CMX_NSLICES];

    // Alloc line storage for suitable filters within additionally supplied memory
    if (pl->useCmxRegMap)
    {
        sippSetPSLnBuffsOPipe (pl);
        pl->cmxMapResult = sippAllocCmxLineBuffersOPipe (pl);
    }

    for (i = 0; i < pl->nFilters; i++)
    {
        fptr = pl->filters[i];

        if (fptr->flags & SIPP_SHIFT_PLANES)
        {
            for (o = 0; o < fptr->numOBufs; o++)
            {
                if ((fptr->nLines[o]) &&
                    (fptr->oBufAlloc[o] == 0))
                {
                    if (fptr->nPlanes[o] > numSlices)
                    {
                        fptr->flags &= ~SIPP_SHIFT_PLANES;
                        break;
                    }
                }
            }
        }

        for (o = 0; o < fptr->numOBufs; o++)
        {
            if (fptr->nLines[o] > 0) // If it has an output buffer:
            {
                if (fptr->oBufAlloc[o] == 0)   // Wasn't already allocated in an alternative CMX region
                {
                    if (fptr->flags & SIPP_SHIFT_PLANES)
                    {
                        // Subsequent planes start in next slice
                        fptr->planeStride[o] = pl->gi.sliceSize;
                    }
                    else
                    {
                        // Compute the plane stride - line stride already calculated by sippOPipeSetBuffLineStrides
                        fptr->planeStride[o] = fptr->lineStride[o] * fptr->nLines[o];
                    }

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

    // Sort the list of filters that have output buffers based on the memory size
    sippListSort ((s32 *)sortedOBufInfo,
                  (s32 *)filtOBufsSize,
                  numOBufsAlloc,
                  true);

    for (i = 0; i < numOBufsAlloc; i++)
    {
        fptr = sortedOBufInfo[i]->ptrFilt;
        o    = sortedOBufInfo[i]->oBufIdx;

        #ifdef SIPP_PC
        if (pl->dbgLevel > 0)
        {
            sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "sippBuildLnBuffsOPipe - Filter %u Buffer %u Size %u\n",
                                                  fptr->unit,
                                                  o,
                                                  filtOBufsSize[i]);
        }
        #endif

        if (fptr->flags & SIPP_SHIFT_PLANES)
        {
            u32  numPlanes  = fptr->nPlanes[o];
            u32  planeSize  = fptr->lineStride[o] * fptr->nLines[o];
            u32  startSlice = sliceIdx;
            u32  sliceCount = 0;
            u32  firstSlice = 0;
            u32  freeSpace  = 0;
            bool bSuccess   = FALSE;

            // For this what we need is space for one plane in all of a number of consecutive
            // buffers (consecutive can include a wrap in terms of the CMX slice)

            while (sliceCount < numPlanes)
            {
                freeSpace = sippMemCheck (&pl->tHeapMCB,
                                          (SippVirtualPool)(vPoolFilterLineBuf0 + sliceIdx));

                if (freeSpace > planeSize)
                {
                    if (sliceCount == 0x0)
                    {
                        firstSlice = sliceIdx;
                    }
                    sliceCount++;
                }
                else
                {
                    sliceCount = 0;
                }

                if (sliceCount == numPlanes)
                {
                    bSuccess = TRUE;
                    break;
                }

                // Proceed to next slice
                sliceIdx++;
                if (sliceIdx == numSlices)
                {
                    sliceIdx = 0;
                }

                // Check if we have now checked all slices
                if (sliceIdx == startSlice)
                {
                    break;
                }
            }

            if (bSuccess)
            {
                sliceIdx = firstSlice;

                fptr->outputBuffer[o] = (u8 *)sippMemAlloc (&pl->tHeapMCB,
                                                            (SippVirtualPool)(vPoolFilterLineBuf0 + sliceIdx),
                                                            planeSize);

                for (sliceCount = 1; sliceCount < numSlices; sliceCount++)
                {
                    sliceIdx++;
                    if (sliceIdx == numSlices)
                    {
                        sliceIdx = 0;
                    }

                    sippMemAlloc (&pl->tHeapMCB,
                                  (SippVirtualPool)(vPoolFilterLineBuf0 + sliceIdx),
                                  planeSize);
                }
            }
            else
            {
                sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "sippBuildLnBuffsOPipe : Alloc failed for filter %d, buffer %d\n", fptr->unit, o);
                sippError (fptr->errorStatus, E_OUT_OF_MEM);
                return 0x1;
            }
        }
        else
        {
            if (filtOBufsSize[i] > gSippSliceSz)
            {
                // The buffer is greater than 1 slice, nothing to do here but
                // leak into the next slice
                // We can only do that though if the next slice is as yet unused!!!
                // What we need is to find the maximum contiguous chunk of unused memory
                // irrespective of slice boundary
                SippVirtualPool vPoolStart = sippMemFindMaxLnMemPoolFree (&pl->tHeapMCB);
                u32             allocReq;

                // Allocate this line buffer, starting from the pool containing
                // the start of the longest contiguously free memory region
                // Calculate how much to allocate to each pool

                // 1st pool
                allocReq = sippMemCheck (&pl->tHeapMCB,
                                         vPoolStart);

                fptr->outputBuffer[o] = (u8 *)sippMemAlloc (&pl->tHeapMCB,
                                                            vPoolStart,
                                                            allocReq);

                if (fptr->outputBuffer[o] == (u8 *)NULL)
                {
                    sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "sippBuildLnBuffsOPipe : Alloc failed for filter %d, buffer %d\n", fptr->unit, o);
                    sippError (fptr->errorStatus, E_OUT_OF_MEM);
                    return 0x1;
                }

                #ifdef SIPP_PC
                extern u8* sippCmxBase;
                printf ("%u, %u, %08x\n", fptr->id, o, (u32)fptr->outputBuffer[o] - (u32)sippCmxBase);
                #endif

                filtOBufsSize[i] -= allocReq;
                sliceIdx          = (vPoolStart - vPoolFilterLineBuf0) + 0x1;

                if (sliceIdx == numSlices)
                {
                    sliceIdx = 0x0;
                }

                while (filtOBufsSize[i])
                {
                    if (filtOBufsSize[i] > gSippSliceSz)
                    {
                        allocReq = gSippSliceSz;
                    }
                    else
                    {
                        allocReq = filtOBufsSize[i];
                    }

                    if (sippMemAlloc (&pl->tHeapMCB,
                                      (SippVirtualPool)(vPoolFilterLineBuf0 + sliceIdx),
                                       allocReq) == (void *)0x0)
                    {
                        sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "sippBuildLnBuffsOPipe : Alloc failed for filter %d, buffer %d\n", fptr->unit, o);
                        sippError (fptr->errorStatus, E_OUT_OF_MEM);
                        return 0x1;
                    }
                    else
                    {
                        filtOBufsSize[i] -= allocReq;
                        ++sliceIdx;

                        if (sliceIdx == numSlices)
                        {
                            sliceIdx = 0x0;
                        }
                    }
                }
            }
            else
            {

                fptr->outputBuffer[o] = (u8 *)sippMemAlloc (&pl->tHeapMCB,
                                                            (SippVirtualPool)(vPoolFilterLineBuf0 + sliceIdx),
                                                            filtOBufsSize[i]);

                // If allocation failed, try in the next slices
                if (fptr->outputBuffer[o] == (u8 *)NULL)
                {
                    u32 success = 0;
                    for (idx = 1; idx < numSlices; idx++)
                    {
                        u32 slice               = (sliceIdx + idx) % numSlices;
                        fptr->outputBuffer[o] = (u8 *)sippMemAlloc (&pl->tHeapMCB,
                                                                    (SippVirtualPool)(vPoolFilterLineBuf0 + slice),
                                                                    filtOBufsSize[i]);

                        if (fptr->outputBuffer[o] != (u8 *)NULL)
                        {
                            success  = 0x1;
                            sliceIdx = slice;
                            break;
                        }
                    }

                    if (!success)
                    {
                        sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "sippBuildLnBuffsOPipe : Alloc failed for filter %d, buffer %d\n", fptr->unit, o);
                        sippError (fptr->errorStatus, E_OUT_OF_MEM);
                        return 0x1;
                    }
                }
                else
                {
                    sliceIdx++;
                    if (sliceIdx == numSlices)
                        sliceIdx = 0;
                }
            }
        }

        #ifdef RT_DBG
        sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "sippMemAlloc : fptr[%d]->outputBuffer[%d] = %x (Lines = %d)\n", fptr->unit, o, fptr->outputBuffer[o], fptr->nLines[o]);
        #endif
    }

    // DEBUG: At the end of all allocs, display the status
    sippMemStatus ();

    return 0x0;
}

#endif

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippComputePaddingOffsets                                        //
//                                                                                //
//  DESCRIPTION: Precompute Padding addr offsets to lower Shave overhead          //
//                                                                                //
//  INPUTS:      pl - pipeline                                                    //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       Each Shave will translate Left/Right, Source/Destination Padding //
//               address by adding precomputed offsets (specific for each shave   //
//               number) to "fptr->lnToPad"                                       //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippComputePaddingOffsets (SippPipeline * pl)
{
    u32         nFilters  = pl->nPadFilters;
    CommInfo *  ci        = &pl->gi;
    HPadInfo *  padList   = pl->padList;
    u32         i, svuNo, plane, oBufIdx;
    s32         off, lineOff, totalBits;
    SippFilter *fptr;

    u32 sliceF  = ci->sliceFirst;
    u32 sliceL  = ci->sliceLast;
    u32 sliceSz = ci->sliceSize;
    u32 padding;
    u32 bpp, sliceWidth;

    for (i = 0; i < nFilters; i++)
    {
        HorizPaddingOff *pO;

        // Get reference to current filter
        fptr    = padList[i].fptr;
        oBufIdx = padList[i].oBufId;
        padding = fptr->hPadding[oBufIdx];
        bpp     = fptr->bpp[oBufIdx];

        padList[i].CT1 = fptr->nPlanes[oBufIdx] * sizeof(HorizPaddingOff);

        // Allocate Shave padding helper offsets:
        // These offsets are filter specific, by they get allocated as part of Pipeline data struct
        // to remove 2 level indicrection (Speedup Shave implementation)
        padList[i].padOffsets = (HorizPaddingOff *)sippMemAlloc (&pl->tHeapMCB,
                                                                 vPoolPipeStructs,
                                                                 12 * fptr->nPlanes[oBufIdx] * sizeof(HorizPaddingOff));

        //Compute offsets for all shaves in use (for each plane)
        for (svuNo = sliceF; svuNo <= sliceL; svuNo++)
        {
            pO = (HorizPaddingOff *)((u8*)padList[i].padOffsets + svuNo * padList[i].CT1);

            for (plane = 0; plane < fptr->nPlanes[oBufIdx]; plane++)
            {
                //==========================================================
                //Offset from Slice0 based "lnToPad" address
                //==========================================================
                #ifdef SIPP_SLICE0_ADDRESS
                lineOff = (s32)sliceSz * (fptr->firstOutSlc + (svuNo - sliceF));
                #else
                lineOff = (s32)sliceSz * (fptr->firstOutSlc + (svuNo - (sliceF<<1)));
                #endif

                // Top Wrap after each lineOff change
                #ifdef SIPP_SLICE0_ADDRESS
                if (lineOff >= (s32)(sliceSz * (sliceL + 1)))
                #else
                if (lineOff >= (s32)(sliceSz * ((sliceL-sliceF) + 1)))
                #endif
                {
                    lineOff += (sliceF - (sliceL + 1)) * sliceSz;
                }

                totalBits  = (plane * fptr->planeStride[oBufIdx]) * bpp;
                totalBits += 0x7;
                totalBits &= 0xFFFFFFF8;
                lineOff += totalBits >> 3;

                // Top Wrap after each lineOff change
                #ifdef SIPP_SLICE0_ADDRESS
                if (lineOff >= (s32)(sliceSz * (sliceL + 1)))
                #else
                if (lineOff >= (s32)(sliceSz * ((sliceL-sliceF) + 1)))
                #endif
                {
                    lineOff += (sliceF - (sliceL + 1)) * sliceSz;
                }

                //==========================================================
                //Figure out slice width:
                //==========================================================
                sliceWidth = fptr->sliceWidth;

                if (svuNo == sliceL)
                    sliceWidth = fptr->sliceWidthLastSvu;  // *** KMC - Can this work if fptr->firstOutSlc != sliceF
                                                           // or is it simply acceptable that the last shave does less even if its chunk happens
                                                           // to be in the middle of the picture and not at the right edge...


                //==========================================================
                // Left Padding Offsets
                //==========================================================
                totalBits  = padding * bpp;
                totalBits += 0x7;
                totalBits &= 0xFFFFFFF8;
                pO->dstLeftO = lineOff - (totalBits >> 3);

                if (svuNo == sliceF) // *** KMC - Again can this work if fptr->firstOutSlc != sliceF
                {
                    // Replication for First Shave, thus src = line ptr, so offset = 0
                    pO->srcLeftO = lineOff + 0;
                }
                else
                {
                    // Copy from Previous slice; figure out previous slice:
                    // Go back 1 slice, then move towards the end of the line
                    totalBits  = (fptr->sliceWidth - padding) * bpp;
                    totalBits += 0x7;
                    totalBits &= 0xFFFFFFF8;
                    off = lineOff + (-(s32)sliceSz + (totalBits >> 3));
                    // Bottom addr wrap
                    #ifdef SIPP_SLICE0_ADDRESS
                    if (off < (s32)(sliceF * sliceSz))
                    #else
                    if (off < 0)
                    #endif
                    {
                        off += (sliceL - sliceF + 1) * sliceSz;
                    }

                    // Save offset
                    pO->srcLeftO = off;
                }

                //==========================================================
                // Right Padding offsets:
                //==========================================================
                totalBits  = sliceWidth * bpp ;
                totalBits += 0x7;
                totalBits &= 0xFFFFFFF8;
                pO->dstRightO = lineOff + (totalBits >> 3);

                if (svuNo == sliceL) // *** KMC - Again can this work if fptr->firstOutSlc != sliceF
                {
                    // Replication KMC - Sure if we have offset slices, then the last shave will not be running the last slice
                    // and so we should not do pixel replication!!!
                    totalBits  = (sliceWidth-1) * bpp ;
                    totalBits += 0x7;
                    totalBits &= 0xFFFFFFF8;
                    pO->srcRightO = lineOff + (totalBits >> 3);
                }
                else
                {
                    // Copy from Next slice; figure out next slice:
                    off = lineOff + sliceSz;
                    // Top Wrap
                    #ifdef SIPP_SLICE0_ADDRESS
                    if (off >= (s32)(sliceSz * (sliceL + 1)))
                    #else
                    if (off >= (s32)(sliceSz * ((sliceL-sliceF) + 1)))
                    #endif
                    {
                        off += (sliceF - (sliceL + 1)) * sliceSz;
                    }

                    // Save offset
                    pO->srcRightO = off;
                }

                // Moving to next plane
                pO++; //+= sizeof(HorizPaddingOff)
            }

        }

    }
}


////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippGetFirstHwFiltIdx                                            //
//                                                                                //
//  DESCRIPTION: Get an index to the first filter of a given type that will run   //
//               on ExecUnit given by "type"                                      //
//                                                                                //
//  INPUTS:      pl - Pipeline of interest                                        //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       This is a 2x8x only feature really as we would not expect the    //
//               same filter to be used in the same pipeline on 2x5x              //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippGetFirstHwFiltIdx (SippPipeline * pl)
{
    u32 i, f, u, found;

    for (u = 0; u < SIPP_MAX_FILTER_ID; u++)
    {
        // Only scan units that are used:
        if (pl->hwSippFltCnt[u] > 0)
        {
            // For current unit, scan through all iterations,
            // and return first filter of type "u" that will run
            found = 0;

            #ifdef SIPP_SCRT_ENABLE_OPIPE
            if (pl->pfnSippRuntime == sippOPipeRuntime)
            {
                // For a non-scheduled runtime, how do we establish which filter unit will get used first?
                // At the moment, just take the first in order of creation
                for (f = 0; f < pl->nFilters; f++)
                {
                    if (pl->filters[f]->unit == u)
                    {
                        pl->hwSippFirst [u] = f;
                        break;
                    }
                }
            }
            else
            #endif
            for (i = 0; (i < pl->nIter) && (!found); i++)
            {
                for (f = 0; f < pl->nFilters; f++)
                {

                    if ((pl->filters[f]->unit == u) &&
                        (pl->schedInfo[i].allMask & (1 << f)))
                    {
                        pl->hwSippFirst [u] = f;
                        found               = 1;
                        break;
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippIniHwFilters                                                 //
//                                                                                //
//  DESCRIPTION: Call the registered hw initialise function for all HW filters in //
//               a pipeline                                                       //
//                                                                                //
//  INPUTS:      pl - Pipeline of interest                                        //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

u32 sippIniHwFilters (SippPipeline *pl)
{
    u32 i;
    SippFilter **filters = pl->filters;
    SippFilter * fptr;

    for (i = 0; i < pl->nFilters; i++)
    {
        fptr = filters[i];
        if (fptr->unit < SIPP_SVU_ID)
        {
            if (gSipp.uInfo[fptr->unit].hwFnInit(fptr))
            {
                return 1;
            }
        }
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippComputeSwOutCt                                               //
//                                                                                //
//  DESCRIPTION: Build helper constants to derive actual output chunk of a filter //
//               given slice0-based output pointer computed for Leon side.        //
//                                                                                //
//  INPUTS:      pl - Pipeline of interest                                        //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       These would be used when translating Input/Output line pointers  //
//               from SHAVE side. The assumption is that shave_N produces         //
//               chunk_N, even if start/planes is/are shifted                     //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippComputeSwOutCt (SippPipeline *pl)
{
    u32          i, o;
    u32          svuNo; //sweeps through all consumer shaves of a filter
    SippFilter * fptr;
    u32          sliceF  = pl->gi.sliceFirst;
    u32          sliceL  = pl->gi.sliceLast;
    u32          sliceSz = pl->gi.sliceSize;
    u32          planeNo;
    u32          totalBits;
    s32          off;

    for (i = 0; i < pl->nFilters; i++)
    {
        //Ref to current filter
        fptr = pl->filters[i];

        #if defined(__sparc) //on Myriad, just allocate & compute offsets for filters that need a SW view
        if (fptr->flags & SIPP_REQ_SW_VIEW)
        #endif               // on PC M2 simulation, allocate for all fitlers to help Filter DUMPS
        {
            for (o = 0; o < fptr->numOBufs; o++)
            {
                // Alloc storage for constants
                // Once again this must be a number of buffers also
                fptr->outOff[o] = (s32*)sippMemAlloc (&pl->tHeapMCB,
                                                      vPoolSchedule,
                                                      fptr->nPlanes[o] * 12 * sizeof(u8*));

                // Compute constants for each Shave and Plane
                // Note planes might be in different slices (therefore wrap)
                for (svuNo = sliceF; svuNo <= sliceL; svuNo++)
                {
                    for (planeNo=0; planeNo < fptr->nPlanes[o]; planeNo++)
                    {
                        #ifdef SIPP_SLICE0_ADDRESS

                        off  = sliceSz * (fptr->firstOutSlc + (svuNo - sliceF));
                        // After each OFFSET operation, we need to clamp to valid slice range
                        if (off >= (s32)(sliceSz * (sliceL + 1)))
                           off += (sliceF - (sliceL + 1)) * sliceSz;

                        totalBits  = (planeNo * fptr->planeStride[o]) * fptr->bpp[o];
                        totalBits += 0x7;
                        totalBits &= 0xFFFFFFF8;
                        off += totalBits >> 3;
                        // After each OFFSET operation, we need to clamp to valid slice range
                        if (off >= (s32)(sliceSz * (sliceL + 1)) )
                           off += (sliceF - (sliceL + 1)) * sliceSz;

                        fptr->outOff[o][svuNo * fptr->nPlanes[o] + planeNo] = off;

                        #else

                        s32 tempVal = (svuNo - (sliceF << 1));
                        off  = sliceSz * (fptr->firstOutSlc + tempVal);
                        // After each OFFSET operation, we need to clamp to valid slice range
                        if (off >= (s32)(sliceSz * ((sliceL - sliceF) + 1)) )
                           off -= ((sliceL + 1) * sliceSz);

                        totalBits  = (planeNo * fptr->planeStride[o]) * fptr->bpp[o];
                        totalBits += 0x7;
                        totalBits &= 0xFFFFFFF8;
                        off += totalBits >> 3;
                        //After each OFFSET operation, we need to clamp to valid slice range
                        if (off >= (s32)(sliceSz * ((sliceL - sliceF) + 1)) )
                           off -= ((sliceL + 1) * sliceSz);

                        fptr->outOff[o][svuNo * fptr->nPlanes[o] + planeNo] = off;

                        #endif
                    }
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippInitSyncMutexes                                              //
//                                                                                //
//  DESCRIPTION: initialise the SHAVE/Leon sync mutexes for a pipe                //
//                                                                                //
//  INPUTS:      pl - Pipeline of interest                                        //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippInitSyncMutexes(SippPipeline *pl)
{
    #if defined(MYRIAD2)
    //Release_and_take MTX0
    DrvMutexUnlock(        pl->svuSyncMtx[0]);
    while(!DrvMutexTryLock(pl->svuSyncMtx[0]));
    //Take MTX1
    DrvMutexUnlock(pl->svuSyncMtx[1]);

    pl->svuSyncMtxParity = 0;
    #endif
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippAsmOptSetup                                                  //
//                                                                                //
//  DESCRIPTION: Duplicate and pack some of the parent data members into consumer //
//               filter to remove one level of indirection and so speedup a bit   //
//               pointer translation                                              //
//                                                                                //
//  INPUTS:      pl - Pipeline of interest                                        //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippAsmOptSetup(SippPipeline *pl)
{
    #if defined(MYRIAD2)
    u32     i, p, parObufIdx;
    SippFilter *f, *par;

    for (i = 0; i < pl->nFiltersSvu; i++)
    {
        f = pl->filtersSvu[i];

        for (p = 0; p < f->nParents; p++)
        {
            par                   = f->parents[p];
            parObufIdx            = f->parentOBufIdx[p];
            f->parInfo[p].nPlanes = par->nPlanes[parObufIdx];
            f->parInfo[p].outOff  = (u32)par->outOff[parObufIdx];
        }
    }
    #endif
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippComputeSliceLayout                                           //
//                                                                                //
//  DESCRIPTION: Compute CMX slice layout incl. Shave Code and Data requirements  //
//                                                                                //
//  INPUTS:      pl - Pipeline of interest                                        //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       Also sets up stack                                               //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippComputeSliceLayout (SippPipeline *pl)
{
    u32 maxCode = 0;
    u32 maxData = 0;
    u32 curPtr  = 0;

    extern u8* sippCmxBase;

    #if defined(__sparc)
    if (pl->mbinImg)
    {
        void sippGetSvuSecSize(u8 *svuElf, u32 *maxCode, u32 *maxData);
        sippGetSvuSecSize(pl->mbinImg, &maxCode, &maxData);
    }
    #endif

    curPtr = 0; //this is 1KB aligned !

    //If user didn't already force its own setup here:...
    if (pl->svuWinRegs[1] == 0) //CODE
    {
        //1st section is code (0x1D window as in default ldscript)
        if (pl->flags & PLF_MAP_SVU_CODE_IN_DDR) //where else is this used and how is mallocing going to get the code where we want???
        {
            pl->svuWinRegs[1] = (u32)sippMemAlloc(&pl->tHeapMCB,
                                                  vPoolGeneral,
                                                  maxCode);
        }
        else
        {
            //code locally copied in each slice
            pl->svuWinRegs[1] = (u32)sippCmxBase + curPtr;
            curPtr += maxCode; //Code
            curPtr = (curPtr + 1023) & (~1023); //1KB align for next allocs...
        }
    }

    if (pl->svuWinRegs[0] == 0)//DATA, STACK, LN_POOL
    {
        //2nd section is static slice data (0x1C....)
        pl->svuWinRegs[0] = (u32)sippCmxBase + curPtr;
        curPtr           += maxData; //Static data
        //3rd zone is stack (advances downards)
        pl->svuStack = 0x1C000000 + maxData + SIPP_SVU_STACK_SZ;
        curPtr      += SIPP_SVU_STACK_SZ;
        // 4th zone is dynamic data (line pool buffer) : allocates ABSOLUTE addr (slice0 based)
        pl->lineMemPoolBase = (u8 *)(sippCmxBase + curPtr);
        sippInitLnMemPool (&pl->tHeapMCB,
                           pl->lineMemPoolBase);
    }

    #if defined(SIPP_VCS) //Debug
    VCS_PRINT_INT(0x12344321);
    VCS_PRINT_INT(pl->svuWinRegs[0]);    //win 0
    VCS_PRINT_INT(pl->svuWinRegs[1]);    //win 1
    VCS_PRINT_INT(pl->svuStack);         //stack
    VCS_PRINT_INT(sippCmxBase + curPtr); //line pool
    #endif
}

//####################################################################################
// A single 2D descriptor can save/restore all cmx slices to DDR
// This is used in conjunction with sippProcessIters() if we decide to reintroduce this

#ifdef SIPP_SUPPORT_PROCESS_ITERS

void sippDataSectAction (SippPipeline *pl, u64 action)
{
    #if defined(MYRIAD2) || defined(SIPP_PC)
    #if defined(SIPP_PC)
    if (!(pl->flags & PLF_RUNS_ITER_GROUPS))
    {
        sippError (pl->errorStatus, E_RUNS_ITER_GROUPS);
    }
    #endif

    // Kick the DMA
    SET_REG_DWORD(CDMA_SKIP_DES_ADR,                           0x0LL);
    SET_REG_DWORD(CDMA_INT_FREQ0_ADR + (8*SIPP_CDMA_INT_NO),   0x0LL);
    SET_REG_DWORD(CDMA_LINK_CFG0_ADR + (8*SIPP_CDMA_AGENT_NO), action);

    // The wait
    sippWaitDma();

    #endif
}

#endif // SIPP_SUPPORT_PROCESS_ITERS

//####################################################################################
//The following filters have NO shadow registers:
//  -  Colour combination
//  -  MIPI Rx filters
//  -  MIPI Tx filters
void sippGetCtxOrder (SippPipeline *pl)
{
    // Assume no multi-ctx support is needed
    pl->multiHwCtx = 0;
}

//##############################################################################
// calculates the source co-ordinate for a given row/column zoom
// factor and centre point for different source and dest sizes
float sippGetCoord2 (s32   in,
                     float factor,
                     float centreIn,
                     float centreOut)
{
    float shifted = (float)in;
    shifted -= centreOut;
    shifted /= factor;
    shifted += centreIn;
    return shifted;
}

////////////////////////////////////////////////////////////////////////////////////
//  FUNCTION:    sippCQInit                                                       //
//                                                                                //
//  DESCRIPTION: Initialise a pipelines SW command queue structures               //
//                                                                                //
//  INPUTS:      pPipe - Pipeline under consideration                             //
//                                                                                //
//  OUTPUTS:     None                                                             //
//                                                                                //
//  RETURNS:     None                                                             //
//                                                                                //
//  NOTES:       None                                                             //
//                                                                                //
//  CONTEXT:                                                                      //
//                                                                                //
////////////////////////////////////////////////////////////////////////////////////

void sippCQInit (SippPipeline * pPipe)
{
    if (pPipe->flags & PLF_ENABLE_SW_QU_USE)
    {
        u32 numEntries;

        if (pPipe->linesPerIter > 1)
        {
            numEntries = (pPipe->nFilters - (pPipe->nFiltersDMA + pPipe->nFiltersSvu)) * (pPipe->linesPerIter + 0x3);

            pPipe->tCMDUpdateQu.quEntry = (psSippCMDQuEntry)sippMemAlloc (&pPipe->tHeapMCB,
                                                                          vPoolPipeStructs,
                                                                          numEntries * sizeof(sSippCMDQuEntry));

            if (pPipe->tCMDUpdateQu.quEntry == (psSippCMDQuEntry)0x0)
            {
                sippError (pPipe->errorStatus, E_OUT_OF_MEM);

                // Reset the SW QU flag
                pPipe->flags &= ~PLF_ENABLE_SW_QU_USE;
                return;
            }
            else
            {
                pPipe->tCMDUpdateQu.quSize = numEntries;
                pPipe->tCMDUpdateQu.quNum  = 0;
            }
        }

        numEntries = (pPipe->nFilters - (pPipe->nFiltersDMA + pPipe->nFiltersSvu)) * (pPipe->linesPerIter + 0x3);

        pPipe->tCMDStartQu.quEntry = (psSippCMDQuEntry)sippMemAlloc (&pPipe->tHeapMCB,
                                                                     vPoolPipeStructs,
                                                                     numEntries * sizeof(sSippCMDQuEntry));

        if (pPipe->tCMDStartQu.quEntry == (psSippCMDQuEntry)0x0)
        {
            sippError (pPipe->errorStatus, E_OUT_OF_MEM);

            // Reset the SW QU flag
            pPipe->flags &= ~PLF_ENABLE_SW_QU_USE;
        }
        else
        {
            pPipe->tCMDStartQu.quSize = numEntries;
            pPipe->tCMDStartQu.quNum  = 0;
        }
    }

    return;
}
