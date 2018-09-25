#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (3.40)
#define EXPECTED_CC_M2 (2.53)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner sLaplacian5x5Fp16ToFp16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int sLaplacian5x5Fp16ToFp16CycleCount;

void sLaplacian5x5Fp16ToFp16_asm_test(half** in,half** out, unsigned int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	// for calculation of the last values on the line a padding of kenrnel_size - 1 is needed
	const unsigned int padding = 16;
	// maxWidth - the maximum image width for which was allocated space in the board application;
	// it is needed to permit proper memory initialization for line sizes lower than it
	unsigned int maxWidth = 1920 + padding;

	sLaplacian5x5Fp16ToFp16TestRunner.Init();
	//add padding values at the end of each line
	sLaplacian5x5Fp16ToFp16TestRunner.SetInput("input", in, width + padding, maxWidth, 5, SHAVE0);
	
	sLaplacian5x5Fp16ToFp16TestRunner.SetInput("width", (unsigned int)width, SHAVE0);

	sLaplacian5x5Fp16ToFp16TestRunner.GuardInsert("output", SHAVE0, width, maxWidth, 1, out);
	sLaplacian5x5Fp16ToFp16TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		sLaplacian5x5Fp16ToFp16CycleCount = sLaplacian5x5Fp16ToFp16TestRunner.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(sLaplacian5x5Fp16ToFp16CycleCount - 14)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	sLaplacian5x5Fp16ToFp16TestRunner.GetOutput("output", SHAVE0, width, maxWidth, 1, out);
	sLaplacian5x5Fp16ToFp16TestRunner.GuardCheck("output", SHAVE0, width, maxWidth, 1, out);
}
