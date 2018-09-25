#ifndef COMMSTRUCTS_H
#define COMMSTRUCTS_H

#include <swcFrameTypes.h>
#define MAX_FRAMES_COUNT 2
#include <superFrame.h>
#include <string>
#define MAX_DATA_SIZE (1280*720*2) //todo find better location for this


//TODO: parts of this is plugin specific. This structure should instead mirror the major types like vtrack and stereo, the subtype being handled at plugin level.
enum MsgUid { CAMERA_LEFT=0, CAMERA_RIGHT, DISPARITY_META, DEPTH_META,  VTRACK_LEFT_META, VTRACK_RIGHT_META, IMU_META };

enum MsgGid { GID_CAMERA=0, GID_VTRACK, GID_STEREO, GID_IMU };


typedef enum outStreamType_t
{
    FRAME_BUFF,
    META,
    IMU,
}outStreamType_t;

typedef struct frameThroughMeta
{
    frameSpec spec;
    uint8_t frameData[MAX_DATA_SIZE];
}frameThroughMeta;

typedef struct BuffDesc
{
    MsgUid uid;
    std::string WindowStr;
    uint8_t *Buffer;
    outStreamType_t Type;
}BuffDesc;

#endif
