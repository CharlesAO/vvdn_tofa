///
/// @file
/// @copyright All code copyright Movidius Ltd 2018, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief     vTrack visualization
///

#ifndef VISUALIZER_HPP_
#define VISUALIZER_HPP_

#include <vector>

#include <opencv2/opencv.hpp>

class mvVisualizer
{
public:
    cv::Mat drawFlow(const cv::Mat &srcImg,
            const std::vector<cv::Point2f> &srcPts,
            const std::vector<cv::Point2f> &dstPts,
            const cv::Scalar &colorPoint, const cv::Scalar &colorLine);
    template<typename T>
    cv::Mat drawFlow(const cv::Mat &srcImg, const T srcPts[], const T dstPts[],
            const unsigned int len, const cv::Scalar &colorPoint,
            const cv::Scalar &colorLine)
    {
        cv::Mat dstImg;
        srcImg.copyTo(dstImg);
        for (uint32_t i = 0; i < len; i++)
        {
            const cv::Point &srcXY = cv::Point(cvRound(srcPts[i].x), cvRound(srcPts[i].y));
            const cv::Point &dstXY = cv::Point(cvRound(dstPts[i].x), cvRound(dstPts[i].y));
            cv::line(dstImg, srcXY, dstXY, colorLine);
            cv::circle(dstImg, dstXY, 1, colorPoint, -1);
            //        cv::arrowedLine(dstImg, srcXY, dstXY, colorLine, 2);
        }
        return dstImg;
    }
};

#endif /* VISUALIZER_HPP_ */
