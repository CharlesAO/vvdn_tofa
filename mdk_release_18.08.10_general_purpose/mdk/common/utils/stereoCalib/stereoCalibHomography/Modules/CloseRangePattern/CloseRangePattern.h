///
/// @file CloseRangePattern.h
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

#include <opencv2/core/core.hpp>

/**
 * @name CloseRangePatternCalib
 *
 * @brief Calibrates the stereo cameras if the input images are pattern images
 *
 * @param[in] srcL                  : left input image
 *
 * @param[in] srcR                  : right input image
 *
 * @param[out] predictKeyPoints     : a vector of keypoints (contains the
 * coordinates of the pattern corners)
 *
 * @return void
 */

void CloseRangePatternCalib(const cv::Mat &srcL, const cv::Mat &srcR,
                            std::vector<cv::Point2f> &cornersL,
                            std::vector<cv::Point2f> &cornersR,
                            float tHoldH = 0.001, float tHoldV = 1500);
