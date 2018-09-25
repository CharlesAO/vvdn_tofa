///
/// @file
/// @copyright All code copyright Movidius Ltd 2018, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     vTrack visualization
///

#include "featuresDrawer.h"
#include "VtrackDrawerMoviOf.h"

VtrackDrawerMoviOf::VtrackDrawerMoviOf(uint32_t frameWidth, uint32_t frameHeight, uint32_t max_features, bool showGraphs) :
        VtrackDrawer(frameWidth, frameHeight, max_features, showGraphs)
{
    stats = new vTrackMoviKpiStats();
}

VtrackDrawerMoviOf::~VtrackDrawerMoviOf()
{
    deleteFeaturesDrawer();
    if(stats)
        delete stats;
}

void VtrackDrawerMoviOf::selectCandidatePoints(cv::Mat *out, t_vTrackResultSF *vTrackRes,
        vTrackKpiStats *stats)
{
    // Process the received features.
    trackFeatures(vTrackRes, (vTrackMoviKpiStats*) stats, m_showGraphs);
    // Draw o/p image and graphs
    drawFeatures(*out);
    if(m_showGraphs) {
        drawLiveGraphs(vTrackRes);
        drawHistograms(vTrackRes);
    }
}

void VtrackDrawerMoviOf::initDrawer()
{
    VtrackDrawer::initDrawer();
    initFeaturesDrawer(maxFeatures);
}
