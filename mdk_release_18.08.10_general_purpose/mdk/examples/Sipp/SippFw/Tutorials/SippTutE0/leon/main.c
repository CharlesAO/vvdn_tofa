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
#define EXP_CRC_VALUE0  0xBABAF4D2
#define EXP_CRC_VALUE1  0x4636ADC1
#endif

#include "app_config.h"
#include "sippTutE0Model.h"

SippTutE0Model myPl;

int main(int argc, char *argv[])
{
    UNUSED (argc);
    UNUSED (argv);
    u32    sippErrorLog[SIPP_ERROR_HISTORY_SIZE];

#if defined(SIPP_VCS)
   unsigned int crc0, crc1;
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

   buildSippTutE0Model(&myPl);

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

   configSippTutE0Model(&myPl);

   // change pipeline config/parameters manually if needed
   //myPl->

   readSippTutE0ModelInput(&myPl);
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

   writeSippTutE0ModelOutput(&myPl);

#if defined(SIPP_VCS)
   crc0 = swcCalcCrc32(SippTutE0Model_dmaOut0_buffer, 256 * 256, 0);
   crc1 = swcCalcCrc32(SippTutE0Model_dmaOut1_buffer, 512 * 512, 0);
   unitTestCompare(crc0, EXP_CRC_VALUE0);
   unitTestCompare(crc1, EXP_CRC_VALUE1);
   unitTestFinalReport();
#endif

   return 0;
}
