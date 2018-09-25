
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M2 (31.304688)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#endif

TestRunner ssdPointLine7x7U8U32TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int ssdPointLine7x7U8U32CycleCount;

void ssdPointLine7x7U8U32_asm_test(unsigned char** in1, unsigned char** in2, unsigned int** out, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	ssdPointLine7x7U8U32TestRunner.Init();
	//add padding values at the end of each line
	ssdPointLine7x7U8U32TestRunner.SetInput("input1", in1, width + padding, maxWidth, 7, SHAVE0);
	ssdPointLine7x7U8U32TestRunner.SetInput("input2", in2, width + padding, maxWidth, 7, SHAVE0);
	ssdPointLine7x7U8U32TestRunner.SetInput("width", (unsigned int)width, SHAVE0);
	ssdPointLine7x7U8U32TestRunner.SetInput("output", (unsigned char**)out, (width + 16) * 4, maxWidth, 1, SHAVE0);
	
	ssdPointLine7x7U8U32TestRunner.GuardInsert("output", SHAVE0, (width + 16) * 4, maxWidth, 1, (unsigned char**)out);
	ssdPointLine7x7U8U32TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		ssdPointLine7x7U8U32CycleCount = ssdPointLine7x7U8U32TestRunner.GetVariableValue("cycleCount");
		//value 157 substracted from boxFilterCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//running the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(ssdPointLine7x7U8U32CycleCount - 21)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	ssdPointLine7x7U8U32TestRunner.GetOutput("output", SHAVE0, (width + 16) * 4, maxWidth, 1, (unsigned char**)out);
	ssdPointLine7x7U8U32TestRunner.GuardCheck("output", SHAVE0, (width + 16) * 4, maxWidth, 1, (unsigned char**)out);

}
