///
/// @file
/// @copyright All code copyright Movidius Ltd 2018, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Stereo PC wrapper
///


#ifndef PC_STEREO_H
#define PC_STEREO_H

#include "visionFrame.h"
#include <array>
#include "iVisionAlgorithm.h"

class PcStereo: public iVisionAlgorithm {
public:
    PcStereo(std::array<char, 36> calib_array);
    virtual ~PcStereo();
    void configure(const std::map<std::string, std::string> &options);
    void getConfigSerialization(char** data, int* size);
    void initAlgorithm();
    int runAlgOnFrame(VisionFrame* frameL, VisionFrame* frameR, void* resultsPtr);
    int visualize();//TODO:params
    int visualizeKPI();//TODO:


private:
    std::array<char, 36> m_calib_array;
};
#endif
