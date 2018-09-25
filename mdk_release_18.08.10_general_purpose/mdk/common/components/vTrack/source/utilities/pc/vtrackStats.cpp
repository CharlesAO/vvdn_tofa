#include <cstdio>
#include <cstring>

#include <vtrackStats.h>

vTrackKpiStats::vTrackKpiStats()
{
    avgNrOfFeatures = avgNrOfInliers = avgNrOfTrackedFeatures = 0;
    framesCnt = 0;
    nrOfInliers = totalInliers = totalFeatures = totalTrackedFeatures = 0;
}

vTrackKpiStats::~vTrackKpiStats()
{

}

void vTrackKpiStats::finalizeStats()
{
    avgNrOfInliers = (double) totalInliers / framesCnt;
    avgNrOfFeatures = (double) totalFeatures / framesCnt;
    avgNrOfTrackedFeatures = (double) totalTrackedFeatures / framesCnt;
}

void vTrackKpiStats::displayStats()
{
    char buf[4096];
    int n = 0;
    const int length = sizeof(buf);

    n += snprintf(buf + n, length - n, "%-15s", "Frames count");
    n += snprintf(buf + n, length - n, "%-15s", "Avg inliers");
    n += snprintf(buf + n, length - n, "%-15s", "Avg tracked / frame");
    n += snprintf(buf + n, length - n, "%-25s", "Avg tracked features");
    printf("%s\n", buf);

    memset(buf, '=', n);
    buf[n] = '\0';
    printf("%s\n", buf);

    n = 0;
    n += snprintf(buf + n, length - n, "%-15d", framesCnt);
    n += snprintf(buf + n, length - n, "%-15f", avgNrOfInliers);
    n += snprintf(buf + n, length - n, "%-15f", avgNrOfFeatures);
    n += snprintf(buf + n, length - n, "%-25f", avgNrOfTrackedFeatures);
    printf("%s\n", buf);
}


vTrackMoviKpiStats::vTrackMoviKpiStats() : vTrackKpiStats()
{
    avgNrOfTrackingLength = 0;
    totalTrackingLength = 0;
    nrOfTrackingLength = 0;
}

vTrackMoviKpiStats::~vTrackMoviKpiStats()
{

}

void vTrackMoviKpiStats::finalizeStats()
{
    vTrackKpiStats::finalizeStats();
    avgNrOfTrackingLength = (double) totalTrackingLength / nrOfTrackingLength;
}

void vTrackMoviKpiStats::displayStats()
{
    char buf[4096];
    int n = 0;
    const int length = sizeof(buf);

    n += snprintf(buf + n, length - n, "%-15s", "Frames count");
    n += snprintf(buf + n, length - n, "%-15s", "Avg inliers");
    n += snprintf(buf + n, length - n, "%-15s", "Avg features");
    n += snprintf(buf + n, length - n, "%-25s", "Avg tracked / frame");
    n += snprintf(buf + n, length - n, "%-25s", "Avg tracking length");
    printf("%s\n", buf);

    memset(buf, '=', n);
    buf[n] = '\0';
    printf("%s\n", buf);

    n = 0;
    n += snprintf(buf + n, length - n, "%-15d", this->framesCnt);
    n += snprintf(buf + n, length - n, "%-15f", this->avgNrOfInliers);
    n += snprintf(buf + n, length - n, "%-15f", this->avgNrOfFeatures);
    n += snprintf(buf + n, length - n, "%-25f", this->avgNrOfTrackedFeatures);
    n += snprintf(buf + n, length - n, "%-25f", this->avgNrOfTrackingLength);
    printf("%s\n", buf);
}


vTrackMestKpiStats::vTrackMestKpiStats() : vTrackKpiStats()
{
}

vTrackMestKpiStats::~vTrackMestKpiStats()
{

}

void vTrackMestKpiStats::finalizeStats()
{
    vTrackKpiStats::finalizeStats();
}
