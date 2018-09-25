#ifndef __HELPERS_H__
#define __HELPERS_H__

#include "bli_config.h"
#include "Defines.h"
#include "swcShaveLoader.h"
#include <DrvShaveL2Cache.h>


#ifdef SHAVE_PERFORMANCE_ONLY
extern double time_shave;
#endif

extern swcShaveUnit_t listShaves[MAX_SHAVES];
extern int numberShaves;

extern u32 entryPointsSGEMM[MAX_SHAVES];
extern u32 entryPointsSGEMMTRSM_LL[MAX_SHAVES];
extern u32 entryPointsSGEMMTRSM_LU[MAX_SHAVES];
extern u32 entryPointsSGEMMTRSM_RU[MAX_SHAVES];
extern u32 entryPointsSGEMMTRSM_RL[MAX_SHAVES];

#endif /*__HELPERS_H__*/
