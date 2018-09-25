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
#include "swcCrc.h"
#define EXP_CRC_VALUE 0x1FA4B29C
#endif


#include "sippTutA3Model.h"
#include "app_config.h"

SippTutA3Model myPl;

int main(int argc, char *argv[])
{
    UNUSED (argc);
    UNUSED (argv);
    u32    sippErrorLog[SIPP_ERROR_HISTORY_SIZE];

#if defined(SIPP_VCS)
   int crc = 0;
#endif

   sippPlatformInit();


#if defined(SIPP_VCS)
   initClocksAndMemory();
   unitTestInit();
   DrvTimerInit();
   DrvDdrInitialise(NULL);
#endif

   readSippTutA3ModelInput(&myPl);

   buildSippTutA3Model(&myPl);

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

   configSippTutA3Model(&myPl);


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

   sippStopSvus(myPl.pl);
   writeSippTutA3ModelOutput(&myPl);
#if defined(SIPP_VCS)
   crc = swcCalcCrc32(SippTutA3Model_dmaOut1_buffer, 512 * 512, 0);

   unitTestCompare(crc, EXP_CRC_VALUE);
   unitTestFinalReport();
#endif
   return 0;
}
