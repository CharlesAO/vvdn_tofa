///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
///
#ifndef FEATURES_DRAWER_H
#define FEATURES_DRAWER_H


#include "vTrackOutput.h"
#include <opencv2/core/core.hpp> //added
#include <opencv2/highgui/highgui.hpp> //added
#include "vtrackStats.h"

#include <deque>


#define HMAX 16
#define XMOD 20
#define WIDTH 500
#define NR_FRAMES 500
#ifndef FALSE
#define FALSE (0)
#endif

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef NULL
#define NULL (0)
#endif

struct Points
{
    float x, y;

    Points(float x, float y)
    {
        this->x = x;
        this->y = y;
    }
};

struct FeatureTrackerProperties
{
    uint64_t featureId;                  // unique
    uint8_t keepTracking;                // marks if a feature should be tracked or dumped
    std::deque<Points> path;             // array of points which stores the path of the feature
    uint64_t numTrackedPoints;
    uint16_t lastElement;                // last tracked point number
    bool isInlier;
};

extern uint32_t MAX_FEATURE_TRACK;

void initTrackingBuffers();

void drawPoint(cv::Mat& img, int32_t x, int32_t y,cv::Scalar color);
void drawLine(cv::Mat& img, int32_t x1, int32_t y1, int32_t x2, int32_t y2);

void trackFeatures(t_vTrackResultSF* features, vTrackMoviKpiStats *stats, bool showGraphs);
void drawFeatures(cv::Mat& img);
void initLiveGraphPlotter();
void drawHistograms(t_vTrackResultSF* vTrackRes);
void drawLiveGraphs(t_vTrackResultSF* vTrackRes);
void initFeaturesDrawer(uint32_t max_features);
void deleteFeaturesDrawer();
#endif
