#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>
#include <mv_types.h>
#include "bilinearInterpolation_asm_test.h"

#define EXPECTED_CC_M1 (999)
#define EXPECTED_CC_M2 (2.3456789)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif


TestRunner bilinearInterpolationTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int bilinearInterpolationCycleCount;

void bilinearInterpolation_asm_test(u8 * line1,
                    u8 * line2,
                    u8 * dst,
                    u16 * precalcPos,
                    u16 * lut,
                    int stopW,
                    int godValue)
{
	int transferSize = MAX_SIZE;

	FunctionInfo& functionInfo = FunctionInfo::Instance();
	bilinearInterpolationTestRunner.Init();
	bilinearInterpolationTestRunner.SetInput("line1", line1, transferSize , SHAVE0);
    bilinearInterpolationTestRunner.SetInput("line2", line2, transferSize , SHAVE0);
	bilinearInterpolationTestRunner.SetInput("dst", dst, transferSize, SHAVE0);
	bilinearInterpolationTestRunner.SetInput("precalcPos", precalcPos, 10, SHAVE0);
	bilinearInterpolationTestRunner.SetInput("lut", lut, 40, SHAVE0);
	bilinearInterpolationTestRunner.SetInput("stopW", stopW, SHAVE0);
	bilinearInterpolationTestRunner.SetInput("godValue", godValue, SHAVE0);



	bilinearInterpolationTestRunner.GuardInsert("dst", SHAVE0, transferSize, dst);
	bilinearInterpolationTestRunner.Run(SHAVE0);
	// if(width >= 1280)
	// {
		 bilinearInterpolationCycleCount = bilinearInterpolationTestRunner.GetVariableValue("cycleCount");
		 functionInfo.AddCyclePerPixelInfo((float)(bilinearInterpolationCycleCount) / stopW);
		 functionInfo.setExpectedCycles((float)EXPECTED_CC);
	// }
	bilinearInterpolationTestRunner.GetOutput("dst", SHAVE0, transferSize, dst);
	bilinearInterpolationTestRunner.GuardCheck("dst", SHAVE0, transferSize, dst);

}
