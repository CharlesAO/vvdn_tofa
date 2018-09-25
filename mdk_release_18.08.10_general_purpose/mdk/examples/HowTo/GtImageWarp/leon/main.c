///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     main leon file
///
// 1: Includes
// ----------------------------------------------------------------------------
// System settings
#include "app_config.h"
#include "mesh.h"
#include "imageWarpDefines.h"
#include "Defines.h"
#include "swcLeonUtils.h"

// Standard libraries
#include <stdio.h>
#include <string.h>


// Drivers
#include <DrvShaveL2Cache.h>

// Common code
#include <swcShaveLoader.h>
#include <swcFrameTypes.h>

#include <swcTestUtils.h>
#include <swcCrc.h>

// Components
#include <UnitTestApi.h>
#include <VcsHooksApi.h>

// Local code
#include <Buffers.h>

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------
#define SHAVE_NUMBER 0
#define CRC_USED 0x572104A1

// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
// Shave entrypoints

extern u32 fish_eye0_imageWarp;
ALIGNED(128) tileList tileNodes[MESH_HEIGHT * MESH_WIDTH];

// 4: Static Local Data
// ----------------------------------------------------------------------------

static u32 Fnfish_Sched 	= (u32)&fish_eye0_imageWarp;

// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------

void runSched(swcShaveUnit_t svu, meshStruct* mesh,
              frameBuffer *inputFb,
              frameBuffer *outputFb,
              unsigned short paddingvalue)
{
    // Declare performance monitoring structure
    performanceStruct perfStr;

    // Initialize the profiling structure
    swcShaveProfInit(&perfStr);

    swcResetShave(svu);
    swcSetAbsoluteDefaultStack(svu);

    // Start profiling
    swcShaveProfStartGatheringFields(svu, PERF_INSTRUCT_COUNT);
    swcStartShaveCC(svu, Fnfish_Sched, "iiiiiii", mesh,
                    inputFb,
                    outputFb,
                    tileNodes,
                    paddingvalue,0,0);
    swcWaitShave(svu);
    // Stop profiling
    swcShaveProfStopFieldsGathering(svu, PERF_INSTRUCT_COUNT);

    // Request Profiling report
    swcShaveProfPrint(svu, &perfStr);

    DrvShaveL2CachePartitionFlushAndInvalidate(svu);
}

// Main function
int main()
{

    // Init board
    initClocksAndMemory();
    DrvShaveL2CachePartitionFlushAndInvalidate(SHAVE_NUMBER);

    loadMemFromFileSimple("input_240x320_P400.raw", IMG_WIDTH * IMG_HEIGHT * sizeof(unsigned char), (void*)inputframe);
    memset(outputframe, 0x0, OUT_IMG_WIDTH * OUT_IMG_HEIGHT);
    printf("fish_eye algorithm.\n");
    frameBuffer inFrame, outFrame;
    frameSpec frSpec;
    frSpec.bytesPP = 1;
    frSpec.height = IMG_HEIGHT;
    frSpec.width = IMG_WIDTH;
    frSpec.stride = frSpec.width * frSpec.bytesPP;
    frSpec.type = RAW8;
    inFrame.p1 = inputframe;
    inFrame.p2 = NULL;
    inFrame.p3 = NULL;
    inFrame.spec = frSpec;
    frSpec.height = OUT_IMG_HEIGHT;
    frSpec.width = OUT_IMG_WIDTH;
    frSpec.stride = frSpec.width * frSpec.bytesPP;
    outFrame.p1 = outputframe;
    outFrame.p2 = NULL;
    outFrame.p3 = NULL;
    outFrame.spec = frSpec;

    meshStruct mesh;
    mesh.meshWidth = MESH_WIDTH;
    mesh.meshHeight = MESH_HEIGHT;
    mesh.meshX = (float*) mesh_x;
    mesh.meshY = (float*) mesh_y;
    mesh.coord_min_y = 0;
    mesh.coord_min_x = 0;
    mesh.coord_max_x = OUT_IMG_WIDTH;
    mesh.coord_max_y = OUT_IMG_HEIGHT;

    // Init and run Fish-eye computation
    runSched(SHAVE_NUMBER, &mesh, &inFrame, &outFrame, 0);
     //Unit test based on Crc
	unitTestInit();

    unitTestCrcCheck(outputframe, outFrame.spec.stride * outFrame.spec.height, CRC_USED);
	unitTestFinalReport();
    saveMemoryToFile((unsigned int)outputframe, outFrame.spec.stride * outFrame.spec.height, "output_240x320_P400.raw");

    return 0;
}
