///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Implements graphics with opencv
///
#include <cmath>
#include <cstring>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <string>
#include <iostream>
#include "graphPlotter.h"

#ifdef GUI_VISUALIZER
#include <sys/mman.h>
#include <fcntl.h>    /* For O_RDWR */
#include <unistd.h>   /* For open(), creat() */
#endif


using namespace std;
graphPlotter::graphPlotter(const string graphTitle,
                           unsigned int width, unsigned int height, unsigned int graphsNo, unsigned int xDim,
                           int yMin,
                           int yMax,
                           unsigned int xPts,
                           unsigned int yPts,
                           cv::Scalar *colors,
                           bool isHistogram,
                           char **legend, unsigned int legendSize)
{

    this->frameCounter = 0;
    if (xPts < 20)
        this->xAxisLegendEnable = true;
    else
        this->xAxisLegendEnable = false;
    this->isHistogram = isHistogram;
#ifdef GUI_VISUALIZER
    this->fd_graph = open("/tmp/mem_mapped_graph", O_RDWR | O_CREAT, 0700);
    mmap(0, 640*480*3+100, PROT_READ | PROT_WRITE, MAP_SHARED, this->fd_graph, (mode_t)0700);
    write(this->fd_graph, "Latency\0", 100);
    this->width = 640;
    this->height = 480;
#else
    this->width = width;
    this->height = height;
#endif
    this->graphsNo = graphsNo;
    this->xDim = xDim;
    this->xAxisPts = xPts - 1;
    this->yAxisPts = yPts - 1;
    this->ptsSize = 2;
    this->drawPriority = 0;
    this->xMaximum = xDim;
    this->legendSize = legendSize * 20;
    this->graphTitle = graphTitle;
    for (int i = 0; i < NO_OF_GRAPHS; i++)
    {
        lineColors[i] = cv::Scalar(0, 0, 0);
        pointColors[i] = cv::Scalar(0, 0, 0);
    }
    if (colors)
    {
        for (int i = 0; i < graphsNo; i++)
        {
            lineColors[i] = colors[i];
            pointColors[i] = colors[i];
        }
    }

    if (legend)
    {
        for (int i = 0; i < legendSize; i++)
        {
            strcpy(this->legend[i], legend[i]);
        }
    }
    else
        memset(this->legend, 0, sizeof(this->legend));
    graphs[0].clear();
    this->yMax = yMax;
    this->yMin = yMin;
    this->yDim = abs(yMax) + abs(yMin);
    if(this->yDim == 0){
        this->yDim = 1;
    }
    this->padY = 80 + legendSize + 20;
    this->padX = 80;
    this->scaleX = 1.0 * (width - padX * 2) / xDim;
    this->scaleY = 1.0 * (height - padY * 2) / yDim;
    this->shiftX = this->padX;
    this->shiftY = this->padY + yMax * this->scaleY;
}
void graphPlotter::resetProp(unsigned int xDim, unsigned int xAxisPts, int yMin, int yMax)
{
    this->xAxisPts = xAxisPts - 1;
    this->xDim = xDim;
    this->xMaximum = xDim;
    this->yMax = yMax;
    this->yMin = yMin;
    this->yDim = abs(yMax) + abs(yMin);
    if(this->yDim == 0){
        this->yDim = 1;
    }
    this->padY = 80 + legendSize + 20;
    this->padX = 80;
    this->scaleX = 1.0 * (width - padX * 2) / xDim;
    this->scaleY = 1.0 * (height - padY * 2) / yDim;
    this->shiftX = this->padX;
    this->shiftY = this->padY + yMax * this->scaleY;
    for (int i = 0; i < NO_OF_GRAPHS; i++)
    {
        graphs[i].clear();
    }
}

void graphPlotter::addPointToTheLiveGraph(unsigned int idx, float y)
{
    addPointToGraph(idx,graphs[idx].size(),y);
}
void graphPlotter::addPointToGraph(unsigned int idx, float x, float y)
{
    graphs[idx].push_back(std::make_pair(x, -y));
}

void graphPlotter::renderGraph(unsigned int idx)
{
    if (graphs[idx].empty())
        return;

    if (drawPriority == 0)
    {
        drawPriority = 1;
        outputImg = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
    }

    int i, j, k = 1;
    int newX[2], newY[2], minusX = 0;

    i = graphs[idx].size() - 1;
    if(!isHistogram)
    {
        xMaximum = std::max(graphs[idx][i].first, (float) xDim);

        minusX = graphs[idx][i].first - xDim;
        if (minusX <= 0)
            minusX = 0;
        newX[0] = shiftX + (graphs[idx][i].first - minusX) * scaleX;
        newY[0] = shiftY + (graphs[idx][i].second) * scaleY;
        newX[1] = shiftX + (graphs[idx][i-1].first - minusX) * scaleX;
        newY[1] = shiftY + (graphs[idx][i-1].second) * scaleY;

        cv::circle(outputImg,
                   cv::Point2f(newX[0], newY[0]),
                   ptsSize,
                   pointColors[idx],
                   -1, CV_AA);
    }
    for (j = i--; i >= 0 && k < xDim; i--, j--, k++)
    {
        newX[0] = shiftX + (graphs[idx][i].first - minusX) * scaleX;
        newY[0] = shiftY + (graphs[idx][i].second) * scaleY;
        newX[1] = shiftX + (graphs[idx][j].first - minusX) * scaleX;
        newY[1] = shiftY + (graphs[idx][j].second) * scaleY;

        if(!isHistogram)
        {
            cv::circle(outputImg,
                       cv::Point2f(newX[0], newY[0]),
                       ptsSize,
                       pointColors[idx],
                       -1,
                       CV_AA);
            cv::line(outputImg,
                     cv::Point2f(newX[0], newY[0]),
                     cv::Point2f(newX[1], newY[1]),
                     lineColors[idx],
                     ptsSize,
                     CV_AA);
        }
        else

        {
            cv::rectangle(outputImg,
                          cv::Point2f(newX[0],shiftY),
                          cv::Point2f(newX[1], newY[0]),
                          lineColors[idx],
                          CV_FILLED);
        }


    }

    if (drawPriority == 1)
    {
        plotGraph();
        drawPriority = 0;
    }
}

void graphPlotter::renderAllGraphs()
{
    frameCounter = (frameCounter + 1)%FPS_DIVIDER;
    if(frameCounter == 1)
    {
        drawPriority = 2;
        outputImg = cv::Mat(height, width, CV_8UC3, cv::Scalar(255, 255, 255));
        for (int i = 0; i < NO_OF_GRAPHS; i++)
            renderGraph(i);
        plotGraph();
        drawPriority = 0;
    }
}

void graphPlotter::plotGraph()
{
    std::ostringstream outstr;
    outstr.clear();

    //y-axis
    cv::line(outputImg,
             cv::Point2f(padX,
                         1.5 * padY + (abs(yMax) + abs(yMin)) * scaleY),
                         cv::Point2f(padX, 0.5 * padY),
                         cv::Scalar(0, 0, 0),
                         4,
                         CV_AA);
    for (int i = 0; i <= yAxisPts; i++)
    {
        int yPos = padY + i * (yDim / yAxisPts) * scaleY;
        cv::line(outputImg, cv::Point(padX - 10, yPos), cv::Point(padX, yPos),
                 cv::Scalar(0, 0, 0),
                 1, CV_AA);

        outstr << std::setw(6) << (int) (yMax - i * (yDim / yAxisPts));
        cv::putText(outputImg, outstr.str(),
                    cv::Point(0.2 * padX, yPos + 5),
                    cv::FONT_HERSHEY_SIMPLEX,
                    0.45,
                    cv::Scalar(0, 0, 0), 1, CV_AA);
        outstr.str("");
    }

    //x-axis
    cv::line(outputImg, cv::Point2f(padX, shiftY),
             cv::Point2f(1.5 * padX + xDim * scaleX, shiftY),
             cv::Scalar(0, 0, 0), 4,
             CV_AA);
    if(xAxisLegendEnable)
    {
        for (int i = 0; i <= xAxisPts; i++)
        {
            int xPos = shiftX + (i*xDim/(xAxisPts+1)) * scaleX;
            cv::line(outputImg, cv::Point(xPos, shiftY - 10),
                     cv::Point(xPos, shiftY + 10),
                     cv::Scalar(0, 0, 0),
                     1,
                     CV_AA);
            outstr << std::setw(4) << (i * (xDim / (xAxisPts+1)));
            cv::putText(outputImg, outstr.str(),
                        cv::Point(xPos - 15, shiftY + 25),
                        cv::FONT_HERSHEY_SIMPLEX,
                        0.45,
                        cv::Scalar(0, 0, 0), 1, CV_AA);
            outstr.str("");
        }

        outstr << "x" << std::setprecision(4) << (float) (xMaximum / xDim);
        cv::putText(outputImg, outstr.str(),
                    cv::Point(width - 0.7 * padX, shiftY + 30),
                    cv::FONT_HERSHEY_SIMPLEX,
                    0.45,
                    cv::Scalar(0, 0, 0), 1, CV_AA);
        outstr.str("");
    }

    for (int j = 0; j*20< legendSize; j++)
    {
        cv::putText(outputImg, legend[j],
                    cv::Point(width / 2, 20 + j * 20),
                    cv::FONT_HERSHEY_SIMPLEX,
                    0.5,
                    cv::Scalar(0,0,0), 1, CV_AA);
    }

#ifdef GUI_VISUALIZER
    char title[100];
    read(this->fd_graph, title, 100);
    if( strcmp(title, this->graphTitle.c_str())==0 ){
        write(this->fd_graph, outputImg.data, 640*480*3);
    }
    lseek(this->fd_graph, 0, SEEK_SET);
#else
    cv::imshow(graphTitle, outputImg);
#endif
    outputImg.release();

}
