#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (4.52)
#define EXPECTED_CC_M2 (1.443612)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner mvcvHammingDistanceTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvHammingDistanceCycleCount;

void mvcvHammingDistance_asm_test(unsigned char *d1, unsigned char *d2, int array_size, int descriptor_size, unsigned short *distances)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();

	mvcvHammingDistanceTestRunner.Init();
	mvcvHammingDistanceTestRunner.SetInput("d1", d1, descriptor_size, SHAVE0);
	mvcvHammingDistanceTestRunner.SetInput("d2", d2, descriptor_size * array_size, SHAVE0);
    mvcvHammingDistanceTestRunner.SetInput("array_size", (unsigned int)array_size, SHAVE0);
    mvcvHammingDistanceTestRunner.SetInput("descriptor_size", (unsigned int)descriptor_size, SHAVE0);

	mvcvHammingDistanceTestRunner.GuardInsert(string("distances"), SHAVE0, array_size, distances);
	mvcvHammingDistanceTestRunner.Run(SHAVE0);
	mvcvHammingDistanceCycleCount = mvcvHammingDistanceTestRunner.GetVariableValue(std::string("cycleCount"));
    functionInfo.AddCyclePerPixelInfo((float)mvcvHammingDistanceCycleCount/ (float)(array_size * descriptor_size));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	
	mvcvHammingDistanceTestRunner.GetOutput(string("distances"), SHAVE0, array_size, distances);
	mvcvHammingDistanceTestRunner.GuardCheck(string("distances"), SHAVE0, array_size, distances);
}
