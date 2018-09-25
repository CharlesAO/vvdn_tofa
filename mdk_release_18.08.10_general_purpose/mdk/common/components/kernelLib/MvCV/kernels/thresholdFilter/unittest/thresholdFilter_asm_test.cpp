#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>
#include <mv_types.h>

#define EXPECTED_CC_M1 (999)
#define EXPECTED_CC_M2 (2.53)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner thresholdFilterTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int thresholdFilterCycleCount;

void thresholdFilter_asm_test(float *cornerScores, float threshold, unsigned int width, 
								unsigned int posOffset, unsigned short *candPos, unsigned int *candCount)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	thresholdFilterTestRunner.Init();
	thresholdFilterTestRunner.SetInput("cornerScores", cornerScores, width + posOffset , SHAVE0);
	thresholdFilterTestRunner.SetInput("threshold", threshold, SHAVE0);
	thresholdFilterTestRunner.SetInput("width", width, SHAVE0);
	thresholdFilterTestRunner.SetInput("posOffset", posOffset, SHAVE0);
	thresholdFilterTestRunner.SetInput("candPos", candPos, width + posOffset , SHAVE0);
	thresholdFilterTestRunner.SetInput("candCount", cornerScores, 1 , SHAVE0);


	thresholdFilterTestRunner.GuardInsert("candPos", SHAVE0, width + posOffset, candPos);
	thresholdFilterTestRunner.GuardInsert("candCount", SHAVE0, 1, (float*)candCount);
	thresholdFilterTestRunner.Run(SHAVE0);
	if(width >= 1280)
	{
		 thresholdFilterCycleCount = thresholdFilterTestRunner.GetVariableValue("cycleCount");
		 functionInfo.AddCyclePerPixelInfo((float)(thresholdFilterCycleCount - 2) / width);
		 functionInfo.setExpectedCycles((float)EXPECTED_CC);
	}
	thresholdFilterTestRunner.GetOutput("candPos", SHAVE0, width + posOffset, candPos);
	thresholdFilterTestRunner.GuardCheck("candPos", SHAVE0, width + posOffset, candPos);
	thresholdFilterTestRunner.GetOutput("candCount", SHAVE0, 1, (float*)candCount);
	thresholdFilterTestRunner.GuardCheck("candCount", SHAVE0, 1, (float*)candCount);

}
