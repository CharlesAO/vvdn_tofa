///
/// @file
/// @copyright All code copyright Movidius Ltd 2018, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     vision frame definition
///

#ifndef VISION_FRAME_H
#define VISION_FRAME_H

class VisionFrame{
public:
    uint32_t frameCount;       // frame count since streaming started
    uint32_t frameType;        // 0xDEADDEAD Frame invalid, anything else frame valid
    uint64_t timestampNs;      // timestamp in Nanoseconds
    uint32_t exposureNs;       // timestamp in Nanoseconds
    uint32_t latencyNs;        // latency in Nanoseconds
    uint32_t width;
    uint32_t stride;
    uint32_t bpp;
    uint32_t height;
    uint8_t* p1;
    uint8_t* p2;
    uint8_t* p3;
    VisionFrame(uint32_t width,
             uint32_t height,
             uint32_t stride,
             uint32_t bpp,
             uint8_t* p1,
             uint32_t frameCount,
             uint32_t frameType,
             uint64_t timestampNs,
             uint32_t exposureNs,
             uint32_t latencyNs): frameCount(frameCount), frameType(frameType),
                             timestampNs(timestampNs),exposureNs(exposureNs),
                             latencyNs(latencyNs),width(width), height(height),
                             stride(stride), p1(p1), p2(nullptr), p3(nullptr){

    }
    VisionFrame(uint32_t width,
             uint32_t height,
             uint32_t stride,
             uint32_t bpp,
             uint32_t frameCount,
             uint32_t frameType,
             uint64_t timestampNs,
             uint32_t exposureNs,
             uint32_t latencyNs): frameCount(frameCount), frameType(frameType),
                             timestampNs(timestampNs),exposureNs(exposureNs),
                             latencyNs(latencyNs),width(width), height(height),
                             stride(stride), p2(nullptr), p3(nullptr){
        p1 = (uint8_t*)malloc(stride * height);
    }
    VisionFrame(uint32_t width,
             uint32_t height,
             uint32_t stride): frameCount(0), frameType(0),
                             timestampNs(0),exposureNs(0),
                             latencyNs(0),width(width), height(height),
                             stride(stride), p2(nullptr), p3(nullptr){
        p1 = (uint8_t*)malloc(stride * height);
    }
    VisionFrame(uint32_t width,
             uint32_t height): frameCount(0), frameType(0),
                             timestampNs(0),exposureNs(0),
                             latencyNs(0),width(width), height(height),
                             stride(width), p2(nullptr), p3(nullptr){
        p1 = (uint8_t*)malloc(stride * height);
    }
    ~VisionFrame(){
        free(p1);
        free(p2);
        free(p3);
    }
};

#endif
