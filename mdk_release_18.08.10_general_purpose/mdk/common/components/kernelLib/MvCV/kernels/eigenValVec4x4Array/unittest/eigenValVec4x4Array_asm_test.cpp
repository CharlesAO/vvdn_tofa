///
#include "TestRunner.h"
#include "FunctionInfo.h"
#include "moviDebugDll.h"
#include <cstdio>


#define EXPECTED_CC  372237



TestRunner eigenValVec4x4ArrayTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);
unsigned int eigenValVec4x4ArrayCycleCount;

void eigenValVec4x4Array_asm_test(float** matrix, float* eigvals, float** eigvec, unsigned int num_matrix, float threshold)
{
	FunctionInfo& functionInfo = FunctionInfo::Instance();
	unsigned int maxWidth = 16;
	unsigned int row_width = 16;

	eigenValVec4x4ArrayTestRunner.Init();
	
	eigenValVec4x4ArrayTestRunner.SetInput("num_matrix", (unsigned int)num_matrix, SHAVE0);
	eigenValVec4x4ArrayTestRunner.SetInput("input", matrix, row_width, maxWidth, num_matrix, SHAVE0);
	eigenValVec4x4ArrayTestRunner.SetInput("threshold", threshold, SHAVE0);
	eigenValVec4x4ArrayTestRunner.GuardInsert(string("out_eigen_val"), SHAVE0, num_matrix, eigvals);
	eigenValVec4x4ArrayTestRunner.GuardInsert(string("out_eigen_vec"), SHAVE0, row_width, maxWidth, num_matrix, eigvec);
	eigenValVec4x4ArrayTestRunner.Run(SHAVE0);

	eigenValVec4x4ArrayCycleCount = eigenValVec4x4ArrayTestRunner.GetVariableValue("cycleCount");

	functionInfo.AddCyclePerPixelInfo((float)(eigenValVec4x4ArrayCycleCount-6010));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	
	eigenValVec4x4ArrayTestRunner.GetOutput(string("out_eigen_val"), SHAVE0, num_matrix, eigvals);
	eigenValVec4x4ArrayTestRunner.GuardCheck(string("out_eigen_val"), SHAVE0, num_matrix, eigvals);
	eigenValVec4x4ArrayTestRunner.GetOutput(string("out_eigen_vec"), SHAVE0, row_width, maxWidth, num_matrix, eigvec);
	eigenValVec4x4ArrayTestRunner.GuardCheck(string("out_eigen_vec"), SHAVE0, row_width, maxWidth, num_matrix, eigvec);


}
