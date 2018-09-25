#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (1.523438)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner whiteBalanceRGBTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int whiteBalanceRGBCycleCount;

void whiteBalanceRGB_asm_test(unsigned short **inputR, unsigned short **inputG, unsigned short **inputB, unsigned short awbCoef[3], unsigned short **Rout, unsigned short **Gout, unsigned short **Bout, unsigned short clamp[1], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	whiteBalanceRGBTestRunner.Init();
	whiteBalanceRGBTestRunner.SetInput("inputR", inputR, width, maxWidth, 1, SHAVE0);
	whiteBalanceRGBTestRunner.SetInput("inputG", inputG, width, maxWidth, 1, SHAVE0);
	whiteBalanceRGBTestRunner.SetInput("inputB", inputB, width, maxWidth, 1, SHAVE0);
	whiteBalanceRGBTestRunner.SetInput("awbCoef", (unsigned short*)awbCoef, 3, SHAVE0);
	whiteBalanceRGBTestRunner.SetInput("clamp",   (unsigned short*)clamp, 1, SHAVE0);
	whiteBalanceRGBTestRunner.SetInput("width", width, SHAVE0);
	
	whiteBalanceRGBTestRunner.GuardInsert("outputR", SHAVE0, width, maxWidth, 1, Rout);
	whiteBalanceRGBTestRunner.GuardInsert("outputG", SHAVE0, width, maxWidth, 1, Gout);
	whiteBalanceRGBTestRunner.GuardInsert("outputB", SHAVE0, width, maxWidth, 1, Bout);
	whiteBalanceRGBTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		whiteBalanceRGBCycleCount = whiteBalanceRGBTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(whiteBalanceRGBCycleCount - 27) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}	
	whiteBalanceRGBTestRunner.GetOutput("outputR", SHAVE0, width, maxWidth, 1, Rout);
	whiteBalanceRGBTestRunner.GuardCheck("outputR", SHAVE0, width, maxWidth, 1, Rout);
	whiteBalanceRGBTestRunner.GetOutput("outputG", SHAVE0, width, maxWidth, 1, Gout);
	whiteBalanceRGBTestRunner.GuardCheck("outputG", SHAVE0, width, maxWidth, 1, Gout);
	whiteBalanceRGBTestRunner.GetOutput("outputB", SHAVE0, width, maxWidth, 1, Bout);
	whiteBalanceRGBTestRunner.GuardCheck("outputB", SHAVE0, width, maxWidth, 1, Bout);
}
