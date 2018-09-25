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
#define EXP_CRC_VALUE 0x62A6BD7E
#endif


#include "sippTutB1Model.h"
#include "app_config.h"

SippTutB1Model myPl;

int main(int argc, char *argv[])
{
    UNUSED (argc);
    UNUSED (argv);
    u32    sippErrorLog[SIPP_ERROR_HISTORY_SIZE];

#if defined(SIPP_VCS)
    unsigned int crc = 0;
#endif

   sippPlatformInit();

#if defined(SIPP_VCS)
   s32 sc;
   sc = initClocksAndMemory();
   if(sc)
      return sc;
   unitTestInit();
   DrvTimerInit();
   DrvDdrInitialise(NULL);
#endif

   buildSippTutB1Model(&myPl);

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

   configSippTutB1Model(&myPl);

   // change pipeline config/parameters manually if needed
   //myPl->

   readSippTutB1ModelInput(&myPl);
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

   writeSippTutB1ModelOutput(&myPl);

#if defined(SIPP_VCS)
   crc = swcCalcCrc32(SippTutB1Model_dmaOut3_buffer, 512 * 512, 0);
   unitTestCompare(crc, EXP_CRC_VALUE);
   unitTestFinalReport();
#endif

   return 0;
}
