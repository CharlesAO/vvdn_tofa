#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC_M2 (3.58)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#endif


TestRunner mvispGenChromaSSTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvispGenChromaSSCycleCount;

void mvispGenChromaSS_asm_test(unsigned char* outC[3], unsigned char** inRGB, int eps, float chromaScale[3], unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// to calculate the last values on the line a padding of kernel_size - 1 is needed
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for linesizes lower than it
	unsigned int maxWidth = 1920;

	mvispGenChromaSSTestRunner.Init();
	mvispGenChromaSSTestRunner.SetInput("input", inRGB, width*2, maxWidth, 6, SHAVE0);
	mvispGenChromaSSTestRunner.SetInput("width", (unsigned int)width, SHAVE0);
	mvispGenChromaSSTestRunner.SetInput("eps", eps, SHAVE0);
	mvispGenChromaSSTestRunner.SetInput("chromaScale", chromaScale, 3, SHAVE0);

	mvispGenChromaSSTestRunner.GuardInsert(string("output"), SHAVE0, width*2, maxWidth, 3, outC);
	mvispGenChromaSSTestRunner.Run(SHAVE0);
	if(width >= 640)
	{
		mvispGenChromaSSCycleCount = mvispGenChromaSSTestRunner.GetVariableValue("cycleCount");
		//value 157 substracted from boxFilterCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//runnung the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(mvispGenChromaSSCycleCount-22)/ ((float)width));
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}

		mvispGenChromaSSTestRunner.GetOutput(string("output"), SHAVE0, width*2, maxWidth, 3, outC);
		mvispGenChromaSSTestRunner.GuardCheck(string("output"), SHAVE0, width*2, maxWidth, 3, outC);


}
