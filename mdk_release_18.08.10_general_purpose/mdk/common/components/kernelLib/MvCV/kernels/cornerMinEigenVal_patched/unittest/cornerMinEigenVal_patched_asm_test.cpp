#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (139.00)
#define EXPECTED_CC_M2 (175)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner CornerMinEigenVal_patchedTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int CornerMinEigenVal_patchedCycleCount;

void CornerMinEigenVal_patched_asm_test(unsigned char **input_buffer, int posx, unsigned char *out_pix, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	CornerMinEigenVal_patchedTestRunner.Init();
	//add padding values at the end of each line
	CornerMinEigenVal_patchedTestRunner.SetInput("input", input_buffer, width + padding, maxWidth, 5, SHAVE0);
	CornerMinEigenVal_patchedTestRunner.SetInput("width", (unsigned int)width, SHAVE0);
	CornerMinEigenVal_patchedTestRunner.SetInput("posx",  (unsigned int)posx, SHAVE0);

	CornerMinEigenVal_patchedTestRunner.GuardInsert(string("output"), SHAVE0, 1, out_pix);
	CornerMinEigenVal_patchedTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		CornerMinEigenVal_patchedCycleCount = CornerMinEigenVal_patchedTestRunner.GetVariableValue("cycleCount");
		//value 157 substracted from boxFilterCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//running the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(CornerMinEigenVal_patchedCycleCount - 33));
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	CornerMinEigenVal_patchedTestRunner.GetOutput(string("output"), SHAVE0, 1, out_pix);
	CornerMinEigenVal_patchedTestRunner.GuardCheck(string("output"), SHAVE0, 1, out_pix);
	
}
