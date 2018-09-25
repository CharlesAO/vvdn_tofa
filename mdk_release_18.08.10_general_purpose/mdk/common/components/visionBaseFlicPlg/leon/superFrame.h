#ifndef SUPERFRAME_H__
#define SUPERFRAME_H__

#include <stdint.h>

#define ROUND_UPP_TO(x, roundNr) ((int)((x) / (roundNr) + 1) * (roundNr))

#define FRAME_GENERIC_META_SIZE     1024
#define FRAME_CUSTOM_META_SIZE (2*1024*1024 - FRAME_GENERIC_META_SIZE)

#define SF_FRAME_TYPE_VALID         0x55AA55AA
#define SF_FRAME_TYPE_CALIBRATION   0xCA1BCA1B
#define SF_FRAME_TYPE_INVALID       0xDEADDEAD

typedef struct frameMetadataGeneric {
    uint32_t frameCount;       // frame count since streaming started
    uint32_t frameType;        // 0xDEADDEAD Frame invalid, anything else frame valid
    uint64_t timestampNs;      // timestamp in Nanoseconds
    uint32_t exposureNs;       // timestamp in Nanoseconds
    uint32_t latencyNs;        // latency in Nanoseconds
} frameMetadataGeneric;

typedef struct frameMeta
{
    frameMetadataGeneric genericMeta;
    uint32_t customMetaSize;
    uint8_t padGenericMeta[FRAME_GENERIC_META_SIZE - sizeof(frameMetadataGeneric) - sizeof(uint32_t)];
    uint8_t customMetadata[FRAME_CUSTOM_META_SIZE];
} frameMeta;


typedef enum metadataFieldSelect_t {
    FRAME_COUNT,
    FRAME_TYPE,
    TIMESTAMP_NS,
    EXPOSURE_NS,
    LATENCY_NS,
} metadataFieldSelect_t;

static inline void populateFrameMetadataField(void *metaAddr,
                                              metadataFieldSelect_t field,
                                              uint64_t val)
{
    frameMetadataGeneric *meta = (frameMetadataGeneric*)metaAddr;

    switch(field)
    {
    case FRAME_COUNT:
        meta->frameCount = val;
        break;
    case FRAME_TYPE:
        meta->frameType = val;
        break;
    case TIMESTAMP_NS:
        meta->timestampNs = val;
        break;
    case EXPOSURE_NS:
        meta->exposureNs = val;
        break;
    case LATENCY_NS:
        meta->latencyNs = val;
        break;
    default:
        break;
    }
}

#endif
