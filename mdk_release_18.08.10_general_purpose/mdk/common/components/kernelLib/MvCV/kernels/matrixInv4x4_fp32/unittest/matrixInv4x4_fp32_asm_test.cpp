#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define SIZE 4
#define EXPECTED_CC_M1 (999)
#define EXPECTED_CC_M2 (146.0)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner matrixInv4x4_fp32TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int matrixInv4x4_fp32CycleCount;

void matrixInv4x4_fp32_asm_test(float* in, float* out)
{
  FunctionInfo& functionInfo = FunctionInfo::Instance();  
	matrixInv4x4_fp32TestRunner.Init();
	matrixInv4x4_fp32TestRunner.SetInput("input", in, SIZE*SIZE, SHAVE0);

	matrixInv4x4_fp32TestRunner.GuardInsert("output", SHAVE0, SIZE*SIZE*4, (unsigned char*)out);
	matrixInv4x4_fp32TestRunner.Run(SHAVE0);

  matrixInv4x4_fp32CycleCount = matrixInv4x4_fp32TestRunner.GetVariableValue("cycleCount");
	functionInfo.AddCyclePerPixelInfo((float)(matrixInv4x4_fp32CycleCount - 2));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
  
	matrixInv4x4_fp32TestRunner.GetOutput("output", SHAVE0, SIZE*SIZE*4, (unsigned char*)out);
	matrixInv4x4_fp32TestRunner.GuardCheck("output", SHAVE0, SIZE*SIZE*4, (unsigned char*)out);
  
}
