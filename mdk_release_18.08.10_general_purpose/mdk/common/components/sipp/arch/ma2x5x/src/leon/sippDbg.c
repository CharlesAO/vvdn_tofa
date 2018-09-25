///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippInternal.h>

#if defined(SIPP_VCS)
#include <UnitTestApi.h>
#include <VcsHooksApi.h>
#elif defined(__myriad1__)
#include <UnitTestApi.h>
#endif

#ifdef RT_DBG
u32 sippDebugHistory[SIPP_DEBUG_HISTORY_INDEX];
u32 sippDebugHistoryIdx = 0;
u32 sippPipeHistory0[SIPP_DEBUG_HISTORY_INDEX];
u32 sippPipeHistory1[SIPP_DEBUG_HISTORY_INDEX];
u32 sippPipeHistory2[SIPP_DEBUG_HISTORY_INDEX];
u32 sippPipeHistory3[SIPP_DEBUG_HISTORY_INDEX];
u32 sippPipeHistoryIdx[SIPP_MAX_SUPPORTED_PIPELINES] = {0,0,0,0};
#endif

#ifdef MYRIAD2
#ifdef SIPP_RUNTIME_PROFILING
#include <DrvTimer.h>
#endif
#endif



#if defined(SIPP_PC)

    #define NUM_COLORS 19
    static u32 colorTab[NUM_COLORS] =
    {
        (217<<16)|( 72<<8)|(  1),
        (241<<16)|(105<<8)|( 19),
        (253<<16)|(141<<8)|( 60),
        (253<<16)|(174<<8)|(107),
        (255<<16)|(208<<8)|(162),
        (254<<16)|(230<<8)|(204),

        (236<<16)|(226<<8)|(240),
        (208<<16)|(209<<8)|(230),
        (166<<16)|(189<<8)|(219),
        (103<<16)|(169<<8)|(207),
        ( 54<<16)|(144<<8)|(192),
        ( 30<<16)|(129<<8)|(138),
        (  5<<16)|(108<<8)|( 90),

        ( 35<<16)|(132<<8)|( 67),
        ( 65<<16)|(171<<8)|( 93),
        (120<<16)|(198<<8)|(121),
        (173<<16)|(221<<8)|(142),
        (217<<16)|(240<<8)|(163),
        (247<<16)|(252<<8)|(185),

    };
#endif

//###########################################################################
#if defined(__myriad2__)
    //Bypass Leon's L1 and L2 caches
    static inline u32 bypassLeonCaches(u32 addr)
    {
        if((addr >> 28) == 0x7) return addr | 0x08000000; //CMX addr
        if((addr >> 28) == 0x8) return addr | 0x40000000; //DDR addr
        return addr; //else ?
    }

#endif

//###########################################################################
void sippDbgCompareU8(u8 *refA, u8* refB, int len)
{
    #if defined(__myriad2__)
    refA = (u8*)bypassLeonCaches((u32)refA);
    refB = (u8*)bypassLeonCaches((u32)refB);
    unitTestMemCompare((void*)refA, (void*)refB, len*sizeof(u8));

    #else //PC world

    int i, nPass = 1;
    u32 numFails = 0;
    for (i=0; i<len; i++)
    {
        if (refA[i] != refB[i])
        {
            if (numFails < 100) sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "U8 Compare FAIL at element %d: %d vs %d \n", i, refA[i], refB[i]);
            else if (numFails == 100) sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "Fail Count reaches 100, suppressing further output\n");
            nPass = 0;
            numFails++;
            //exit(99);
        }
    }

    if ( nPass ) sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "U8 Compare Passed \n");
    else         sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "U8 Compare Failed (Number = %d) \n", numFails);

    #endif
}

//###########################################################################
void sippDbgCompareDeltaU8(u8 *refA, u8* refB, int len, u8 delta)
{
    int i,diff, negDelta = -delta;
    int num_fails = 0;

    for(i = 0; i < len; i++)
    {
        diff = refA[i] - refB[i];

        if(( diff > delta) || ( diff < negDelta))
        {
            sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "U8 Compare FAIL: %d vs %d \n", refA[i], refB[i]);
            num_fails++;
        }
    }

    #ifndef SIPP_PC
    if(num_fails)
        unitTestLogFail();
    #endif
}

//###########################################################################
void sippDbgCompareU16(u16 *refA, u16* refB, int len)
{
 #if   defined(__myriad1__)
   unitTestMemCompare((void*)refA, (void*)refB, len*sizeof(u16));

 #elif defined(__myriad2__)
   refA = (u16*)bypassLeonCaches((u32)refA);
   refB = (u16*)bypassLeonCaches((u32)refB);
   unitTestMemCompare((void*)refA, (void*)refB, len*sizeof(u16));

 #else //PC world
   int i;
   for(i=0; i<len; i++)
    if(refA[i] != refB[i])
    {
      sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "U16 Compare FAIL at element %d: %d vs %d \n", i, refA[i], refB[i]);
      //exit(99);
    }
 #endif
}

//###########################################################################
void sippDbgCompareU32(u32 *refA, u32* refB, int len)
{
 #if   defined(__myriad1__)
   unitTestMemCompare((void*)refA, (void*)refB, len*sizeof(u32));

 #elif defined(__myriad2__)
   refA = (u32*)bypassLeonCaches((u32)refA);
   refB = (u32*)bypassLeonCaches((u32)refB);
   unitTestMemCompare((void*)refA, (void*)refB, len*sizeof(u32));

 #else //PC world
   int i;
   for(i=0; i<len; i++)
    if(refA[i] != refB[i])
    {
      sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "U32 Compare FAIL at element %d: %d vs %d \n", i, refA[i], refB[i]);
      //exit(99);
    }
 #endif
}

//###########################################################################
// Dump a representation of the graph in 'dot' format.  A PDF of the graph
// can be generated from the .dot file with the following command:
//
//       dot -T pdf -o pipeline.pdf < pipeline.dot
//
// The 'dot' utility is part of the "graphviz" package.

// TBD - Update for multiple output buffers

void sippDbgDumpGraph(SippPipeline *pl, const char *fname)
{
    UNUSED (pl);    //hush the compiler warning
    UNUSED (fname); //hush the compiler warning

#if defined(SIPP_F_DUMPS)
    FILE *       f = fopen(fname, "w");
    SippFilter * fptr, *pc;
    u32          i, c;
    const char * n;

    if (f == NULL)
    {
        perror(fname);
        exit(2);
    }

    fprintf ( f, "// This file is automatically generated.\n");
    fprintf ( f, "// Use the following command to generate a diagram (PDF) of "
                 "the SIPP graph:\n//\n");
    fprintf ( f, "//    dot -T pdf -o pipeline.pdf < pipeline.dot\n//\n");
    fprintf ( f, "// The \"dot\" utility is part of the \"graphviz\" package.\n");
    fprintf ( f, "digraph G {\n");

    for (i = 0; i < pl->nFilters; i++)
    {
        fptr = pl->filters[i];

        for (c = 0; c < fptr->nCons; c++)
        {
            pc = fptr->cons[c];
            fprintf ( f, "\t%s_%d -> %s_%d;\n",
                      pl->filtName[fptr->id],
                      fptr->id,
                      pl->filtName[pc->id],
                      pc->id);
        }
    }

    for (i = 0; i < pl->nFilters; i++)
    {
        fptr = pl->filters[i];
        n = pl->filtName[fptr->id];
        fprintf ( f, "\t%s_%d [ label = \"# %d\\n%s\\n %d / %d / %d\" ] [ style = \"filled\" ][ color = \"#%x\" ];\n",
                  n, i, i, n, fptr->nLines[0], fptr->nPlanes[0], fptr->bpp[0],
                  colorTab[i%NUM_COLORS]);
        fptr++;
    }

    fprintf ( f, "\tLegend [ label = \"LEGEND\\nLines / Planes / Bits\" ];\n");
    fprintf ( f, "}\n");

    fclose(f);
#endif
}

//###########################################################################
// Debug trace of which filters ran at each iteration
void sippDbgDumpRunMask(u32 mask, int iteration, int dbgDump)
{
    UNUSED (mask);      //hush the compiler warning
    UNUSED (iteration); //hush the compiler warning
    UNUSED (dbgDump);   //hush the compiler warning

  #if defined(SIPP_PC)
    int    i = 0;
    FILE  *dump;

    if(dbgDump)
    {
        if(iteration == 0)
           dump = fopen("sched.txt", "w");
        else
           dump = fopen("sched.txt", "a");

        if (dump == NULL) {
        perror("sched.txt");
        exit(2);
        }
    }

    sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "%04d: ", iteration); if(dbgDump) fprintf ( dump, "%04d: ", iteration);
    while (mask)
    {
        if (mask & 1)
        {
            if (i < 10)
            {
                sippPalTrace (SIPP_ACC_SCH_TL_ERROR, " ");
            }
            sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "%d ", i);   if(dbgDump) fprintf ( dump, "%3d", i);
        }
        else
        {
            sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "   ");      if(dbgDump) fprintf ( dump, "%3d", -1);
        }
        mask >>= 1;
        i++;
    }
    sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "\n");               if(dbgDump) fprintf ( dump, "\n");


    if(dbgDump) fclose(dump);
  #endif
}

//###########################################################################
// Dump the schedule as a C array

void sippDbgDumpSchedForVcsCArr (SippPipeline * pl)
{
    UNUSED (pl); //hush the compiler warning

    #if defined(SIPP_PC)
    ptSippPipelineSuper pSPipe = (ptSippPipelineSuper)pl;
    u32 i, j;
    char filename[80];

    if (pSPipe->uPipeIdx == 0)
    {
        sprintf (filename, "%s", "pcDumpSchedule.hh");
    }
    else
    {
        sprintf (filename, "%s_%X.hh", "pcDumpSchedule", pSPipe->uPipeIdx);
    }

    FILE   *f = fopen(filename, "w");

    if (f == NULL) {
        perror(filename);
        exit(2);
    }

    fprintf ( f, "//===============================================================\n");
    fprintf ( f, "// PC generated file (sippDbgDumpSchedForVcsCArr)\n");
    fprintf ( f, "// Custom for each test ! \n");
    fprintf ( f, "// This config file can be generate by re-running the test on PC.\n");
    fprintf ( f, "// Purpose: save Leon time in VCS to compute the schedule\n");
    fprintf ( f, "//===============================================================\n\n");

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //First, dump the schedule array as a global variable
    //MANDATORY:  dump member is order or definition
    fprintf ( f, "SchedInfo sippDumpedSched_%x[%d] = \n{ \n", pSPipe->uPipeIdx, pl->nIter);
    for(i=0; i<pl->nIter; i++)
    {
        fprintf ( f, "    {");
        fprintf ( f, "0x%08x,", pl->schedInfo[i].sippHwWaitMask);
        fprintf ( f, "0x%08x,", pl->schedInfo[i].shaveMask);
        fprintf ( f, "0x%08x,", pl->schedInfo[i].dmaMask);
        fprintf ( f, "0x%08x,", pl->schedInfo[i].allMask);
        #if defined(MA2150) || defined(MA2155) || defined(MA2450) || defined(MA2455)
        fprintf ( f, "0x%08x", pl->schedInfo[i].sippIbflIncMask);
        #ifdef BUGZILLA_22709
        fprintf ( f, ",0x%08x", pl->schedInfo[i].sippHwWaitEoFMask);
        #endif
        #endif
        //fprintf ( f, "0x%08x,", 0); sched info

        fprintf ( f, "},\n");
    }
    fprintf ( f, "};\n\n");

    // If any filters have individual KS - need to dump them here...
    for (i = 0; i < pl->nFilters; i++)
    {
        for (j = 0; j < SIPP_FILTER_MAX_IBUFS; j++)
        {
            if (pl->filters[i]->KSIterList[j])
            {
                u32 numIters = (pl->filters[i]->sch->maxOutputH / pl->filters[i]->linesPerIter) +
                               ((pl->filters[i]->sch->maxOutputH % pl->filters[i]->linesPerIter) ? 0x1 : 0x0);
                u32 arrSize  = (numIters + 1) * pl->filters[i]->linesPerIter;
                u32 k;

                fprintf (f, "u16 filt%dList%d [%d] = \n{\n", i, j, arrSize);

                for (k = 0; k < arrSize; k+=8)
                {
                    if ((k+7) < arrSize)
                    {
                        fprintf (f, "  0x%04x, 0x%04x, 0x%04x, 0x%04x, 0x%04x, 0x%04x, 0x%04x, 0x%04x,\n",
                                 pl->filters[i]->KSIterList[j][k+0],
                                 pl->filters[i]->KSIterList[j][k+1],
                                 pl->filters[i]->KSIterList[j][k+2],
                                 pl->filters[i]->KSIterList[j][k+3],
                                 pl->filters[i]->KSIterList[j][k+4],
                                 pl->filters[i]->KSIterList[j][k+5],
                                 pl->filters[i]->KSIterList[j][k+6],
                                 pl->filters[i]->KSIterList[j][k+7]
                                 );
                    }
                    else
                    {
                        fprintf (f, "  0x%04x,", pl->filters[i]->KSIterList[j][k+0]);
                        if ((k+1) < arrSize) fprintf (f, " 0x%04x,", pl->filters[i]->KSIterList[j][k+1]);
                        if ((k+2) < arrSize) fprintf (f, " 0x%04x,", pl->filters[i]->KSIterList[j][k+2]);
                        if ((k+3) < arrSize) fprintf (f, " 0x%04x,", pl->filters[i]->KSIterList[j][k+3]);
                        if ((k+4) < arrSize) fprintf (f, " 0x%04x,", pl->filters[i]->KSIterList[j][k+4]);
                        if ((k+5) < arrSize) fprintf (f, " 0x%04x,", pl->filters[i]->KSIterList[j][k+5]);
                        if ((k+6) < arrSize) fprintf (f, " 0x%04x,", pl->filters[i]->KSIterList[j][k+6]);
                        fprintf (f, "\n");
                    }
                }

                fprintf ( f, "};\n\n");
            }
        }
    }


    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Then, call the init func

    fprintf ( f, "//===============================================================\n");

    if (pSPipe->uPipeIdx == 0)
    {
        fprintf ( f, "void dbgSchedInit(SippPipeline *pl)\n{\n");
    }
    else
    {
        fprintf ( f, "void dbgSchedInit_%x(SippPipeline *pl)\n{\n", pSPipe->uPipeIdx);
    }

    // Run a lines per iteration check - the value used when the schedule was generated
    // must match the value set when re-used as a pre-compiled schedule
    fprintf ( f, "  if (pl->linesPerIter != %d)\n  {\n    sippError (pl->errorStatus, E_PRECOMP_SCHED);\n  }\n\n" , pl->linesPerIter);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Set number of lines - 1 (as alloc will add one if filter has out buff)
    fprintf ( f, "  // Filters buffer sizes [lines]\n");
    for (i = 0; i < pl->nFilters; i++)
    {
        for (j = 0; j < pl->filters[i]->numOBufs; j++)
        {
            fprintf ( f, "  pl->filters[%2d]->nLines[%d] = %d; \n", i, j, pl->filters[i]->nLines[j]);
        }
    }
    fprintf ( f, "\n");

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //Kernel Starts
    fprintf ( f, "  // Kernel Starts (only for filters with parents !)\n");
    for(i=0; i<pl->nFilters; i++)
    {
      for(j=0; j<pl->filters[i]->nParents; j++)
         fprintf ( f, "  pl->filters[%2d]->parentsKS[%2d] = %d;\n", i, j, pl->filters[i]->parentsKS[j]);
    }
    fprintf ( f, "\n");

    // KS Iter list where needed
    fprintf ( f, "  // Add KSIteration lists for filters which need it\n");
    for (i = 0; i < pl->nFilters; i++)
    {
        for (j = 0; j < SIPP_FILTER_MAX_IBUFS; j++)
        {
            if (pl->filters[i]->KSIterList[j])
            {
                fprintf (f, "  pl->filters[%2d]->KSIterList[%2d] = filt%dList%d;\n", i, j, i, j);
            }
        }
    }
    fprintf ( f, "\n");

    // Multiple Line per Iteration variables
    fprintf ( f, "  // Add MLPI info for filters which need it\n");
    for (i = 0; i < pl->nFilters; i++)
    {
        fprintf (f, "  pl->filters[%2d]->firstRunRollLines = pl->filters[%2d]->firstRunNumLines = pl->filters[%2d]->firstIterLines = %d;\n", i, i, i, pl->filters[i]->firstRunNumLines);
    }
    fprintf ( f, "\n");

    // It is convenient for me to add padding here
    fprintf ( f, "  // HPad requirements\n");
    for (i=0; i<pl->nFilters; i++)
    {
        for (j = 0; j < pl->filters[i]->numOBufs; j++)
        {
            fprintf ( f, "  pl->filters[%2d]->hPadding[%2d] = %d;\n", i, j, pl->filters[i]->hPadding[j]);
        }
    }
    fprintf ( f, "\n");

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    //The schedule as C array:
    fprintf ( f, "  // The schedule mask for all iterations\n");
    fprintf ( f, "  pl->nIter          = %d;\n", pl->nIter);
    fprintf ( f, "  pl->schedInfo      = sippDumpedSched_%x;\n", pSPipe->uPipeIdx);



    //Close the func
    fprintf ( f, "}\n");
    fclose(f);
    #endif
}

//###########################################################################
void sippDbgDumpAsmOffsets(SippPipeline *pl)
{
  UNUSED (pl); //hush the compiler warning

#if defined(SIPP_PC)
  //Assume filter 1 has consumers and parents!
  FILE       *f    = fopen("myriad2SippDefs.inc", "w");
  SippFilter *fptr = pl->filters[1];
  u32 off;

  fprintf ( f, ";--------------------------------------------------------------\n");
  fprintf ( f, ";automatically generated from PC app (sippDbgDumpAsmOffsets)   \n");
  fprintf ( f, ";--------------------------------------------------------------\n\n");

  fprintf ( f, ".ifndef SIPP_SHAVE_ASM_DEFS\n");
  fprintf ( f, ".set    SIPP_SHAVE_ASM_DEFS\n\n");

  fprintf ( f, ";offsets within SippFilter struct\n");
  off = ((u32)&fptr->funcSvuRun    ) - (u32)fptr;        fprintf (f, ".set F_O_SVU_RUN      %6d ; fptr->funcSvuRun      \n", off);
  off = ((u32)fptr->parInfo        ) - (u32)fptr;        fprintf (f, ".set F_O_PAR_INFO     %6d ; fptr->parInfo         \n", off);
  off = ((u32)fptr->dbLinesIn[0]   ) - (u32)fptr;        fprintf (f, ".set F_O_LINES_I      %6d ; fptr->dbLinesIn[0]    \n", off);
  off = ((u32)fptr->dbLineOut[0]   ) - (u32)fptr;        fprintf (f, ".set F_O_LINES_O      %6d ; fptr->dbLineOut[0]    \n", off);
  off = ((u32)&fptr->exeNo         ) - (u32)fptr;        fprintf (f, ".set F_O_EXENO        %6d ; fptr->exeNo           \n", off);
  off = ((u32)fptr->hPadding       ) - (u32)fptr;        fprintf (f, ".set F_O_H_PADDING    %6d ; fptr->hPadding        \n", off);
  off = ((u32)&fptr->gi            ) - (u32)fptr;        fprintf (f, ".set F_O_COMM_INFO    %6d ; fptr->gi              \n", off);
  off = ((u32)fptr->outLinePtr     ) - (u32)fptr;        fprintf (f, ".set F_O_OUT_LPTR     %6d ; fptr->outLinePtr      \n", off);
  off = ((u32)&fptr->firstOutSlc   ) - (u32)fptr;        fprintf (f, ".set F_O_FIRST_O_SLC  %6d ; fptr->firstOutSlc     \n", off);
  off = ((u32)fptr->lnToPad        ) - (u32)fptr;        fprintf (f, ".set F_O_LN_TO_PAD    %6d ; fptr->lnToPad         \n", off);
  off = ((u32)fptr->outOff         ) - (u32)fptr;        fprintf (f, ".set F_O_OUT_OFFS     %6d ; fptr->outOff          \n", off);
  off = ((u32)&fptr->nLinesUsed[0] ) - (u32)fptr;        fprintf (f, ".set F_O_LINES_USED   %6d ; fptr->nLinesUsed      \n", off);
  off = ((u32)&fptr->ptrChunkPos   ) - (u32)fptr;        fprintf (f, ".set F_O_PIXEL_POS    %6d ; fptr->ptrChunkPos     \n", off);
  off = ((u32)&fptr->planeStride[0]) - (u32)fptr;        fprintf (f, ".set F_O_PL_STRIDE    %6d ; fptr->planeStride     \n", off);
  off = ((u32)&fptr->nLines[0]     ) - (u32)fptr;        fprintf (f, ".set F_O_N_LINES      %6d ; fptr->nLines          \n", off);
  off = ((u32)&fptr->parents[0])     - (u32)fptr;        fprintf (f, ".set F_O_PARENTS      %6d ; fptr->parents         \n", off);
  off = ((u32)&fptr->parentOBufIdx[0]) - (u32)fptr;      fprintf (f, ".set F_O_PAR_OBUF_IDX %6d ; fptr->parentOBufIdx   \n", off);
  off = ((u32)&fptr->bytesPerPix   ) - (u32)fptr;        fprintf (f, ".set F_O_BYTESPP      %6d ; fptr->bytesPerPix     \n", off);
  off = ((u32)&fptr->linesPerIter  ) - (u32)fptr;        fprintf (f, ".set F_O_LNS_PER_ITER %6d ; fptr->linesPerIter    \n", off);
  off = ((u32)&fptr->linesThisIter[0]) - (u32)fptr;      fprintf (f, ".set F_O_LNS_THIS_ITER%6d ; fptr->linesThisIter   \n", off);
  off = ((u32)&fptr->bpp           ) - (u32)fptr;        fprintf (f, ".set F_O_BPP          %6d ; fptr->bpp             \n", off);

  fprintf ( f, "\n;offsets within CommInfo struct\n");
  off = ((u32)&fptr->gi->sliceFirst) - ((u32)fptr->gi);  fprintf (f, ".set G_O_FIRST_S     %6d ; gi->sliceFirst \n", off);
  off = ((u32)&fptr->gi->sliceLast)  - ((u32)fptr->gi);  fprintf (f, ".set G_O_LAST_S      %6d ; gi->sliceLast  \n", off);
  off = ((u32)&fptr->gi->sliceSize)  - ((u32)fptr->gi);  fprintf (f, ".set G_O_SLC_SZ      %6d ; gi->sliceSize  \n", off);
  off = ((u32)&fptr->gi->curFrame)   - ((u32)fptr->gi);  fprintf (f, ".set G_O_CUR_F       %6d ; gi->curFrame   \n", off);

  fprintf ( f, "\n;offsets within SippPipeline struct\n");
  off = ((u32)&pl->svuCmd      )      - (u32)pl;         fprintf (f, ".set P_O_SVU_CMD      %6d ; pl->svuCmd     \n", off);
  off = ((u32)&pl->svuSyncMtx  )      - (u32)pl;         fprintf (f, ".set P_O_SVU_SYNC_MTX %6d ; pl->svuSyncMtx \n", off);
  off = ((u32)&pl->svuSyncSem  )      - (u32)pl;         fprintf (f, ".set P_O_SVU_SYNC_SEM %6d ; pl->svuSyncSem \n", off);
  off = ((u32)&pl->filtersSvu  )      - (u32)pl;         fprintf (f, ".set P_O_SVU_LIST     %6d ; pl->filtersSvu \n", off);
  off = ((u32)&pl->nPadFilters )      - (u32)pl;         fprintf (f, ".set P_O_N_PAD_FILTS  %6d ; pl->nPadFilters\n", off);
  off = ((u32)&pl->nFiltersSvu )      - (u32)pl;         fprintf (f, ".set P_O_N_SVU_FILTS  %6d ; pl->nFiltersSvu\n", off);
  off = ((u32)&pl->iteration   )      - (u32)pl;         fprintf (f, ".set P_O_ITER         %6d ; pl->iteration  \n", off);
  off = ((u32)&pl->schedInfo   )      - (u32)pl;         fprintf (f, ".set P_O_SCHED_NFO    %6d ; pl->schedInfo  \n", off);
  off = ((u32)&pl->gi          )      - (u32)pl;         fprintf (f, ".set P_O_GLOBAL_NFO   %6d ; pl->gi         \n", off);
  off = ((u32)&pl->padList     )      - (u32)pl;         fprintf (f, ".set P_O_PAD_LIST     %6d ; pl->padList    \n", off);


  fprintf ( f, "\n;offsets within SchedInfo struct\n");
  off = ((u32)&pl->schedInfo[0].shaveMask) - (u32)&pl->schedInfo[0];   fprintf ( f, ".set S_O_SVU_MASK    %6d ; schedInfo[0].shaveMask\n"  , off);
  off = sizeof(pl->schedInfo[0]);                                      fprintf ( f, ".set SZ_SCH_INFO     %6d ; sizeof(pl->schedInfo[0])\n", off);

  fprintf ( f, "\n.endif\n");
  fclose(f);
#endif
}

//###########################################################################
//
void sippDbgShowBuffPtr(SippFilter* fptr, const char *buff_name)
{
    UNUSED (fptr);      //hush the compiler warning
    UNUSED (buff_name); //hush the compiler warning

#if 0
    int i;

    #define SVU_NO 0

    sippPalTrace (SIPP_ACC_SCH_TL_ERROR, " _____ Buffer lines = %d ___ Buff name : %s\n", fptr->nLines, buff_name);

    for(i=0; i<fptr->nLines; i++)
    {
        //alu: display the linear view of all lines pointers exposed by this buffer
        sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "   [%2d] 0x%x : %02x %02x %02x %02x %02x %02x %02x %02x", i, (u32)fptr->linePtrs[i],
            ((u8*)WRESOLVE(fptr->linePtrs[i], SVU_NO))[0],
            ((u8*)WRESOLVE(fptr->linePtrs[i], SVU_NO))[1],
            ((u8*)WRESOLVE(fptr->linePtrs[i], SVU_NO))[2],
            ((u8*)WRESOLVE(fptr->linePtrs[i], SVU_NO))[3],
            ((u8*)WRESOLVE(fptr->linePtrs[i], SVU_NO))[4],
            ((u8*)WRESOLVE(fptr->linePtrs[i], SVU_NO))[5],
            ((u8*)WRESOLVE(fptr->linePtrs[i], SVU_NO))[6],
            ((u8*)WRESOLVE(fptr->linePtrs[i], SVU_NO))[7]
        );

        if( (u32)fptr->linePtrs[i] == (u32)fptr->outLinePtr)
            sippPalTrace (SIPP_ACC_SCH_TL_ERROR, " -> CURR:WR_in_progress \n");
        else
            sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "\n");
    }

    i = 0;//alu: dummy... so I can put a breakpoint here
#endif
}

//###########################################################################
void sippRdFileU8(u8 *buff, int count, const char *fName)
{
  #if defined(SIPP_PC)
    FILE *f = fopen(fName, "rb");

    if(f == NULL)
    {
        perror(fName);
        exit(2);
    }
    else{
        fread(buff, 1, count, f);
        fclose(f);
    }
  #elif defined(SIPP_VCS)
    loadMemFromFileSimple((char*)fName, count, buff);
  #endif
}
//################################################1###########################
void sippWrFileU8(u8 *buff, int count, const char *fName)
{
  #if defined(SIPP_PC)
    FILE *f = fopen(fName, "wb");
    if (f == NULL) {
    perror(fName);
    exit(2);
    }
    fwrite(buff, 1, count, f);
    fclose(f);
  #elif defined(SIPP_VCS) //MYRIAD2 only VCS test
    // save to actual filename passed ...
    saveMemoryToFile((u32)buff, count, fName);
  #endif
}

//###########################################################################
void sippRdFileU8toF16(half *buff, int count, const char *fName)
{
    UNUSED (buff);  //hush the compiler warning
    UNUSED (count); //hush the compiler warning
    UNUSED (fName); //hush the compiler warning

  #if defined(SIPP_PC)
    FILE *f = fopen(fName, "rb");
    int i;
    u8 data_in;

    if (f == NULL) {
        perror(fName);
    exit(2);
    }

    for(i=0; i<count; i++)
    {
      fread(&data_in, 1, 1, f);   //read "u8"
      buff[i] = data_in / 255.0f; //convert to "half"
    }

    fclose(f);
  #endif
}

//###########################################################################
void sippWrFileF16toU8(half *buff, int count, const char *fName)
{
    UNUSED (buff);  //hush the compiler warning
    UNUSED (count); //hush the compiler warning
    UNUSED (fName); //hush the compiler warning

   #if defined(SIPP_PC)
    FILE *f = fopen(fName, "wb");
    int i;
    u8 data_out;

    for(i=0; i<count; i++)
    {
      data_out = (u8)(buff[i] * 255.0f); //from fp16 [0..1] to u8
      //clamp neded here?
      fwrite(&data_out, 1, 1, f);   //write "u8"
    }

    fclose(f);
   #endif
}

//###########################################################################
//
void sippDbgCreateDumpFiles(SippPipeline *pl)
{
    UNUSED (pl); //hush the compiler warning

#if defined (SIPP_F_DUMPS)
    u32 i, totalBits;
    SippFilter *fptr;

    if (pl->dbgLevel < 1)
    {
	    return;
    }

    for (i=0; i<pl->nFilters; i++)
    {
        fptr = pl->filters[i];

        totalBits  = 1 * fptr->bpp[0];
        totalBits += 0x7;
        totalBits &= 0xFFFFFFF8;
        // replace with a pal function so we can use a cut down version of printf
        sprintf (pl->filtName[i], "filt_out_ID=%02d_%dx%d_%dplanes_%dbpp.raw",
                 fptr->id,
                 fptr->outputW,
                 fptr->outputH,
                 fptr->nPlanes[0],
                 totalBits >> 3);

	    if (pl->filtFile[i] != NULL) {
	        fclose(pl->filtFile[i]);
	    }

        if ((pl->filtFile[i] = fopen(pl->filtName[i], "wb")) == NULL)
        {
            perror(pl->filtName[i]);
            exit(2);
        }
    }
#endif
}

//###########################################################################
void sippDbgDumpFilterOuts (SippPipeline * pl)
{
    UNUSED (pl); //hush the compiler warning

    #if defined(SIPP_F_DUMPS) && 1

    u32           i, remaining, size;
    SippFilter ** filters = pl->filters;
    SippFilter *  fptr;
    u32           dumpOutput   = 0;
    u32           linesPerIter;

    // on PC, all models are blocking, so can do dumps here.
    // no need to wait() for units to execute

    for (i = 0; i < pl->nFilters; i++, dumpOutput = 0)
    {
        fptr         = filters[i];
        linesPerIter = fptr->linesPerIter;

        if (fptr->unit <= SIPP_MAX_FILTER_ID)
        {
            if ((0x1 << fptr->unit) & pl->schedInfo[pl->iteration].sippHwWaitMask) dumpOutput = 1;
        }
        else if (pl->schedInfo[pl->iteration].allMask & (1 << i))
        {
            dumpOutput = 1;
        }

        if ((dumpOutput) &&
            (((fptr->exeNo + 1) * linesPerIter) <= (fptr->outputH + linesPerIter - 0x1)))
        {
            u32 j = fptr->id;
            u32 s;         // slice
            u32 curPlane;  // current plane
            u32 x;
            u32 lineIterNum;
            u32 totalBits;

            // Can only dump filters that have associated output buffer
            // Also check for UNIT id to skip the tasks that are in the list just for h-padding
            if (fptr->nLines[0])
            {
                u32 linesToDump, noDumpLines;
                if (((fptr->exeNo + 1) * linesPerIter) > fptr->outputH)
                {
                    noDumpLines = (((fptr->exeNo + 1) * linesPerIter) - fptr->outputH);
                    if (noDumpLines > linesPerIter) linesToDump = 0;
                    else                            linesToDump = linesPerIter - noDumpLines;
                }
                else
                {
                    linesToDump = linesPerIter;
                }

                for (lineIterNum = 0; lineIterNum < linesToDump; lineIterNum++)
                {
                    remaining = fptr->outputW;

                    if (pl->dbgLevel > 1)
                    {
                        printf("EXE %4d : %08x\n", fptr->exeNo, (u32)fptr->outLinePtrPrev[0][lineIterNum]);
                    }

                    for (s = pl->gi.sliceFirst; s <= pl->gi.sliceLast; s++)
                    {
                        // Save multi-plane data in interleaved mode (so that I don't need
                        // to alloc huge buffers)
                        if (remaining < fptr->sliceWidth)
                            size = remaining;
                        else
                            size = fptr->sliceWidth;

                        for (x = 0; x < size; x++)
                        {
                            //Loop through all planes and write a single element from each (thus interleved)
                            for (curPlane = 0; curPlane < fptr->nPlanes[0]; curPlane++)
                            {
                                u32 addr;

                                totalBits  = x * fptr->bpp[0];
                                totalBits += 0x7;
                                totalBits &= 0xFFFFFFF8;
                                addr       =(u32)(fptr->outLinePtrPrev[0][lineIterNum] + (totalBits >> 3));
                                //update addr for output slice s (considering .firstOutSlc) and current Plane
                                addr += fptr->outOff[0][s * fptr->nPlanes[0] + curPlane];

                                fwrite ((void*)addr,
                                        fptr->bpp[0]>>3,
                                        1,
                                        pl->filtFile[j]);
                            }
                        }

                        remaining -= size;

                        fflush(pl->filtFile[j]);
                    }
                }
            }
        }//if(
    }//for(
   #endif
}

//####################################################################################
void sippDbgLevel(SippPipeline *pl, int dbgLevel)
{
  pl->dbgLevel = dbgLevel;
}

extern u8 *sippCmxBase;

//####################################################################################
void sippDumpHtmlMap(SippPipeline *pl)
{
    UNUSED (pl); //hush the compiler warning

  #if defined(SIPP_PC)
    u32 i, s;

    FILE *f = fopen("sippLayout.html", "w");

    fprintf ( f, "<head>\n");
    fprintf ( f, "<style type=\"text/css\">\n");
    fprintf ( f, "table {font-family: arial, verdana, sans-serif; font-size:100%%;}\n");
    fprintf ( f, "</style>\n");
    fprintf ( f, "</head>\n");

    fprintf ( f, "<TABLE BORDER=1 CELLSPACING=4 CELLPADDING=4 font face=\"Arial\">\n");
    fprintf ( f, " <caption> <font size=\"6\"> General Info</caption>\n");
    fprintf ( f, " <TR> <TD><B> Test name   <TD> TBD </TR>\n");
    fprintf ( f, " <TR> <TD><B> Num Filters <TD> %d </TR>\n"  , pl->nFilters);
    fprintf ( f, " <TR> <TD><B> First Slice <TD> %d  </TR>\n" , pl->gi.sliceFirst);
    fprintf ( f, " <TR> <TD><B> Last  Slice <TD> %d </TR>\n"  , pl->gi.sliceLast);
    fprintf ( f, " <TR> <TD><B> Slice Size  <TD> %dKB </TR>\n", pl->gi.sliceSize / 1024);
    fprintf ( f, "</TABLE>\n");
    fprintf ( f, "<br>\n");
    fprintf ( f, "<br>\n");

    fprintf ( f, "<TABLE BORDER=2 CELLSPACING=4 CELLPADDING=4 font face=\"Arial\">\n");
    fprintf ( f, "<caption> <font size=\"6\" align = \"Left\"> Slice allocation</caption>\n");
    fprintf ( f, "<TR>\n");
    fprintf ( f, "    <TD> \n");
    fprintf ( f, "    <TD><B>SLICE[0]\n");
    fprintf ( f, "    <TD><B>SLICE[1]\n");
    fprintf ( f, "    <TD><B>SLICE[2]\n");
    fprintf ( f, "    <TD><B>SLICE[3]\n");
    fprintf ( f, "    <TD><B>SLICE[4]\n");
    fprintf ( f, "    <TD><B>SLICE[5]\n");
    fprintf ( f, "    <TD><B>SLICE[6]\n");
    fprintf ( f, "    <TD><B>SLICE[7]\n");
    fprintf ( f, "    <TD><B>SLICE[8]\n");
    fprintf ( f, "    <TD><B>SLICE[9]\n");
    fprintf ( f, "    <TD><B>SLICE[10]\n");
    fprintf ( f, "    <TD><B>SLICE[11]\n");
    fprintf ( f, "</TR>\n");

    for(i=0; i<pl->nFilters; i++)
    {
        fprintf(f, "<TR>\n");

      //Name:
       #if defined(SIPP_F_DUMPS)
         if(strlen(pl->filtName[i]))
            fprintf(f, "<TD> %s </TD>\n",pl->filtName[i]);
         else
            fprintf(f, "<TD> Filt %d </TD>\n",pl->filters[i]->id);
       #else
         fprintf(f, "<TD> Filt %d </TD>\n",pl->filters[i]->id);
       #endif

        // Slices:
        if (pl->filters[i]->nLines[0] == 0)
        {
            for(s=0; s<=11; s++)
            {
                fprintf(f, "<TD BGCOLOR=\"#C0C0C0\"> unused");
            }

        }
        else
        {
            if (pl->filters[i]->oBufAlloc)
            {
                u32 obufCMXAddrOff;
                u32 firstOutSlice, lastOutSlice;

#ifdef SIPP_SLICE0_ADDRESS
                obufCMXAddrOff  = (pl->gi.sliceFirst * pl->gi.sliceSize);
                obufCMXAddrOff += (pl->filters[i]->outputBuffer[0] - sippCmxBase);
#else
                obufCMXAddrOff = pl->filters[i]->outputBuffer[0] - sippCmxBase;
#endif

                firstOutSlice  = (obufCMXAddrOff >> 17);
                lastOutSlice   = firstOutSlice + (pl->gi.sliceLast-pl->gi.sliceFirst);
                for(s=0; s<=11; s++)
                {
                    if ((s < firstOutSlice) || (s > lastOutSlice))
                    {
                        fprintf(f, "<TD BGCOLOR=\"#C0C0C0\"> unused");
                    }
                    else
                    {//slice is used, refine a bit:
                        s32 chunkNo = (s32)(s - firstOutSlice ) - (pl->filters[i]->firstOutSlc - pl->gi.sliceFirst);

                        if (chunkNo < 0)
                            chunkNo+= (pl->gi.sliceLast-pl->gi.sliceFirst+1);

                        if (chunkNo == 0)
                            fprintf(f, "<TD BGCOLOR=\"#%x\"><B><u>chunk_%d", colorTab[i%NUM_COLORS], chunkNo);
                        else
                            fprintf(f, "<TD BGCOLOR=\"#%x\">chunk_%d",        colorTab[i%NUM_COLORS], chunkNo);

                    }
                }
            }
            else
            {
                for(s=0; s<=11; s++)
                {
                    if((s<pl->gi.sliceFirst) || (s>pl->gi.sliceLast))
                    {
                        fprintf(f, "<TD BGCOLOR=\"#C0C0C0\"> unused");
                    }
                    else
                    {//slice is used, refine a bit:
                        s32 chunkNo = (s32)s - pl->filters[i]->firstOutSlc;

                        if(chunkNo<0)
                            chunkNo+= (pl->gi.sliceLast-pl->gi.sliceFirst+1);

                        if(s == pl->filters[i]->firstOutSlc)
                            fprintf(f, "<TD BGCOLOR=\"#%x\"><B><u>chunk_%d", colorTab[i%NUM_COLORS], chunkNo);
                        else
                            fprintf(f, "<TD BGCOLOR=\"#%x\">chunk_%d",        colorTab[i%NUM_COLORS], chunkNo);

                    }
                }
            }
        }
        fprintf(f, "</TR>\n");
    }

    fclose(f);

#endif
}

//###########################################################################
void sippPrintSliceWidth(SippPipeline *pl)
{
  UNUSED (pl); //hush the compiler warning

 #if defined(SIPP_PC)
   u32 i;

   sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "\n\n");
   for(i=0; i<pl->nFilters; i++)
     sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "Filt[%d]->sliceWidth = %d\n", i, pl->filters[i]->sliceWidth);
 #endif
}


static char *sippDbgPassFail(int pass)
{
    char *result = (char *)"FAIL";
    if (pass)
    {
        result = (char *)"PASS";
    }
    return result;
}

void sippTestCrcCheck(const void * pStart, u32 lengthBytes, u32 expectedCrc)
{
    UNUSED (expectedCrc);     //hush the compiler warning
    UNUSED (sippDbgPassFail); //hush the compiler warning

    const u32 CRC32_POLY = 0x04C11DB7;
    u32 crc32 = 0xffffffff;
    u32 temp = 0;
    u32 i;
    u32 bitNum;
    u8 *pData = (u8 *)pStart;
    for (i=0;i<lengthBytes;i++)
    {
        u8 current_byte = pData[i];
        for (bitNum = 0; bitNum < 8; bitNum++)
        {
            int msb = (crc32 >> 31) & 1;
            crc32 = crc32 << 1;
            if (msb != ((current_byte >> bitNum) & 1))
            {
                crc32 ^= CRC32_POLY;
                crc32 |= 1;
            }
        }
    }
    for (bitNum = 0; bitNum < 32; bitNum++)
    {
        if ((crc32 >> bitNum) & 1)
        {
            temp |= 1 << (31 - bitNum);
        }
    }
    crc32 = ~temp;
    sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "sippTestCrcCheck() : (addr:0x%08x len:0x%x actualCrc:%08x expectedCrc: %08x)  => %s\n", (int)pStart, lengthBytes, crc32, expectedCrc, sippDbgPassFail(crc32 == expectedCrc));
}



#ifdef RT_DBG
void sippDbgDumpLine0 (SippPipeline *pl)
{
#ifdef SIPP_NO_PRINTF
    UNUSED(pl);
#else
    u32 allMask  = pl->schedInfo[pl->iteration].allMask;
    u32 i, totalBits;
    for (i = 0; i < pl->nFilters; i++)
    {
        if (allMask & (1<<i))
        {
            u32 oBufIdx, planeIdx;

            for (oBufIdx = 0x0; oBufIdx < pl->filters[i]->numOBufs; oBufIdx++)
            {
                if ((pl->filters[i]->exeNo == 0x1) &&
                    (pl->filters[i]->nLines[oBufIdx]))
                {
                    u32  lineAddr = (u32)pl->filters[i]->dbLineOut[oBufIdx][(pl->filters[i]->schNo)&0x1][0];
                    u8 * pByte;

                    lineAddr += (pl->gi.sliceFirst * pl->gi.sliceSize);
                    // Make uncached accesses
                    #ifndef SIPP_PC
                    lineAddr |= 0x8000000;
                    #endif

                    // This filter just ran its first line
                    // Dump first 10 bytes of its output
                    sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "Filt[%d] (of %d) OBuf %d Line Buffer @ %08x ( unmodified %08x)\n", i, pl->nFilters,oBufIdx,lineAddr,pl->filters[i]->dbLineOut[oBufIdx][0][0]);
                    for (planeIdx = 0; planeIdx < pl->filters[i]->nPlanes[oBufIdx]; planeIdx++)
                    {
                        totalBits  = pl->filters[i]->planeStride[oBufIdx] * (pl->filters[i]->bpp[oBufIdx]);
                        totalBits += 0x7;
                        totalBits &= 0xFFFFFFF8;
                        pByte     = (u8 *)lineAddr + (planeIdx * (totalBits >> 3));
                        sippPalTrace (SIPP_ACC_SCH_TL_ERROR, "Filt[%d]Plane%d %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x\n",
                              i,
                              planeIdx,
                              pByte[0],
                              pByte[1],
                              pByte[2],
                              pByte[3],
                              pByte[4],
                              pByte[5],
                              pl->iteration,
                              pl->filters[i]->exeNo,
                              pl->filters[i]->schNo,
                              pl->filters[i]->oBufs[oBufIdx]->latency,
                              (u32)pByte);
                    }
                }
            }
        }
    }
#endif
}

#endif



#ifdef SIPP_RUNTIME_PROFILING

void sippDbgParseRTStats (SippPipeline * pPipe,
                          u8             printCtrl)
{
    ptSippPipelineSuper pSPipe     = (ptSippPipelineSuper)pPipe;
    ptRTStats           pStats     = pPipe->pRTStats;
    u32                 numIters   = pPipe->nIter;
    u32                 lPCount[SIPP_RT_PER_ITER_STATS_SIZE]  = {0};
    u32                 lP;
    u64                 lPTime;
    u32                 idx;

    // Long-Pole Detection Algo
    if (pPipe->useSyncRuntime)
    {
        u32 numIterVals = numIters * SIPP_RT_PER_ITER_STATS_SIZE;
        u32 iterNum;

        // Using sync runtimes...
        // we have DMA Count in entry[2],
        //         HW Count in entry [3] and
        //         SW Count in entry[4]
        // entry[0] is a post iteration unit start timestamp and
        // entry[1] is a post unit completion timestamp
        // So entry[iteration x+1][0] - entry[iteration x][1] gives a framework overhead

        if (printCtrl)
        {
            printf ("Pipeline %lu - Sync mode\n", pSPipe->uPipeIdx);
            printf (" Iter   ItTime    DMACy       HWCy     SWCy   EOL OvHead\n");
            printf ("=====================================================\n");
        }

        for (idx = 0, iterNum = 0; idx < numIterVals; idx += SIPP_RT_PER_ITER_STATS_SIZE, iterNum++)
        {
            u32 HWCy     = (pPipe->schedInfo[iterNum].sippHwWaitMask) ? pStats->iterTime[idx+SIPP_ITER_STAT_HW_CYCLE_POS] : 0x0;
            u32 SWCy     = (pPipe->schedInfo[iterNum].shaveMask)      ? pStats->iterTime[idx+SIPP_ITER_STAT_SW_CYCLE_POS] : 0x0;
            u32 CMXDMACy = (pPipe->schedInfo[iterNum].dmaMask)        ? pStats->iterTime[idx+SIPP_ITER_STAT_CDMA_CYCLE_POS] : 0x0;
            u32 FWOHead  = (idx < (numIterVals - 0x1)) ? (u32)(pStats->iterTime[idx+SIPP_RT_PER_ITER_STATS_SIZE+SIPP_ITER_STAT_STARTED_POS] - pStats->iterTime[idx+SIPP_ITER_STAT_COMPLETE_POS]) : 0;

            if (printCtrl)
            {
                printf ("%4lu %8lu, %8lu, %8lu, %8lu, %8lu\n", iterNum,
                                                               (u32)(pStats->iterTime[idx+SIPP_RT_PER_ITER_STATS_SIZE] - pStats->iterTime[idx]),
                                                               CMXDMACy,
                                                               HWCy,
                                                               SWCy,
                                                               FWOHead);
            }

            // The HW units are waited on in the order DMA / HW / SW (i.e. fastest to slowest)
            // This does mean that we do not have 100% clarity here on long-pole detection
            // The counts express only an indication of how long we were specifically waiting on that
            // execution block. For example the HW count will not start to accumulate until the DMA
            // has completed - so actually non-zero values are important as they tell us something
            // not simply the size of the non-zero value...

            if (SWCy)
            {
                lP = SIPP_ITER_STAT_SW_CYCLE_POS;
            }
            else if (HWCy)
            {
                lP = SIPP_ITER_STAT_HW_CYCLE_POS;
            }
            else if (CMXDMACy)
            {
                lP = SIPP_ITER_STAT_CDMA_CYCLE_POS;
            }
            else
            {
                lP = SIPP_ITER_STAT_COMPLETE_POS;
            }

            lPCount[lP]++;
        }

        lP = 0;

        for (idx = 1; idx < SIPP_RT_PER_ITER_STATS_SIZE; idx++)
        {
            if (lPCount[idx] > lPCount[0])
            {
                lPCount[0] = lPCount[idx];
                lP         = idx;
            }
        }

        switch (lP)
        {
            case SIPP_ITER_STAT_HW_CYCLE_POS   : printf ("\n ** HW most often long-pole **\n\n"); break;
            case SIPP_ITER_STAT_SW_CYCLE_POS   : printf ("\n ** SW most often long-pole **\n\n"); break;
            case SIPP_ITER_STAT_CDMA_CYCLE_POS : printf ("\n ** CMX DMA most often long-pole **\n\n"); break;
            case SIPP_ITER_STAT_COMPLETE_POS   : printf ("\n ** Framework most often long-pole **\n\n"); break;
            default                            : printf ("\n ** Error in long-pole detection **\n\n"); break;
        }

        printf ("HW Cnt: %5lu, SW Cnt: %5lu, CDMA Cnt: %5lu, FWork Cnt: %5lu\n\n",
                lPCount[SIPP_ITER_STAT_HW_CYCLE_POS],
                lPCount[SIPP_ITER_STAT_SW_CYCLE_POS],
                lPCount[SIPP_ITER_STAT_CDMA_CYCLE_POS],
                lPCount[SIPP_ITER_STAT_COMPLETE_POS]);
    }
    else
    {
        // Using async runtime - what do the results mean in this context?

        u32 numIterVals = numIters * SIPP_RT_PER_ITER_STATS_SIZE;
        u32 iterNum;

        // We have
        // * The time the iteration was started - and the time the FW finished preparation for next iter
        // * Time (we noticed) CMX DMA completed
        // * Time (we noticed) SW completed
        // * Time (we noticed) HW complete

        if (printCtrl)
        {
            printf ("Pipeline %lu - Async mode\n", pSPipe->uPipeIdx);
            printf (" Iter   ItTime      HW        SW       CDMA       FW\n");
            printf ("=====================================================\n");
        }

        for (idx = 0, iterNum = 0; idx < numIterVals; idx += SIPP_RT_PER_ITER_STATS_SIZE, iterNum++)
        {
            u32 HWTime     = (pPipe->schedInfo[iterNum].sippHwWaitMask) ? (u32)(pStats->iterTime[idx+SIPP_ITER_STAT_ASYNC_HW_POS] - pStats->iterTime[idx]) : 0x0;
            u32 SWTime     = (pPipe->schedInfo[iterNum].shaveMask)      ? (u32)(pStats->iterTime[idx+SIPP_ITER_STAT_ASYNC_SW_POS] - pStats->iterTime[idx]) : 0x0;
            u32 CMXDMATime = (pPipe->schedInfo[iterNum].dmaMask)        ? (u32)(pStats->iterTime[idx+SIPP_ITER_STAT_ASYNC_CDMA_POS] - pStats->iterTime[idx]) : 0x0;
            u32 FWTime     = (u32)(pStats->iterTime[idx+SIPP_ITER_STAT_ASYNC_FW_POS] - pStats->iterTime[idx]);

            if (printCtrl)
            {
                u32 iterTime = (idx < (numIterVals - 0x1)) ? (u32)(pStats->iterTime[idx+SIPP_RT_PER_ITER_STATS_SIZE] - pStats->iterTime[idx]) : 0xFFFF;

                printf ("%4lu %8lu, %8lu, %8lu, %8lu, %8lu\n", iterNum,
                                                               iterTime,
                                                               HWTime,
                                                               SWTime,
                                                               CMXDMATime,
                                                               FWTime);
            }

            if (HWTime > SWTime)
            {
                if (HWTime > CMXDMATime)
                {
                    lP     = SIPP_ITER_STAT_ASYNC_HW_POS;
                    lPTime = HWTime;
                }
                else
                {
                    lP = SIPP_ITER_STAT_ASYNC_CDMA_POS;
                    lPTime = CMXDMATime;
                }
            }
            else
            {
                if (SWTime > CMXDMATime)
                {
                    lP = SIPP_ITER_STAT_ASYNC_SW_POS;
                    lPTime = SWTime;
                }
                else
                {
                    lP = SIPP_ITER_STAT_ASYNC_CDMA_POS;
                    lPTime = CMXDMATime;
                }
            }

            // Is the framework count sufficiently close to the long-pole to make it the problem??
            if ((lPTime - FWTime) < 500)
            {
                lP = SIPP_ITER_STAT_ASYNC_FW_POS;
            }

            lPCount[lP]++;
        }

        lP = 0;

        for (idx = 1; idx < SIPP_RT_PER_ITER_STATS_SIZE; idx++)
        {
            if (lPCount[idx] > lPCount[0])
            {
                lPCount[0] = lPCount[idx];
                lP         = idx;
            }
        }

        switch (lP)
        {
            case SIPP_ITER_STAT_ASYNC_HW_POS   : printf ("\n ** HW most often long-pole **\n\n"); break;
            case SIPP_ITER_STAT_ASYNC_SW_POS   : printf ("\n ** SW most often long-pole **\n\n"); break;
            case SIPP_ITER_STAT_ASYNC_CDMA_POS : printf ("\n ** CMX DMA most often long-pole **\n\n"); break;
            case SIPP_ITER_STAT_ASYNC_FW_POS   : printf ("\n ** Framework most often long-pole **\n\n"); break;
            default                            : printf ("\n ** Error in long-pole detection **\n\n"); break;
        }

        printf ("HW Cnt: %5lu, SW Cnt: %5lu, CDMA Cnt: %5lu, FWork Cnt: %5lu\n\n",
                lPCount[SIPP_ITER_STAT_ASYNC_HW_POS],
                lPCount[SIPP_ITER_STAT_ASYNC_SW_POS],
                lPCount[SIPP_ITER_STAT_ASYNC_CDMA_POS],
                lPCount[SIPP_ITER_STAT_ASYNC_FW_POS]);

    }
}

#endif
