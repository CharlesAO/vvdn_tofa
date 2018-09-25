#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>


#define EXPECTED_CC (381)
#define TILE_SIZE 16
#define PADDING 32

TestRunner warpMeshSample10bitTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int warpMeshSample10bitCycleCount;

void warpMeshSample10bit_asm_test(unsigned short **input_tile, unsigned short *output_tile, half *mx, half *my, unsigned int tile_width, unsigned int tile_height)
{
unsigned int maxWidth = 100;
	FunctionInfo& functionInfo = FunctionInfo::Instance();

	warpMeshSample10bitTestRunner.Init();
	warpMeshSample10bitTestRunner.SetInput("input_tile", input_tile, tile_width,maxWidth,tile_height, SHAVE0);
	warpMeshSample10bitTestRunner.SetInput("mx", mx, TILE_SIZE * (TILE_SIZE + PADDING), SHAVE0);
	warpMeshSample10bitTestRunner.SetInput("my", my, TILE_SIZE * (TILE_SIZE + PADDING), SHAVE0);

	warpMeshSample10bitTestRunner.SetInput("tile_width", (unsigned int)tile_width, SHAVE0);
	warpMeshSample10bitTestRunner.SetInput("tile_height", (unsigned int)tile_height, SHAVE0);

	 warpMeshSample10bitTestRunner.GuardInsert("output_tile", SHAVE0, TILE_SIZE * TILE_SIZE, output_tile);
	 warpMeshSample10bitTestRunner.Run(SHAVE0);
	
	warpMeshSample10bitCycleCount = warpMeshSample10bitTestRunner.GetVariableValue(std::string("cycleCount"));
	functionInfo.AddCyclePerPixelInfo((float)(warpMeshSample10bitCycleCount - 509)/ (float)(TILE_SIZE * TILE_SIZE));
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	
	warpMeshSample10bitTestRunner.GetOutput("output_tile", SHAVE0, TILE_SIZE * TILE_SIZE, output_tile);
	warpMeshSample10bitTestRunner.GuardCheck("output_tile", SHAVE0, TILE_SIZE * TILE_SIZE, output_tile);
}
