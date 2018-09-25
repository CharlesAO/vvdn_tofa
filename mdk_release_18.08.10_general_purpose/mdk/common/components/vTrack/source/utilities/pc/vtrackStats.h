#ifndef VTRACK_STATS_H
#define VTRACK_STATS_H

#include <stdint.h>

struct vTrackKpiStats
{
    double avgNrOfInliers, avgNrOfFeatures, avgNrOfTrackedFeatures;
    uint32_t totalInliers, totalFeatures, totalTrackedFeatures;
    uint32_t nrOfInliers;
    uint64_t framesCnt;

    vTrackKpiStats();
    virtual ~vTrackKpiStats();
    virtual void finalizeStats();
    virtual void displayStats();
};

struct vTrackMoviKpiStats : public vTrackKpiStats
{
    double avgNrOfTrackingLength;
    uint32_t totalTrackingLength;
    uint32_t nrOfTrackingLength;

    vTrackMoviKpiStats();
    virtual ~vTrackMoviKpiStats();
    virtual void finalizeStats();
    virtual void displayStats();
};

struct vTrackMestKpiStats : public vTrackKpiStats
{
    vTrackMestKpiStats();
    virtual ~vTrackMestKpiStats();
    virtual void finalizeStats();
};

#endif // VTRACK_STATS_H
