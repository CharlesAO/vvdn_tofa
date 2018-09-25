///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippInternal.h>

// These get allocated PER SHAVE
extern SippPipeline *sipp_pl;     //The global Pipeline structure shared by Shaves and Leon.

#if defined(__MOVICOMPILE__)
#include <svuCommonShave.h> //scGetShaveNumber(); asm inline version
#elif defined(SIPP_PC)
#endif

#ifdef __MOVICOMPILE__

#else //PC land
extern u32 dbg_svu_no;
extern int scGetShaveNumber();
#endif


//#################################################################
void memsetBpp(u8 *i_dest,
               u8 *i_source,
               u32 i_padding,
               u32 i_bpp)
{
    u8   Format_8b;
    u16  Format_16b;
    u32  Format_32b;
    u32  i;
    u32 totalBits;

    switch(i_bpp)
    {   //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        case 8: //8bits
           Format_8b =  *((u8*)i_source);
           for (i = 0; i < i_padding; i++)
           {
              *(u8*)i_dest = Format_8b;
              totalBits  = 1 * i_bpp;
              totalBits += 0x7;
              totalBits &= 0xF8;
            //i_dest += (1<<i_bpp);
              i_dest += (totalBits >> 3);
           }
           break;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        case 16: //16Bits
           Format_16b = *((u16*)i_source);
           for (i = 0; i < i_padding; i++)
           {
             *((u16*)i_dest) = Format_16b;
             totalBits  = 1 * i_bpp;
             totalBits += 0x7;
             totalBits &= 0xFFF8;
         //  i_dest += (1<<i_bpp);
             i_dest += (totalBits >> 3);

           }
           break;
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        case 32: //32Bits
            Format_32b = *((u32*)i_source);
            for (i = 0; i < i_padding; i++) {
              *(u32*)i_dest = Format_32b;
              totalBits  = 1 * i_bpp;
              totalBits += 0x7;
              totalBits &= 0xFFFFFFF8;
            //i_dest += (1<<i_bpp);
              i_dest += (totalBits >> 3);
            }
            break;
    }//switch
}

//########################################################################################
//Padding operates on previous generated line !
// - the last line of a buffer gets padded when the bottom replication starts
// - this is when last line of that buffer will get used due to the DELAY required by HW
//Shave K will pad segment K; due to shifted starts, segment K might be in different slice
//########################################################################################
//########################################################################################
// I need this again for ma2x5x as the asm versions need to be updated for the
// structure changes

void sippHorizontalPadding (SippPipeline *pl)
{
    CommInfo *        ci     = &pl->gi;
    u32               svuNo  = scGetShaveNumber();
    u32               i, planeNo, totalBits;
    u8 *              linePtr, *dest, *src;
    HorizPaddingOff * pO;

    #if defined(SIPP_PC)
    //------------------------------------------------------------------------------------
    if(pl->svuCmd & CMD_H_PAD) //disregard this test on Shave, only here for PC model
    //------------------------------------------------------------------------------------
    #endif
    {
        u32          nFilters     = pl->nPadFilters;
        HPadInfo   * padList      = pl->padList;
        SippFilter * fptr;
        u32          oBufIdx;
        u32          lineIdx;
        u32          svuFirst, svuLast;
        u32          linesPerIter;

        for (i = 0; i < nFilters; i++)
        {
            fptr         = padList[i].fptr;
            oBufIdx      = padList[i].oBufId;

            // Not necessarily linesPerIter - this should really be lines2roll
            // as if we were padding the output of a latency filter or an OSE
            // for input to a SW kernel, teh first time we try it we may not have
            // all linesPerIter lines - instead of addtional checks here though
            // a better way may be to set lnToPad to something harmless - ISPMixedPipe must have found this after
            // the debayer so what happenned there???
            linesPerIter = fptr->linesPerIter;
            svuFirst     = fptr->firstOutSlc;
            svuLast      = ( svuFirst == ci->sliceFirst ) ? ci->sliceLast : (ci->sliceFirst - 1);

            for (lineIdx = 0; lineIdx < linesPerIter; lineIdx++)
            {
                // Get ptr to line to be padded
                linePtr  = fptr->lnToPad[oBufIdx][fptr->exeNo & 1][lineIdx]; //load both pointers + select using PEU

                //Pad all plane chunks; get a ptr to current Shave constants section
                pO = (HorizPaddingOff *)((u8*)padList[i].padOffsets + svuNo * padList[i].CT1);

                for (planeNo = 0; planeNo < fptr->nPlanes[oBufIdx]; planeNo++)
                {
                    //======================================================
                    //Left side :
                    //======================================================
                    dest = linePtr + pO->dstLeftO;
                    src  = linePtr + pO->srcLeftO;

                    if (svuNo == fptr->firstOutSlc)
                    {
                        //Replication
                        memsetBpp(dest, src, fptr->hPadding[oBufIdx], fptr->bpp[oBufIdx]);
                    }
                    else
                    {
                        //The actual padding
                        totalBits  = fptr->hPadding[oBufIdx] * fptr->bpp[oBufIdx];
                        totalBits += 0x7;
                        totalBits &= 0xFFFFFFF8;
                        memcpy   (dest, src, totalBits >> 3);
                    }

                    //======================================================
                    //Right side:
                    //======================================================
                    dest = linePtr + pO->dstRightO;
                    src  = linePtr + pO->srcRightO;

                    if (svuNo == svuLast)
                    {
                        //Replication
                        memsetBpp(dest, src, fptr->hPadding[oBufIdx], fptr->bpp[oBufIdx]);
                    }
                    else
                    {
                        //The actual padding:
                        totalBits  = fptr->hPadding[oBufIdx] * fptr->bpp[oBufIdx];
                        totalBits += 0x7;
                        totalBits &= 0xFFFFFFF8;
                        memcpy   (dest, src, totalBits >> 3);
                    }

                    //Move to next plane
                    pO++;

                }//for(planes
            }
        }//for(filters
    }//if(CMD_H_PAD
}
