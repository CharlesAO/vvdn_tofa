#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>


#define EXPECTED_CC (455)


TestRunner tile_samplerRGB888TestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int tile_samplerRGB888CycleCount;

void tile_samplerRGB888_asm_test(unsigned char *input_tile, unsigned char * output_tile, float *xcoord, float *ycoord, unsigned int tile_width, unsigned int tile_height)
{

	FunctionInfo& functionInfo = FunctionInfo::Instance();

	tile_samplerRGB888TestRunner.Init();
	tile_samplerRGB888TestRunner.SetInput("input_tile", input_tile, (tile_width)* 3  * (tile_height), SHAVE0);
	tile_samplerRGB888TestRunner.SetInput("xcoord", xcoord, 4, SHAVE0);
	tile_samplerRGB888TestRunner.SetInput("ycoord", ycoord, 4, SHAVE0);

	tile_samplerRGB888TestRunner.SetInput("tile_width", tile_width, SHAVE0);
	tile_samplerRGB888TestRunner.SetInput("tile_height", tile_height, SHAVE0);

	tile_samplerRGB888TestRunner.GuardInsert("output_tile", SHAVE0, 16 * 3 * 16, output_tile);
	tile_samplerRGB888TestRunner.Run(SHAVE0);
	
	tile_samplerRGB888CycleCount = tile_samplerRGB888TestRunner.GetVariableValue(std::string("cycleCount"));
	functionInfo.AddCyclePerPixelInfo((float)(tile_samplerRGB888CycleCount - 5)/ (float)256);
	functionInfo.setExpectedCycles((float)EXPECTED_CC);

	
	tile_samplerRGB888TestRunner.GetOutput("output_tile", SHAVE0, 16 * 3 * 16, output_tile);
	tile_samplerRGB888TestRunner.GuardCheck("output_tile", SHAVE0, 16 * 3 * 16, output_tile);
}
