#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (0.810937)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner whiteBalanceBayerGBRGTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int whiteBalanceBayerGBRGCycleCount;

void whiteBalanceBayerGBRG_asm_test(unsigned short **input, unsigned short awbCoef[3], unsigned short **out, unsigned short clamp[1], unsigned int width, unsigned char line)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920;
	whiteBalanceBayerGBRGTestRunner.Init();
	whiteBalanceBayerGBRGTestRunner.SetInput("input", input, width, maxWidth, 1, SHAVE0);
	whiteBalanceBayerGBRGTestRunner.SetInput("awbCoef", (unsigned short*)awbCoef, 3, SHAVE0);
	whiteBalanceBayerGBRGTestRunner.SetInput("clamp",   (unsigned short*)clamp, 1, SHAVE0);
	whiteBalanceBayerGBRGTestRunner.SetInput("width", width, SHAVE0);
	whiteBalanceBayerGBRGTestRunner.SetInput("line", line, SHAVE0);
	
	whiteBalanceBayerGBRGTestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, out);
	whiteBalanceBayerGBRGTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		whiteBalanceBayerGBRGCycleCount = whiteBalanceBayerGBRGTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(whiteBalanceBayerGBRGCycleCount - 11) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}	
	whiteBalanceBayerGBRGTestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, out);
	whiteBalanceBayerGBRGTestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, out);

}
