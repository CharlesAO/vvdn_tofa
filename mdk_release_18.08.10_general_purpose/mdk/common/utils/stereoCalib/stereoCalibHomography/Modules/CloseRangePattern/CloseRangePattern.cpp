///
/// @file CloseRangePattern.cpp
/// @copyright All code copyright Movidius Ltd 2016, all rights reserved.
///            For License Warranty see: common/license.txt
///
/// @brief
///

#include <iostream>

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "CloseRangePattern.h"

using namespace std;
using namespace cv;

void CloseRangePatternCalib(const Mat &srcL, const Mat &srcR,
                            vector<Point2f> &cornersL,
                            vector<Point2f> &cornersR, float tHoldHorizontal,
                            float tHoldVertical) {
    int board_w = 9;
    int board_h = 6;

    Size board_sz(board_w, board_h);
    bool found1 = findChessboardCorners(
        srcL, board_sz, cornersL,
        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

    bool found2 = findChessboardCorners(
        srcR, board_sz, cornersR,
        CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

    // check if checkerboards are detected in both images
    if (!found1 || !found2) {
        cout << "[Error] Not all pattern corners are detected...\n The pattern "
                "must be completely visible in both left and right images."
             << endl;
        cout << "Please retake images according to the user-guide" << endl;
        throw exception();
    }

    // refine corner locations to sub pixel resolution
    cornerSubPix(srcL, cornersL, Size(11, 11), Size(-1, -1),
                 TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
    cornerSubPix(srcR, cornersR, Size(11, 11), Size(-1, -1),
                 TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

    // check the checkeroard is placed correctly to the camera
    double deltaY = cornersL[board_w - 1].y - cornersL[0].y;
    double deltaX = cornersL[board_w - 1].x - cornersL[0].x;
    double currentSlopeH = deltaX != 0 ? deltaY / deltaX : 1.7e+308;

    deltaY = cornersL[board_w * (board_h - 1)].y - cornersL[0].y;
    deltaX = cornersL[board_w * (board_h - 1)].x - cornersL[0].x;
    double currentSlopeV = deltaX != 0 ? deltaY / deltaX : 1.7e+308;
    if ((currentSlopeV < tHoldVertical && currentSlopeV > -tHoldVertical) ||
        (currentSlopeH > tHoldHorizontal && currentSlopeH < -tHoldHorizontal)) {
        cout << "[Error] The camera is not perpendicular to the pattern!"
             << endl;
        cout << "Please retake images according to the user-guide" << endl;
        throw exception();
    }
    
    // check the images are in correct order
    if((cornersR[0].x - cornersL[0].x) > 0){
        cout << "[ERROR] Please check images are correctly labeled, possibly swapping left and right images" << endl;
        throw exception();
    }
}
