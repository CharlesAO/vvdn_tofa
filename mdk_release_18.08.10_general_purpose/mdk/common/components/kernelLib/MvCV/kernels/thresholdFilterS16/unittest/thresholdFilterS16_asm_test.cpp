#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>
#include <mv_types.h>

#define EXPECTED_CC (1.65)

TestRunner thresholdFilterS16TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int thresholdFilterS16CycleCount;

void thresholdFilterS16_asm_test(short *line, unsigned int threshold, unsigned int width, 
								 unsigned int *indexList, unsigned int *count)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	thresholdFilterS16TestRunner.Init();
	thresholdFilterS16TestRunner.SetInput("line", (unsigned char*)line, width*2 , SHAVE0);
	thresholdFilterS16TestRunner.SetInput("threshold", threshold, SHAVE0);
	thresholdFilterS16TestRunner.SetInput("width", width, SHAVE0);	
	 
	thresholdFilterS16TestRunner.GuardInsert("indexList", SHAVE0, width*4 ,(unsigned char*) indexList);
	thresholdFilterS16TestRunner.GuardInsert("count", SHAVE0, 4, (unsigned char*)count);
	thresholdFilterS16TestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		 thresholdFilterS16CycleCount = thresholdFilterS16TestRunner.GetVariableValue(std::string("cycleCount"));
		 functionInfo.AddCyclePerPixelInfo((float)(thresholdFilterS16CycleCount - 2) / width);
		 functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	thresholdFilterS16TestRunner.GetOutput("indexList", SHAVE0, width*4 ,(unsigned char*) indexList);
	thresholdFilterS16TestRunner.GuardCheck("indexList", SHAVE0, width*4 ,(unsigned char*) indexList);
	thresholdFilterS16TestRunner.GetOutput("count", SHAVE0, 4, (unsigned char*)count);
	thresholdFilterS16TestRunner.GuardCheck("count", SHAVE0, 4, (unsigned char*)count);

}
