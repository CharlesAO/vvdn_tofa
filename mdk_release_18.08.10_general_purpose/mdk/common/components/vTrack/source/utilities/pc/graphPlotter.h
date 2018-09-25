///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
///
///
#ifndef GRAPHPLOTTER_H
#define GRAPHPLOTTER_H

#include <vector>
#include <opencv2/opencv.hpp>
#define NO_OF_GRAPHS 20
#define FPS_DIVIDER 4

class graphPlotter
{
private:
    int xDim, yDim, padX, padY, yMax, yMin;
    unsigned int xAxisPts, yAxisPts;
    unsigned int legendSize;
    float shiftX, shiftY, scaleX, scaleY;
    unsigned int width, height, ptsSize, graphsNo;
    int drawPriority;
    int frameCounter;
    bool isHistogram;
    float xMaximum;
    std::string graphTitle;
    bool xAxisLegendEnable;
    std::vector<std::pair<float, float> > graphs[NO_OF_GRAPHS];
    char legend[NO_OF_GRAPHS][30];
    #ifdef GUI_VISUALIZER
    int fd_graph;
    #endif
    cv::Scalar pointColors[NO_OF_GRAPHS];
    cv::Scalar lineColors[NO_OF_GRAPHS];
    cv::Mat outputImg;


    void plotGraph();

public:
    graphPlotter(const std::string graphTitle,
                 unsigned int width, unsigned int height, unsigned int graphsNo, unsigned int xDim, int yMin,
                 int yMax,
                 unsigned int xPts = 10,
                 unsigned int yPts = 10,
                 cv::Scalar *colors = 0,
                 bool isHistogram = false,
                 char **legend = 0,
                 unsigned int legendSize = 0
                 );
    void resetProp(unsigned int xDim, unsigned int xAxisPts, int yMin, int yMax);
    void addPointToTheLiveGraph(unsigned int idx, float y);
    void addPointToGraph(unsigned int gr, float x, float y);
    void renderGraph(unsigned int index);
    void renderAllGraphs();
};

#endif

