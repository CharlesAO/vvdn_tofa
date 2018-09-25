///
/// @file
/// @copyright All code copyright Movidius Ltd 2018, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     vTrack PC wrapper
///

/// compiler includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include <iostream>
#include <fstream>
// vTrackModules
#include "vTrack.h"
#include "vPipe.h"
#include "global_constants.h"
#include "pc_constants.h"
#include "featuresDrawer.h"
//openCv includes
#include <highgui.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/opencv.hpp>
//common components
#include <superFrame.h>
#include <sys/time.h>
///  Local Modules
#include <pcVtrack.h>

//TODO: clean header files


/// Namespace
using namespace std;


/// Special functions for double buffering
#define CURR(ring_buffer) ring_buffer[(vpipeFrames + 0) % 2]
#define NEXT(ring_buffer) ring_buffer[(vpipeFrames + 1) % 2]
#define WEB_CAM

#define MAX_FEATURES 896
static t_vpVgaCmxHeap vpCmxHeap __attribute__((aligned(0x1000)));
static t_vpVgaDdrHeap vpDdrHeap __attribute__((aligned(0x1000)));
static t_vpCmxInternals<MAX_FEATURES> vpBuffs __attribute__((aligned(0x1000)));
static t_vpVgaPpBuffs vpPpBuffs __attribute__((aligned(0x1000)));
static t_vpMestBuffs vpMestBuffs __attribute__((aligned(0x1000)));

///////////////////////////////////////////////////////////////////////////////
///  Main Buffers
///////////////////////////////////////////////////////////////////////////////

// Just for cross compilability
#define HEAP_SIZE (100 * 1024)
static uint8_t heap[HEAP_SIZE];
t_vPipeShaveConfig shaveCfg;
#define MIN_DISTANCE_SQUARED 3
#define NUMBER_OF_SHAVE_THREADS 3
Vpipe vp;

PcVtrack::PcVtrack(){
}
PcVtrack::PcVtrack(VTRACK_OF_MODE ofType)
    : PcVtrack()
{
    ofMode = ofType;
}
void
PcVtrack::configure(const std::map<std::string, std::string> &options) {

}
void
PcVtrack::getConfigSerialization(char** data, int* size) {
    char* serializedData;
    serializedData = (char*) malloc(64);
    serializedData[0] = 0xAA;
    if(ofMode == OF_MOVI){
        serializedData[1] = 0;
    }else{
        serializedData[1] = 1;
    }
    *data = serializedData;
    *size = 64;
}

int
PcVtrack::visualize() {

}
int
PcVtrack::visualizeKPI() {

}
void
PcVtrack::initAlgorithm() {

    vPipeInitDefaultHarrisRuntimeParameters(&params);

    params.lostFeatureError = 50000;
    params.thresholdMin = 6000000.0;
    params.minDistanceSquared = MIN_DISTANCE_SQUARED;
    params.minHarrisResponseOldFeatures = 200000;
    initBuffers();
    isInitialized = false;
}

PcVtrack::~PcVtrack() {
}

int PcVtrack::runAlgOnFrame(VisionFrame* frameL, VisionFrame* frameR, void* resultsPtr){
    inputImage[0].spec.width = frameL->width;
    inputImage[0].spec.height = frameL->height;
    inputImage[0].spec.bytesPP = frameL->bpp;
    inputImage[0].spec.stride = frameL->stride;
    inputImage[0].spec.type = RAW8;
    inputImage[0].p1 = frameL->p1;
    inputImage[0].p2 = frameL->p2;
    inputImage[0].p3 = frameL->p3;
    if(!isInitialized){
        if(ofMode == OF_MOVI)
        {
            vPipeInitDefaultAlgoCfg(&algoCfg, &inputImage[0].spec);
            algoCfg.targetNumFeatures = 768;
            algoCfg.maxNumFeatures = 896;
        }
        else if(ofMode == OF_MEST)
        {
            vPipeInitDefaultAlgoMESTCfg(&algoCfg, &inputImage[0].spec);
            algoCfg.targetNumFeatures = 768;
            algoCfg.maxNumFeatures = 896;
        }
        else
            assert(0);
        assert(algoCfg.maxNumFeatures <= MAX_FEATURES);
        vp.vPipeInit(&vpResCfg, VPIPE_MODE_PP_FM_OF, &algoCfg);
        isInitialized = true;
    }
    t_vTrackResultConfig resultCfg = vp.vPipeGetResultConfig();
    uint32_t sizeBuff = bulkResult.setConfigAndPtrs(resultCfg, (uint8_t*) resultsPtr);
    assert(sizeBuff < FRAME_CUSTOM_META_SIZE);
    int status = vp.vPipeRun(&params, &inputImage[0], NULL, &bulkResult);
    t_vTrackResultSF* resultSF;
    bulkResult.getConfigAndResults(NULL, &resultSF);
    resultSF->header.frameId = 0;

    struct timespec timestamp;
    clock_gettime(CLOCK_REALTIME, &timestamp);
    resultSF->header.timestampNs = (timestamp.tv_sec * 1e9 + timestamp.tv_nsec);
//    memcpy(resultsPtr, (void*) resultSF, sizeof(t_vTrackResultSF));
    return sizeof(resultSF);
}

void
PcVtrack::initBuffers()
{
    memset((void*)&shaveCfg, 0, sizeof(shaveCfg));
    vpResCfg.vp_shaveCfg = &shaveCfg;

    vpResCfg.vp_memCfg.vpCmxBuffs = vpBuffs.buf;
    vpResCfg.vp_memCfg.vpCmxHeap  = vpCmxHeap;
    vpResCfg.vp_memCfg.vpDdrHeap  = vpDdrHeap;
    vpResCfg.vp_memCfg.vpPpBuffs = vpPpBuffs;
    vpResCfg.vp_memCfg.vpMestBuffs = vpMestBuffs;

    vpResCfg.vp_cacheCfg.pp_part_instr = 0;
    vpResCfg.vp_cacheCfg.pp_part_data = 0;
    vpResCfg.vp_cacheCfg.of_part_instr = 0;
    vpResCfg.vp_cacheCfg.of_part_data = 0;
    vpResCfg.vp_cacheCfg.fm_part_instr = 0;
    vpResCfg.vp_cacheCfg.fm_part_data = 0;

    vpResCfg.vp_fifoCfg.fifo1 = 0;
    vpResCfg.vp_fifoCfg.fifo2 = 1;
    vpResCfg.vp_fifoCfg.fifo3 = 2;
    vpResCfg.vp_fifoCfg.fifo4 = 4;
}
