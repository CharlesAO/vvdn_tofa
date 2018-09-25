///
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC (0.686833)


TestRunner convertYUV400ToYUV422TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int convertYUV400ToYUV422CycleCount;

void convertYUV400ToYUV422_asm_test(unsigned char* input, unsigned short* output, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	const unsigned int padding = 16; 


	convertYUV400ToYUV422TestRunner.Init();
	convertYUV400ToYUV422TestRunner.SetInput("input", input, width , SHAVE0);
	convertYUV400ToYUV422TestRunner.SetInput("output", output, width +  padding, SHAVE0);
    
	convertYUV400ToYUV422TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	convertYUV400ToYUV422TestRunner.GuardInsert(string("output"), SHAVE0, width + padding, output);
	convertYUV400ToYUV422TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		convertYUV400ToYUV422CycleCount = convertYUV400ToYUV422TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(convertYUV400ToYUV422CycleCount-2)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}

	convertYUV400ToYUV422TestRunner.GetOutput(string("output"), SHAVE0, width + padding, output);
	convertYUV400ToYUV422TestRunner.GuardCheck(string("output"), SHAVE0, width + padding, output);

}
