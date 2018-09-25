#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>


#define EXPECTED_CC (381)
#define TILE_SIZE 16
#define PADDING 32

TestRunner warpMeshSample8bitStrideTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int warpMeshSample8bitStrideCycleCount;

void warpMeshSample8bitStride_asm_test(unsigned char **input_tile, unsigned char * output_tile, half *mx, half *my, unsigned int tile_width, unsigned int tile_height)
{
	unsigned int maxWidth = 100;
	FunctionInfo& functionInfo = FunctionInfo::Instance();

	warpMeshSample8bitStrideTestRunner.Init();
	warpMeshSample8bitStrideTestRunner.SetInput("input_tile", input_tile, tile_width, maxWidth, tile_height, SHAVE0);
	warpMeshSample8bitStrideTestRunner.SetInput("output_tile", output_tile, tile_width * TILE_SIZE, SHAVE0);
	warpMeshSample8bitStrideTestRunner.SetInput("mx", mx, TILE_SIZE * (TILE_SIZE + PADDING), SHAVE0);
	warpMeshSample8bitStrideTestRunner.SetInput("my", my, TILE_SIZE * (TILE_SIZE + PADDING), SHAVE0);

	warpMeshSample8bitStrideTestRunner.SetInput("tile_width", (unsigned int)tile_width, SHAVE0);
	warpMeshSample8bitStrideTestRunner.SetInput("tile_height", (unsigned int)tile_height, SHAVE0);
	warpMeshSample8bitStrideTestRunner.GuardInsert("output_tile", SHAVE0, TILE_SIZE * tile_width, output_tile);
	warpMeshSample8bitStrideTestRunner.Run(SHAVE0);
	
	warpMeshSample8bitStrideCycleCount = warpMeshSample8bitStrideTestRunner.GetVariableValue(std::string("cycleCount"));
	functionInfo.AddCyclePerPixelInfo((float)(warpMeshSample8bitStrideCycleCount - 509)/ (float)(TILE_SIZE * TILE_SIZE));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	
	warpMeshSample8bitStrideTestRunner.GetOutput("output_tile", SHAVE0, TILE_SIZE * tile_width, output_tile);
	warpMeshSample8bitStrideTestRunner.GuardCheck("output_tile", SHAVE0, TILE_SIZE * tile_width, output_tile);
}
