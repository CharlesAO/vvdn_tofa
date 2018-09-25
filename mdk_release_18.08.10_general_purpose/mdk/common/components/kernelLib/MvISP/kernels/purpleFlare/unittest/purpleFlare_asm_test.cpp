#include <TestRunner.h>
#include <moviDebugDll.h>
#include <FunctionInfo.h>
#include <cstdio>

#define EXPECTED_CC_M1 (0)
#define EXPECTED_CC_M2 (4.546875)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner appTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int PurpleFlareCycleCount;

void purpleFlare_asm_test(unsigned short *inGreen, unsigned short *blue[3], unsigned short *outBlue, unsigned char strength, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	const unsigned int PADDING = 8;
	unsigned int maxWidth = 1920 + PADDING;
	appTestRunner.Init();
	appTestRunner.SetInput("inputGreen", inGreen, width + PADDING, SHAVE0);
	appTestRunner.SetInput("inputBlue", blue, width + PADDING, maxWidth, 3, SHAVE0);
	appTestRunner.SetInput("strength", strength, SHAVE0);
	appTestRunner.SetInput("width", (unsigned int) width, SHAVE0);
	
	appTestRunner.GuardInsert(string("output"), SHAVE0, width, (unsigned short*)outBlue);
	appTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
	PurpleFlareCycleCount = appTestRunner.GetVariableValue("cycleCount");
	functionInfo.AddCyclePerPixelInfo((float)(PurpleFlareCycleCount - 12)/ (float)width);
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	appTestRunner.GetOutput(string("output"), SHAVE0, width, (unsigned short*)outBlue);
	appTestRunner.GuardCheck(string("output"), SHAVE0, width, (unsigned short*)outBlue);
}
