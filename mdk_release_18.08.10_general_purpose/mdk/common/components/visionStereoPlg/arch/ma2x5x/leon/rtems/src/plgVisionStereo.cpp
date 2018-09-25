#include "plgVisionStereo.h"

#include <pthread.h>
#include "superFrame.h"
#define MVLOG_UNIT_NAME MIGFlicStereo
#include <mvLog.h>
#include <mvMacros.h>
#include "commStructs.h"




#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <mvMacros.h>
#include <semaphore.h>

#include "swcFrameTypes.h"
#include "OsDrvTimer.h"
#include "swcLeonUtils.h"
#include "swcShaveLoader.h"
#include <swcTestUtils.h>
#include <swcCrc.h>
#include <UnitTestApi.h>
#include <DrvLeon.h>

#include <DrvLeonL2C.h>
#include <DrvShaveL2Cache.h>
#include <DrvCmxDma.h>
#include <DrvIcbDefines.h>
#include <OsDrvSvu.h>
#include <theDynContext.h>
#include <OsDrvShaveL2Cache.h>

#include <imageWarp.h>
#include <startDisparityMap.h>
#include <hw_filt.h>
#include <Defines.h>
#include <swcFrameTypes.h>
//#include <XLink.h>

#include <mesh.h>

#define CONFIDENCE_STREAM_NAME "ConfStream"
#define PIPELINE_THREADS 2
// number of custom processing threads
#ifndef MV_STEREO_THREADS
 #ifdef V4
  #define MV_STEREO_THREADS 2
 #else
  #define MV_STEREO_THREADS 2 // TODO: use 3 threads here
 #endif
#endif


typedef enum
{
    raw         = 0, // raw images from the camera
    rectified   = 1, // rectified images
} outputMode_t;
// Buffers used for preprocessing: stereo calibration and pyramid creation
u8 ALIGNED(32) lutBuffArea[MV_STEREO_THREADS][STEREO_FRAMES][MAX_WIDTH * MAX_HEIGHT] DDR_AREA;
u8 ALIGNED(32) warpedOutputFrames[MV_STEREO_THREADS][STEREO_FRAMES][MAX_WIDTH * MAX_HEIGHT] DDR_AREA;
u8 ALIGNED(32) downsampledBicubicBuffArea[MV_STEREO_THREADS][STEREO_FRAMES][MAX_WIDTH/SCALE_FACTOR * MAX_HEIGHT/SCALE_FACTOR] DDR_AREA;

// Final disparity and confidence maps
u8 ALIGNED(32) combinedCostArea[MV_STEREO_THREADS][MAX_WIDTH/SCALE_FACTOR * MAX_HEIGHT/SCALE_FACTOR * MAX_DISPARITIES_LEVEL0 + 2*MAX_HEIGHT/SCALE_FACTOR * MAX_DISPARITIES_LEVEL0] DDR_AREA;
qvgaType ALIGNED(32) predictedDisparityBuffArea[MV_STEREO_THREADS][MAX_WIDTH/SCALE_FACTOR * MAX_HEIGHT/SCALE_FACTOR] DDR_AREA;
u8 ALIGNED(32) confidenceBuffArea[MV_STEREO_THREADS][MAX_WIDTH / SCALE_FACTOR * MAX_HEIGHT / SCALE_FACTOR] DDR_AREA;
outputType ALIGNED(32) disparityBuffArea[MV_STEREO_THREADS][MAX_WIDTH * MAX_HEIGHT] DDR_AREA;
outputType ALIGNED(32) disparityMedianSubpixelBuffArea[MV_STEREO_THREADS][MAX_WIDTH * MAX_HEIGHT] DDR_AREA;
u8 ALIGNED(32) disparityMedianIntegerBuffAreaFinal[MV_STEREO_THREADS][MAX_WIDTH * MAX_HEIGHT] DDR_AREA;
outputType ALIGNED(32) disparityMedianIntegerBuffArea[MV_STEREO_THREADS][MAX_WIDTH * MAX_HEIGHT] DDR_AREA;

extern DynamicContext_t MODULE_DATA(downsampleApp);
extern DynamicContext_t MODULE_DATA(imageWarpDynlib);
extern DynamicContext_t MODULE_DATA(costApp);

sem_t global_sem1, global_sem2;



#define THREADS 2

extern int* partitions[CFG_NUM_SHAVES];
tileList tileNodes[MV_STEREO_THREADS][SHAVES_USED][MESH_HEIGHT * MESH_WIDTH] ALIGNED(128);

CMX_NOCACHE
volatile outputMode_t output = rectified;

// OV7251 85 degrees FOV
#define LEFT_CAMERA     0//1
#define RIGHT_CAMERA    1//0

pthread_t preprocessingThread;
pthread_t confidenceThresholdThread;
u32 confidenceThresholdValue;

static stereoUserConfig_t CMX_NOCACHE algoCfg;
static frameSpec cameraRawFrameSpecRaw8;
static frameSpec frSpecRaw8_qvga;
static const u32 shavesToUse[SHAVES_USED] ={0,1,2,3,4};
void* confidenceThresholdRead(void*)
{
//    while ((usbLinkStream = XLinkOpenStream(0, CONFIDENCE_STREAM_NAME, 0)) == INVALID_STREAM_ID)
//    {
//        usleep(1);
//    }
//
//    while(1)
//    {
//        streamPacketDesc_t* packet;
//        XLinkReadData(usbLinkStream, &packet);
//        printf("Info: new confidence threshold received %x %d \n", packet, *(u32*)packet->data);
//        assert(sizeof(u32) == packet->length);
//        confidenceThresholdValue = *(u32*)packet->data;
//        XLinkReleaseData(usbLinkStream);
//    }
    return NULL;
}

void postprocessing(u8* dst, u8* src, u32 w, u32 h)
{
    const u32 med_k = 7;

    //printf("%p <- %p %d %d\n", dst, src, w, h);

    initMedHw(dst, src, w, h, med_k);
    runMedHw();
}

//void* preprocessFrame(void* msg)
void preprocessFrame(ImgFramePtr left, ImgFramePtr right, frameBuffer* warpedFrame)
{

    frameBuffer outFb[MV_STEREO_THREADS][STEREO_FRAMES];
    meshStruct mesh[MV_STEREO_THREADS][STEREO_FRAMES];
    tyTimeStamp stamp;
    u32 camIdx;
    u64 elapsed;
    u32 running, threadNo = 0;
    u32 statusSetup, statusOpen, statusFlush, statusClose, statusCleanup;
    u32 statusRun1, statusRun2, statusWait;
    u32 list_shaves_preproc[NB_SHAVES_THREAD_0];
    list_shaves_preproc[0] = SHAVE_THREAD_0;

    if (left.ptr->seqNo % MV_STEREO_THREADS == 0)
    {
        threadNo = 0;
    }
    else if (right.ptr->seqNo % MV_STEREO_THREADS == 1)
    {
        threadNo = 1;
    }

    /* --------------- 1. PREPROCESSING STEP ----------------
        --------------------------------------------------------- */
    for(camIdx = 0; camIdx < STEREO_FRAMES; camIdx++)
    {
        outFb[threadNo][camIdx].p2 = outFb[threadNo][camIdx].p3 = NULL;
        outFb[threadNo][camIdx].spec = cameraRawFrameSpecRaw8;
    }
    outFb[threadNo][0].p1 = (u8*)left.ptr->fb.p1;
    outFb[threadNo][1].p1 = (u8*)right.ptr->fb.p1;

    /* ----- 1.1. Calibration - warp both images using meshes ----- */
    OsDrvTimerStartTicksCount(&stamp);

    mvLog(MVLOG_DEBUG, "---warp both images: shv%ld\n", list_shaves_preproc[0]);
    for(camIdx = 0; camIdx < STEREO_FRAMES; camIdx++)
    {
        if (output == rectified)
        {
            warpedFrame[camIdx].p1 = (u8*)warpedOutputFrames[threadNo][camIdx];
        }
        else // raw
        {
            if(camIdx == 0) //TODO:ugly
                warpedFrame[camIdx].p1 = left.ptr->fb.p1;
            else
                warpedFrame[camIdx].p1 = right.ptr->fb.p1;
        }
        warpedFrame[camIdx].p2 = warpedFrame[camIdx].p3 = NULL;
        warpedFrame[camIdx].spec = cameraRawFrameSpecRaw8;

        mesh[threadNo][camIdx].meshWidth = MESH_WIDTH;
        mesh[threadNo][camIdx].meshHeight = MESH_HEIGHT;
        mesh[threadNo][camIdx].meshX = (float*) mesh_x[camIdx];
        mesh[threadNo][camIdx].meshY = (float*) mesh_y[camIdx];
    }

    statusSetup = OsDrvSvuSetupDynShaveApps(&MODULE_DATA(imageWarpDynlib), list_shaves_preproc, NB_SHAVES_THREAD_0);
    statusOpen = OsDrvSvuOpenShaves(list_shaves_preproc, NB_SHAVES_THREAD_0, OS_MYR_PROTECTION_SELF);

    // Warp Left
    statusRun1 = OsDrvSvuRunShaveAlgoOnAssignedShaveCC(&MODULE_DATA(imageWarpDynlib), list_shaves_preproc[0], "iiiii",
            &mesh[threadNo][RIGHT_CAMERA], &outFb[threadNo][RIGHT_CAMERA], &warpedFrame[RIGHT_CAMERA], tileNodes[threadNo][RIGHT_CAMERA], 0 );
    statusWait = OsDrvSvuDynWaitShaves(list_shaves_preproc, NB_SHAVES_THREAD_0, OS_DRV_SVU_WAIT_FOREVER, &running);

    // Warp right
    statusRun2 = OsDrvSvuRunShaveAlgoOnAssignedShaveCC(&MODULE_DATA(imageWarpDynlib), list_shaves_preproc[0], "iiiii",
            &mesh[threadNo][LEFT_CAMERA], &outFb[threadNo][LEFT_CAMERA], &warpedFrame[LEFT_CAMERA], tileNodes[threadNo][LEFT_CAMERA], 0 );
    statusWait = OsDrvSvuDynWaitShaves(list_shaves_preproc, NB_SHAVES_THREAD_0, OS_DRV_SVU_WAIT_FOREVER, &running);

    statusFlush = OsDrvShaveL2CachePartitionFlush((u32)partitions[1],PERFORM_INVALIDATION);
    statusClose = OsDrvSvuCloseShaves(list_shaves_preproc, NB_SHAVES_THREAD_0);
    statusCleanup = OsDrvSvuCleanupDynShaveApps(&MODULE_DATA(imageWarpDynlib));

    if (statusSetup || statusOpen || statusRun1 || statusRun2 || statusWait || statusFlush || statusClose || statusCleanup)
        printf("Error loading and running warp app... %ld %ld %ld %ld %ld %ld %ld %ld\n",
                statusSetup, statusOpen, statusRun1, statusRun2, statusWait, statusFlush, statusClose, statusCleanup);

    OsDrvTimerGetElapsedTicks(&stamp, &elapsed);
    PROF_PRINT("Calib runtime: %lld\n", elapsed);

    /* ----- 1.2. Downsample - create image pyramid with 2 levels ----- */
    //u8* originalSizeImages[STEREO_FRAMES] = { right.ptr->fb.p1, left.ptr->fb.p1};
    u8* originalSizeImages[STEREO_FRAMES] = {warpedFrame[RIGHT_CAMERA].p1, warpedFrame[LEFT_CAMERA].p1};

    u8* halfSizeImages[STEREO_FRAMES] = {downsampledBicubicBuffArea[threadNo][RIGHT_CAMERA], downsampledBicubicBuffArea[threadNo][LEFT_CAMERA]};

    mvLog(MVLOG_DEBUG, "---downsample orig %ld\n", list_shaves_preproc[0]);
    OsDrvTimerStartTicksCount(&stamp);


    statusSetup = OsDrvSvuSetupDynShaveApps(&MODULE_DATA(downsampleApp), list_shaves_preproc, NB_SHAVES_THREAD_0);
    statusOpen = OsDrvSvuOpenShaves(list_shaves_preproc, NB_SHAVES_THREAD_0, OS_MYR_PROTECTION_SELF);

    // Downsample Left
    statusRun1 = OsDrvSvuRunShaveAlgoOnAssignedShaveCC(&MODULE_DATA(downsampleApp), list_shaves_preproc[0], "iiii", originalSizeImages[0], halfSizeImages[0], MAX_WIDTH, MAX_HEIGHT);
    statusWait = OsDrvSvuDynWaitShaves(list_shaves_preproc, NB_SHAVES_THREAD_0, OS_DRV_SVU_WAIT_FOREVER, &running);
    statusFlush = OsDrvShaveL2CachePartitionFlush((u32)partitions[1],PERFORM_INVALIDATION);

    // Downsample Right
    statusRun2 = OsDrvSvuRunShaveAlgoOnAssignedShaveCC(&MODULE_DATA(downsampleApp), list_shaves_preproc[0], "iiii", originalSizeImages[1], halfSizeImages[1], MAX_WIDTH, MAX_HEIGHT);
    statusWait = OsDrvSvuDynWaitShaves(list_shaves_preproc, NB_SHAVES_THREAD_0, OS_DRV_SVU_WAIT_FOREVER, &running);
    statusFlush = OsDrvShaveL2CachePartitionFlush((u32)partitions[1],PERFORM_INVALIDATION);

    statusClose = OsDrvSvuCloseShaves(list_shaves_preproc, NB_SHAVES_THREAD_0);
    statusCleanup = OsDrvSvuCleanupDynShaveApps(&MODULE_DATA(downsampleApp));

    OsDrvTimerGetElapsedTicks(&stamp, &elapsed);
    PROF_PRINT("Downsample runtime: %lld\n", elapsed);

    if (statusSetup || statusOpen || statusRun1 || statusRun2 || statusWait || statusFlush || statusClose || statusCleanup)
        printf("Error loading and running downsample app... %ld %ld %ld %ld %ld %ld %ld %ld\n",
                statusSetup, statusOpen, statusRun1, statusRun2, statusWait, statusFlush, statusClose, statusCleanup);

    /* ----- 1.3. Cost - compute cost cube (input for sgbm aggregation) ----- */
    stereoMatImage_t leftQvgaImg, rightQvgaImg;
    leftQvgaImg.data    = downsampledBicubicBuffArea[threadNo][LEFT_CAMERA];
    leftQvgaImg.w       = MAX_WIDTH / SCALE_FACTOR;
    leftQvgaImg.h       = MAX_HEIGHT / SCALE_FACTOR;
    leftQvgaImg.step    = MAX_WIDTH / SCALE_FACTOR;

    rightQvgaImg.data    = downsampledBicubicBuffArea[threadNo][RIGHT_CAMERA];
    rightQvgaImg.w       = MAX_WIDTH / SCALE_FACTOR;
    rightQvgaImg.h       = MAX_HEIGHT / SCALE_FACTOR;
    rightQvgaImg.step    = MAX_WIDTH / SCALE_FACTOR;

    OsDrvTimerStartTicksCount(&stamp);

    statusSetup = OsDrvSvuSetupDynShaveApps(&MODULE_DATA(costApp), list_shaves_preproc, NB_SHAVES_THREAD_0);
    statusOpen = OsDrvSvuOpenShaves(list_shaves_preproc, NB_SHAVES_THREAD_0, OS_MYR_PROTECTION_SELF);

    statusRun1 = OsDrvSvuRunShaveAlgoOnAssignedShaveCC(&MODULE_DATA(costApp), list_shaves_preproc[0],
            "iiii", &algoCfg, &leftQvgaImg, &rightQvgaImg, combinedCostArea[threadNo]);
    statusWait = OsDrvSvuDynWaitShaves(list_shaves_preproc, NB_SHAVES_THREAD_0, OS_DRV_SVU_WAIT_FOREVER, &running);
    statusFlush = OsDrvShaveL2CachePartitionFlush((u32)partitions[1],PERFORM_INVALIDATION);

    statusClose = OsDrvSvuCloseShaves(list_shaves_preproc, NB_SHAVES_THREAD_0);
    statusCleanup = OsDrvSvuCleanupDynShaveApps(&MODULE_DATA(costApp));

    OsDrvTimerGetElapsedTicks(&stamp, &elapsed);
    PROF_PRINT("--- Cost runtime for QVGA: %f ms\n", elapsed/600000.0);

    if (statusSetup || statusOpen || statusRun1 || statusWait || statusFlush || statusClose || statusCleanup)
        printf("Error loading and running cost app...\n");

    mvLog(MVLOG_DEBUG, "in %p %p, w %p %p -> %p %p -> cost %p\n", right.ptr->fb.p1, left.ptr->fb.p1,
            warpedFrame[RIGHT_CAMERA].p1, warpedFrame[LEFT_CAMERA].p1,
            downsampledBicubicBuffArea[threadNo][RIGHT_CAMERA], downsampledBicubicBuffArea[threadNo][LEFT_CAMERA],
            combinedCostArea[threadNo]);
    //return NULL;
}



void* plgVisionStereo::pluginSpecificInit(void)
{
    mvLogLevelSet(MVLOG_WARN);
    confidenceThresholdValue = DEFAULT_CONFIDENCE_VAL;
    static int threadCount = 0;

    sem_init(&global_sem1, 0, 1);
    sem_init(&global_sem2, 0, 1);

    if (threadCount % MV_STEREO_THREADS == 0)
    {
        // Create confidence threshold stream once
        pthread_attr_t attr;
        if(pthread_attr_init(&attr) !=0) {
            mvLog(MVLOG_ERROR, "pthread_attr_init error");
        }
        if(pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0) {
            mvLog(MVLOG_ERROR, "pthread_attr_setinheritsched error");
        }
        if(pthread_attr_setschedpolicy(&attr, SCHED_FIFO) != 0) {
            mvLog(MVLOG_ERROR, "pthread_attr_setschedpolicy error");
        }

        int sc=pthread_create( &confidenceThresholdThread, &attr, confidenceThresholdRead, NULL);
        if (sc) {
            mvLog(MVLOG_ERROR, "Thread confidence creation failed");
        }
    }

    OsDrvSvuInit();
    threadCount++;

    return NULL;
}


void* plgVisionStereo::pluginSpecificProcess()
{
    assert(frameToProcess[0]->ptr->fb.spec.width <= MAX_WIDTH && frameToProcess[0]->ptr->fb.spec.height <= MAX_HEIGHT);
    u32 threadNo = 0;
    u64 ticks_start;


    // configuration for VGA disparity map
    cameraRawFrameSpecRaw8.type    = RAW8;
    cameraRawFrameSpecRaw8.height  = frameToProcess[0]->ptr->fb.spec.height;
    cameraRawFrameSpecRaw8.width = frameToProcess[0]->ptr->fb.spec.width;
    cameraRawFrameSpecRaw8.stride  = frameToProcess[0]->ptr->fb.spec.width;
    cameraRawFrameSpecRaw8.bytesPP = 1;

    frSpecRaw8_qvga.type    = RAW8;
    frSpecRaw8_qvga.height  = frameToProcess[0]->ptr->fb.spec.height / SCALE_FACTOR;
    frSpecRaw8_qvga.width = frameToProcess[0]->ptr->fb.spec.width / SCALE_FACTOR;
    frSpecRaw8_qvga.stride  = frameToProcess[0]->ptr->fb.spec.width / SCALE_FACTOR;
    frSpecRaw8_qvga.bytesPP = 1;

    algoCfg.frame = &cameraRawFrameSpecRaw8;
    algoCfg.maxDisparities = DISP; /* maximum disparity; default value = MAX_DISPARITIES_LEVEL1 */
    algoCfg.confidenceThreshold = DEFAULT_CONFIDENCE_VAL; /* confidence threshold; default value = DEFAULT_CONFIDENCE_VAL */
    for(int i =0; i<SHAVES_USED; i++) {
        algoCfg.list_of_shaves_thread_1[i] = shavesToUse[i];
    }
    for(int i =0; i<2;i++){
        for(int j=0;j<SHAVES_USED;j++) {
            algoCfg.aggCostPaths[i][j] = NULL;
        }
    }

    OsDrvTimerGetSystemTicks64(&ticks_start);
    mvLog(MVLOG_DEBUG, "%d. %lld\n", frameToProcess[0]->ptr->seqNo, frameToProcess[0]->ptr->ts);

    if (frameToProcess[0]->ptr->seqNo % MV_STEREO_THREADS == 0)
    {
        threadNo = 0;
    }
    else if (frameToProcess[0]->ptr->seqNo % MV_STEREO_THREADS == 1)
    {
        threadNo = 1;
    }

    sem_wait(&global_sem1);

    /* ---------- 1. Run preprocessing ----------------------
    --------------------------------------------------------- */
    frameBuffer warpedFrame[STEREO_FRAMES];
    preprocessFrame(*frameToProcess[0], *frameToProcess[1], &warpedFrame[0]);
    sem_post(&global_sem1);

    /* --------------- 2. Run disparity map-- -----------------
    --------------------------------------------------------- */
    sem_wait(&global_sem2);
    algoCfg.confidenceThreshold = confidenceThresholdValue;
    frameMeta *pfm[2] = {
        (frameMeta*)(metaOut[0]->ptr->fb.p1),
        (frameMeta*)(metaOut[1]->ptr->fb.p1)
    };
    frameThroughMeta *p_ftm = (frameThroughMeta*)(&pfm[0]->customMetadata[0]);


    u8* disparityBuff = p_ftm->frameData;
    u8* depthBuff;

    metaOut[0]->ptr->size = sizeof(frameSpec) + (frameToProcess[0]->ptr->fb.spec.width * frameToProcess[0]->ptr->fb.spec.height);
#ifdef V4
    p_ftm->spec = frameToProcess[0]->ptr->fb.spec;
#else
    p_ftm->spec = frSpecRaw8_qvga;
#endif
    p_ftm = (frameThroughMeta*)(pfm[1]->customMetadata);
    depthBuff = p_ftm->frameData;

    UNUSED(depthBuff);

    metaOut[1]->ptr->size = sizeof(frameSpec) + (frameToProcess[0]->ptr->fb.spec.width * frameToProcess[0]->ptr->fb.spec.height);

    // depth map will be computed on PC side
    p_ftm->spec.width = 0;
    p_ftm->spec.height = 0;

    runDisparityMap(&algoCfg, partitions[1],
                    warpedFrame[LEFT_CAMERA].p1, warpedFrame[RIGHT_CAMERA].p1,
                    downsampledBicubicBuffArea[threadNo][LEFT_CAMERA], downsampledBicubicBuffArea[threadNo][RIGHT_CAMERA],
                    combinedCostArea[threadNo],
#ifdef V4
                    predictedDisparityBuffArea[threadNo],                   // QVGA disparity map
                    (outputType*)(disparityBuff), // VGA disparity map
#else
                    (outputType*)(disparityBuff), // QVGA disparity map
                    lutBuffArea[threadNo][LEFT_CAMERA],                     // VGA disparity map - not used
#endif
                    confidenceBuffArea[threadNo],
                    NULL, NULL,
                    0, threadNo);
    DrvLL2CFlushOpOnAllLines(LL2C_OPERATION_INVALIDATE, 0);

    /* --------------- 3. POSTPROCESSING --------------------
        --------------------------------------------------------- */
    mvLog(MVLOG_DEBUG, "%p %p -> %p\n", warpedFrame[LEFT_CAMERA].p1, warpedFrame[RIGHT_CAMERA].p1, (outputType*)(disparityBuff));
#if 0
#ifdef V4
    // convert conv u16 result to u8 for median
    runDisparityMap(&algoCfg, partitions[1],
                warpedFrame[LEFT_CAMERA].p1, warpedFrame[RIGHT_CAMERA].p1,
                downsampledBicubicBuffArea[threadNo][LEFT_CAMERA], downsampledBicubicBuffArea[threadNo][RIGHT_CAMERA],
                combinedCostArea[threadNo],
                predictedDisparityBuffArea[threadNo],
                (outputType*)(disparityBuff),
                confidenceBuffArea[threadNo],
                (outputType*)(disparityBuff),
                disparityMedianIntegerBuffArea[threadNo],
                1, threadNo);

    // run median filter HW block
    postprocessing((u8*)disparityMedianIntegerBuffAreaFinal[threadNo], (u8*)disparityMedianIntegerBuffArea[threadNo], MAX_WIDTH, MAX_HEIGHT);
    rtems_cache_flush_data_range(&disparityMedianIntegerBuffAreaFinal[0], MAX_WIDTH * MAX_HEIGHT);

    // combine median_u8 + disparity_u16 on SHAVE
    runDisparityMap(&algoCfg, partitions[1],
            warpedFrame[LEFT_CAMERA].p1, warpedFrame[RIGHT_CAMERA].p1,
            downsampledBicubicBuffArea[threadNo][LEFT_CAMERA], downsampledBicubicBuffArea[threadNo][RIGHT_CAMERA],
            combinedCostArea[threadNo],
            predictedDisparityBuffArea[threadNo],
            disparityBuffArea[threadNo],//(outputType*)(meta[0]->customMetadata),//disparityBuffArea[threadNo],
            confidenceBuffArea[threadNo],
            disparityMedianIntegerBuffAreaFinal[threadNo],
            disparityBuffArea[threadNo],//(outputType*)(meta[0]->customMetadata),//disparityBuffArea[threadNo],
            2, threadNo);
#endif
#endif

    u64 ticks_end;
    OsDrvTimerGetSystemTicks64(&ticks_end);
    //printf("Total stereo runtime: %f ms\n", (ticks_end - ticks_start) / 600000.0);
    sem_post(&global_sem2);

    return NULL;
}
