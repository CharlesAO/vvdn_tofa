///
/// @file
/// @copyright All code copyright Movidius Ltd 2013, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     this file allow auto verification capability for unit tests, in vcs testing
///

#include "mv_types.h"

#ifdef VCS_TESTING_DUMP_ELF

#define ALIGNED(x) __attribute__((aligned(x)))
#define ALIGNED16 ALIGNED(16)

//=======================================================
// used for VCS testing purpose
// do not remove
// data is stored on CMX, slice 8
//=======================================================

#define MAX_CHECK_VARIABLE 28
#define MAX_PC_EXPECTED_OUTPUT 30720
volatile u32 savedElf = 0;
u32 nrOfCheckVariable = 0;
u32 StartAddrPcBoardLength[MAX_CHECK_VARIABLE*3] ALIGNED16;
u8  pcExpectedOutput[MAX_PC_EXPECTED_OUTPUT] ALIGNED16;
u32 *pcExpectedOutputPointer = (u32*)pcExpectedOutput;

#endif
//=======================================================

