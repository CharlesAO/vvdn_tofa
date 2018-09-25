///
/// @file Helpers.cpp
/// @copyright All code copyright Movidius Ltd 2015, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

#include "Helpers.h"

#include <iostream>

#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

//----------------------------------------------------------------------------//
// Save homography to file (type 0 = binary file; type 1 = OpenCV format file //
//----------------------------------------------------------------------------//
void saveToFile(Mat &homography, const RunConfiguration &runCfg) {
    /// Display homography
    cout << "--> Homography saved in: " << runCfg.homographyPath << endl;

    /// Save homography
    FILE *f = fopen(runCfg.homographyPath.c_str(), "wb");
    Mat homographyFloat(homography.rows, homography.cols, CV_32FC1);
    homography.convertTo(homographyFloat, CV_32F);

    float *test = (float *)homographyFloat.data;
    fwrite(test, sizeof(float), homography.rows * homography.cols, f);
    fclose(f);
    homographyFloat.release();
}

//---------------------------------------------------------------------------//
// Read images                                                               //
//---------------------------------------------------------------------------//
void loadImages(const RunConfiguration &runCfg, Mat &srcL, Mat &srcR) {
    srcL = imread(runCfg.leftImage, 0);
    srcR = imread(runCfg.rightImage, 0);

    // Exit if images cannot be read
    if (!srcL.data) {
        cout << "\n[Error] Could not open or find left image \n" << endl;
        throw exception();
    }

    if (!srcR.data) {
        cout << "\n[Error] Could not open or find right image \n" << endl;
        throw exception();
    }
}

//---------------------------------------------------------------------------//
// Compute quality metrics for current homography                            //
//---------------------------------------------------------------------------//
void computeQualityMetrics(Mat h, vector<Point2f> srcP, vector<Point2f> dstP,
                           int th) {
    // Compute Inliers & error
    double h11 = h.at<double>(0, 0);
    double h12 = h.at<double>(0, 1);
    double h13 = h.at<double>(0, 2);
    double h21 = h.at<double>(1, 0);
    double h22 = h.at<double>(1, 1);
    double h23 = h.at<double>(1, 2);
    double h31 = h.at<double>(2, 0);
    double h32 = h.at<double>(2, 1);
    double h33 = h.at<double>(2, 2);
    int inliers = 0;         // #number of inliers
    double avgError = 0;     // inliers average error
    double reprojError = 0;  // inliers reprojection error
    double avgVerError =
        0;  // inliers average error computed on vertical(OY) direction only

    for (unsigned int idx = 0; idx < srcP.size(); idx++) {
        double x =
            dstP[idx].x - (h11 * srcP[idx].x + h12 * srcP[idx].y + h13) /
                              (h31 * srcP[idx].x + h32 * srcP[idx].y + h33);
        double y =
            dstP[idx].y - (h21 * srcP[idx].x + h22 * srcP[idx].y + h23) /
                              (h31 * srcP[idx].x + h32 * srcP[idx].y + h33);
        double dist = sqrt(x * x + y * y);

        if (dist < th) {
            inliers++;
            avgError += dist;
            avgVerError += sqrt(y * y);
            reprojError += dist * dist;
        }
    }
    avgError /= inliers;
    avgVerError /= inliers;

    if (avgError < 0.5 && avgVerError < 0.2)
        cout << "\n\nGood Calibration !: Error ==  " << avgVerError << endl;
    else {
        cout << "Poor Calibration , Error = " << avgVerError
             << " (must be smaller 0.2)  " << endl;
        cout << "Please retake images" << endl;
    }
}
