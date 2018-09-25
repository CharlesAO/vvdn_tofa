///
/// @file
/// @copyright All code copyright Movidius Ltd 2014, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     SIPP engine
///

#include <sipp.h>
#include <sippInternal.h>

// These get allocated PER SHAVE
extern SippPipeline *sipp_pl;     //The global Pipeline structure shared by Shaves and Leon.

#if defined(__MOVICOMPILE__)
#include <svuCommonShave.h> //scGetShaveNumber(); asm inline version
#elif defined(SIPP_PC)
#endif

//#ifdef __MOVICOMPILE__
//
//#else //PC land
//extern u32 dbg_svu_no;
//extern int scGetShaveNumber();
//#endif

void sippShaveDebug (SippPipeline *pl, u32 val)
{
    UNUSED (pl);
    UNUSED (val);

    #ifdef RT_DBG
    pl->dbgArea[pl->dbgAreaIndex++] = val;
    if (pl->dbgAreaIndex == 32) pl->dbgAreaIndex = 0;
    #endif
}
