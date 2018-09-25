#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (3.06)
#define EXPECTED_CC_M2 (6.4)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner monoImbalanceTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int monoImbalanceCycleCount;

void MonoImbalance_asm_test(u16** in, half** out,int thrDark, int thrBright, int inputBits, u32 width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	// for calculation of the last values on the line a padding is needed
	const unsigned int padding = 8;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	monoImbalanceTestRunner.Init();
	//add padding values at the end of each line
	monoImbalanceTestRunner.SetInput("input", in, width + padding, maxWidth, 3, SHAVE0);
	monoImbalanceTestRunner.SetInput("thrDark", (unsigned int)thrDark, SHAVE0);
	monoImbalanceTestRunner.SetInput("thrBright", (unsigned int)thrBright,SHAVE0);
	monoImbalanceTestRunner.SetInput("inputBits", (unsigned int)inputBits,SHAVE0);
	monoImbalanceTestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	monoImbalanceTestRunner.GuardInsert(string("output"), SHAVE0, width, maxWidth, 1, out);
	monoImbalanceTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		monoImbalanceCycleCount = monoImbalanceTestRunner.GetVariableValue("cycleCount");
		//the value substracted from boxFilterCycleCount is the value of measured cycles
		//for computations not included in boxfilter_asm function. It was obtained by
		//running the application without the function call
		functionInfo.AddCyclePerPixelInfo((float)(monoImbalanceCycleCount - 65)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
		
	}
	
	monoImbalanceTestRunner.GetOutput(string("output"), SHAVE0, width, maxWidth, 1, out);
	monoImbalanceTestRunner.GuardCheck(string("output"), SHAVE0, width, maxWidth, 1, out);
}
