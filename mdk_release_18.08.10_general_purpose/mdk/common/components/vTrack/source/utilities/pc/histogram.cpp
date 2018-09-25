///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Implements the histograms
///
#include <stdio.h>
#include <unistd.h>

#include <list>
#include "histogram.h"
using namespace std;
List::List(int value)
{
    List::value = value;
    isMaxValue = false;
}
List::List()
{

}

Histogram::Histogram(int maxNumberOfElements, const string graphTitle , int nrColumns , int xMax, char **legend)
{
    this->graphTitle = graphTitle;
    this->maxNumberOfElements = maxNumberOfElements;
    nrOfElements = 0;
    resolution = xMax/nrColumns;
    dynamicLimits = true;
    this->legend = legend;
    this->nrColumns = nrColumns;
    this->xMax = xMax;
    cv::Scalar Colors[] = {cv::Scalar(255, 0, 0), cv::Scalar(255, 0, 0)};
    graph=new graphPlotter(graphTitle, 800, 480, 1, nrColumns*resolution, 0,
                           20,
                           nrColumns,
                           10,
                           Colors,
                           true,legend,2);
    initValues();
}
Histogram::~Histogram()
{
    if(graph)
        delete graph;
}
Histogram::Histogram(int maxNumberOfElements, const string graphTitle, char ** legend)
{
    int i;
    this->graphTitle = graphTitle;
    this->maxNumberOfElements = maxNumberOfElements;
    nrOfElements = 0;
    resolution = 1;
    yMax = 0;
    dynamicLimits = true;
    this->legend = legend;
    this->nrColumns = NR_OF_COLUMNS_IN_HISTOGRAM;
    cv::Scalar Colors[] = {cv::Scalar(255, 0, 0), cv::Scalar(255, 0, 0)};
    graph=new graphPlotter(graphTitle, 800, 480, 1, nrColumns*resolution, 0,
                           20,
                           nrColumns,
                           10,
                           Colors,
                           true,legend,2);
    initValues();
}
Histogram::Histogram(int maxNumberOfElements, const string graphTitle , int yMax, char ** legend)
{
    this->yMax = yMax;
    this->graphTitle = graphTitle;
    this->maxNumberOfElements = maxNumberOfElements;
    nrOfElements = 0;
    resolution = 1;
    dynamicLimits = false;
    this->nrColumns = NR_OF_COLUMNS_IN_HISTOGRAM;
    this->legend = legend;
    cv::Scalar Colors[] = {cv::Scalar(255, 0, 0), cv::Scalar(255, 0, 0)};
    graph=new graphPlotter(graphTitle, 800, 480, 1, nrColumns*resolution, 0,
                           20,
                           nrColumns,
                           10,
                           Colors,
                           true,legend,2);
    initValues();
}
void Histogram::initValues()
{
    int i;


    for(i = 0; i < NR_OF_COLUMNS_IN_HISTOGRAM ; i++ )
    {
        yAxisValues[i]=0;
    }
}
void Histogram::drawHistogram()
{
    graph->renderAllGraphs();

}
void Histogram::callRebuild()
{
    int i;
    std::list<std::list<List> >::iterator max = list.begin();
    std::list<std::list<List> >::iterator it=list.begin();
    std::list<List>::iterator pmax=max->begin();
    pmax->isMaxValue=true;
    std::list<List>::iterator p = ++pmax;
    --pmax;
    if(xMax == 0)
    {
        maxValueOnXAxis = pmax->value;
        for (; it != list.end(); )
        {
            for( ; p!=it->end() ; ++p)
            {
                if(p->value > pmax->value)
                {
                    pmax->isMaxValue = false;
                    pmax = p;
                    p->isMaxValue = true;
                }

            }
            it++;
            p=it->begin();

        }
        maxValueOnXAxis = pmax->value;
        resolution = maxValueOnXAxis/nrColumns + 1;
    }
    graph->resetProp(nrColumns*resolution, nrColumns, 0, 20);
    initValues();
    it=list.begin();
    p=it->begin();
    for (; it != list.end(); )
    {
        for(; p!=it->end() ; ++p)
        {
            i=p->value/resolution;
            yAxisValues[i]++;

        }
        it++;
        p=it->begin();

    }
}
void Histogram::updateHistogram(std::list<List> newValue)
{
    int i;

    list.push_back(newValue);
    std::list<List> root = list.front();
    std::list<List> last = list.back();
    if(list.size() > maxNumberOfElements)
    {
        list.pop_front();
    }
    callRebuild();

    for(i = 0; i <  nrColumns; i++)
    {
        if(yAxisValues[i] > yMax)
        {
            if(dynamicLimits)
            {
                yMax = yAxisValues[i];
            }
            else
            {
                yAxisValues[i] = yMax;
            }
        }
    }
    if(xMax == 0)
    {
        graph->resetProp(NR_OF_COLUMNS_IN_HISTOGRAM*resolution,NR_OF_COLUMNS_IN_HISTOGRAM,0,yMax);
    }
    else
    {
        graph->resetProp(xMax, nrColumns, 0 , yMax);

    }
    for(i = 0; i <  nrColumns; i++)
    {
        graph->addPointToGraph(0, i*(resolution), yAxisValues[i]);
    }



}
