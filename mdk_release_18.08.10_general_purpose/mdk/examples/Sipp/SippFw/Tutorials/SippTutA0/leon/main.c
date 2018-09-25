///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

#if defined(SIPP_VCS)
#include <DrvTimer.h>
#include <DrvDdr.h>
#endif

#include "app_config.h"
#include "sippTutA0Model.h"

SippTutA0Model myPl;

int main(int argc, char *argv[])
{
    UNUSED (argc);
    UNUSED (argv);
    u32    sippErrorLog[SIPP_ERROR_HISTORY_SIZE];

    sippPlatformInit();

#if defined(SIPP_VCS)
    s32 sc;
    sc = initClocksAndMemory();
   if(sc)
       exit(sc);

   unitTestInit();
   DrvTimerInit();
   DrvDdrInitialise(NULL);
   unitTestInit();
#endif

   buildSippTutA0Model(&myPl);

   if (sippPipeGetErrorStatus (myPl.pl))
   {
        u32 errNum;
        printf ("Pipeline creation error\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
   }

   configSippTutA0Model(&myPl);

   // change pipeline config/parameters manually if needed
   //myPl->

   readSippTutA0ModelInput(&myPl);
   sippProcessFrame(myPl.pl);

   if (sippPipeGetErrorStatus (myPl.pl))
   {
        u32 errNum;
        printf ("Pipeline runtime error\nError codes:\n");
        errNum = sippGetErrorHistory (sippErrorLog);
        while (errNum)
        {
            printf ("%08lx\n", sippErrorLog[errNum - 0x1]);
            errNum--;
        }
   }

   writeSippTutA0ModelOutput(&myPl);
#if defined(SIPP_VCS)
   unitTestMemCompare(SippTutA0Model_dmaIn0_buffer, SippTutA0Model_dmaOut0_buffer, 512*512);

   unitTestFinalReport();
#endif
   return 0;
}
