#include "TestRunner.h"
#include "RandomGenerator.h"
#include "moviDebugDll.h"
#include "FunctionInfo.h"
#include <cstdio>

#define EXPECTED_CC (381)
#define TILE_SIZE 16
#define PADDING 32

TestRunner warpMeshSampleFp16bitTestRunner(APP_PATH, APP_ELFPATH, KERNEL_ASM_LABEL, DBG_INTERFACE);

unsigned int warpMeshSampleFp16bitCycleCount;

void warpMeshSampleFp16bit_asm_test(half **input_tile, half *output_tile, half *mx, half *my, unsigned int tile_width, unsigned int tile_height)
{
unsigned int maxWidth = 100;
    FunctionInfo& functionInfo = FunctionInfo::Instance();

    warpMeshSampleFp16bitTestRunner.Init();
    warpMeshSampleFp16bitTestRunner.SetInput("input_tile", input_tile, tile_width,maxWidth,tile_height, SHAVE0);
    warpMeshSampleFp16bitTestRunner.SetInput("mx", mx, TILE_SIZE * (TILE_SIZE + PADDING), SHAVE0);
    warpMeshSampleFp16bitTestRunner.SetInput("my", my, TILE_SIZE * (TILE_SIZE + PADDING), SHAVE0);

    warpMeshSampleFp16bitTestRunner.SetInput("tile_width", (unsigned int)tile_width, SHAVE0);
    warpMeshSampleFp16bitTestRunner.SetInput("tile_height", (unsigned int)tile_height, SHAVE0);

    warpMeshSampleFp16bitTestRunner.Run(SHAVE0);
    
    warpMeshSampleFp16bitCycleCount = warpMeshSampleFp16bitTestRunner.GetVariableValue(std::string("cycleCount"));
    functionInfo.AddCyclePerPixelInfo((float)(warpMeshSampleFp16bitCycleCount - 509)/ (float)(TILE_SIZE * TILE_SIZE));
    functionInfo.setExpectedCycles((float)EXPECTED_CC);

    
    warpMeshSampleFp16bitTestRunner.GetOutput("output_tile", SHAVE0, TILE_SIZE * TILE_SIZE, output_tile);
}
