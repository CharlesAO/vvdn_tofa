#include "half.h"
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>

#define SIZE 3
#define EXPECTED_CC_M1 (999)
#define EXPECTED_CC_M2 (58.0)

#ifdef MYRIAD2
#define EXPECTED_CC EXPECTED_CC_M2
#else
#define EXPECTED_CC EXPECTED_CC_M1
#endif

TestRunner matrixInv3x3_fp32TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int matrixInv3x3_fp32CycleCount;

void matrixInv3x3_fp32_asm_test(float** in, float** out)
{
  FunctionInfo& functionInfo = FunctionInfo::Instance();  
	matrixInv3x3_fp32TestRunner.Init();
	matrixInv3x3_fp32TestRunner.SetInput("input", in, SIZE, SIZE, SIZE, SHAVE0);

	matrixInv3x3_fp32TestRunner.GuardInsert("output", SHAVE0, SIZE*4, SIZE*4, SIZE, (unsigned char**)out);
	matrixInv3x3_fp32TestRunner.Run(SHAVE0);

  matrixInv3x3_fp32CycleCount = matrixInv3x3_fp32TestRunner.GetVariableValue("cycleCount");
	functionInfo.AddCyclePerPixelInfo((float)(matrixInv3x3_fp32CycleCount - 12));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);
  
	matrixInv3x3_fp32TestRunner.GetOutput("output", SHAVE0, SIZE*4, SIZE*4, SIZE, (unsigned char**)out);
	matrixInv3x3_fp32TestRunner.GuardCheck("output", SHAVE0, SIZE*4, SIZE*4, SIZE, (unsigned char**)out);
	//matrixInv3x3_fp32TestRunner.GetOutput("output", SHAVE0, SIZE, SIZE, SIZE, out);
  
}
