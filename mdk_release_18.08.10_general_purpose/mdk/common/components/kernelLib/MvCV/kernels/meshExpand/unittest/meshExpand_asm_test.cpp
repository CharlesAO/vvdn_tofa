#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M2 (5.1)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#endif

#define OUT_PADDING 16
#define INPUT_PADDING 16
#define HEIGHT_PADDING 64

TestRunner meshExpandTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int meshExpandCycleCount;

void meshExpand_asm_test(unsigned char *output, unsigned char **input, unsigned int width, half** mesh, float my)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 1920 + INPUT_PADDING;
	meshExpandTestRunner.Init();
	meshExpandTestRunner.SetInput("input", input, width + INPUT_PADDING, maxWidth, 64, SHAVE0);
	meshExpandTestRunner.SetInput("mesh",  mesh,  width + INPUT_PADDING, maxWidth, 2, SHAVE0);
	meshExpandTestRunner.SetInput("width", width, SHAVE0);
	meshExpandTestRunner.SetInput("output", output, width + 2*OUT_PADDING, SHAVE0);
	meshExpandTestRunner.SetInput("my", my, SHAVE0);
	meshExpandTestRunner.GuardInsert(string("output"), SHAVE0, width + 2*OUT_PADDING, output);	
	meshExpandTestRunner.Run(SHAVE0);

	if(width >= 1280)
	{
		meshExpandCycleCount = meshExpandTestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(meshExpandCycleCount - 482) / (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	
	meshExpandTestRunner.GetOutput(string("output"), SHAVE0, width + 2*OUT_PADDING, output);	
	meshExpandTestRunner.GuardCheck(string("output"), SHAVE0, width + 2*OUT_PADDING, output);	
}
