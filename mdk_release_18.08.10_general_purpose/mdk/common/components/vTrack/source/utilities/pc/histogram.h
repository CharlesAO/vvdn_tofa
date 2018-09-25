///
/// @file
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
///
///
#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__

#include <stdio.h>
#include <unistd.h>
#include "graphPlotter.h"
#include <list>
#define NR_OF_COLUMNS_IN_HISTOGRAM 16
//using namespace std;
class List
{
public:
    int value;
    bool isMaxValue;
    List(int value);
    List();
};
class Histogram
{
    graphPlotter * graph;
    std::list< std::list<List> > list;
    int yAxisValues[NR_OF_COLUMNS_IN_HISTOGRAM];
    bool dynamicLimits;
    int yMax;
    int xMax;
    int nrColumns;
    int yLimit;
    int resolution;
    int nrOfElements;
    int maxValueOnXAxis;
    int maxNumberOfElements;
    char ** legend;
    std::string graphTitle;
    void initValues();
    void callRebuild();
public:
    Histogram(int maxNumberOfElements, const std::string graphTitle , char ** legend = 0 );
    Histogram(int maxNumberOfElements, const std::string graphTitle, int yMax , char **legend = 0);
    Histogram(int maxNumberOfElements, const std::string graphTitle, int nrColumns , int xMax , char **legend = 0);
    Histogram();
    ~Histogram();
    void updateHistogram(std::list<List> newValue);
    void drawHistogram();
};

#endif
