///
/// @file
/// @copyright All code copyright Movidius Ltd 2018, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     Stereo PC wrapper
///

/// compiler includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <cstring>
#include <iostream>
#include <fstream>
// vTrackModules
#include "pcStereo.h"

//TODO: clean header files


/// Namespace
using namespace std;



PcStereo::PcStereo(array<char, 36> calib_array) :
    m_calib_array(calib_array) {
    where = ALGO_PROCESS_MYRIAD;
}
void
PcStereo::configure(const std::map<std::string, std::string> &options) {

}
void
PcStereo::getConfigSerialization(char** data, int* size) {
//    char* serializedData;
    *data = (char*) malloc(m_calib_array.size());
    memcpy(*data, m_calib_array.data(), m_calib_array.size());
    *size = m_calib_array.size();
}

int
PcStereo::visualize() {

}
int
PcStereo::visualizeKPI() {

}

PcStereo::~PcStereo() {
}
void
PcStereo::initAlgorithm() {
}
int PcStereo::runAlgOnFrame(VisionFrame* frameL, VisionFrame* frameR, void* resultsPtr){
    assert(0); //TODO: not implemented
    return 0;
}

