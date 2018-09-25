/*
 * testL2Cache.c
 *
 *  Created on: May 30, 2017
 */

// 1: Includes
// ----------------------------------------------------------------------------
#include <mv_types.h>
#include <stdio.h>
#include <rtems.h>
#include <rtems/bspIo.h>
#include "swcLeonUtils.h"
#include "swcShaveLoader.h"
#include "OsDrvSvu.h"
#include <OsDrvShaveL2Cache.h>
#include "UnitTestApi.h"
#include "VcsHooksApi.h"
#include "swcMutexUsage.h"

#include "SharingDataWithShave.h"
#include "app_config.h"

// 2:  Source Specific #defines and types  (typedef, enum, struct)
// ----------------------------------------------------------------------------

#ifndef RESOURCE_FOLDER
#error No resource folder defined. \
Pls check variable RESOURCE_FOLDER definition in app Makefile
#endif

#define DEF_FRAME_WIDTH   (1280)
#define DEF_FRAME_HEIGHT  (50)

const u32 FRAME_WIDTH = DEF_FRAME_WIDTH;
const u32 FRAME_HEIGHT = DEF_FRAME_HEIGHT;


// 3: Global Data (Only if absolutely necessary)
// ----------------------------------------------------------------------------
extern DynamicContext_t MODULE_DATA(SmallData);
extern DynamicContext_t MODULE_DATA(BigData);

u32 CMX_DIRECT_DATA bufferSharedBetweenLeonAndShave[SMALL_DATA_LENGTH];
u32 CMX_DIRECT_DATA bufferdSharedBetweenShaves[SMALL_DATA_LENGTH];
extern __attribute__((aligned(8))) int l2cDataPartitionId;

// 4: Static Local Data
// ----------------------------------------------------------------------------
static const char * ResourceFolder = RESOURCE_FOLDER;

static swcShaveUnit_t shaveList[SHAVES_USED] = {0, 1};
swcShaveUnit_t shaveListBigData[SHAVES_USED] = {2, 3};

DDR_DATA ALIGNED(64) u8 inputBuffer[DEF_FRAME_HEIGHT * DEF_FRAME_WIDTH];
DDR_DATA ALIGNED(64) u8 outputBuffer[DEF_FRAME_HEIGHT * DEF_FRAME_WIDTH];


// 5: Static Function Prototypes
// ----------------------------------------------------------------------------

// 6: Functions Implementation
// ----------------------------------------------------------------------------


void runShaves(DynamicContext_t * pDynModContext, swcShaveUnit_t * pDynModShaveList, u8* input, u8* output)
{
    int status, i;
    uint32_t running;

    // Configure dynamic module
    status = OsDrvSvuSetupDynShaveApps(pDynModContext,
                                        pDynModShaveList,
                                        SHAVES_USED);
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    // Allocate rtems resources
    status = OsDrvSvuOpenShaves(pDynModShaveList,
                                SHAVES_USED,
                                OS_MYR_PROTECTION_SELF);
    
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    // Flush and invalidate shave L2 cache, before reading information sotred in DDR
    OsDrvShaveL2CachePartitionInvalidate(l2cDataPartitionId);

    if(input)
    {
        // Start dynamic moduel execution
        for (i=0;i<SHAVES_USED;i++){
            u8* src =  (u8*)(input + i*FRAME_WIDTH*FRAME_HEIGHT/2);
            u8* dest = (u8*)(output + i*FRAME_WIDTH*FRAME_HEIGHT/2);
            status = OsDrvSvuRunShaveAlgoOnAssignedShaveCC(
                        pDynModContext,                 // points to Dynamic Module Data Variable location in memory
                        pDynModShaveList[i],            // using ShaveList index 0 because it's running only on one shave
                        "iiii",                         // string describing shave parameters
                        src,                            // input buffer sent as a parameter
                        dest,                           // output buffer sent as a parameter
                        FRAME_WIDTH, FRAME_HEIGHT/2);   // image dimensions sent as a parameter

            if (status != OS_MYR_DYN_INFR_SUCCESS)
                OSDRV_DYN_INFR_CHECK_CODE(status);
        }

    }
    else
    {
        for (i=0;i<SHAVES_USED;i++){
            status = OsDrvSvuRunShaveAlgoOnAssignedShaveCC(
                                    &MODULE_DATA(SmallData),
                                    shaveList[i],
                                    "i",
                                    bufferdSharedBetweenShaves);
            
            if (status != OS_MYR_DYN_INFR_SUCCESS)
                OSDRV_DYN_INFR_CHECK_CODE(status);
        }
    }
    //Wait for shaves to finish
    status = OsDrvSvuDynWaitShaves(pDynModShaveList, 
                                    SHAVES_USED,
                                    OS_DRV_SVU_WAIT_FOREVER,
                                    &running);

    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);


    /* Close the shaves used */
    status = OsDrvSvuCloseShaves(pDynModShaveList, SHAVES_USED);
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);

    /* Free the Leon OS applications memory */
    status = OsDrvSvuCleanupDynShaveApps(pDynModContext);
    if (status != OS_MYR_DYN_INFR_SUCCESS)
        OSDRV_DYN_INFR_CHECK_CODE(status);
}

void LeonShareSmallDataWithShave(u32* expectedBufferShaveLeon, u32* expectedBufferShaveToShave)
{
    unsigned int i;
    DynamicContext_t * pDynModContext;
    swcShaveUnit_t * pDynModShaveList;


    for(i = 0; i < SMALL_DATA_LENGTH ;i++)
    {
        bufferSharedBetweenLeonAndShave[i] = i;
    }

    pDynModContext = &MODULE_DATA(SmallData);
    pDynModShaveList = shaveList;
    runShaves(pDynModContext, pDynModShaveList, NULL, NULL);

    printf("Share small data between leon and shave \n");
    unitTestMemCompare((void *)bufferSharedBetweenLeonAndShave,
                        expectedBufferShaveLeon,
                        sizeof(u32) * SMALL_DATA_LENGTH);
    printf("Write small data from two shaves \n");
    unitTestMemCompare((void *)bufferdSharedBetweenShaves,
                        expectedBufferShaveToShave,
                        sizeof(u32) * SMALL_DATA_LENGTH);
}

int LeonShareBigDataWithShave(void)
{
    char filename[256];
    DynamicContext_t * pDynModContext;
    swcShaveUnit_t * pDynModShaveList;

    printf("Share big data between leon and shaves \n");
    printf("\n---- Loading ImageFile ----\n");
    sprintf(filename,"%sCobKitchen_1280x720.yuv",ResourceFolder);
    loadMemFromFileSimple(filename, FRAME_WIDTH * FRAME_HEIGHT, (void*)inputBuffer);

    // Flush and invalidate shave L2 cache, before reading information stored in DDR
    OsDrvShaveL2CachePartitionFlush(l2cDataPartitionId, PERFORM_INVALIDATION);

    pDynModContext = &MODULE_DATA(BigData);
    pDynModShaveList = shaveListBigData;
    runShaves(pDynModContext, pDynModShaveList, inputBuffer, outputBuffer);
    
    // Flush and invalidate shave L2 cache, before reading information sotred in DDR
    OsDrvShaveL2CachePartitionFlush(l2cDataPartitionId, PERFORM_INVALIDATION);
    // Invalidate Leon L1 and L2 cache before reading
    rtems_cache_invalidate_data_range((void *)outputBuffer,
                                    sizeof(unsigned char) * FRAME_WIDTH * FRAME_HEIGHT);

    printf("Write big data from two shaves\n");
    unitTestMemCompare((void *)outputBuffer,
                        inputBuffer,
                        sizeof(unsigned char) * FRAME_WIDTH * FRAME_HEIGHT);

    return 0;
}

