#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (44.04)
#define EXPECTED_CC_M2 (38.151306)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner CornerMinEigenVal_fp32TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int CornerMinEigenVal_fp32CycleCount;

void CornerMinEigenVal_fp32_asm_test(unsigned char** in, float** out, float* buffer, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;
    buffer+=1;
	CornerMinEigenVal_fp32TestRunner.Init();
	//add padding values at the end of each line
	CornerMinEigenVal_fp32TestRunner.SetInput("input", in, width + padding, maxWidth, 5, SHAVE0);
	CornerMinEigenVal_fp32TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	CornerMinEigenVal_fp32TestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	CornerMinEigenVal_fp32TestRunner.Run(SHAVE0);
	if(width >= 320)
	{
		CornerMinEigenVal_fp32CycleCount = CornerMinEigenVal_fp32TestRunner.GetVariableValue("cycleCount");
		//value 157 substracted from boxFilterCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//running the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(CornerMinEigenVal_fp32CycleCount - 55)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	CornerMinEigenVal_fp32TestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
	CornerMinEigenVal_fp32TestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);

}
