///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved
///            For License Warranty see: common/license.txt
///
/// @brief  Implements functionality for tracking and displaying image features.
///
#include "featuresDrawer.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include "histogram.h"
#include "coverage.h"
#include "inliers.h"
#include "vtrackStats.h"

#define EnableHistogramTrackingLength
#define EnableHistogramCurrentAge
#define EnableLiveCoverage
#define EnableHistogramNrOfFeatures
#define EnableHistogramCoverage
#define EnableLiveNrOfFeartures
#define EnableHistogramInliers

#define WEBCAM_VGA

#if defined(MYRIAD_CAMERA)
    #define fxCam 686.70
    #define fyCam 689.57
    #define cxCam 312.41
    #define cyCam 245.93
    #define wCam 0.926859
    #define dist0 -4.3152944534888320e-002
    #define dist1 4.1044229073992000e-001
#endif

using namespace cv;
using namespace std;

// List of all tracked features.
uint32_t maxFeatures;
FeatureTrackerProperties* trackedFeatures;
// Flags for marking if the new features have been analyzed or not.
uint8_t* featureAnalyzed;

char* currentAgeLegend[2] = {(char *)"X axis: Current Age [frames]", (char *)"Y axis: NO Features"};
char* trackingLengthLegend[2] = {(char *)"X axis: Maximum age [frames]", (char *)"Y axis: NO Features"};
char* NOFeaturesLegend[2] = {(char *)"X axis: NO Features ", (char *)"Y axis: NO Frames"};
char* CoverageLegend[2] = {(char *)"X axis: Coverage [pixels]", (char *)"Y axis: NO Frames"};
char* InliersLegend[2] = {(char *)"X axis: Inliers percentage ", (char *)"Y axis: NO Frames"};
char* LiveCoverageLegend[2] = {(char *)"X axis: last 100 frames ", (char *)"Y axis: Coverage [pixels]"};
char* LiveNOFeaturesLegend[2] = {(char *)"X axis: last 100 frames ", (char *)"Y axis: NO Features"};
Histogram histogramCurrentAge(1,"Current age histogram",currentAgeLegend);
Histogram histogramTrackingLength(NR_FRAMES,"Tracking length histogram", 1000,trackingLengthLegend);
Histogram histogramNOFeatures(NR_FRAMES,"Nr Of Features Histogram",NOFeaturesLegend);
Histogram histogramCoverage(NR_FRAMES,"Coverage",CoverageLegend);
#ifdef EnableHistogramInliers
Histogram histogramInliers(NR_FRAMES,"Inliers Histogram",12,120,InliersLegend);
#endif


graphPlotter * featureNoGraph;
graphPlotter * coverageGraph;
#if defined(MYRIAD_CAMERA)
Inliers inliersCalculator(fxCam, fyCam, cxCam, cyCam, true , true, dist0 , dist1);
#elif defined(WEBCAM_VGA)
Inliers inliersCalculator(640, 480, 320, 240, true , false, 0 , 0);
#elif defined(WEBCAM_720P)
Inliers inliersCalculator(1280, 720, 640, 360, true , false, 0 , 0);
#endif

void drawHistograms(t_vTrackResultSF* vTrackRes)
{
#ifdef EnableHistogramNrOfFeatures
    std::list<List> tmpList;
    List tmpObj(vTrackRes->curr.featuresCount);
    tmpList.push_back(tmpObj);
    histogramNOFeatures.updateHistogram(tmpList);
    histogramNOFeatures.drawHistogram();
#endif
#ifdef EnableHistogramCoverage
    int i;
    float coverage;
    std::vector<std::pair<float , float> > covList;
    std::list<List> tmpList2;
    for (i = 0; i < vTrackRes->curr.featuresCount; i++)
    {
        covList.push_back(std::make_pair(vTrackRes->curr.features.ptr[i].x,
                vTrackRes->curr.features.ptr[i].y));
    }
    coverage=calcCoverage(covList);
    List tmpRez((int)coverage);
    tmpList2.push_back(tmpRez);
    histogramCoverage.updateHistogram(tmpList2);
    histogramCoverage.drawHistogram();
#endif
}
void initLiveGraphPlotter()
{
    cv::Scalar colors[2] = {cv::Scalar(255, 0, 0), cv::Scalar(0, 0, 255)};
#ifdef EnableLiveNrOfFeartures
    featureNoGraph=new graphPlotter("Live Feature count",
                                    640, 320, 1, 100, 0,
                                    maxFeatures,
                                    100,
                                    10,
                                    colors,
                                    false,
                                    LiveNOFeaturesLegend,2);
#endif
#ifdef  EnableLiveCoverage
    coverageGraph=new graphPlotter("Live Coverage",
                                   640, 320, 1, 100, 0,
                                   300,
                                   100,
                                   10,
                                   colors,
                                   false,
                                   LiveCoverageLegend,2);
#endif
}

void deleteLiveGraphPlotter()
{
#ifdef EnableLiveNrOfFeartures
    if(featureNoGraph)
        delete featureNoGraph;
#endif
#ifdef  EnableLiveCoverage
    if(coverageGraph)
        delete coverageGraph;
#endif
}

void drawLiveGraphs(t_vTrackResultSF* vTrackRes)
{
#ifdef EnableLiveNrOfFeartures
    featureNoGraph->addPointToTheLiveGraph(0,vTrackRes->curr.featuresCount);
    featureNoGraph->renderAllGraphs();
#endif
#ifdef EnableLiveCoverage
    std::vector<std::pair<float, float> > covList;
    covList.clear();
    float coverage;
    int i;
    for (i = 0; i < vTrackRes->curr.featuresCount; i++)
    {
        covList.push_back(std::make_pair(vTrackRes->curr.features.ptr[i].x,
                vTrackRes->curr.features.ptr[i].y));
    }
    coverage=calcCoverage(covList);
    coverageGraph->addPointToTheLiveGraph(0, coverage);
    coverageGraph->renderAllGraphs();
#endif
}
// Adds a new point to the feature's path.
static void addTrackPointToPath(FeatureTrackerProperties* pTrackedFeature,
        float newPositionX, float newPositionY)
{
    const int nrOfFeaturesTrackedMotion = 15;
    pTrackedFeature->path.push_back(Points(newPositionX, newPositionY));
    if(pTrackedFeature->path.size() == nrOfFeaturesTrackedMotion + 1)
        pTrackedFeature->path.pop_front();
    pTrackedFeature->numTrackedPoints++;
}
// Draws a feature point.
void drawPoint(Mat& img, int x, int y, cv::Scalar color)
{
    circle( img, Point(x,y), 1, color, -1, CV_AA, 0);
}

void drawLine(Mat& img, int x1, int y1, int x2, int y2)
{
    line(img, Point(x1,y1), Point(x2,y2),  CV_RGB(200,0,200), 1, CV_AA, 0);
}

static inline void initTrackingBufferElement(int i)
{
    trackedFeatures[i].path.clear();
    trackedFeatures[i].numTrackedPoints = 0;
    trackedFeatures[i].lastElement = 0;
    trackedFeatures[i].featureId = -1;
}

// Initializes the tracking buffers.
void initTrackingBuffers()
{
    for(int i = 0; i < maxFeatures; i++)
    {
        initTrackingBufferElement(i);
    }
}
void initFeaturesDrawer(uint32_t max_features)
{
    maxFeatures = max_features;
    trackedFeatures = new FeatureTrackerProperties[max_features];
    initTrackingBuffers();
    featureAnalyzed = new uint8_t[max_features];

    initLiveGraphPlotter();
}
void deleteFeaturesDrawer()
{
    if(trackedFeatures) {
        delete [] trackedFeatures;
}
    if(featureAnalyzed)
        delete []featureAnalyzed;

    deleteLiveGraphPlotter();
}
// Updates the existing tracked features with new data.
// Known features are either
//     removed (if no movement occurred since last update) or
//     updated (the feature's path is extended with the new position).
// Unknown features are handled by addNewFeatures().
static void trackExistingFeatures(t_vTrackResultSF* features, vTrackMoviKpiStats *stats, bool showGraphs)
{
    int i, j, k, x, y;
    std::list<List> currentAgeTmpList;
    std::list<List> trackingLenTmpList;
    if((uint32_t)features->curr.featuresCount >= maxFeatures)
        return;

    // Clear flags used in tracking.
    for(i = 0; i < maxFeatures; i++)
    {
        featureAnalyzed[i] = 0;
        trackedFeatures[i].keepTracking = FALSE;
        trackedFeatures[i].isInlier = FALSE;
    }

    if(stats)
    {
        stats->totalFeatures += features->curr.featuresCount;
    }

    // Go through all current features
    for(i = 0; i < features->curr.featuresCount; i++)
    {
        // Go through all tracked features
        for(j = 0; j < maxFeatures; j++)
        {
            // Known feature
            if(features->curr.metadata.ptr[i].id == trackedFeatures[j].featureId)
            {
                featureAnalyzed[i] = TRUE;

                x = (int)features->curr.features.ptr[i].x;
                y = (int)features->curr.features.ptr[i].y;

                // New movement: continue to track this features.
                trackedFeatures[j].keepTracking = TRUE;
                inliersCalculator.addMatchesToList((bool *)(&trackedFeatures[j].isInlier),
                        features->curr.features.ptr[i].x,
                        features->curr.features.ptr[i].y,
                        trackedFeatures[j].path.back().x,
                        trackedFeatures[j].path.back().y);

                addTrackPointToPath(&trackedFeatures[j],
                                    features->curr.features.ptr[i].x,
                                    features->curr.features.ptr[i].y);
                break;
            }
        }
    }

    List tmpInliersObj(inliersCalculator.calcPercentageInliers());
    if(showGraphs){
#ifdef EnableHistogramInliers
        std::list<List> inliersTmpList;
        inliersTmpList.push_back(tmpInliersObj);
        histogramInliers.updateHistogram(inliersTmpList);
        histogramInliers.drawHistogram();
#endif

        //Tracking Length Histogram

        for(j = 0; j < maxFeatures; j++)
            if(trackedFeatures[j].keepTracking)
            {
#ifdef EnableHistogramCurrentAge
                List tmpObj(trackedFeatures[j].numTrackedPoints);
                currentAgeTmpList.push_back(tmpObj);
#endif
            }else if (trackedFeatures[j].featureId != -1)
            {
#ifdef EnableHistogramTrackingLength
                List tmpObj(trackedFeatures[j].numTrackedPoints);
                trackingLenTmpList.push_back(tmpObj);
#endif
            }
#ifdef EnableHistogramCurrentAge
        histogramCurrentAge.updateHistogram(currentAgeTmpList);
        histogramCurrentAge.drawHistogram();
#endif
#ifdef EnableHistogramTrackingLength
        histogramTrackingLength.updateHistogram(trackingLenTmpList);
        histogramTrackingLength.drawHistogram();
#endif
    }
    // Remove features marked for removal.
    for(j = 0; j < maxFeatures; j++)
        if (!trackedFeatures[j].keepTracking)
        {
            if (stats && trackedFeatures[j].featureId != -1)
            {
                stats->totalTrackingLength += trackedFeatures[j].numTrackedPoints;
                stats->nrOfTrackingLength++;
                stats->totalInliers += (trackedFeatures[j].isInlier == true);
            }
            initTrackingBufferElement(j);
        }
        else if (stats)
        {
            stats->totalTrackedFeatures++;
            stats->totalInliers += (trackedFeatures[j].isInlier == true);
        }
}

// Adds new features to the tracking array.
static void trackNewFeatures(t_vTrackResultSF* features, vTrackMoviKpiStats *stats)
{
    int i, j;

    if((uint32_t)features->curr.featuresCount >= maxFeatures)
        return;

    // Go through all features received.
    for(i = 0, j = 0; i < features->curr.featuresCount; i++)
    {
        // Skip known feature.
        if(TRUE == featureAnalyzed[i])
            continue;

        // Add new feature.
        while(TRUE == trackedFeatures[j].keepTracking)  // Find an empty location.
            j++;
        if(j >= maxFeatures)
            return;
        trackedFeatures[j].keepTracking = TRUE;
        trackedFeatures[j].featureId = features->curr.metadata.ptr[i].id;
        addTrackPointToPath(&trackedFeatures[j], features->curr.features.ptr[i].x,
                features->curr.features.ptr[i].y);
    }
}
void trackFeatures(t_vTrackResultSF* features, vTrackMoviKpiStats *stats, bool showGraphs)
{
    trackExistingFeatures(features, stats, showGraphs);
    trackNewFeatures(features, stats);
}
// Draws the points and paths for all tracked features.
void drawFeatures(Mat& img)
{
    int i, j, k, x1, y1, x2, y2, startPos, limitPos;

    // For every feature,
    for(i = 0; i < maxFeatures; i++)
    {
        if(FALSE == trackedFeatures[i].keepTracking)
            continue;
        startPos = 0;
        limitPos = (int) trackedFeatures[i].path.size() - 1;

        // Draw the feature movement path.
        for(j = startPos; j < limitPos; j++)
        {
            x1 = trackedFeatures[i].path[j].x;
            y1 = trackedFeatures[i].path[j].y;

            x2 = trackedFeatures[i].path[j + 1].x;
            y2 = trackedFeatures[i].path[j + 1].y;

            drawLine(img, x1, y1, x2, y2);
        }

        //Draw the feature points.
        x1 = trackedFeatures[i].path[j].x;
        y1 = trackedFeatures[i].path[j].y;
        if(trackedFeatures[i].isInlier)
        {
            drawPoint(img, x1, y1,cv::Scalar(0,255,0));
        }
        else
        {
            drawPoint(img, x1, y1,cv::Scalar(0,0,255));
        }
    }
}
