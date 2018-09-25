///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippInternal.h>

//These get allocated PER SHAVE
SippPipeline *sipp_pl;     //The global Pipeline structure shared by Shaves and Leon.

#if defined(__MOVICOMPILE__)
#include <svuCommonShave.h> //scGetShaveNumber(); asm inline version
#elif defined(SIPP_PC)
#endif

extern void sippHorizontalPadding(SippPipeline *pl);

#ifdef __MOVICOMPILE__
//ASM implementations
extern void waitLeonStart(u32 svuSyncMtx);
extern void syncBarrier  (SippPipeline *pl, u32 svuSyncMtx);
//=======================================================================================
#else //PC land
u32 dbg_svu_no;
int scGetShaveNumber()
{
    return dbg_svu_no; //set by "sippKickShaveM1PC" on PC
}

#endif

//########################################################################################
u32 sippGetChunkStartPixelPos (SippFilter *fptr, u32 * YPos, u32 * XPos)
{
    if (fptr->ptrChunkPos)
    {
        u32 svuNo = scGetShaveNumber();
        *XPos = fptr->ptrChunkPos->XPos[svuNo];
        *YPos = fptr->ptrChunkPos->YPos[fptr->exeNo & 0x1];
        return 0x1;
    }
    return 0x0;
}

//########################################################################################
//########################################################################################
//########################################################################################
int SHAVE_MAIN (void)
{
    SippFilter *fptr;
    u32      i;
    #if defined(__myriad2__)
    u32      mtxParity;

    mtxParity = 0; // CRITICAL, symetric behavior on Leon side: start with parity = 0
    #endif

    while(1)
    {
        //=======================================================
        //Barrier1: wait for Leon start
        //=======================================================
        #if defined(__myriad2__) //MoviCompile for M2
        waitLeonStart(sipp_pl->svuSyncMtx[mtxParity]);
        #endif

        //=======================================================
        //Horizontal Padding : must happen even if nobody consumes data in current iteration.
        //At some point later some filter will start to use the lines, and they all need to be padded.
        //=======================================================
        sippHorizontalPadding(sipp_pl);

        //No need for syncBarrier here, as hPadding operates on lines produced in PREVIOUS iterations
        //=======================================================
        //The Run
        //=======================================================
        if (sipp_pl->svuCmd & CMD_RUN)
        {
            u32          nFilters = sipp_pl->nFiltersSvu;
            SippFilter **filters  = sipp_pl->filtersSvu;
            u32          runMask  = sipp_pl->schedInfo[sipp_pl->iteration].shaveMask;
            for (i = 0; i < nFilters; i++)
            {
                if (runMask & (1 << i))
                {
                    fptr = filters[i];
                    fptr->funcSvuRun(fptr);
                }
            }
        }

        //=======================================================
        //Barrier2: all shaves wait for all other shaves to finish
        //=======================================================
        #if defined(__myriad2__) //MoviCompile for M2
        syncBarrier(sipp_pl, sipp_pl->svuSyncMtx[mtxParity]);
        mtxParity = 1 - mtxParity; //Alternate the waiting mutex !
        #endif

        //=======================================================
        //Graceful Shaves STOP !
        //=======================================================
        if(sipp_pl->svuCmd & CMD_EXIT)
        {
           #if defined(SIPP_PC)
            return 1;
           #else //Sabre
            __asm("BRU.swih 0x1E \n\t nop 5");
           #endif
        }
    }

    return 0;
}
