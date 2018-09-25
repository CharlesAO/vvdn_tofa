#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC (40)

TestRunner mvcvSmoothed_intensityTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int mvcvSmoothed_intensityCycleCount;

void mvcvSmoothed_intensity_asm_test(float kp_x, float kp_y, u8* integral, int integral_stride, float* pattern_pnt_lst, int pattern_pnt_sz, unsigned short* intensities, unsigned int width, unsigned int height)
{

    FunctionInfo& functionInfo = FunctionInfo::Instance();
    mvcvSmoothed_intensityTestRunner.Init();
    mvcvSmoothed_intensityTestRunner.SetInput("kp_x", kp_x, SHAVE0);
    mvcvSmoothed_intensityTestRunner.SetInput("kp_y", kp_y, SHAVE0);
    mvcvSmoothed_intensityTestRunner.SetInput("integral", integral, width * (height+4) * 4, SHAVE0);
    mvcvSmoothed_intensityTestRunner.SetInput("intensities", intensities, pattern_pnt_sz, SHAVE0);
    mvcvSmoothed_intensityTestRunner.SetInput("integral_stride", integral_stride, SHAVE0);
    mvcvSmoothed_intensityTestRunner.SetInput("pattern_pnt_lst", pattern_pnt_lst, pattern_pnt_sz * 3, SHAVE0);
    mvcvSmoothed_intensityTestRunner.SetInput("pattern_pnt_sz", pattern_pnt_sz, SHAVE0);

    mvcvSmoothed_intensityTestRunner.GuardInsert("intensities", SHAVE0, pattern_pnt_sz, intensities);
    mvcvSmoothed_intensityTestRunner.Run(SHAVE0);
    mvcvSmoothed_intensityCycleCount = mvcvSmoothed_intensityTestRunner.GetVariableValue(std::string("cycleCount"));
    functionInfo.AddCyclePerPixelInfo((float)(mvcvSmoothed_intensityCycleCount - 13)/ (float)pattern_pnt_sz);
    functionInfo.setExpectedCycles((float)EXPECTED_CC);

    mvcvSmoothed_intensityTestRunner.GetOutput("intensities", SHAVE0, pattern_pnt_sz, intensities);
    mvcvSmoothed_intensityTestRunner.GuardCheck("intensities", SHAVE0, pattern_pnt_sz, intensities);
}
