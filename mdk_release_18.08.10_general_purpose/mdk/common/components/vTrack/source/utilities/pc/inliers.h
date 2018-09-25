#include <stdio.h>
#include <vector>
#include <math.h>
#include "vTrackOutput.h"
#include <opencv2/opencv.hpp>
#include <limits>

#define MAX_EPIPOLAR_DISTANCE 0.5

using namespace cv;
class Inliers
{
    float fx;
    float fy;
    float cx;
    float cy;
    float w;
    bool useOpenCV;
    bool useUndistort;
    cv::Mat * camera_matrix;
    cv::Mat * dist_coeffs;
    std::vector<Point2f> matches_currImage;
    std::vector<Point2f> matches_prevImage;
    std::vector<bool *> isInlierList;
    void unwarpPoints(std::vector<Point2f> * unwarpedPoints,std::vector<Point2f> matches_image);
    void initCameraParams(float dist0, float dist1);
    void clearPoints();
public:
    Inliers(float fx, float fy, float cx, float cy, bool useOpenCV = true , bool useUndistort = false, float param1 = 0 , float param2 = 0);
    int calcPercentageInliers();
    void addMatchesToList(bool * isInlier,float currX, float currY, float prevX, float prevY);
};
