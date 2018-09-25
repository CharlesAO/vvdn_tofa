///
/// @file
/// @copyright All code copyright Movidius Ltd 2018, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Stereo visualization
///


#ifndef STEREO_DRAWER_H
#define STEREO_DRAWER_H
#include <swcFrameTypes.h>
#include <opencv2/opencv.hpp>
#include "visionFrame.h"

////
////    @brief Abstract class of interface to the class that gets the algo results
////
class StereoDrawer{

public:
    StereoDrawer(bool show_image);
    void initDrawer() {}
    int draw(VisionFrame* left,VisionFrame* right, VisionFrame* disp, VisionFrame* depth);
    float getAvgFPS(uint64_t ts);
    
    // Empty virtual destructor
    virtual ~StereoDrawer() {}

private:
    uint64_t m_frame_count;
    uint64_t m_prev_ts_ns;
    float baselineOv7251 = 0.08;
    float fovOv7251 = 50.0;

    void stretchDisparityRange(cv::Mat& frame, int disp);
    cv::Mat convToMat(VisionFrame* frame, bool col_map, bool median);
    cv::Mat convhalfToMat(VisionFrame* frame, bool col_map, bool median);
    cv::Mat convToMatDepth(VisionFrame* frame, float b, float fov);

    bool m_show_image;

};

#endif // STEREO_DRAWER_H
