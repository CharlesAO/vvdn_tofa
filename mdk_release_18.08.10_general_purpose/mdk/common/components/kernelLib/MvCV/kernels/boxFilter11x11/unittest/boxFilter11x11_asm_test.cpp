#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (16.51)
#define EXPECTED_CC_M2 (2.890556)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner boxFilter11x11TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int boxFilter11x11CycleCount;

void boxfilter11x11_asm_test(unsigned char** in, unsigned char** out, unsigned char normalize, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	boxFilter11x11TestRunner.Init();
	//add padding values at the end of each line
	boxFilter11x11TestRunner.SetInput("input", in, width + padding, maxWidth, 11, SHAVE0);
	boxFilter11x11TestRunner.SetInput("normalize", normalize, SHAVE0);
	boxFilter11x11TestRunner.SetInput("width", width, SHAVE0);

	boxFilter11x11TestRunner.GuardInsert(string("output"), SHAVE0, width * 2, maxWidth, 1, out);
	boxFilter11x11TestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	boxFilter11x11TestRunner.Run(SHAVE0);
if (width >=1280)
{
	boxFilter11x11CycleCount = boxFilter11x11TestRunner.GetVariableValue("cycleCount");
	//value 65 substracted from boxFilterCycleCount is the value of measured cycles
	//for computations not included in boxfilter_asm function. It was obtained by
	//running the application without the function call
	functionInfo.AddCyclePerPixelInfo((float)(boxFilter11x11CycleCount - 65)/ (float)width);	
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
}
	if(normalize != 1)
	{
		//for unnormalized variant the output values are 16bits, so the output size is width * 2
		boxFilter11x11TestRunner.GetOutput(string("output"), SHAVE0, width * 2, maxWidth, 1, out);
		boxFilter11x11TestRunner.GuardCheck(string("output"), SHAVE0, width * 2, maxWidth, 1, out);
	}
	else {
		boxFilter11x11TestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
		boxFilter11x11TestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
	}
}
