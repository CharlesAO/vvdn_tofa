///
/// @file Helpers.cpp
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

#include <opencv2/core/core.hpp>
#include <string>

#define PATH_SIZE 255

typedef struct {
    std::string leftImage;
    std::string rightImage;
    std::string homographyPath;
    float trueDisparity;
    int input_imgs;
    bool is_h_set;
} RunConfiguration;

// Save homography to file
void saveToFile(cv::Mat& homography, const RunConfiguration& runCfg);

// Load input images
void loadImages(const RunConfiguration& runCfg, cv::Mat& srcL, cv::Mat& srcR);

// Compute homography quality metric
void computeQualityMetrics(cv::Mat h, std::vector<cv::Point2f> srcP,
                           std::vector<cv::Point2f> dstP, int th = 1);
