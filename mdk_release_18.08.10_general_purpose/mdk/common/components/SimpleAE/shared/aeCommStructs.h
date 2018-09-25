#ifndef AECOMMSTRUCTS_H
#define AECOMMSTRUCTS_H

#include <swcFrameTypes.h>
#include <FramePump.h>

// Maximum number of fisheye cameras available on any board that we support.
// The actual number of cameras in the board is provided by the ae_camera_count
// shared global (called lrt_ae_camera_count on LOS).
#define MAX_NR_CAMS         2

#define AE_EXPGAIN_TBL_MAX_SZ   (FP_EXPGAIN_TBL_MAX_SZ)

typedef enum aeMsgType_t
{
    AE_MSG_PROCESS_FRAME = 1,
    AE_MSG_CAM_PARAM_CFG,
    AE_MSG_EXP_GAIN_TBL
} aeMsgType_t;

typedef struct convertMessage_t
{
    aeMsgType_t type;

    union
    {
        /// VGA frame buffer sent to LRT AE algo for processing + conversion and then returned back
        struct
        {
            FramePumpBuffer fpbIn;
            float curTotalExp;
            float statsTotalExp;
        };

        /// Camera AE algo configuration: sent as 1-st message on LOS->LRT IPC channel
        struct
        {
            u32 minExpTime;
            float minGain;
            u32 maxExpTime;
            float maxGain;
            u32 gainFractBits;
        };

        /// Camera {Exposure,Gain} regions table: sent as 2-nd message on LOS->LRT IPC channel
        struct
        {
            FramePumpCamExpGain region[FP_EXPGAIN_TBL_MAX_SZ];
        };
    };
}convertMessage_t;

typedef struct aeMessage_t
{
    u32 integrationTimeUs;
    float gainCode[MAX_NR_CAMS];
}aeMessage_t;

#endif
