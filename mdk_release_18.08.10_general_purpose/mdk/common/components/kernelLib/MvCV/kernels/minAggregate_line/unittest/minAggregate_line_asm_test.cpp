#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (10)
#define PADDING 16 

TestRunner mvcvMinAggregate_lineTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvMinAggregate_lineCycleCount;

void mvcvMinAggregate_line_asm_test(unsigned char *input1, unsigned char *input2,  unsigned char *output, unsigned int *penaltyP1,  unsigned int *penaltyP2, unsigned int width, unsigned char activateClamp, unsigned char direction, unsigned int disparities)
{

FunctionInfo& functionInfo = FunctionInfo::Instance();
	
	mvcvMinAggregate_lineTestRunner.Init();
	mvcvMinAggregate_lineTestRunner.SetInput("input1", input1, width*disparities, SHAVE0);
	mvcvMinAggregate_lineTestRunner.SetInput("input2", input2, width*disparities, SHAVE0);
	mvcvMinAggregate_lineTestRunner.SetInput("output", output, (width*disparities) + PADDING, SHAVE0);
	mvcvMinAggregate_lineTestRunner.SetInput("penaltyP1", penaltyP1, width, SHAVE0);
	mvcvMinAggregate_lineTestRunner.SetInput("penaltyP2", penaltyP2, width, SHAVE0);
	mvcvMinAggregate_lineTestRunner.SetInput("activateClamp", activateClamp, SHAVE0);
	mvcvMinAggregate_lineTestRunner.SetInput("direction", direction, SHAVE0);
	mvcvMinAggregate_lineTestRunner.SetInput("width", width, SHAVE0);
	mvcvMinAggregate_lineTestRunner.SetInput("disparities", disparities, SHAVE0);

	mvcvMinAggregate_lineTestRunner.GuardInsert("output", SHAVE0, (width*disparities) + PADDING, output);
	mvcvMinAggregate_lineTestRunner.Run(SHAVE0);

	if(width >= 64)
	{
	mvcvMinAggregate_lineCycleCount = (mvcvMinAggregate_lineTestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    	functionInfo.AddCyclePerPixelInfo((float)(mvcvMinAggregate_lineCycleCount)/ (float)(width * disparities));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	mvcvMinAggregate_lineTestRunner.GetOutput(string("output"), SHAVE0, (width*disparities) + PADDING, output);
	mvcvMinAggregate_lineTestRunner.GuardCheck(string("output"), SHAVE0, (width*disparities) + PADDING, output);
}
