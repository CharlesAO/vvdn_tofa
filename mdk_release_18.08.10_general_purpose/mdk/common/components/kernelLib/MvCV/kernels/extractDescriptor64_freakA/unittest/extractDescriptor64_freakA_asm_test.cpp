///
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (321)


#ifdef MYRIAD2
#define EXPECTED_CC 817
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif


TestRunner extractDescriptor64_freakATestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int extractDescriptor64_freakACycleCount;

void extractDescriptor64_freakA_asm_test(unsigned short *in_pixel_array,  unsigned char *out_descriptor)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();


	extractDescriptor64_freakATestRunner.Init();
	extractDescriptor64_freakATestRunner.SetInput("input", in_pixel_array, 64 , SHAVE0);

	extractDescriptor64_freakATestRunner.GuardInsert(string("output"), SHAVE0, 64, out_descriptor);
	extractDescriptor64_freakATestRunner.Run(SHAVE0);

	extractDescriptor64_freakACycleCount = extractDescriptor64_freakATestRunner.GetVariableValue("cycleCount");

	functionInfo.AddCyclePerPixelInfo((float)(extractDescriptor64_freakACycleCount-3));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	extractDescriptor64_freakATestRunner.GetOutput(string("output"), SHAVE0, 64, out_descriptor);
	extractDescriptor64_freakATestRunner.GuardCheck(string("output"), SHAVE0, 64, out_descriptor);


}
