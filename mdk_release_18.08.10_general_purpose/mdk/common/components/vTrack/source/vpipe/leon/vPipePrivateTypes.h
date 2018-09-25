#ifndef VPIPE_PRIVATE_TYPES_H
#define VPIPE_PRIVATE_TYPES_H

#define HARRIS_THRESHOLD_AUTO 0

#include "vTrackPrivateTypes.h"

struct tvPyramidBuffer;
typedef struct tvPyramidBuffer tvPyramidBuffer;


// Internal states that vPipe passes until a frame is processed.
// VPIPE_DONE is returned.
enum t_vPipeState
{
    VPIPE_STATE_NOT_INIT = 0,
    VPIPE_STATE_INIT = 1,
    VPIPE_STATE_RUN_PIXEL_PIPE = 2,
    VPIPE_STATE_RUN_GYRO_PREDICT = 3,

    VPIPE_STATE_RUN_OF = 4,
    VPIPE_STATE_RUN_FM = 5,
    VPIPE_STATE_FILL_OUTPUT = 6,
    VPIPE_STATE_FILL_DUMMY_OUTPUT = 7,
    VPIPE_STATE_ERROR = 8,
    VPIPE_STATE_DONE = 9,
    VPIPE_STATE_LAST= 10

};
// A structure for vPipe
typedef struct vPipe_t
{
    // Current state of processing
    volatile t_vPipeState currentState; // = VPIPE_NOT_INIT;

    // vPipe mode
    t_vPipeMode mode;

    // vPipe Params
    t_vPipeParams params;

    // The previous pyramid in optical flow
    tvPyramidBuffer *previousPyramid;

    // The current pyramid in optical flow
    tvPyramidBuffer *currentPyramid;

    // Features currently being detected by the PixelPipeline
    tvFeatureCell *ppFeatureCells;

    // Previous features detected by the PixelPipeline
    tvFeatureCell *currentFeatureCells;

    // Total number of pyramid levels
    u32 num_pyr_levels;

    // Pixels per degree
    float pix_per_deg;
    float deg_per_pix;

    // Center of projection in x
    int camCenterX;

    // Center of projection in y
    int camCenterY;
} vPipe_t;




#endif
