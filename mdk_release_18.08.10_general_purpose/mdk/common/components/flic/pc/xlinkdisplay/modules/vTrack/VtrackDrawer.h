///
/// @file
/// @copyright All code copyright Movidius Ltd 2018, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     vTrack visualization
///

#ifndef VTRACK_DRAWER_H
#define VTRACK_DRAWER_H
#include <vtrackStats.h>
#include <graphPlotter.h>
#include "visionFrame.h"
#include <vTrackOutput.h>


class VtrackDrawer{
public:
    VtrackDrawer(uint32_t frameWidth, uint32_t frameHeight, uint32_t max_features, bool showGraphs);
    virtual void initDrawer();
    virtual void changeDrawerDims(uint32_t frameWidth, uint32_t frameHeight);
    virtual cv::Mat* draw(VisionFrame* frame1, uint8_t* customMeta);
    virtual vTrackKpiStats* getStatistics();
    // Empty virtual destructor
    virtual ~VtrackDrawer() {
        if(latencyGraph) {
            delete latencyGraph;
        }
    }

protected:
    virtual void selectCandidatePoints(cv::Mat *out, t_vTrackResultSF *vTrackRes,
            vTrackKpiStats *stats=NULL) = 0;
    uint32_t m_frameWidth;
    uint32_t m_frameHeight;
    uint32_t maxFeatures;
    uint64_t previousTimestampNs;
    uint32_t countFrames;
    float avgFps;
    bool m_showGraphs;
    vTrackKpiStats *stats;
    graphPlotter * latencyGraph;
};

#endif // VTRACK_DRAWER_H

