#ifndef _VTRACK_PRIVATE_TYPES_H
#define _VTRACK_PRIVATE_TYPES_H
#include <mv_types.h>

struct t_vTrackHarrisFeat
{
    float harris_score;
    int x;
    int y;
};

// Candidate features in a cell
struct tvFeatureCell
{
    uint32_t featureCount;                  // number of features in this cell
    t_vTrackHarrisFeat* features;
};

#endif
