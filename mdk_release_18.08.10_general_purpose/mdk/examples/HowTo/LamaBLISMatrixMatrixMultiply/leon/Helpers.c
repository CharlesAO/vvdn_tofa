
#include "Helpers.h"

#include <mv_types.h>
#include <swcFrameTypes.h>
#include <Defines.h>

#ifdef SHAVE_PERFORMANCE_ONLY
double time_shave;
#endif
int numberShaves = MAX_SHAVES;

extern void* (BLISTest0_startSGEMM);
extern void* (BLISTest0_startSGEMMTRSM_LL);
extern void* (BLISTest0_startSGEMMTRSM_LU);
extern void* (BLISTest0_startSGEMMTRSM_RU);
extern void* (BLISTest0_startSGEMMTRSM_RL);


swcShaveUnit_t listShaves[MAX_SHAVES] = {0};

u32 entryPointsSGEMM[MAX_SHAVES] =
{
   (u32)&BLISTest0_startSGEMM
};

u32 entryPointsSGEMMTRSM_LL[MAX_SHAVES] =
{
   (u32)&BLISTest0_startSGEMMTRSM_LL
};

u32 entryPointsSGEMMTRSM_LU[MAX_SHAVES] =
{
   (u32)&BLISTest0_startSGEMMTRSM_LU
};

u32 entryPointsSGEMMTRSM_RU[MAX_SHAVES] =
{
   (u32)&BLISTest0_startSGEMMTRSM_RU
};

u32 entryPointsSGEMMTRSM_RL[MAX_SHAVES] =
{
   (u32)&BLISTest0_startSGEMMTRSM_RL
};

