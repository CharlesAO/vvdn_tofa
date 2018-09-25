///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief  Vtrack main pipeline
///

#include <cstdio>
#include <new>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "mv_types.h"
// common includes
#include "vPipe.h"
#include "vPipePrivateTypes.h"
#include "vPipeImpl.h"

inline static void defaultHarrisConfig(t_vPipeAlgoCfg* algoCfg);
inline static void defaultShiTomasiConfig(t_vPipeAlgoCfg* algoCfg);

// public funcs
void vPipeInitDefaultAlgoCfg(t_vPipeAlgoCfg* algoCfg, frameSpec* inputFrameSpec)
{
    algoCfg->windowHeight = 5;
    algoCfg->windowWidth = 5;
    algoCfg->cam.camCenterX = 312.417;
    algoCfg->cam.camCenterY = 245.933;
    algoCfg->maxOfIterations  = 9;
    algoCfg->epsilon = 0.01;
    algoCfg->numPyrLevels = 0;
    algoCfg->cam.fov = 45;
    algoCfg->inputFrameSpec = inputFrameSpec;
    algoCfg->cellGridDimension = 4;
    algoCfg->targetNumFeatures = 320;
    algoCfg->maxNumFeatures = 480;
    algoCfg->pyrCfg = PYRAMID_ON;
    algoCfg->ofCfg = OF;
    algoCfg->outputType = PP_PLUS_TRACKED;
    defaultHarrisConfig(algoCfg);
    algoCfg->corCfg.insertAndSort = true;
}

void vPipeInitDefaultAlgoShiTomasiCfg(t_vPipeAlgoCfg* algoCfg, frameSpec* inputFrameSpec)
{
    algoCfg->windowHeight = 5;
    algoCfg->windowWidth = 5;
    algoCfg->cam.camCenterX = 312.417;
    algoCfg->cam.camCenterY = 245.933;
    algoCfg->maxOfIterations  = 9;
    algoCfg->epsilon = 0.01;
    algoCfg->numPyrLevels = 0;
    algoCfg->cam.fov = 45;
    algoCfg->inputFrameSpec = inputFrameSpec;
    algoCfg->cellGridDimension = 4;
    algoCfg->targetNumFeatures = 320;
    algoCfg->maxNumFeatures = 480;
    algoCfg->pyrCfg = PYRAMID_ON;
    algoCfg->ofCfg = OF;
    algoCfg->outputType = PP_PLUS_TRACKED;
    defaultShiTomasiConfig(algoCfg);
    algoCfg->corCfg.insertAndSort = true;
}

void vPipeInitDefaultAlgoMESTCfg(t_vPipeAlgoCfg* algoCfg, frameSpec* inputFrameSpec)
{
    algoCfg->windowHeight = 5;
    algoCfg->windowWidth = 5;
    algoCfg->cam.camCenterX = 312.417;
    algoCfg->cam.camCenterY = 245.933;
    algoCfg->maxOfIterations  = 9;
    algoCfg->epsilon = 0.01;
    algoCfg->numPyrLevels = 0;
    algoCfg->cam.fov = 45;
    algoCfg->inputFrameSpec = inputFrameSpec;
    algoCfg->cellGridDimension = 1;
    algoCfg->targetNumFeatures = 1024;
    algoCfg->maxNumFeatures = inputFrameSpec->width*inputFrameSpec->height/4;
    algoCfg->pyrCfg = PYRAMID_OFF;
    algoCfg->ofCfg = MEST;
    algoCfg->outputType = TRACKED;
    defaultHarrisConfig(algoCfg);
    algoCfg->corCfg.insertAndSort = false;
}

void vPipeInitDefaultHarrisRuntimeParameters(t_vPipeParams* params)
{
    // NOT TUNED YET FOR SHI_TOMASI
    u32 i;
    for (i = 0; i < MAX_NUM_CELLS; i++)
    {
        params->harrisThresholds[i] = HARRIS_THRESHOLD_AUTO;
    }
    params->thresholdDecreaseVelocity = 0.9;
    params->thresholdIncreaseVelocity = 1.1;
    params->thresholdMax = 1000000000000000.0;
    params->thresholdMin = 1000000.0;
    params->lostFeatureError = 100;
    params->minDistanceSquared = 1;
    params->minHarrisResponseOldFeatures = 100;
}

Vpipe::Vpipe() {
    if(posix_memalign((void**)&p_impl, 4096, sizeof(VpipeImpl)))
    {
        puts("Could not allocate at aligned address!");
        assert(0);
    }
    new (p_impl) VpipeImpl();
}

Vpipe::~Vpipe() {
    delete p_impl;
}
void
Vpipe::vPipeCapture(t_vPipeCapt captureMode)
{
    return p_impl->vPipeCapture(captureMode);
}

int
Vpipe::vPipeInit(t_vPipeResourceCfg* vResourceCfg,volatile t_vPipeMode vp_mode,
              t_vPipeAlgoCfg* algoCfg)
{
    return p_impl->vPipeInit(vResourceCfg, vp_mode, algoCfg);
}

int
Vpipe::vPipeRun(volatile t_vPipeParams *vp_params,
        frameBuffer *in_frame,
        float (*rpy)[3],
        vTrackBulkResult* bulkResult)
{
   return p_impl->vPipeRun(vp_params, in_frame, rpy, bulkResult);
}

t_vTrackResultConfig Vpipe::vPipeGetResultConfig(){
    return p_impl->vPipeGetResultConfig();
}

static void defaultHarrisConfig(t_vPipeAlgoCfg* algoCfg) {

    algoCfg->harrisThrInitValue = 10000000;
    algoCfg->corCfg.alg = HARRIS;
    algoCfg->corCfg.useSobel = 1;
}

static void defaultShiTomasiConfig(t_vPipeAlgoCfg* algoCfg) {

    algoCfg->harrisThrInitValue = 2000;
    algoCfg->corCfg.alg = SHI_TOMASI;
    algoCfg->corCfg.useSobel = 0;
}
