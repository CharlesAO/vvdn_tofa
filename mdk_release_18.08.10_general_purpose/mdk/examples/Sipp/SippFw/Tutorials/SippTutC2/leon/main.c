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
#define EXP_CRC_VALUE 0x0825CF94
#endif

#include "app_config.h"
#include "sippTutC2Model.h"

SippTutC2Model myPl;
UInt32 dMat[10] = {
   0x01010101, 0x01010101,
   0x01010101, 0x01010101,
   0x01010101, 0x01010101,
   0x01010101, 0x01010101,
   0x01010101, 0x01010101
};

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

   buildSippTutC2Model(&myPl);

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

   configSippTutC2Model(&myPl);

   // change pipeline config/parameters manually if needed
   //myPl->

   readSippTutC2ModelInput(&myPl);
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

   writeSippTutC2ModelOutput(&myPl);

#if defined(SIPP_VCS)
   crc = swcCalcCrc32(SippTutC2Model_dmaOut1_buffer, 512 * 512, 0);
   unitTestCompare(crc, EXP_CRC_VALUE);
   unitTestFinalReport();
#endif

   return 0;
}
