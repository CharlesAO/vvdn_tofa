///
/// @file
/// @copyright All code copyright Movidius Ltd 2018, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     vTrack visualization
///

/// compiler includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <fstream>
// vTrackModules
#include "vTrack.h"
#include "vPipe.h"
#include "global_constants.h"
#include "pc_constants.h"
#include "featuresDrawer.h"
#include "graphPlotter.h"
//openCv includes
#include <highgui.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/opencv.hpp>
//common components
#include <superFrame.h>
#include <sys/time.h>
///  Local Modules
#include <pcVtrack.h>
#include "VtrackDrawerMest.h"
#include "visualizer.hpp"
//TODO: clean header files

using namespace std;
using namespace cv;

VtrackDrawerMest::VtrackDrawerMest(uint32_t frameWidth, uint32_t frameHeight, uint32_t max_features, bool showGraphs) :
        VtrackDrawer(frameWidth, frameHeight, max_features, showGraphs)
{
    stats = new vTrackMestKpiStats();
}

VtrackDrawerMest::~VtrackDrawerMest()
{
    if(stats)
        delete stats;
}

void VtrackDrawerMest::selectCandidatePoints(Mat *out, t_vTrackResultSF *vTrackRes,
            vTrackKpiStats *stats)
{
    mvVisualizer vis;
    vector<Point2f> src, dst;
    int inliersCnt = 0;

    if(stats)
    {
        stats->totalFeatures += vTrackRes->prev.featuresCount;
    }

    for (uint32_t i = 0; i < vTrackRes->prev.featuresCount; i++)
    {
        for (uint32_t j = 0; j < vTrackRes->curr.featuresCount; j++)
        {
            if (vTrackRes->prev.metadata.ptr[i].id == vTrackRes->curr.metadata.ptr[j].id)
            {
                const tvXYLoc &prev = vTrackRes->prev.features.ptr[i];
                const tvXYLoc &curr = vTrackRes->curr.features.ptr[j];
                src.push_back(Point2f(prev.x, prev.y));
                dst.push_back(Point2f(curr.x, curr.y));
                inliersCnt++;
                break;
            }
        }
    }
    if(stats)
    {
        stats->totalInliers += inliersCnt;
    }
    *out = vis.drawFlow(*out, src, dst, Scalar(0, 255, 0), Scalar(0, 0, 255));
}
