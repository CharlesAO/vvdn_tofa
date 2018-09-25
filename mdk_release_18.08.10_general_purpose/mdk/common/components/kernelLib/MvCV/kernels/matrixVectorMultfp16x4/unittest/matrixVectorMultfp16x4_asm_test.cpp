#include "mv_types.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>
#include "matrixVectorMultfp16x4_asm_test.h"

#define EXPECTED_CC 0.56

TestRunner matrixVectorMultfp16x4TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int matrixVectorMultfp16x4CycleCount;

void matrixVectorMultfp16x4_asm_test(half **input1, half* input2, unsigned int width, half* output)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();

    unsigned int maxWidth = TEST_ARRAY_SIZE;
	matrixVectorMultfp16x4TestRunner.Init();
	matrixVectorMultfp16x4TestRunner.SetInput("inputA", input1, width, maxWidth, 4, SHAVE0);
	matrixVectorMultfp16x4TestRunner.SetInput("inputB", input2, width, SHAVE0);
	matrixVectorMultfp16x4TestRunner.SetInput("num_elements", width, SHAVE0);
	matrixVectorMultfp16x4TestRunner.SetInput("outVector", output, 4, SHAVE0);
	matrixVectorMultfp16x4TestRunner.GuardInsert("outVector", SHAVE0, 4, output);
	matrixVectorMultfp16x4TestRunner.Run(SHAVE0);
	matrixVectorMultfp16x4CycleCount = matrixVectorMultfp16x4TestRunner.GetVariableValue("cycleCount");
    functionInfo.AddCyclePerPixelInfo((float)(matrixVectorMultfp16x4CycleCount - 89) / (float)width);
    functionInfo.setExpectedCycles((float)EXPECTED_CC);
	matrixVectorMultfp16x4TestRunner.GetOutput("outVector", SHAVE0, 4, output);
	matrixVectorMultfp16x4TestRunner.GuardCheck("outVector", SHAVE0, 4, output);
}
