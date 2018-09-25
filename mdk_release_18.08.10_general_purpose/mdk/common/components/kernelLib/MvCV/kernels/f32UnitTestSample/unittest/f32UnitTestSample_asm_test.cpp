#include "f32UnitTestSample.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

TestRunner f32UnitTestSampleTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

void f32UnitTestSample_asm_test(float *a, float **b, float *out, unsigned int num_vectors)
{
    f32UnitTestSampleTestRunner.Init();
    f32UnitTestSampleTestRunner.SetInput("input_a", a, 4, SHAVE0);
    f32UnitTestSampleTestRunner.SetInput("input_b", b, 4, 4, num_vectors, SHAVE0);
    f32UnitTestSampleTestRunner.SetInput("num_vectors", num_vectors, SHAVE0);
    f32UnitTestSampleTestRunner.GuardInsert("output", SHAVE0, 4, out);
    f32UnitTestSampleTestRunner.Run(SHAVE0);
    f32UnitTestSampleTestRunner.GetOutput("output", SHAVE0, 4, out);
    f32UnitTestSampleTestRunner.GuardCheck("output", SHAVE0, 4, out);
}
