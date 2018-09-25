#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>


#define EXPECTED_CC (1.51)
#define TILE_SIZE 16


TestRunner mvcvWarpMeshExpandTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int mvcvWarpMeshExpandCycleCount;

void mvcvWarpMeshExpand_asm_test(half *xcoord, half *ycoord, half *mx, half *my)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();

	mvcvWarpMeshExpandTestRunner.Init();
	mvcvWarpMeshExpandTestRunner.SetInput("xcoord", xcoord, 4, SHAVE0);
	mvcvWarpMeshExpandTestRunner.SetInput("ycoord", ycoord, 4, SHAVE0);

	mvcvWarpMeshExpandTestRunner.GuardInsert("mx", SHAVE0, TILE_SIZE * TILE_SIZE, mx);
	mvcvWarpMeshExpandTestRunner.GuardInsert("my", SHAVE0, TILE_SIZE * TILE_SIZE, my);
	mvcvWarpMeshExpandTestRunner.Run(SHAVE0);
	
	mvcvWarpMeshExpandCycleCount = mvcvWarpMeshExpandTestRunner.GetVariableValue(std::string("cycleCount"));
	functionInfo.AddCyclePerPixelInfo((float)(mvcvWarpMeshExpandCycleCount - 5)/ (float)( TILE_SIZE * TILE_SIZE));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	
	mvcvWarpMeshExpandTestRunner.GetOutput("mx", SHAVE0, TILE_SIZE * TILE_SIZE, mx);
	mvcvWarpMeshExpandTestRunner.GuardCheck("mx", SHAVE0, TILE_SIZE * TILE_SIZE, mx);
	mvcvWarpMeshExpandTestRunner.GetOutput("my", SHAVE0, TILE_SIZE * TILE_SIZE, my);
	mvcvWarpMeshExpandTestRunner.GuardCheck("my", SHAVE0, TILE_SIZE * TILE_SIZE, my);
}                                           
