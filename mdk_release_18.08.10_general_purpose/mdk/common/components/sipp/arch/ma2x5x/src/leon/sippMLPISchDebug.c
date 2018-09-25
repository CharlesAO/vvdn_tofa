///
/// @file      sippMLPISchedDebug.c
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Multiple Line Per Iteration (MLPI) Scheduler Debug functions
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

// Possible for communal sharing??
//#############################################################################
void sippMLPIDbgPrintRunnable (SippSchEnt *SEs[], u32 nSE, u32 iteration)
{
    UNUSED (SEs);       //hush the compiler warning
    UNUSED (nSE);       //hush the compiler warning
    UNUSED (iteration); //hush the compiler warning

    #if defined(SIPP_PC)
    u32       i, p;
    SippSchEnt * pSE;
    char         bS, bF;

    printf("%04d : ", iteration);
    for (i = 0; i < nSE; i++)
    {
        pSE = SEs[i];
        if((pSE->canRunP == RS_CAN_RUN) &&
           (pSE->canRunC == RS_CAN_RUN)  )
        {
          //figure out brackets
          if(pSE->dbgJustRoll)  { bS = '('; bF = ')'; }
          else                  { bS = '['; bF = ']'; }
          printf("%c%3d", bS, i);
          //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
          //Kernel starts: for filters with parents
          if (pSE->numIBufs)
          {
              printf(":");
              for (p = 0; p < pSE->numIBufs; p++)
              {
                  printf("%3d", pSE->parentKS[p]);
                  if (p < pSE->numIBufs -1)
                      printf(",");
              }
          }
          //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
          printf("%c ", bF);
        }
        else
        {
            //If can't run, just print spaces instead of filter info...
            printf("      ");//for the 2xbrackets, filter num %3 and a space
            if (pSE->numIBufs)
            {
                printf(" "); //for :
                for (p = 0; p < pSE->numIBufs;   p++) printf("   ");//for KS %3
                for (p = 0; p < pSE->numIBufs-1; p++) printf(" "); //for ","
            }
        }
    }
    #endif
}

//#############################################################################
//Display number of lines in each buffer and KernelStarts in parent buffers
//#############################################################################
void sippMLPIDbgShowBufferReq (SippSchEnt *SEs[], u32 nSE)
{
    UNUSED (SEs); //hush the compiler warning
    UNUSED (nSE); //hush the compiler warning

    #if defined(SIPP_PC)
    u32 i, p;

    printf("\n\n======================================\n");
    printf(" Buff size, Kernel Starts required (will need +1):   \n");
    printf("======================================\n");
    for (i = 0; i < nSE; i++)
    {
        printf("SE %2d : ", i);
        for (p = 0; p < SEs[i]->numOBufs; p++)
        {
            printf("OBuf%d : Num_lines = %2d  ", p, SEs[i]->oBufs[p]->numLines);
        }
        printf("\n");
        printf("Kernel_start: ");
        for (p = 0; p < SEs[i]->numIBufs; p++)
        {
            printf("%2d,", SEs[i]->parentKSMin[p]);
        }
        printf("\n");
    }
    #endif
}

#endif // SIPP_SCRT_ENABLE_MLPI

// End of file
