///
/// @file
/// @copyright All code copyright Movidius Ltd 2018, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     vTrack visualization
///

#ifndef VTRACK_DRAWER_MEST_H
#define VTRACK_DRAWER_MEST_H

#include "VtrackDrawer.h"

////
////    @brief Abstract class of interface to the class that gets the algo results
////
class VtrackDrawerMest : public VtrackDrawer {
public:
    VtrackDrawerMest(uint32_t frameWidth, uint32_t frameHeight, uint32_t max_features, bool showGraphs);
    // Empty virtual destructor
    virtual ~VtrackDrawerMest();

protected:
    virtual void selectCandidatePoints(cv::Mat *out, t_vTrackResultSF *vTrackRes,
            vTrackKpiStats *stats=NULL);
};

#endif // VTRACK_DRAWER_MEST_H
