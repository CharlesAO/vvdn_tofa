///
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (295)

#ifdef MYRIAD2
#define EXPECTED_CC 736
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif


TestRunner extractDescriptor64_briskTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int extractDescriptor64_briskCycleCount;

void extractDescriptor64_brisk_asm_test(unsigned short *in_pixel_array,  unsigned char *out_descriptor)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();


	extractDescriptor64_briskTestRunner.Init();
	extractDescriptor64_briskTestRunner.SetInput("input", in_pixel_array, 64 , SHAVE0);

	extractDescriptor64_briskTestRunner.GuardInsert(string("output"), SHAVE0, 64, out_descriptor);
	extractDescriptor64_briskTestRunner.Run(SHAVE0);

	extractDescriptor64_briskCycleCount = extractDescriptor64_briskTestRunner.GetVariableValue("cycleCount");

	functionInfo.AddCyclePerPixelInfo((float)(extractDescriptor64_briskCycleCount-3));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	extractDescriptor64_briskTestRunner.GetOutput(string("output"), SHAVE0, 64, out_descriptor);
	extractDescriptor64_briskTestRunner.GuardCheck(string("output"), SHAVE0, 64, out_descriptor);


}
