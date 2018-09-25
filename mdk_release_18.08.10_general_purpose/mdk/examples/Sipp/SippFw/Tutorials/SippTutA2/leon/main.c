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
#include <swcCrc.h>
#define EXP_CRC_VALUE 0x2D5053B6
#endif


#include "sippTutA2Model.h"
#include "app_config.h"

SippTutA2Model myPl;

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

   readSippTutA2ModelInput(&myPl);

   buildSippTutA2Model(&myPl);

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
   configSippTutA2Model(&myPl);

   // change pipeline config/parameters manually if needed
   //myPl->


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
   writeSippTutA2ModelOutput(&myPl);

#if defined(SIPP_VCS)

   crc = swcCalcCrc32(SippTutA2Model_dmaOut1_buffer, 512 * 512, 0);
   unitTestCompare(crc, EXP_CRC_VALUE);
   unitTestFinalReport();

#endif
   return 0;
}
