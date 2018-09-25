
#include <Helpers.h>

#include <mv_types.h>
#include <swcFrameTypes.h>
#include <Defines.h>

#ifdef SHAVE_PERFORMANCE_ONLY
double time_shave;
#endif
int numberShaves = MAX_SHAVES;

extern void* (BLISTest0_startSGEMM);
extern void* (BLISTest1_startSGEMM);
extern void* (BLISTest2_startSGEMM);
extern void* (BLISTest3_startSGEMM);
extern void* (BLISTest4_startSGEMM);
extern void* (BLISTest5_startSGEMM);
extern void* (BLISTest6_startSGEMM);
extern void* (BLISTest7_startSGEMM);
#ifdef MYRIAD2
extern void* (BLISTest8_startSGEMM);
extern void* (BLISTest9_startSGEMM);
extern void* (BLISTest10_startSGEMM);
extern void* (BLISTest11_startSGEMM);
#endif

extern void* (BLISTest0_startSGEMMTRSM_LL);
extern void* (BLISTest1_startSGEMMTRSM_LL);
extern void* (BLISTest2_startSGEMMTRSM_LL);
extern void* (BLISTest3_startSGEMMTRSM_LL);
extern void* (BLISTest4_startSGEMMTRSM_LL);
extern void* (BLISTest5_startSGEMMTRSM_LL);
extern void* (BLISTest6_startSGEMMTRSM_LL);
extern void* (BLISTest7_startSGEMMTRSM_LL);
#ifdef MYRIAD2
extern void* (BLISTest8_startSGEMMTRSM_LL);
extern void* (BLISTest9_startSGEMMTRSM_LL);
extern void* (BLISTest10_startSGEMMTRSM_LL);
extern void* (BLISTest11_startSGEMMTRSM_LL);
#endif

extern void* (BLISTest0_startSGEMMTRSM_LU);
extern void* (BLISTest1_startSGEMMTRSM_LU);
extern void* (BLISTest2_startSGEMMTRSM_LU);
extern void* (BLISTest3_startSGEMMTRSM_LU);
extern void* (BLISTest4_startSGEMMTRSM_LU);
extern void* (BLISTest5_startSGEMMTRSM_LU);
extern void* (BLISTest6_startSGEMMTRSM_LU);
extern void* (BLISTest7_startSGEMMTRSM_LU);
#ifdef MYRIAD2
extern void* (BLISTest8_startSGEMMTRSM_LU);
extern void* (BLISTest9_startSGEMMTRSM_LU);
extern void* (BLISTest10_startSGEMMTRSM_LU);
extern void* (BLISTest11_startSGEMMTRSM_LU);
#endif

extern void* (BLISTest0_startSGEMMTRSM_RU);
extern void* (BLISTest1_startSGEMMTRSM_RU);
extern void* (BLISTest2_startSGEMMTRSM_RU);
extern void* (BLISTest3_startSGEMMTRSM_RU);
extern void* (BLISTest4_startSGEMMTRSM_RU);
extern void* (BLISTest5_startSGEMMTRSM_RU);
extern void* (BLISTest6_startSGEMMTRSM_RU);
extern void* (BLISTest7_startSGEMMTRSM_RU);
#ifdef MYRIAD2
extern void* (BLISTest8_startSGEMMTRSM_RU);
extern void* (BLISTest9_startSGEMMTRSM_RU);
extern void* (BLISTest10_startSGEMMTRSM_RU);
extern void* (BLISTest11_startSGEMMTRSM_RU);
#endif

extern void* (BLISTest0_startSGEMMTRSM_RL);
extern void* (BLISTest1_startSGEMMTRSM_RL);
extern void* (BLISTest2_startSGEMMTRSM_RL);
extern void* (BLISTest3_startSGEMMTRSM_RL);
extern void* (BLISTest4_startSGEMMTRSM_RL);
extern void* (BLISTest5_startSGEMMTRSM_RL);
extern void* (BLISTest6_startSGEMMTRSM_RL);
extern void* (BLISTest7_startSGEMMTRSM_RL);
#ifdef MYRIAD2
extern void* (BLISTest8_startSGEMMTRSM_RL);
extern void* (BLISTest9_startSGEMMTRSM_RL);
extern void* (BLISTest10_startSGEMMTRSM_RL);
extern void* (BLISTest11_startSGEMMTRSM_RL);
#endif

swcShaveUnit_t listShaves[MAX_SHAVES] = {0, 1, 2, 3, 4, 5, 6, 7
#ifdef MYRIAD2
                                         , 8, 9, 10, 11
#endif
                                        };

u32 entryPointsSGEMM[MAX_SHAVES] =
{
   (u32)&BLISTest0_startSGEMM,
   (u32)&BLISTest1_startSGEMM,
   (u32)&BLISTest2_startSGEMM,
   (u32)&BLISTest3_startSGEMM,
   (u32)&BLISTest4_startSGEMM,
   (u32)&BLISTest5_startSGEMM,
   (u32)&BLISTest6_startSGEMM,
   (u32)&BLISTest7_startSGEMM
#ifdef MYRIAD2
   ,(u32)&BLISTest8_startSGEMM,
   (u32)&BLISTest9_startSGEMM,
   (u32)&BLISTest10_startSGEMM,
   (u32)&BLISTest11_startSGEMM
#endif
};

u32 entryPointsSGEMMTRSM_LL[MAX_SHAVES] =
{
   (u32)&BLISTest0_startSGEMMTRSM_LL,
   (u32)&BLISTest1_startSGEMMTRSM_LL,
   (u32)&BLISTest2_startSGEMMTRSM_LL,
   (u32)&BLISTest3_startSGEMMTRSM_LL,
   (u32)&BLISTest4_startSGEMMTRSM_LL,
   (u32)&BLISTest5_startSGEMMTRSM_LL,
   (u32)&BLISTest6_startSGEMMTRSM_LL,
   (u32)&BLISTest7_startSGEMMTRSM_LL
#ifdef MYRIAD2
   , (u32)&BLISTest8_startSGEMMTRSM_LL,
   (u32)&BLISTest9_startSGEMMTRSM_LL,
   (u32)&BLISTest10_startSGEMMTRSM_LL,
   (u32)&BLISTest11_startSGEMMTRSM_LL
#endif
};

u32 entryPointsSGEMMTRSM_LU[MAX_SHAVES] =
{
   (u32)&BLISTest0_startSGEMMTRSM_LU,
   (u32)&BLISTest1_startSGEMMTRSM_LU,
   (u32)&BLISTest2_startSGEMMTRSM_LU,
   (u32)&BLISTest3_startSGEMMTRSM_LU,
   (u32)&BLISTest4_startSGEMMTRSM_LU,
   (u32)&BLISTest5_startSGEMMTRSM_LU,
   (u32)&BLISTest6_startSGEMMTRSM_LU,
   (u32)&BLISTest7_startSGEMMTRSM_LU
#ifdef MYRIAD2
   , (u32)&BLISTest8_startSGEMMTRSM_LU,
   (u32)&BLISTest9_startSGEMMTRSM_LU,
   (u32)&BLISTest10_startSGEMMTRSM_LU,
   (u32)&BLISTest11_startSGEMMTRSM_LU
#endif
};

u32 entryPointsSGEMMTRSM_RU[MAX_SHAVES] =
{
   (u32)&BLISTest0_startSGEMMTRSM_RU,
   (u32)&BLISTest1_startSGEMMTRSM_RU,
   (u32)&BLISTest2_startSGEMMTRSM_RU,
   (u32)&BLISTest3_startSGEMMTRSM_RU,
   (u32)&BLISTest4_startSGEMMTRSM_RU,
   (u32)&BLISTest5_startSGEMMTRSM_RU,
   (u32)&BLISTest6_startSGEMMTRSM_RU,
   (u32)&BLISTest7_startSGEMMTRSM_RU
#ifdef MYRIAD2
   , (u32)&BLISTest8_startSGEMMTRSM_RU,
   (u32)&BLISTest9_startSGEMMTRSM_RU,
   (u32)&BLISTest10_startSGEMMTRSM_RU,
   (u32)&BLISTest11_startSGEMMTRSM_RU
#endif
};

u32 entryPointsSGEMMTRSM_RL[MAX_SHAVES] =
{
   (u32)&BLISTest0_startSGEMMTRSM_RL,
   (u32)&BLISTest1_startSGEMMTRSM_RL,
   (u32)&BLISTest2_startSGEMMTRSM_RL,
   (u32)&BLISTest3_startSGEMMTRSM_RL,
   (u32)&BLISTest4_startSGEMMTRSM_RL,
   (u32)&BLISTest5_startSGEMMTRSM_RL,
   (u32)&BLISTest6_startSGEMMTRSM_RL,
   (u32)&BLISTest7_startSGEMMTRSM_RL
#ifdef MYRIAD2
   , (u32)&BLISTest8_startSGEMMTRSM_RL,
   (u32)&BLISTest9_startSGEMMTRSM_RL,
   (u32)&BLISTest10_startSGEMMTRSM_RL,
   (u32)&BLISTest11_startSGEMMTRSM_RL
#endif
};

