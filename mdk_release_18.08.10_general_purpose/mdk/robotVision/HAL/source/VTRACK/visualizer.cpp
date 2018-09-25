///
/// @file
/// @copyright All code copyright Movidius Ltd 2018, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     vTrack visualization
///

#include "visualizer.hpp"

using namespace std;
using namespace cv;

Mat mvVisualizer::drawFlow(const Mat &srcImg, const vector<Point2f> &srcPts,
        const vector<Point2f> &dstPts, const Scalar &colorPoint, const Scalar &colorLine)
{
    return drawFlow<Point2f>(srcImg, srcPts.data(), dstPts.data(),
            dstPts.size(), colorPoint, colorLine);
}
