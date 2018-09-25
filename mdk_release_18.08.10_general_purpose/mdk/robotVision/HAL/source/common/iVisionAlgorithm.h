///
/// @file
/// @copyright All code copyright Movidius Ltd 2018, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief    Abstract class for vision algorithms
///

#ifndef VISION_ALGO_H
#define VISION_ALGO_H

#include <queue>
#include <string>
#include <map>

#include "visionFrame.h"

enum AlgoProcess
{
    ALGO_PROCESS_MYRIAD, ALGO_PROCESS_PC
};
class iVisionAlgorithm{
public:
    virtual void configure(const std::map<std::string, std::string> &options) = 0;
    virtual void getConfigSerialization(char** data, int* size) = 0;

    virtual void initAlgorithm() = 0;
    virtual int runAlgOnFrame(VisionFrame* frameL, VisionFrame* frameR, void* resultsPtr) = 0;
    virtual int visualize() = 0;//TODO:params
    virtual int visualizeKPI() = 0;//TODO:

    AlgoProcess where;
private:
};
#endif
