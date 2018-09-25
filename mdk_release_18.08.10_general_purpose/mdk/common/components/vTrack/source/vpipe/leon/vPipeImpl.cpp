///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief  Vtrack main pipeline
///



// common includes
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <mv_types.h>
#include <swcLeonMath.h>

#include "assert.h"
#include "time.h"

// project includes
#include "global_constants.h"
#ifndef __PC__
#include "vTrackPrivate.h"
#endif
#include "vPipeHelpers.h"
#include "vPipe.h"
#include "vPipePrivateTypes.h"
#include "vPipeImpl.h"

#define LOST_FEAT_ERR_THRESH 20000          // OpenCV OF Error Threshold: 2000

// Transition table for all modes. This table will show the next state for each state
// If a state should not happen in a mode, the next state is always VPIPE_STATE_ERROR
t_vPipeState vPipeTransitionTable[VPIPE_MODE_LAST][VPIPE_STATE_LAST] =
{
    // VPIPE_MODE_PP
    {
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_NOT_INIT
        VPIPE_STATE_RUN_PIXEL_PIPE, // Next state for VPIPE_STATE_INIT
        VPIPE_STATE_RUN_FM, // Next state for VPIPE_STATE_RUN_PIXEL_PIPE
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_RUN_GYRO_PREDICT
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_RUN_OF
        VPIPE_STATE_FILL_OUTPUT, // Next state for VPIPE_STATE_RUN_FM
        VPIPE_STATE_DONE, // Next state for VPIPE_STATE_FILL_OUTPUT
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_FILL_DUMMY_OUTPUT
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_ERROR
        VPIPE_STATE_INIT, // Next state for VPIPE_STATE_DONE
    },
    // VPIPE_MODE_PP_FM
    {
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_NOT_INIT
        VPIPE_STATE_RUN_PIXEL_PIPE, // Next state for VPIPE_STATE_INIT
        VPIPE_STATE_RUN_GYRO_PREDICT, // Next state for VPIPE_STATE_RUN_PIXEL_PIPE
        VPIPE_STATE_RUN_FM, // Next state for VPIPE_STATE_RUN_GYRO_PREDICT
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_RUN_OF
        VPIPE_STATE_FILL_OUTPUT, // Next state for VPIPE_STATE_RUN_FM
        VPIPE_STATE_DONE, // Next state for VPIPE_STATE_FILL_OUTPUT
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_FILL_DUMMY_OUTPUT
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_ERROR
        VPIPE_STATE_INIT, // Next state for VPIPE_STATE_DONE
    },
    // VPIPE_MODE_PP_FM_OF
    {
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_NOT_INIT
        VPIPE_STATE_RUN_PIXEL_PIPE, // Next state for VPIPE_STATE_INIT
        VPIPE_STATE_RUN_GYRO_PREDICT, // Next state for VPIPE_STATE_RUN_PIXEL_PIPE
        VPIPE_STATE_RUN_OF, // Next state for VPIPE_STATE_RUN_GYRO_PREDICT
        VPIPE_STATE_RUN_FM, // Next state for VPIPE_STATE_RUN_OF
        VPIPE_STATE_FILL_OUTPUT, // Next state for VPIPE_STATE_RUN_FM
        VPIPE_STATE_DONE, // Next state for VPIPE_STATE_FILL_OUTPUT
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_FILL_DUMMY_OUTPUT
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_ERROR
        VPIPE_STATE_INIT, // Next state for VPIPE_STATE_DONE
    },
    // VPIPE_MODE_DUMMY
    {
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_NOT_INIT
        VPIPE_STATE_FILL_DUMMY_OUTPUT, // Next state for VPIPE_STATE_INIT
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_RUN_PIXEL_PIPE
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_RUN_GYRO_PREDICT
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_RUN_OF
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_RUN_FM
        VPIPE_STATE_DONE, // Next state for VPIPE_STATE_FILL_OUTPUT
        VPIPE_STATE_FILL_OUTPUT, // Next state for VPIPE_STATE_FILL_DUMMY_OUTPUT
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_ERROR
        VPIPE_STATE_INIT, // Next state for VPIPE_STATE_DONE
    },
    // VPIPE_MODE_GYRO_ASSIST_TEST
    {
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_NOT_INIT
        VPIPE_STATE_RUN_PIXEL_PIPE, // Next state for VPIPE_STATE_INIT
        VPIPE_STATE_RUN_GYRO_PREDICT, // Next state for VPIPE_STATE_RUN_PIXEL_PIPE
        VPIPE_STATE_RUN_FM, // Next state for VPIPE_STATE_RUN_GYRO_PREDICT
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_RUN_OF
        VPIPE_STATE_FILL_OUTPUT, // Next state for VPIPE_STATE_RUN_FM
        VPIPE_STATE_DONE, // Next state for VPIPE_STATE_FILL_OUTPUT
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_FILL_DUMMY_OUTPUT
        VPIPE_STATE_ERROR, // Next state for VPIPE_STATE_ERROR
        VPIPE_STATE_INIT, // Next state for VPIPE_STATE_DONE
    },
};

// Local functions
static u32 timespec_diff_ns(struct timespec *start, struct timespec *stop)
{
    if ((stop->tv_nsec - start->tv_nsec) < 0) {
        start->tv_sec = stop->tv_sec - start->tv_sec - 1;
        start->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
    } else {
        start->tv_sec = stop->tv_sec - start->tv_sec;
        start->tv_nsec = stop->tv_nsec - start->tv_nsec;
    }

    return start->tv_nsec + (start->tv_sec*1000000000);
}

// Public member functions



int
VpipeImpl::vPipeInit(t_vPipeResourceCfg* vResourceCfg,volatile t_vPipeMode vp_mode,
              t_vPipeAlgoCfg* algoCfg)
{
    if (vp_mode == VPIPE_MODE_PP || vp_mode == VPIPE_MODE_GYRO_ASSIST_TEST)
    {
        algoCfg->outputType=PP_ONLY;
    }
    u32 i;

    globalShaveCfg = vResourceCfg->vp_shaveCfg;
#ifndef __PC__
    //Turn off shaves
    // pp shave
    localTurnOffShave(globalShaveCfg->ppShaveNum);
    // gauss shave
    for(uint32_t i = 0; i<globalShaveCfg->gaussNoShaves; i++) {
        localTurnOffShave(globalShaveCfg->gaussShaveList[i]);
    }
    // corner shave
    for(uint32_t i = 0; i<globalShaveCfg->cornerNoShaves; i++) {
        localTurnOffShave(globalShaveCfg->cornerShaveList[i]);
    }
    // of shaves
    for(uint32_t i = 0; i<globalShaveCfg->ofNoShaves; i++) {
        localTurnOffShave(globalShaveCfg->ofShaveList[i]);
    }
    // fm shave
    localTurnOffShave(globalShaveCfg->fmShaveNum);
#endif
    // Setup the total number of pyramid levels based on resolution
    if( algoCfg->numPyrLevels == 0 ){
        if( algoCfg->inputFrameSpec->width == 640 )
            vPipe.num_pyr_levels = 3;
        else if( algoCfg->inputFrameSpec->width == 1280)
            vPipe.num_pyr_levels = 4;
        else
        {
            vPipe.currentState = VPIPE_STATE_ERROR;
            return -1;
        }
    } else{
        vPipe.num_pyr_levels = algoCfg->numPyrLevels;
    }
    vPipe.pix_per_deg = algoCfg->inputFrameSpec->width / algoCfg->cam.fov;
    vPipe.deg_per_pix = 1.0 / vPipe.pix_per_deg;
    vPipe.camCenterX = algoCfg->cam.camCenterX;
    vPipe.camCenterY = algoCfg->cam.camCenterY;



    cellGridDimension = algoCfg->cellGridDimension;
    nCells = cellGridDimension*cellGridDimension;//needs to be before allocateInternalBuffers
    maxNumFeatures = algoCfg->maxNumFeatures; //needs to be before allocateInternal Buffer
    targetNumFeatures = algoCfg->targetNumFeatures;
    assert(targetNumFeatures<=maxNumFeatures);
    maxCellFeatures = maxNumFeatures/nCells;

    // Allocate pyramid buffers
    t_vpMemCfg* memCfg = &(vResourceCfg->vp_memCfg);

    initialiseHeap(algoCfg->inputFrameSpec,
                   pyramid,N_PYRS,vPipe.num_pyr_levels + 1,
                   CMX_PYR_LEVELS, pyrImages,
                   memCfg->vpDdrHeap,memCfg->vpCmxHeap);

    allocateInternalBuffers(memCfg->vpCmxBuffs,
                            maxNumFeatures);

    if(algoCfg->ofCfg == MEST) {
        allocateMestBuffers(memCfg->vpMestBuffs,
                            maxNumFeatures);
    }

    for (i = 0; i < nCells; i++)
    {
        harrisThresholds[i] = algoCfg->harrisThrInitValue;
    }


    t_pPipeShaveConfig* ppShave = &ppShaveCfg;
    ppShave->gaussNoShaves = vResourceCfg->vp_shaveCfg->gaussNoShaves;
    ppShave->gaussShaveList = vResourceCfg->vp_shaveCfg->gaussShaveList;
    ppShave->cornerNoShaves = vResourceCfg->vp_shaveCfg->cornerNoShaves;
    ppShave->cornerShaveList = vResourceCfg->vp_shaveCfg->cornerShaveList;
    ppShave->ppShaveNum = &vResourceCfg->vp_shaveCfg->ppShaveNum;

    fifoCfg.fifo1 =  vResourceCfg->vp_fifoCfg.fifo1;
    fifoCfg.fifo2 =  vResourceCfg->vp_fifoCfg.fifo2;
    fifoCfg.fifo3 =  vResourceCfg->vp_fifoCfg.fifo3;
    fifoCfg.fifo4 =  vResourceCfg->vp_fifoCfg.fifo4;

    t_pPipeResourceCfg* ppResource = &pixelPipeResource;
    ppResource->shaveConfig = ppShave;

    ppResource->ppCacheData = vResourceCfg->vp_cacheCfg.pp_part_data;
    ppResource->ppCacheInstr = vResourceCfg->vp_cacheCfg.pp_part_instr;
    ppResource->fifoCfg = fifoCfg;
    ppResource->ppBuffs = vResourceCfg->vp_memCfg.vpPpBuffs.ptr;
    ppResource->ppBuffsSize = vResourceCfg->vp_memCfg.vpPpBuffs.size;
    ppResource->ppCmxBuffs = vResourceCfg->vp_memCfg.vpPpCmxBuffs.ptr;
    ppResource->ppCmxBuffsSize = vResourceCfg->vp_memCfg.vpPpCmxBuffs.size;

    pp.ppInit(ppResource, algoCfg->pyrCfg, algoCfg->corCfg);

    ofCfg.pyrLevels = vPipe.num_pyr_levels;
    ofCfg.termCriteria.max_iter = algoCfg->maxOfIterations;
    ofCfg.termCriteria.epsilon = algoCfg->epsilon;
    ofCfg.winSize.width = algoCfg->windowWidth;
    ofCfg.winSize.height = algoCfg->windowHeight;
    if(algoCfg->ofCfg == MEST) {
        ofCfg.algoType = OF_MEST;
#if defined(MA2150) || defined(MA2450) // no low power mode before MA2480
        vPipe.currentState = VPIPE_STATE_ERROR;
        assert(0 && "MEST mode not supported in MA2x5x or older!");
#endif
    } else
        ofCfg.algoType = OF_MOVI;
    ofCfg.subpixelDetection = 0;
    ofCfg.maxNumFeatures = algoCfg->maxNumFeatures;

    ofResourceCfg_t temp_of;
    temp_of.numShaves = globalShaveCfg->ofNoShaves;
    temp_of.shaveList = globalShaveCfg->ofShaveList;
    temp_of.cachePartData = vResourceCfg->vp_cacheCfg.of_part_data;
    temp_of.cachePartInstr = vResourceCfg->vp_cacheCfg.of_part_instr;
    of.ofInit(&ofCfg, &temp_of);

    if (algoCfg->outputType == PP_ONLY || algoCfg->outputType == TRACKED) {
        //The below line is counter intuitive.
        //In PP_ONLY mode, FM only runs on the PP points to get them in the
        //correct output format.
        //In TRACKED mode, FM only runs on the PP points to get them in the
        //correct format for the next frame. Doesn't run on the tracked points
        //as the vTrack output points directly to the OF output which is already in
        //the correct format for vTrackResultsSF
        fmSetupCfg.mode = FM_PP_ONLY;
        fmSetupCfg.filterFeatures = false;

    } else {
        fmSetupCfg.mode = FM_PP_AND_TRACKED;
        fmSetupCfg.filterFeatures = true;
    }
    fmSetupCfg.featureIdCounter = featureIdCounter;
    fmSetupCfg.featuresCount = featuresCount;
    fmSetupCfg.featureCells = featureCells;
    fmSetupCfg.features = features;
    fmSetupCfg.featuresMetadata = featureMetadata;
    fmSetupCfg.featuresError = featuresError;
    fmSetupCfg.cellFeatureCount = cellFeatureCount;
    fmSetupCfg.cellGridDimension = algoCfg->cellGridDimension;
    fmSetupCfg.targetNumFeatures = algoCfg->targetNumFeatures;
    fmSetupCfg.maxNumFeatures = algoCfg->maxNumFeatures;

    fmResourceCfg_t temp;
    temp.shaveNum = globalShaveCfg->fmShaveNum;
    temp.cachePartData = vResourceCfg->vp_cacheCfg.fm_part_data;
    temp.cachePartInstr = vResourceCfg->vp_cacheCfg.fm_part_instr;
    fm.fmInit(&fmSetupCfg, &temp);

    // Reset counters

    for (i = 0; i < nCells; i++)
    {
        featureIdCounter[i] = 1;
    }

    vpipe_frames = 0;
    featuresCount[0] = 0;
    featuresCount[1] = 0;
    featuresCountMax = 0;

    // Make sure the buffer are clean
    memset(featuresTmp, 0, sizeof(featuresTmp[0]) * maxNumFeatures);
    memset(features[0], 0, sizeof(features[0][0]) * maxNumFeatures);
    memset(features[1], 0, sizeof(features[1][0]) * maxNumFeatures);
    memset(featuresErrorTmp[0], 0,
           sizeof(featuresErrorTmp[0][0]) * maxNumFeatures);
    memset(featuresErrorTmp[1], 0,
           sizeof(featuresErrorTmp[1][0]) * maxNumFeatures);
    memset(featuresError[0], 0,
           sizeof(featuresError[0][0]) * maxNumFeatures);
    memset(featuresError[1], 0,
           sizeof(featuresError[1][0]) * maxNumFeatures);
    memset(featureMetadata[0], 0,
           sizeof(featureMetadata[0][0]) * maxNumFeatures);
    memset(featureMetadata[1], 0,
           sizeof(featureMetadata[1][0]) * maxNumFeatures);
    memset(cellFeatureCount[0], 0, sizeof(cellFeatureCount[0][0]) * nCells);
    memset(cellFeatureCount[1], 0, sizeof(cellFeatureCount[1][0]) * nCells);

    vPipe.ppFeatureCells = NEXT(featureCells);
    vPipe.currentFeatureCells = CURR(featureCells);
    // Init pyramid buffers
    vPipe.currentPyramid = &(pyramid[(vpipe_frames + N_PYRS) % N_PYRS]);
    vPipe.previousPyramid = &(pyramid[(vpipe_frames + N_PYRS - 1) % N_PYRS]);
    vPipe.mode = vp_mode;

    // vPipe's state must be marked as initialized in order to start
    vPipe.currentState = VPIPE_STATE_INIT;

    p_algoCfg = algoCfg;
    return 0;
}

int
VpipeImpl::vPipeRun(volatile t_vPipeParams *vp_params,
        frameBuffer *in_frame,
        float (*rpy)[3],
        vTrackBulkResult* bulkResult)
{
    t_vTrackResultSF* outTrackingDataSF;
    bulkResult->getConfigAndResults(NULL, &outTrackingDataSF);

    if(vPipe.currentState == VPIPE_STATE_INIT) { //go to first state
        vPipeStateTransition(vPipeTransitionTable[vPipe.mode][vPipe.currentState]);
    }

    while(vPipe.currentState != VPIPE_STATE_INIT && // finished this frame
            vPipe.currentState != VPIPE_STATE_NOT_INIT && // Error
            vPipe.currentState != VPIPE_STATE_ERROR //Error
         ) {
        switch (vPipe.currentState)
        {
            case VPIPE_STATE_RUN_PIXEL_PIPE:
                {
#ifdef VPIPE_PROFILE
                    struct timespec pp_start, pp_end;
                    clock_gettime(CLOCK_REALTIME, &pp_start);
                    fullRunStartTime = pp_start;

#endif
                    // Capture mode, params
                    vPipe.params = *((t_vPipeParams*) vp_params);
                    // Start the pixel pipe
                    vPipePpRun(in_frame);
#ifdef VPIPE_PROFILE
                    clock_gettime(CLOCK_REALTIME, &pp_end);
                    profile.ppTime = timespec_diff_ns(&pp_start, &pp_end);
#endif
                    break;
                }
            case VPIPE_STATE_RUN_GYRO_PREDICT:
                {
                    // Forward predict features from CURR to NEXT with rotation matrix
                    if (rpy != NULL)
                    {
                        u32 i, j;
                        float rmat[2][2];
                        rmat[0][0] = swcMathCosf(rpy[0][2]);
                        rmat[0][1] = -swcMathSinf(rpy[0][2]);
                        rmat[1][0] = -rmat[0][1];
                        rmat[1][1] = rmat[0][0];

                        for (i = 0; i < nCells; i++)
                        {
                            tvXYLoc *current = &CURR (features)[i * maxCellFeatures];
                            tvXYLoc *predicted = &featuresTmp[i * maxCellFeatures];
                            float *error = &CURR(featuresErrorTmp)[i * maxCellFeatures];

                            for (j = 0; j < CURR (cellFeatureCount)[i]; j++)
                            {
                                vPipeRotateXY (rpy, rmat, current + j, predicted + j);
                                error[j] = 1;
                            }
                        }
                    }
                    else
                    {// Copy current feature locations to seed search

                        memcpy (featuresTmp, CURR (features),
                                sizeof (featuresTmp[0]) * maxNumFeatures);

                    }
                    vPipeStateTransition(vPipeTransitionTable[vPipe.mode][vPipe.currentState]);
                    break;
                }
            case VPIPE_STATE_RUN_OF:
                {
                    if(ofCfg.algoType == OF_MEST)
                    {
                        vPipe.currentPyramid->pyrLevel[0].spec = in_frame[0].spec;
                        // This copy must exist!
                        memcpy(vPipe.currentPyramid->pyrLevel[0].p1,
                                in_frame[0].p1,
                                in_frame[0].spec.stride * in_frame[0].spec.height);
                    }

#ifdef VPIPE_PROFILE
                    struct timespec of_start, of_end;
                    clock_gettime(CLOCK_REALTIME, &of_start);
#endif
                    vPipeOfRun();
#ifdef VPIPE_PROFILE
                    clock_gettime(CLOCK_REALTIME, &of_end);
                    profile.ofTime = timespec_diff_ns(&of_start, &of_end);
#endif
                    break;
                }
            case VPIPE_STATE_RUN_FM:
                {
#ifdef VPIPE_PROFILE
                    struct timespec fm_start, fm_end;
                    clock_gettime(CLOCK_REALTIME, &fm_start);
#endif
                    vPipeFmRun();
#ifdef VPIPE_PROFILE
                    clock_gettime(CLOCK_REALTIME, &fm_end);
                    profile.fmTime = timespec_diff_ns(&fm_start, &fm_end);
#endif
                    break;
                }

            case VPIPE_STATE_FILL_OUTPUT:
                {
                    vPipeFillSfMeta(outTrackingDataSF);
#ifdef VPIPE_PROFILE
                    struct timespec ts;
                    clock_gettime(CLOCK_REALTIME, &ts);
                    profile.totalTime = timespec_diff_ns(&fullRunStartTime, &ts);
                    profile.miscTime = profile.totalTime - profile.ppTime -
                        profile.ofTime - profile.fmTime;
#endif
                    break;
                }
            case VPIPE_STATE_FILL_DUMMY_OUTPUT:
                {

                    vPipeFillDummyData(in_frame->spec);
                    break;
                }
            case VPIPE_STATE_DONE:
                {
                    vPipeStateTransition(vPipeTransitionTable[vPipe.mode][vPipe.currentState]);
                    break;
                }

            default:
                printf("STATE: %d\n", vPipe.currentState);
                assert(0);
        }
    }
    if(vPipe.currentState == VPIPE_STATE_INIT) {
        return 0;
    }
    else if(vPipe.currentState == VPIPE_STATE_ERROR || vPipe.currentState == VPIPE_STATE_NOT_INIT) {
        return -1;
    }
    else {
        assert(0);
    }
}


void
VpipeImpl::vPipeCapture(t_vPipeCapt captureMode)
{
    capture = captureMode;
}

//Private member functions
void
VpipeImpl::vPipeStateTransition(t_vPipeState nextState)
{
    if (nextState == VPIPE_STATE_ERROR){
        assert(0);
    }
    vPipe.currentState = nextState;
}

void
VpipeImpl::vPipeRotateXY(float (*rpy)[3],
                          float (*rmat)[2],
                          tvXYLoc * current, tvXYLoc * predicted)
{
    float pixels_per_deg = vPipe.pix_per_deg;
    float deg_per_pixel = vPipe.deg_per_pix;

    float cx = vPipe.camCenterX;    // 325.719;
    float cy = vPipe.camCenterY;    // 237.652;
    float uv[2] =
    {
        (current->x) - cx,
        (current->y) - cy
    };

    predicted->x = uv[0] * rmat[0][0] + uv[1] * rmat[0][1] + cx;
    predicted->y = uv[0] * rmat[1][0] + uv[1] * rmat[1][1] + cy;

    predicted->x +=
        swcMathCosf(uv[1] * deg_per_pixel) * rpy[0][1] * pixels_per_deg;
    predicted->y +=
        swcMathCosf(uv[0] * deg_per_pixel) * rpy[0][0] * pixels_per_deg;
}

void
VpipeImpl::vPipePpRun(frameBuffer * in_frame)
{
    u32 i, status = 0;
    if (vPipe.currentState != VPIPE_STATE_RUN_PIXEL_PIPE)
    {
        vPipeStateTransition(VPIPE_STATE_ERROR);
        return;
    }
    // Roll buffers
    vPipe.currentPyramid = &(pyramid[(vpipe_frames + N_PYRS) % N_PYRS]);
    vPipe.previousPyramid = &(pyramid[(vpipe_frames + N_PYRS - 1) % N_PYRS]);

    vpipe_frames++;
    vPipe.ppFeatureCells = NEXT(featureCells);
    vPipe.currentFeatureCells = CURR(featureCells);
    // Update thresholds with application input
    for (i = 0; i < nCells; i++)
    {
        if (vPipe.params.harrisThresholds[i] != HARRIS_THRESHOLD_AUTO)
            harrisThresholds[i] = vPipe.params.harrisThresholds[i];
    }
    ppThresholds_t thresholdCfg;
    thresholdCfg.thresholdDecreaseVelocity =
        vPipe.params.thresholdDecreaseVelocity;
    thresholdCfg.thresholdIncreaseVelocity =
        vPipe.params.thresholdIncreaseVelocity;
    thresholdCfg.thresholdMax = vPipe.params.thresholdMax;
    thresholdCfg.thresholdMin = vPipe.params.thresholdMin;

    // Start PP module --------------------------------------

    status = pp.ppRun(in_frame,
              &vPipe.currentFeatureCells,
              vPipe.currentPyramid,
              harrisThresholds,
              vPipe.num_pyr_levels + 1, 1,
              cellGridDimension, maxNumFeatures,
              targetNumFeatures, &thresholdCfg);
    if(!status) {
        vPipeStateTransition(vPipeTransitionTable[vPipe.mode][vPipe.currentState]);
    } else {
        vPipeStateTransition(VPIPE_STATE_ERROR);
        printf("ppRun returned an error\n");
    }
}

void
VpipeImpl::vPipeOfRun()
{
    int status = -1;
    if (vPipe.currentState != VPIPE_STATE_RUN_OF)
    {
        vPipeStateTransition(VPIPE_STATE_ERROR);
        return;
    }

    status = of.ofRun(vPipe.previousPyramid, // previous pyramid
                      vPipe.currentPyramid, // current pyramid
                      CURR (features),// input (previous) features
                      featuresTmp, // output (current) tracked features
                      CURR(featuresErrorTmp),
                      CURR (cellFeatureCount),
                      nCells, maxCellFeatures);

    if(!status) {
        vPipeStateTransition(vPipeTransitionTable[vPipe.mode][vPipe.currentState]);
    } else {
        vPipeStateTransition(VPIPE_STATE_ERROR);
        printf("ofRun returned an error\n");
    }
}

void
VpipeImpl::vPipeFmRun()
{
    if (vPipe.currentState != VPIPE_STATE_RUN_FM)
    {
        vPipeStateTransition(VPIPE_STATE_ERROR);

        return;
    }
    if (vPipe.mode == VPIPE_MODE_GYRO_ASSIST_TEST)
    {
        // If we are in gyro assist mode we don't want FM to drop bad features.
        vPipe.params.minHarrisResponseOldFeatures = -999999999999999999999.9f;
    }
    if (p_algoCfg->outputType == PP_ONLY){
        // include all pp features in this mode
        vPipe.params.minHarrisResponseOldFeatures = -999999999999999999999.9f;
        vPipe.params.minDistanceSquared = 0;
    }

    t_vPipeParams *params = &vPipe.params;
    featThresholds->lostFeatureTreshold = params->lostFeatureError;
    featThresholds->minimumHarrisResponseOldFeatures = params->minHarrisResponseOldFeatures;
    featThresholds->minimumFeatureDist = params->minDistanceSquared;
    fm.fmRun(vpipe_frames,
            &(vPipe.currentPyramid->pyrLevel[0]),
            featuresTmp,
            CURR(featuresErrorTmp),
            featThresholds
    );
    vPipeStateTransition(vPipeTransitionTable[vPipe.mode][vPipe.currentState]);
}


void
VpipeImpl::vPipeFillDummyData(frameSpec spec)
{
    u32 i, j;
    if (vPipe.currentState != VPIPE_STATE_FILL_DUMMY_OUTPUT)
    {
        vPipeStateTransition(VPIPE_STATE_ERROR);
        return;
    }
    vpipe_frames++;
    NEXT (featuresCount)= maxNumFeatures;
    for (i = 0; i < nCells;i++)
    {
        NEXT (cellFeatureCount)[i] = maxCellFeatures;
        u32 x = i % cellGridDimension;
        u32 y = i / cellGridDimension;
        u32 cellX = (x) * spec.width / cellGridDimension;
        u32 direction = (x + y + vpipe_frames) % 2;
        u32 cellY = (y + direction) * spec.height / cellGridDimension;
        u32 incrementX =
        spec.width / (cellGridDimension * NEXT (cellFeatureCount)[i]);
        u32 incrementY =
        spec.height / cellGridDimension / NEXT (cellFeatureCount)[i];
        s32 dirY = (direction == 0) ? 1 : -1;
        for (j = 0; j < NEXT (cellFeatureCount)[i]; j++)
        {
            u32 featureIndex = i * maxCellFeatures + j;
            NEXT (featureMetadata)[featureIndex].age = vpipe_frames;
            NEXT (featureMetadata)[featureIndex].id = featureIndex + 1;
            NEXT (featureMetadata)[featureIndex].harris_score = 0;
            NEXT (features)[featureIndex].x = cellX + j * incrementX;
            NEXT (features)[featureIndex].y = cellY + dirY * j * incrementY;
            NEXT(featuresError)[featureIndex] = 0;
        }
    }
    vPipeStateTransition(vPipeTransitionTable[vPipe.mode][vPipe.currentState]);
}

void
VpipeImpl::vPipeFillSfMeta(t_vTrackResultSF * outTrackingDataSF)
{
    u32 i;

    if (vPipe.currentState != VPIPE_STATE_FILL_OUTPUT)
    {
        vPipeStateTransition(VPIPE_STATE_ERROR);
        return;
    }

#ifdef VPIPE_PROFILE
    // Gather profiling data
    NEXT (featureCountPP)= 0;
    for (i = 0; i < nCells; i++)
        NEXT (featureCountPP)+= vPipe.ppFeatureCells[i].featureCount;
    profile.vTrackFrameCount = vpipe_frames;
    profile.ppKeypoints = NEXT(featureCountPP); // Fast -> NonMax
    profile.ofInputKeypoints = CURR(featuresCount); // Into OF
    profile.fmInputKeypoints = CURR(featureCountPP); // Into FM
    float sum = 0;
    for(u32 i = 0; i < nCells; ++i)
        sum += harrisThresholds[i];
    profile.avgThreshold = sum/nCells;
    if (profile.ppTime > maxPPtime.ppTime)
    {
        memcpy(&maxPPtime, &profile, sizeof(t_vTrackDebug));
    }
    if (profile.ofTime > maxOFtime.ofTime)
    {
        memcpy(&maxOFtime, &profile, sizeof(t_vTrackDebug));
    }
    if (profile.fmTime > maxFMtime.fmTime)
    {
        memcpy(&maxFMtime, &profile, sizeof(t_vTrackDebug));
    }
    if (NEXT (featuresCount)> featuresCountMax)
    {
        featuresCountMax = NEXT (featuresCount);
        memcpy (&maxFeatureCountTime, &profile, sizeof (t_vTrackDebug));
    }
#endif
    // ------------------------------------------------------
    // Copy results into output buffers
    if(outTrackingDataSF) {
        fillMetadata(outTrackingDataSF);
    }
    vPipeStateTransition(vPipeTransitionTable[vPipe.mode][vPipe.currentState]);
}




void
VpipeImpl::fillMetadata(t_vTrackResultSF * trackingData)
{
    //Results
    tvXYLoc* locPtr;
    FeatureMetadata* metaPtr;
    float* errorPtr;
    uint32_t* cellFeatCountPtr;

    trackingData->prev.featuresCount = CURR(featuresCount);
    tvXYLoc* prevLocPtr = CURR(features);
    FeatureMetadata* prevMetaPtr = CURR(featureMetadata);
    float* prevErrorPtr;
    uint32_t* prevCellFeatCountPtr;

    if(p_algoCfg->outputType == TRACKED)
    {
        //Currently tracked output is not supported for multiple cells
        //TODO: this is because we don't have a buffer of the feature cells
        //for the optical flow/mest output
        //This branch (if(p_algoCfg->outputType == TRACKED)) assumes only one cell
        assert(nCells==1);
        trackingData->curr.featuresCount = CURR(featuresCount);
        locPtr = featuresTmp;
        metaPtr = CURR(featureMetadata);
        errorPtr = CURR(featuresErrorTmp);
        cellFeatCountPtr = &trackingData->curr.featuresCount;
        prevErrorPtr = NEXT(featuresErrorTmp);
        prevCellFeatCountPtr = &trackingData->prev.featuresCount;
    }
    else
    {
        trackingData->curr.featuresCount = NEXT(featuresCount);
        locPtr = NEXT(features);
        metaPtr = NEXT(featureMetadata);
        errorPtr = NEXT(featuresError);
        cellFeatCountPtr = NEXT(cellFeatureCount);
        prevErrorPtr = CURR(featuresError);
        prevCellFeatCountPtr = CURR(cellFeatureCount);
    }

    uint32_t featureIndexCurr=0;
    uint32_t featureIndexPrev=0;
    for(uint32_t j=0; j<nCells; j++) {
        int cellIdx = maxCellFeatures * j;
        // current
        trackingData->curr.thresholds.ptr[j] = harrisThresholds[j];
        trackingData->curr.fmCellFeatureCount.ptr[j] = cellFeatCountPtr[j];
        memcpy((void*)&trackingData->curr.metadata.ptr[featureIndexCurr], (void*)&metaPtr[cellIdx], cellFeatCountPtr[j] * sizeof(FeatureMetadata));
        memcpy((void*)&trackingData->curr.features.ptr[featureIndexCurr], (void*)&locPtr[cellIdx], cellFeatCountPtr[j] * sizeof(tvXYLoc));
        memcpy((void*)&trackingData->curr.trackingError.ptr[featureIndexCurr], (void*)&errorPtr[cellIdx], cellFeatCountPtr[j] * sizeof(float));
        featureIndexCurr+=cellFeatCountPtr[j];

        //previous
        trackingData->prev.thresholds.ptr[j] = 0; // we don't store previous thresholds
        trackingData->prev.fmCellFeatureCount.ptr[j] = prevCellFeatCountPtr[j];
        memcpy((void*)&trackingData->prev.metadata.ptr[featureIndexPrev], (void*)&prevMetaPtr[cellIdx], prevCellFeatCountPtr[j] * sizeof(FeatureMetadata));
        memcpy((void*)&trackingData->prev.features.ptr[featureIndexPrev], (void*)&prevLocPtr[cellIdx], prevCellFeatCountPtr[j] * sizeof(tvXYLoc));
        memcpy((void*)&trackingData->prev.trackingError.ptr[featureIndexPrev], (void*)&prevErrorPtr[cellIdx], prevCellFeatCountPtr[j] * sizeof(float));
        featureIndexPrev+=prevCellFeatCountPtr[j];
    }
    assert(featureIndexCurr == trackingData->curr.featuresCount);
    assert(featureIndexPrev == trackingData->prev.featuresCount);

    //Debug output
    trackingData->debug.fmTime = profile.fmTime;
    trackingData->debug.ofTime = profile.ofTime;
    trackingData->debug.ppTime = profile.ppTime;
    trackingData->debug.totalTime = profile.totalTime;
    trackingData->debug.miscTime = profile.miscTime;
    trackingData->debug.maxPreStart = profile.maxPreStart;

    trackingData->debug.avgThreshold = profile.avgThreshold;
    trackingData->debug.fmInputKeypoints = profile.fmInputKeypoints;
    trackingData->debug.ofInputKeypoints = profile.ofInputKeypoints;
    trackingData->debug.ppKeypoints = profile.ppKeypoints;
    trackingData->debug.vTrackFrameCount = profile.vTrackFrameCount;

    for (u32 lvl = 0; lvl < vPipe.currentPyramid->numPyramidLevels; lvl++)
    {
        trackingData->debug.info[lvl] = (u32)((u64)vPipe.currentPyramid->pyrLevel[lvl].p1 & 0xFFFFFFFF);
    }

    //Age histogram & id test
    u32 *histogramBase = trackingData->debug.ageHist;
    for (int j = 0; j < AGE_HIST_COUNT; j++)
    {
        histogramBase[j] = 0;
        dbgHist[j] = 0;
    }
    for(uint32_t j = 0; j < trackingData->curr.featuresCount; j++) {
        u32 histAge = trackingData->curr.metadata.ptr[j].age;
        u32 histCell = 0;
        while (histAge > AGE_HIST_STEP && histCell < AGE_HIST_COUNT)
        {
            histAge = histAge / AGE_HIST_STEP;
            histCell++;
        }
        histogramBase[histCell]++;
        dbgHist[histCell]++;
#ifdef ENABLE_ID_TEST
        u32 id = trackingData->curr.metadata[j].id;

        u32 idmatch = 0;
        for (i = 0; i < trackingData->curr.featuresCount; i++)
        {
            if (id == trackingData->curr.metadata.ptr[i].id)
            {
                idmatch++;
            }
        }
        assert (idmatch == 1);
        idmatch = 0;
        for (i = 0; i < trackingData->prev.featuresCount; i++)
        {
            if (id == trackingData->prev.metadata.ptr[i].id)
            {
                idmatch++;
            }
        }
        if (trackingData->curr.metadata.ptr[j].age == 1)
            assert (idmatch == 0);
        else
            assert (idmatch == 1);
#endif
    }
}

t_vTrackResultConfig VpipeImpl::vPipeGetResultConfig(){
    assert(vPipe.currentState == VPIPE_STATE_INIT || vPipe.currentState == VPIPE_STATE_DONE);
    t_vTrackResultConfig ret;
    ret.nCells = nCells;
    ret.maxNumFeatures = maxNumFeatures;
    ret.cellGridDimension = cellGridDimension;
    ret.resultsBufferLength = sizeof(t_vTrackResultConfig) +
                                sizeof(t_vTrackResultSF) +
                                2*(sizeof(uint32_t) +
                                    (sizeof(tvXYLoc) * maxNumFeatures) +
                                    (sizeof(FeatureMetadata) * maxNumFeatures)+
                                    (sizeof(float) * maxNumFeatures) +
                                    (sizeof(uint32_t) * nCells) +
                                    (sizeof(float) * nCells));
    return ret;
}

#ifdef ENABLE_SANITYCHECK       //use pp to hush compiler warning
void
VpipeImpl::featuresSanityCheck(tvXYLoc features[],
                                FeatureMetadata metadata[],
                                fp32 featuresError[],
                                u32 cellFeaturesCount[])
{
    u32 cellId;
    u32 i;
    u32 maxWidth = pyrImages[0][0].spec.width;
    u32 maxHeight = pyrImages[0][0].spec.height;

    features = (tvXYLoc *) (((u32) features & 0x0FFFFFF) | 0xA0000000);

    for (cellId = 0; cellId < nCells; cellId++)
    {
        tvXYLoc *cellFeatures = &features[cellId * maxCellFeatures];
        fp32 *cellFeaturesError = &featuresError[cellId * maxCellFeatures];

        for (i = 0; i < cellFeaturesCount[cellId]; i++)
        {
            if (cellFeaturesError[i] > LOST_FEAT_ERR_THRESH)
                badErr++;

            // Bad coordinates
            if ((cellFeatures[i].x > maxWidth) || (cellFeatures[i].x < 0))
            {
                bad_x = cellFeatures[i].x;
                badFeatures++;

                // printf("%d\n", (int)cellFeatures[i].x);
                // printf("%d %d\n", maxWidth, maxHeight);
                // printf("x:%x\n", &cellFeatures[i].x);
                // LEON_HALT;
            }
            else if ((cellFeatures[i].y > maxHeight) || (cellFeatures[i].y < 0))
            {
                bad_y = cellFeatures[i].y;
                badFeatures++;

                // printf("%d\n", (int)cellFeatures[i].y);
                // printf("%d %d\n", maxWidth, maxHeight);
                // printf("y:%x\n", &cellFeatures[i].y);
                // LEON_HALT;
            }

            // Bad age
            if (metadata[i].age > vpipe_frames)
            {
                bad_age = metadata[i].age;
                badAge++;
            }
        }
    }
}
#endif
