/**************************************************************************************************

 @File         : IcTypes.h
 @Author       : xx
 @Brief        : Contains All project specific types definition
 Date          : 30 - April - 2015
 E-mail        : xxx.xxx@movidius.com
 Copyright     : © Movidius Srl 2014, © Movidius Ltd 2015

 Description :


 **************************************************************************************************/


#ifndef _IC_TYPES_H
#define _IC_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
 ~~~  Include standard types
 **************************************************************************************************/
#include <stdint.h> //Myriad and PC

/**************************************************************************************************
 ~~~  Specific #defines
 **************************************************************************************************/

#ifndef MAX_TIMESTAMPS
#define MAX_TIMESTAMPS 4 // Maximum history timestamp associated with a frame
#endif
#ifndef IC_TYPES_FRM_MAX_NR_PLANES
#define IC_TYPES_FRM_MAX_NR_PLANES 4 // Maximum history timestamp associated with a frame
#endif

/**************************************************************************************************
 ~~~  Basic project types definition
 **************************************************************************************************/

typedef struct {
    uint32_t    w;
    uint32_t    h;
} icSize;

typedef struct {
    int32_t     x1;
    int32_t     y1;
    int32_t     x2;
    int32_t     y2;
} icRect;

typedef enum {
    FRAMEMGR_EMPTY      = 0x0000,
    FRAMEMGR_ACQUIRED   = 0x0010,
    FRAMEMGR_PRODUCED   = 0x0100,
    FRAMEMGR_DONE       = 0x1000
} FrameMgrStatus;

typedef enum
{
    FRAME_T_FORMAT_422I,
    FRAME_T_FORMAT_NV12,
    FRAME_T_FORMAT_NV21,
    FRAME_T_FORMAT_RAW_10_PACK,
    FRAME_T_FORMAT_BINARY,  //  metadata
    FRAME_T_FORMAT_RAW_16,   // RAW16 packet
    FRAME_T_FORMAT_NV12_Y,
    FRAME_T_FORMAT_NV12_UV,
    FRAME_T_FORMAT_YUV420,
    FRAME_T_FORMAT_RAW_8,
    FRAME_T_FORMAT_RAW_10,
    FRAME_T_FORMAT_RAW_12,
    FRAME_T_FORMAT_RAW_12_PACK,
    FRAME_T_FORMAT_RAW_14,
    FRAME_T_FORMAT_RGB_888,
    FRAME_T_FORMAT_RGB_101010,
    FRAME_T_FORMAT_RGB_121212,
    FRAME_T_FORMAT_YUV422P,
    FRAME_T_FORMAT_YUV444,

        FRAME_T_FORMAT_MAX
} FrameTFormatTypes;

typedef enum
{
    FRAME_DATA_TYPE_PREVIEW,
    FRAME_DATA_TYPE_STILL,
    FRAME_DATA_TYPE_STILL_RAW,
    FRAME_DATA_TYPE_METADATA_PRV,
    FRAME_DATA_TYPE_METADATA_STILL,
    FRAME_DATA_TYPE_IQ_DEBUG_DATA,

    FRAME_DATA_TYPE_MAX,
} FrameTDataTypes;


struct FrameS;
struct FramePoolS;
typedef struct FrameS FrameT;
typedef struct FramePoolS  FramePool;

typedef void (*FrameProducedCBFunc)(FrameT *frame, void *pluginObj);

typedef struct {
    FrameProducedCBFunc  callback;
    void                 *pluginObj;
} FrameProducedCB;

// Buffer data description structure
struct FrameS {
    FrameT*             next;
    FrameTFormatTypes   type; //
    uint32_t            refcnt;
    FrameMgrStatus      status;
    FramePool*          framePool;
    void *              fbPtr [IC_TYPES_FRM_MAX_NR_PLANES];
    uint32_t            stride[IC_TYPES_FRM_MAX_NR_PLANES];
    uint32_t            height[IC_TYPES_FRM_MAX_NR_PLANES];
    uint32_t            tSize[4]; //Total size, useful for non-stride/width based data
    uint32_t            nPlanes;
    uint64_t            timestamp     [MAX_TIMESTAMPS];
    uint32_t            timestampEvent[MAX_TIMESTAMPS]; // app specific events associated with timestamps
    uint32_t            timestampNr;
    uint32_t            seqNo;
    void                *appSpecificData;
};

// Frames pools linking structure
struct FramePoolS {
    uint32_t        nFrames;
    FrameT          *nextFree;
    FrameT          *frames;
    FrameProducedCB *callbacks;
    uint32_t        nCallbacks;
    uint32_t        seqNo;
};

/* Timestamps are in microseconds */
typedef uint64_t icTimestamp;

#ifdef __cplusplus
}
#endif

#endif  // _IC_TYPES_H
