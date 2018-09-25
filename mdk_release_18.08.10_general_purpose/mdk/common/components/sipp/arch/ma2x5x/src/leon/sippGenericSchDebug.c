
#include <sipp.h>
#include <sippInternal.h>

//#############################################################################
void sippGenericDbgPrintRunnable (SippSchEnt *SEs[], u32 nSE, u32 iteration)
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
void sippGenericDbgShowBufferReq (SippSchEnt *SEs[], u32 nSE)
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

////#############################################################################
//// Dump the lineIndices buffers for all filters here, each column in output
//// file corresponds to an iteration
////#############################################################################
//void sippGenericDbgDumpBuffState(SippFilter *filters[], u32 nFilters, u32 iteration)
//{
//  #if defined(SIPP_PC)
//    FILE *f, *g;
//    u32 i,l;
//    char old_line[1024];
//
//    if(iteration == 0)
//    {
//      remove("global_progress.txt");
//      //create empty file
//      f = fopen("global_progress.txt", "w");
//      for(i=0; i<nFilters; i++)
//      {
//          for(l=0; l<filters[i]->nLines; l++)
//             fprintf(f,"filt_%04d : \n", i);
//           fprintf(f,"============\n");
//      }
//
//      fclose(f);
//    }
//
//    f = fopen("global_progress.txt", "r");
//    g = fopen("global_temp.txt",     "w");
//
//    for(i=0; i<nFilters; i++)
//    {
//
//        for(l=0; l<filters[i]->nLines; l++)
//        {
//          if(fgets(old_line, sizeof(old_line), f)){
//             old_line[strlen(old_line)-1] = 0; //gizas....
//             fputs(old_line, g);
//          }
//
//          if(filters[i]->sch->lineIndices[l] != -1)
//          {
//            fprintf(g, "%4d|\n", filters[i]->sch->lineIndices[l]);
//            fflush(g);
//          }
//          else{
//            fprintf(g, "xxxx|\n");
//            fflush(g);
//          }
//        }
//
//        //the line separator
//        if(fgets(old_line, sizeof(old_line), f))
//        {
//           old_line[strlen(old_line)-1] = 0;
//           fputs(old_line, g);
//        }
//
//        fprintf(g,"=====\n");
//    }
//
//    fclose(f);
//    fclose(g);
//
//    remove("global_progress.txt");
//    rename("global_temp.txt", "global_progress.txt");
//  #endif
//}

////#############################################################################
//void DBG_print_num_par(SippFilter *filters[], u32 nFilters)
//{
//    u32 n_par_links = 0;
//    u32 i;
//
//    for(i=0; i<nFilters; i++)
//        n_par_links += filters[i]->nParents;
//
//    printf("__Number of parent links = %d__ \n", n_par_links);
//}
