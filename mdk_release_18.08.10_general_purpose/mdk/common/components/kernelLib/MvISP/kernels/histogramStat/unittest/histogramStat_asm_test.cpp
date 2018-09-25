#include "TestRunner.h"
#include "RandomGenerator.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC_M1 (999)
#define EXPECTED_CC_M2 (9.2)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner testRunnerHistogramStat(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int histogramStatCycleCount;

void histogramStat_asm_test(unsigned char *inputR, unsigned char *inputG, unsigned char *inputB, unsigned int *histR, unsigned int *histG, unsigned int *histB, unsigned int width, unsigned int step)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();

	unsigned int maxWidth = 1920;
	unsigned int histSize = 64; 
		
	
	testRunnerHistogramStat.Init();
	testRunnerHistogramStat.SetInput("inputR", inputR, maxWidth, SHAVE0);
	testRunnerHistogramStat.SetInput("inputG", inputG, width, SHAVE0);
	testRunnerHistogramStat.SetInput("inputB", inputB, width, SHAVE0);
	testRunnerHistogramStat.SetInput("histR", (float*)histR, histSize, SHAVE0);
	testRunnerHistogramStat.SetInput("histG", (float*)histG, histSize, SHAVE0);
	testRunnerHistogramStat.SetInput("histB", (float*)histB, histSize, SHAVE0);
	testRunnerHistogramStat.SetInput("width", width, SHAVE0);
	testRunnerHistogramStat.SetInput("step", step, SHAVE0);
	
	testRunnerHistogramStat.GuardInsert("histR", SHAVE0, histSize, (float*)histR);
	testRunnerHistogramStat.GuardInsert("histG", SHAVE0, histSize, (float*)histG);
	testRunnerHistogramStat.GuardInsert("histB", SHAVE0, histSize, (float*)histB);
	testRunnerHistogramStat.Run(SHAVE0);
	if(width >= 1280)
	{
		histogramStatCycleCount = testRunnerHistogramStat.GetVariableValue("cycleCount");
		functionInfo.AddCyclePerPixelInfo((float)(histogramStatCycleCount - 2)/ (float)width);
		functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	testRunnerHistogramStat.GetOutput("histR", SHAVE0, histSize, (float*)histR);
	testRunnerHistogramStat.GuardCheck("histR", SHAVE0, histSize, (float*)histR);
	testRunnerHistogramStat.GetOutput("histG", SHAVE0, histSize, (float*)histG);
	testRunnerHistogramStat.GuardCheck("histG", SHAVE0, histSize, (float*)histG);
	testRunnerHistogramStat.GetOutput("histB", SHAVE0, histSize, (float*)histB);
	testRunnerHistogramStat.GuardCheck("histB", SHAVE0, histSize, (float*)histB);
}

