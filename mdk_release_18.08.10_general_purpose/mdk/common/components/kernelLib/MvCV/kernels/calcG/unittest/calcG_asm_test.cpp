#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>
#include <mv_types.h>

#define EXPECTED_CC_M1 (999)
#define EXPECTED_CC_M2 (10.5)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner calcGTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int calcGCycleCount;

void calcG_asm_test(float* ix, unsigned int* isz, unsigned int* minI, float* iy, unsigned int* jsz, unsigned int* minJ, unsigned int* g)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	calcGTestRunner.Init();
	calcGTestRunner.SetInput("isz_u32", isz, 2, SHAVE0);
	calcGTestRunner.SetInput("jsz_u32", jsz, 2, SHAVE0);
	calcGTestRunner.SetInput("minI_u32", minI, 2, SHAVE0);
	calcGTestRunner.SetInput("minJ_u32", minJ, 2, SHAVE0);	
	calcGTestRunner.SetInput("ix", ix, maxWidth, SHAVE0);
	calcGTestRunner.SetInput("iy", iy, maxWidth, SHAVE0);

	calcGTestRunner.GuardInsert("g_u32", SHAVE0, 4, (float*)g);
	calcGTestRunner.Run(SHAVE0);
	calcGCycleCount = calcGTestRunner.GetVariableValue("cycleCount");
	functionInfo.AddCyclePerPixelInfo((float)(calcGCycleCount - 26) / (float)(jsz[0]*jsz[1]));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	
	calcGTestRunner.GetOutput("g_u32", SHAVE0, 4, (float*)g);
	calcGTestRunner.GuardCheck("g_u32", SHAVE0, 4, (float*)g);

}
