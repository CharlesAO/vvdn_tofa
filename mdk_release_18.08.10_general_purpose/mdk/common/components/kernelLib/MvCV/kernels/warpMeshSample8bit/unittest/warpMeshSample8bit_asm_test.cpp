#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>


#define EXPECTED_CC (381)
#define TILE_SIZE 16
#define PADDING 32

TestRunner warpMeshSample8bitTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int warpMeshSample8bitCycleCount;

void warpMeshSample8bit_asm_test(unsigned char **input_tile, unsigned char * output_tile, half *mx, half *my, unsigned int tile_width, unsigned int tile_height)
{
	unsigned int maxWidth = 100;
	FunctionInfo& functionInfo = FunctionInfo::Instance();

	warpMeshSample8bitTestRunner.Init();
	warpMeshSample8bitTestRunner.SetInput("input_tile", input_tile, tile_width, maxWidth, tile_height, SHAVE0);
	warpMeshSample8bitTestRunner.SetInput("mx", mx, TILE_SIZE * (TILE_SIZE + PADDING), SHAVE0);
	warpMeshSample8bitTestRunner.SetInput("my", my, TILE_SIZE * (TILE_SIZE + PADDING), SHAVE0);

	warpMeshSample8bitTestRunner.SetInput("tile_width", (unsigned int)tile_width, SHAVE0);
	warpMeshSample8bitTestRunner.SetInput("tile_height", (unsigned int)tile_height, SHAVE0);

	warpMeshSample8bitTestRunner.GuardInsert("output_tile", SHAVE0, TILE_SIZE * TILE_SIZE, output_tile);
	warpMeshSample8bitTestRunner.Run(SHAVE0);
	
	warpMeshSample8bitCycleCount = warpMeshSample8bitTestRunner.GetVariableValue(std::string("cycleCount"));
	functionInfo.AddCyclePerPixelInfo((float)(warpMeshSample8bitCycleCount - 509)/ (float)(TILE_SIZE * TILE_SIZE));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	
	warpMeshSample8bitTestRunner.GetOutput("output_tile", SHAVE0, TILE_SIZE * TILE_SIZE, output_tile);
	warpMeshSample8bitTestRunner.GuardCheck("output_tile", SHAVE0, TILE_SIZE * TILE_SIZE, output_tile);
}
