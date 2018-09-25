#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.50)
#define DISPARITIES 32
#define PADDING 16 

TestRunner mvcvAggregateThreePaths32TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvAggregateThreePaths32CycleCount;

void mvcvAggregateThreePaths32_asm_test(unsigned char *path0, unsigned char *path1, unsigned char *path2, unsigned char *out, unsigned int width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    mvcvAggregateThreePaths32TestRunner.Init();
    mvcvAggregateThreePaths32TestRunner.SetInput(   "path0", path0, width * DISPARITIES, SHAVE0);
    mvcvAggregateThreePaths32TestRunner.SetInput(   "path1", path1, width * DISPARITIES, SHAVE0);
    mvcvAggregateThreePaths32TestRunner.SetInput(   "path2", path2, width * DISPARITIES, SHAVE0);
    mvcvAggregateThreePaths32TestRunner.SetInput(     "out", out, width * DISPARITIES + PADDING, SHAVE0);
    mvcvAggregateThreePaths32TestRunner.SetInput(   "width", (unsigned int)width, SHAVE0);

    mvcvAggregateThreePaths32TestRunner.GuardInsert(string("out"), SHAVE0, width * DISPARITIES + PADDING, out);
    mvcvAggregateThreePaths32TestRunner.Run(SHAVE0);

    mvcvAggregateThreePaths32CycleCount = (mvcvAggregateThreePaths32TestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvAggregateThreePaths32CycleCount)/ (float)(width * DISPARITIES));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);

    mvcvAggregateThreePaths32TestRunner.GetOutput(string("out"), SHAVE0, width * DISPARITIES + PADDING, out);
    mvcvAggregateThreePaths32TestRunner.GuardCheck(string("out"), SHAVE0, width * DISPARITIES + PADDING, out);
}
