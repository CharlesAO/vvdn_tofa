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
#include "VtrackDrawer.h"

//TODO: clean header files
#define  EnableLiveLatency


static cv::Mat final_output;

// graphs description and buffers
cv::Scalar latencyColors[2] = {cv::Scalar(255, 0, 0), cv::Scalar(0, 0, 255)};
char* latencyLegend[2] = {(char *)"TotalLatency [ms] ", (char *)"vTrack processing [ms] "};


cv::Mat* getColorMatFrombuffer(uint8_t* data, uint32_t frameWidth, uint32_t frameHeight)
{
    cv::Mat grayscaleFrame;

    grayscaleFrame = cv::Mat(cv::Size(frameWidth, frameHeight),  CV_8UC1, data);

//    if(frameWidth != final_output.cols && frameHeight != final_output.rows)
        final_output = cv::Mat(cv::Size(frameWidth, frameHeight),  CV_8UC3);

    cvtColor(grayscaleFrame, final_output, CV_GRAY2BGR);
    return &final_output;
}

VtrackDrawer::VtrackDrawer(uint32_t frameWidth, uint32_t frameHeight, uint32_t max_features, bool showGraphs) :
    m_frameWidth(frameWidth),
    m_frameHeight(frameHeight),
    maxFeatures(max_features),
    stats(NULL),
    previousTimestampNs(0),
    countFrames(0),
    avgFps(0.0F),
    m_showGraphs(showGraphs),
    latencyGraph(NULL){
}

void
VtrackDrawer::initDrawer() {
    if(m_showGraphs) {
#ifdef EnableLiveLatency
        latencyGraph=new graphPlotter("Latency",
                m_frameWidth, m_frameHeight, 2, 100, 0,
                50,
                100,
                10,
                latencyColors,
                false,
                latencyLegend,
                2);
#endif
    }
}

void
VtrackDrawer::changeDrawerDims(uint32_t frameWidth, uint32_t frameHeight) {
    //TODO: not supported yet
    assert(0);
}
int
VtrackDrawer::draw(VisionFrame* frame1, uint8_t* customMeta) {
    t_vTrackResultSF* vTrackRes = (t_vTrackResultSF*)(customMeta);
    if(m_showGraphs) {
#ifdef EnableLiveLatency
        //    latencyGraph->addPointToTheLiveGraph(0, (genericMeta->latencyNs/1000000));
        latencyGraph->addPointToTheLiveGraph(1, (vTrackRes->debug.fmTime + vTrackRes->debug.ppTime + vTrackRes->debug.ofTime)/1000000);
        latencyGraph->renderAllGraphs();
#endif
    }
    //Update average fps
    //Assumes that the drawer is given ALL frames of the vTrack output
    uint64_t timestampNs = vTrackRes->header.timestampNs;
    float fps = (timestampNs - previousTimestampNs)/1000;
    if(fps == 0 )
        printf("repeat at framecount %d\n",countFrames);
    else{
        fps = 1000000 / fps;
        avgFps = (avgFps * countFrames + fps) / (++countFrames);
    }
    previousTimestampNs = timestampNs;

    char key;
    std::ostringstream outstr;
    outstr.clear();
    cv::Mat* out = getColorMatFrombuffer(frame1->p1, frame1->width, frame1->height);

    selectCandidatePoints(out, vTrackRes, stats);

    if(stats)
    {
        stats->framesCnt++;
    }

    outstr<<"FPS :"<< std::setprecision(4) <<avgFps;
    cv::putText(*out, outstr.str() ,
                cv::Point(20, 30),
                cv::FONT_HERSHEY_SIMPLEX,
                0.45,
                cv::Scalar(255, 255, 255), 1, CV_AA);
    cv::imshow("Stream", *out);

    out->release();
    key = cvWaitKey(1);
    return key;
}

cv::Mat*
VtrackDrawer::draw_on_memory(VisionFrame* frame1, uint8_t* customMeta) {
    t_vTrackResultSF* vTrackRes = (t_vTrackResultSF*)(customMeta);
    if(m_showGraphs) {
#ifdef EnableLiveLatency
        //    latencyGraph->addPointToTheLiveGraph(0, (genericMeta->latencyNs/1000000));
        latencyGraph->addPointToTheLiveGraph(1, (vTrackRes->debug.fmTime + vTrackRes->debug.ppTime + vTrackRes->debug.ofTime)/1000000);
        latencyGraph->renderAllGraphs();
#endif
    }
    //Update average fps
    //Assumes that the drawer is given ALL frames of the vTrack output
    uint64_t timestampNs = vTrackRes->header.timestampNs;
    float fps = (timestampNs - previousTimestampNs)/1000;
    if(fps == 0 )
        printf("repeat at framecount %d\n",countFrames);
    else{
        fps = 1000000 / fps;
        avgFps = (avgFps * countFrames + fps) / (++countFrames);
    }
    previousTimestampNs = timestampNs;

    char key;
    std::ostringstream outstr;
    outstr.clear();
    cv::Mat* out = getColorMatFrombuffer(frame1->p1, frame1->width, frame1->height);

    selectCandidatePoints(out, vTrackRes, stats);

    if(stats)
    {
        stats->framesCnt++;
    }

    //cv::imshow("Stream", *out);

    //out->release();
    //key = cvWaitKey(1);
    return out;
}

vTrackKpiStats* VtrackDrawer::getStatistics()
{
    return stats;
}
