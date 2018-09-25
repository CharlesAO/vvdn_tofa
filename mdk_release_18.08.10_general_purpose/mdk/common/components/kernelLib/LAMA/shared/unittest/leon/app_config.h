///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Application configuration Leon header
///

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#include "mv_types.h"

#define MAX_SVU_NUMBER 12

#ifdef MYRIAD2
    extern CmxRamLayoutCfgType __cmx_config;
#else
    extern u32 __cmx_config;   //notify other tools of the cmx configuration
#endif

extern u32 __l2_config ;

/// Setup all the clock configurations needed by this application and also the ddr
///
/// @return    0 on success, non-zero otherwise  
int initClocksAndMemory(void);

//=========================================================================
//=========================Auto Ceck elf file functionality================
//=========================================================================
#ifdef VCS_TESTING_DUMP_ELF
    #include <UnitTestApi.h>
    #include <VcsHooksApi.h>
    extern volatile u32 vcstest8_savedElf;
    extern u32 vcstest8_nrOfCheckVariable;
    extern u32 vcstest8_StartAddrPcBoardLength;
          static inline void parseAndCeckKernels()
    {
        u32 i;
        u32 *startAddrPcBoardLength = &vcstest8_StartAddrPcBoardLength;
        for (i = 0; i < vcstest8_nrOfCheckVariable; i++)
        {
            unitTestMemCompare((void*)startAddrPcBoardLength[0],
                    (void*)startAddrPcBoardLength[1], startAddrPcBoardLength[2]);
            startAddrPcBoardLength +=3;
        }
    }
    #define INIT_END_OF_CODE_AREEA volatile u32 InitEndOfShaveCode[4]  __attribute__((section(".shv8.S.initRem"))) = {0xdededede, 0xdededede, 0xdededede, 0xdededede};
    #define AUTOCHECK_ELF_FILE   InitEndOfShaveCode[0]; if(vcstest8_savedElf) { \
    unitTestInit();parseAndCeckKernels(); unitTestFinalReport();}
#else
     #define AUTOCHECK_ELF_FILE
     #define INIT_END_OF_CODE_AREEA
#endif

#endif



