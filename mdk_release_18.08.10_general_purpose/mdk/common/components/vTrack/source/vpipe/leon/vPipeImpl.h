///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Vision Tracking Pipeline
///

#ifndef _VPIPE_IMPL_H_
#define _VPIPE_IMPL_H_

#include "vTrackOutput.h"
#include "global_constants.h"
#include "vPipe.h"
#include "vPipePublicTypes.h"
#include "vPipeHelpers.h"
#include "vPipePrivateTypes.h"
#include "opticalFlowApi.h"
#include "featureMaintenanceApi.h"
#include "PixelPipeApi.h"


#define VPIPE_PROFILE
//#define ENABLE_SANITYCHECK    //for internal debug only
//#define ENABLE_ID_TEST
#define VPIPE_MAX_FRAME_WIDTH   1280

#ifdef VPIPE_PROFILE
#include <time.h>
#endif
class VpipeImpl {
    public:
        // Initialization function
        // Parameters:
        // @vp_mode: selects the mode: pixelPipe only, pixel pipe with FM, full pipeline, dummy mode
        //    After getting the callback, vPipeRun should be called again to continue processing
        //    When the pipeline finished, the callback will be called with nextState = VPIPE_STATE_DONE
        int vPipeInit(t_vPipeResourceCfg* vResourceCfg,volatile t_vPipeMode vp_mode,
                t_vPipeAlgoCfg* algoCfg);
        // Main function
        // The function must be called several times, until VPIPE_DONE state is returned
        // Parameters:
        // @vp_params: runtime parameters
        // @in_frame: input image
        // @rmat: Rotation matrix for gyro assist calculated from gyro samples. NULL value will disable gyro assist
        // @outTrackingData: Pointers to the internal data structures, where the output can be found. NULL value to disable
        // @outTrackingDataSF: Pointer to the zone where the output should be copied (output frame)
        int vPipeRun(volatile t_vPipeParams* vp_params,
                frameBuffer* in_frame,
                float (*rmat)[3],           // Rotation matrix
                vTrackBulkResult* bulkResult);

        void vPipeCapture(t_vPipeCapt captureMode);
        t_vTrackResultConfig vPipeGetResultConfig();
    private:
        void vPipeStateTransition(t_vPipeState nextState);
        void vPipeRotateXY(float (*rpy)[3],
                          float (*rmat)[2],
                          tvXYLoc * current, tvXYLoc * predicted);
        void vPipePpRun(frameBuffer * in_frame);
        void vPipeOfRun();
        void vPipeFmRun();
        void vPipeFillDummyData(frameSpec spec);
        void vPipeFillSfMeta(t_vTrackResultSF * outTrackingDataSF);
        void fillMetadata(t_vTrackResultSF * trackingData);

void initialiseHeap(frameSpec * input_frame_spec,
              tvPyramidBuffer * pyramid,
              const u32 num_pyr,
              const u32 num_pyr_levels,
              const u32 num_pyr_levels_cmx,
              frameBuffer pyrImages[][MAX_PYRAMID_LEVELS],
              VpipeBuffer& heapDDR,VpipeBuffer& heapCMX);

void allocateInternalBuffers(VpipeBuffer& bufSpace, u16 max_features);
void allocateMestBuffers(VpipeBuffer& bufSpace, u16 max_features);

#ifdef ENABLE_SANITYCHECK       //use pp to hush compiler warning
        void featuresSanityCheck(tvXYLoc
                                features[],
                                FeatureMetadata
                                metadata[],
                                fp32 featuresError[],
                                u32 cellFeaturesCount[]);
#endif


        volatile u32 dbgHist[AGE_HIST_COUNT];
#ifdef ENABLE_SANITYCHECK
        // debug code only...
        static float bad_x;
        static float bad_y;
        static u32 badFeatures = 0;
        static u32 badAge = 0;
        static u32 bad_age;
        static u32 badErr = 0;
#endif

#ifdef VPIPE_PROFILE
        struct timespec fullRunStartTime;
#endif
        // Profiling buffers
        t_vTrackDebug profile;
        t_vTrackDebug maxPPtime;
        t_vTrackDebug maxOFtime;
        t_vTrackDebug maxFMtime;
        t_vTrackDebug maxFeatureCountTime;
        // vPipe parameters
        u32 vpipe_frames;

        // Optical Flow parameters
        tvOpticalFlowCfg ofCfg;

        // Pixel Pipeline buffers
        frameBuffer      pyrImages[N_PYRS][MAX_PYRAMID_LEVELS];          // locate on DDR Heap
        tvPyramidBuffer  pyramid[N_PYRS];                                // aggregation of pyramid images
        tvFeatureCell*   featureCells[2]; // double buffer feature lines
        fp32*            harrisThresholds;
        u32              featureCountPP[2];

        // Optical Flow buffers
        tvXYLoc*         featuresTmp;
        tvXYLoc*         features[2];
        tvXYLoc*         featuresInputOF;
        fp32*            featuresErrorTmp[2];
        fp32*            featuresError[2];
        u32              featuresCount[2];    // number of output features for frame N & N-1
        u32              featuresCountMax;    // maximum number of features since we started
        OpticalFlow of;
        PixelPipe pp;
        FeatMaintenance fm;
        // General config stuff 
        u32 nCells;
        u32 cellGridDimension;
        u32 maxNumFeatures;
        u32 targetNumFeatures;
        u32 maxCellFeatures;
        // Feature Maintenance buffers
        FeatureMetadata* featureMetadata[2];
        u32*             cellFeatureCount[2];
        u32*             featureIdCounter;   // every new feature gets a unique ID
        FMSetupCfg       fmSetupCfg;
        FMFeatureThresholds_t* featThresholds;

        t_pPipeResourceCfg pixelPipeResource;
        t_vPipeAlgoCfg* p_algoCfg;
        //Global information about the shave resources
        t_vPipeShaveConfig* globalShaveCfg;
        t_pPipeShaveConfig ppShaveCfg;
        t_ppFifoCfg fifoCfg;
        u8 ofCachePart;
        /// A static pointer for vPipe
        vPipe_t vPipe;
        t_vPipeCapt capture;
} ALIGNED(4096);
#endif // _VPIPE_H_
