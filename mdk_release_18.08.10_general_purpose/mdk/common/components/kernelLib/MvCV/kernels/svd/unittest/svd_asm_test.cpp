#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (36000)
#define TEST_RANK 3

TestRunner SVDTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int SVDCycleCount;

void svd_asm_test(float **A, float *S, float **V, int rank)

{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	SVDTestRunner.Init();
	SVDTestRunner.SetInput(	"input_A",(unsigned char**)  A, TEST_RANK*TEST_RANK*4,TEST_RANK*4,TEST_RANK, SHAVE0);
	SVDTestRunner.SetInput(	"rank", rank, SHAVE0);
 	SVDTestRunner.GuardInsert("inS", SHAVE0, TEST_RANK*4, (unsigned char*) S);
 	SVDTestRunner.GuardInsert("input_V", SHAVE0, TEST_RANK*4,TEST_RANK*4,TEST_RANK,(unsigned char**) V);
 	SVDTestRunner.GuardInsert("input_A", SHAVE0, TEST_RANK*4,TEST_RANK*4,TEST_RANK,(unsigned char**) A);
 	SVDTestRunner.Run(SHAVE0);

	SVDCycleCount = (SVDTestRunner.GetVariableValue(std::string("cycleCount")));
    functionInfo.AddCyclePerPixelInfo((float)(SVDCycleCount-5));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	 SVDTestRunner.GetOutput("inS", SHAVE0, TEST_RANK*4, (unsigned char*) S);
	 SVDTestRunner.GuardCheck("inS", SHAVE0, TEST_RANK*4, (unsigned char*) S);
	 SVDTestRunner.GetOutput("input_V", SHAVE0, TEST_RANK*4,TEST_RANK*4,TEST_RANK,(unsigned char**) V);
	 SVDTestRunner.GuardCheck("input_V", SHAVE0, TEST_RANK*4,TEST_RANK*4,TEST_RANK,(unsigned char**) V);
     SVDTestRunner.GetOutput("input_A", SHAVE0, TEST_RANK*4,TEST_RANK*4,TEST_RANK,(unsigned char**) A);
     SVDTestRunner.GuardCheck("input_A", SHAVE0, TEST_RANK*4,TEST_RANK*4,TEST_RANK,(unsigned char**) A);
}
