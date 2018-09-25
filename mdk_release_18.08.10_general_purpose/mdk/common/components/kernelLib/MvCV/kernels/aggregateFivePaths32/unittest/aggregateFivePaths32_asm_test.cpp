#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define EXPECTED_CC (0.7)
#define DISPARITIES 32
#define PADDING 16 

TestRunner mvcvAggregateFivePaths32TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int mvcvAggregateFivePaths32CycleCount;

void mvcvAggregateFivePaths32_asm_test(unsigned char *path0, unsigned char *path1, unsigned char *path2,
                                       unsigned char *path3, unsigned char *path4, 
                                       unsigned char *out, unsigned int width)
{
    FunctionInfo& functionInfo = FunctionInfo::Instance();
    
    mvcvAggregateFivePaths32TestRunner.Init();
    mvcvAggregateFivePaths32TestRunner.SetInput(   "path0", path0, width * DISPARITIES, SHAVE0);
    mvcvAggregateFivePaths32TestRunner.SetInput(   "path1", path1, width * DISPARITIES, SHAVE0);
    mvcvAggregateFivePaths32TestRunner.SetInput(   "path2", path2, width * DISPARITIES, SHAVE0);
    mvcvAggregateFivePaths32TestRunner.SetInput(   "path3", path3, width * DISPARITIES, SHAVE0);
    mvcvAggregateFivePaths32TestRunner.SetInput(   "path4", path4, width * DISPARITIES, SHAVE0);
    mvcvAggregateFivePaths32TestRunner.SetInput(     "out", out, width * DISPARITIES + PADDING, SHAVE0);
    mvcvAggregateFivePaths32TestRunner.SetInput(   "width", (unsigned int)width, SHAVE0);

    mvcvAggregateFivePaths32TestRunner.GuardInsert(string("out"), SHAVE0, width * DISPARITIES + PADDING, out);
    mvcvAggregateFivePaths32TestRunner.Run(SHAVE0);

    mvcvAggregateFivePaths32CycleCount = (mvcvAggregateFivePaths32TestRunner.GetVariableValue(std::string("cycleCount"))) - 5;
    functionInfo.AddCyclePerPixelInfo((float)(mvcvAggregateFivePaths32CycleCount)/ (float)(width * DISPARITIES));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);

    mvcvAggregateFivePaths32TestRunner.GetOutput(string("out"), SHAVE0, width * DISPARITIES + PADDING, out);
    mvcvAggregateFivePaths32TestRunner.GuardCheck(string("out"), SHAVE0, width * DISPARITIES + PADDING, out);
}
