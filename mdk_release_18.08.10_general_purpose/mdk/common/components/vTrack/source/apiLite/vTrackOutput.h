///
/// @file
/// @copyright All code copyright Movidius Ltd 2012, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     vTrack output structure definitions
///
#ifndef VTRACKOUTPUT_H_
#define VTRACKOUTPUT_H_
#include <stdint.h>
#include <string.h>

#define AGE_HIST_COUNT 16
#define AGE_HIST_STEP 2

// Used for X,Y locations
typedef struct
{
    float x;      // x location
    float y;      // y location
} tvXYLoc;

// Metadata structure related to each feature
typedef struct
{
    uint32_t id;              // feature ID
    uint32_t age;             // feature age in frames
    float harris_score;    // feature harris score
} FeatureMetadata;

typedef struct
{
    uint64_t ppTime;     // Pixel Pipeline's time (ns)
    uint64_t ofTime;     // Optical Flow's time (ns)
    uint64_t fmTime;     // Feature Maintenance's time (ns)
    uint32_t vTrackFrameCount;
    uint32_t ppKeypoints;
    uint32_t ofInputKeypoints;
    uint32_t fmInputKeypoints;
    uint32_t avgThreshold;
    uint32_t ageHist[AGE_HIST_COUNT];
    uint64_t miscTime;   // Scheduling time (initialization, memcpy etc)
    uint64_t totalTime;   // Sum of the above three + the scheduling time
    uint32_t maxPreStart; // The pixel pipe can be started earlier with this amount of clock cycles
    uint32_t info[37];   // Extra debug & profiling data
} t_vTrackDebug;

typedef struct
{
    uint64_t timestampNs;
    uint64_t frameId;

} t_vTrackOutputHeader;

//Use unions to ensure 64 bits used to store pointer regardless of the platform
union featUnion {
    tvXYLoc* ptr;
    uint64_t dummy;
};

union metadataUnion {
    FeatureMetadata* ptr;
    uint64_t dummy;
};

union trackingErrorUnion {
    float* ptr;
    uint64_t dummy;
};

union cellFeatCountUnion {
    uint32_t* ptr;
    uint64_t dummy;
};

union thresholdsUnion {
    float* ptr;
    uint64_t dummy;
};
typedef struct
{
    uint32_t featuresCount;
    featUnion features;
    metadataUnion metadata;
    trackingErrorUnion trackingError;
    cellFeatCountUnion fmCellFeatureCount;
    thresholdsUnion thresholds;
} t_vTrackOutputMetadata;

struct t_vTrackResultConfig{
    uint32_t nCells;
    uint32_t maxNumFeatures;
    uint32_t cellGridDimension;
    uint32_t resultsBufferLength;
};

struct t_vTrackResultSF
{
    t_vTrackOutputHeader header;
    t_vTrackOutputMetadata curr;
    t_vTrackOutputMetadata prev;
    t_vTrackDebug debug;
};

// This class contains the pointer to the results buffer and methods to setup that buffer and access
// its elements
class vTrackBulkResult {
public:
    // Gets the buffer length required for the vTrack Results buffer
    // Can only be called after setAddrAndPtrs has been called
    // setConfigAndPtrs is required to be called before vPipeRun (and is done so in plgFlicVtrack
    // so if the buffer has been received from vTrack, this can be used
    uint32_t getResultsBufferLength() {
        if(!addr)
            return 0;
        t_vTrackResultConfig* cfg = (t_vTrackResultConfig*) addr;
        return cfg->resultsBufferLength;
    }

    // sets the buffer containing the results and setting/updating the 
    // ptrs that point to the sub-buffers
    int setAddrAndPtrs(uint8_t* buf) {
        if(!buf)
            return -1;
        addr = buf;
        t_vTrackResultConfig* cfg = (t_vTrackResultConfig*) buf;
        buf += sizeof(t_vTrackResultConfig);
        t_vTrackResultSF* results = (t_vTrackResultSF*) buf;
        buf += sizeof(t_vTrackResultSF);
        
        results->curr.features.ptr = (tvXYLoc*) buf;
        buf += (sizeof(tvXYLoc) * cfg->maxNumFeatures);
        results->curr.metadata.ptr = (FeatureMetadata*) buf;
        buf += (sizeof(FeatureMetadata) * cfg->maxNumFeatures);
        results->curr.trackingError.ptr = (float*) buf;
        buf += (sizeof(float) * cfg->maxNumFeatures);
        results->curr.fmCellFeatureCount.ptr = (uint32_t*) buf;
        buf += (sizeof(uint32_t) * cfg->nCells);
        results->curr.thresholds.ptr = (float*) buf;
        buf += (sizeof(float) * cfg->nCells);

        results->prev.features.ptr = (tvXYLoc*) buf;
        buf += (sizeof(tvXYLoc) * cfg->maxNumFeatures);
        results->prev.metadata.ptr = (FeatureMetadata*) buf;
        buf += (sizeof(FeatureMetadata) * cfg->maxNumFeatures);
        results->prev.trackingError.ptr = (float*) buf;
        buf += (sizeof(float) * cfg->maxNumFeatures);
        results->prev.fmCellFeatureCount.ptr = (uint32_t*) buf;
        buf += (sizeof(uint32_t) * cfg->nCells);
        results->prev.thresholds.ptr = (float*) buf;
        buf += (sizeof(float) * cfg->nCells);
        return buf-addr;
    }

    // sets the result configuration (nCells, maxNumFeatures and cellGridDimension)
    // and sets the buffer location for bulkresults
    uint32_t setConfigAndPtrs(t_vTrackResultConfig& cfg, uint8_t* buf){
        memcpy(buf, &cfg, sizeof(cfg));
        uint32_t buffSize = setAddrAndPtrs(buf);
        return buffSize;
    }
    
    // extract useful structures from results buffer
    void getConfigAndResults(t_vTrackResultConfig** _cfg, t_vTrackResultSF** _results){
        t_vTrackResultConfig* cfg = (t_vTrackResultConfig*) addr;
        t_vTrackResultSF* results = (t_vTrackResultSF*) (addr + sizeof(t_vTrackResultConfig));
        if(_cfg) {
            *_cfg = cfg;
        }
        if(_results) {
            *_results = results;
        }
        uint8_t* buf = (uint8_t*)results + sizeof(t_vTrackResultSF);
        
        results->curr.features.ptr = (tvXYLoc*) buf;
        buf += (sizeof(tvXYLoc) * cfg->maxNumFeatures);
        results->curr.metadata.ptr = (FeatureMetadata*) buf;
        buf += (sizeof(FeatureMetadata) * cfg->maxNumFeatures);
        results->curr.trackingError.ptr = (float*) buf;
        buf += (sizeof(float) * cfg->maxNumFeatures);
        results->curr.fmCellFeatureCount.ptr = (uint32_t*) buf;
        buf += (sizeof(uint32_t) * cfg->nCells);
        results->curr.thresholds.ptr = (float*) buf;
        buf += (sizeof(float) * cfg->nCells);

        results->prev.features.ptr = (tvXYLoc*) buf;
        buf += (sizeof(tvXYLoc) * cfg->maxNumFeatures);
        results->prev.metadata.ptr = (FeatureMetadata*) buf;
        buf += (sizeof(FeatureMetadata) * cfg->maxNumFeatures);
        results->prev.trackingError.ptr = (float*) buf;
        buf += (sizeof(float) * cfg->maxNumFeatures);
        results->prev.fmCellFeatureCount.ptr = (uint32_t*) buf;
        buf += (sizeof(uint32_t) * cfg->nCells);
        results->prev.thresholds.ptr = (float*) buf;
        buf += (sizeof(float) * cfg->nCells);
    }
private:
    uint8_t* addr;
};
#endif
