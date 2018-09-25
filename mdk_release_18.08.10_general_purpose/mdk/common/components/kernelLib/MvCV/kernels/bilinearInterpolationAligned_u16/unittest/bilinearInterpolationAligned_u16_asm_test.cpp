#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>
#include <mv_types.h>
#include "bilinearInterpolationAligned_u16_asm_test.h"

#define EXPECTED_CC_M1 (999)
#define EXPECTED_CC_M2 (3.253110)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif


TestRunner bilinearInterpolation16uTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int bilinearInterpolation16uCycleCount;

void bilinearInterpolationAligned16u_asm_test(unsigned char* src, int src_step, unsigned char* dst, int dst_step, unsigned int* win_size, float* center)
{
	int transferSize = MAX_SIZE;

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	bilinearInterpolation16uTestRunner.Init();
	bilinearInterpolation16uTestRunner.SetInput("src", src, transferSize , SHAVE0);
	bilinearInterpolation16uTestRunner.SetInput("dst", dst, transferSize, SHAVE0);
	bilinearInterpolation16uTestRunner.SetInput("src_step", src_step, SHAVE0);
	bilinearInterpolation16uTestRunner.SetInput("dst_step", dst_step, SHAVE0);
	bilinearInterpolation16uTestRunner.SetInput("win_size_s32", win_size, 2, SHAVE0);
	bilinearInterpolation16uTestRunner.SetInput("center_fp32", (float*)center, 2, SHAVE0);



	bilinearInterpolation16uTestRunner.GuardInsert("dst", SHAVE0, transferSize, dst);
	bilinearInterpolation16uTestRunner.Run(SHAVE0);
	// if(width >= 1280)
	// {
		 bilinearInterpolation16uCycleCount = bilinearInterpolation16uTestRunner.GetVariableValue("cycleCount");
		 functionInfo.AddCyclePerPixelInfo((float)(bilinearInterpolation16uCycleCount - 0) / (win_size[0] * win_size[1]));
		 functionInfo.setExpectedCycles((float)EXPECTED_CC);
	// }
	bilinearInterpolation16uTestRunner.GetOutput("dst", SHAVE0, transferSize, dst);
	bilinearInterpolation16uTestRunner.GuardCheck("dst", SHAVE0, transferSize, dst);

}
