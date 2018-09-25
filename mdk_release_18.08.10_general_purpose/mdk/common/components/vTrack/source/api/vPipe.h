///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief     Vision Tracking Pipeline
///

#ifndef _VPIPE_H_
#define _VPIPE_H_

#include "vPipePublicTypes.h"

//forward declarations
struct t_vTrackResult;
struct t_vTrackResultSF;
struct frameElements;
typedef struct frameElements frameBuffer;
class VpipeImpl;
//Function to set the default values for algorithm configuration
void vPipeInitDefaultAlgoCfg(t_vPipeAlgoCfg* algoCfg, frameSpec* inputFrameSpec)__attribute__((visibility("default")));

void vPipeInitDefaultAlgoShiTomasiCfg(t_vPipeAlgoCfg* algoCfg, frameSpec* inputFrameSpec)__attribute__((visibility("default")));

void vPipeInitDefaultAlgoMESTCfg(t_vPipeAlgoCfg* algoCfg, frameSpec* inputFrameSpec)__attribute__((visibility("default")));

//Function to set the default values for the algorithm parameters
void vPipeInitDefaultHarrisRuntimeParameters(t_vPipeParams* params) __attribute__((visibility("default")));

class Vpipe {
    public:
        Vpipe() __attribute__((visibility("default")));
        ~Vpipe() __attribute__((visibility("default")));
        // Initialization function
        // Parameters:
        // @vp_mode: selects the mode: pixelPipe only, pixel pipe with FM, full pipeline, dummy mode
        //    After getting the callback, vPipeRun should be called again to continue processing
        //    When the pipeline finished, the callback will be called with nextState = VPIPE_STATE_DONE
        int vPipeInit(t_vPipeResourceCfg* vResourceCfg,volatile t_vPipeMode vp_mode,
                t_vPipeAlgoCfg* algoCfg) __attribute__((visibility("default")));
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
                vTrackBulkResult* bulkResult) __attribute__((visibility("default")));

        void vPipeCapture(t_vPipeCapt captureMode) __attribute__((visibility("default")));
        t_vTrackResultConfig vPipeGetResultConfig() __attribute__((visibility("default")));
    private:
        VpipeImpl* p_impl;
};
#endif // _VPIPE_H_
