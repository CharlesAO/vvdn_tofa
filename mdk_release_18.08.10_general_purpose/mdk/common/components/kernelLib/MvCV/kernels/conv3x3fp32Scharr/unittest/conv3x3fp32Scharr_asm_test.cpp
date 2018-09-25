#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>
#include <mv_types.h>

#define EXPECTED_CC_M1 (2.64)
#define EXPECTED_CC_M2 (6.0)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner conv3x3fp32ScharrTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int conv3x3fp32ScharrCycleCount;

void conv3x3fp32Scharr_asm_test(unsigned char* src, float* dstX, float* dstY, float* smooth_k, int height, int width)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	conv3x3fp32ScharrTestRunner.Init();
	conv3x3fp32ScharrTestRunner.SetInput("src", src, width*height, SHAVE0);
	conv3x3fp32ScharrTestRunner.SetInput("smooth_k", smooth_k, 2, SHAVE0);
	conv3x3fp32ScharrTestRunner.SetInput("height", height, SHAVE0);
	conv3x3fp32ScharrTestRunner.SetInput("width", width, SHAVE0);

	conv3x3fp32ScharrTestRunner.Run(SHAVE0);

	conv3x3fp32ScharrCycleCount = conv3x3fp32ScharrTestRunner.GetVariableValue("cycleCount");
	functionInfo.AddCyclePerPixelInfo((float)(conv3x3fp32ScharrCycleCount - 2)/(width-2)/(height-2));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	conv3x3fp32ScharrTestRunner.GetOutput("dstX", SHAVE0, (width-2)*(height-2), dstX);
	conv3x3fp32ScharrTestRunner.GetOutput("dstY", SHAVE0, (width-2)*(height-2), dstY);

}
